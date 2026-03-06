#include "configwatcher.hpp"

#include <qdbusconnection.h>
#include <qdbusservicewatcher.h>
#include <qfileinfo.h>
#include <qfilesystemwatcher.h>
#include <qglobal.h>
#include <qlogging.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qstring.h>
#include <qstringliteral.h>
#include <qtimer.h>

#include "config/configmanager.hpp"

Q_LOGGING_CATEGORY(logConfigWatcher, "qtengine.configwatcher", QtWarningMsg)

static const QString SERVICE_NAME = QStringLiteral("org.qtengine.ConfigWatcher");
static const QString OBJECT_PATH = QStringLiteral("/org/qtengine/ConfigWatcher");
static const QString INTERFACE_NAME = QStringLiteral("org.qtengine.ConfigWatcher");

ConfigWatcher& ConfigWatcher::instance() {
	static ConfigWatcher instance;
	return instance;
}

ConfigWatcher::ConfigWatcher(QObject* parent)
    : QObject(parent)
    , mConfigPath(configManager().configPath) {

	if (this->mConfigPath.isEmpty()) {
		qCWarning(logConfigWatcher) << "No config file path available, config watching disabled";
		return;
	}

	this->mDebounceTimer = new QTimer(this);
	this->mDebounceTimer->setSingleShot(true);
	this->mDebounceTimer->setInterval(250);
	connect(this->mDebounceTimer, &QTimer::timeout, this, [this]() {
		qCInfo(logConfigWatcher) << "Config file changed, notifying listeners";
		emit this->configChanged();
	});

	auto bus = QDBusConnection::sessionBus();

	if (!bus.isConnected()) {
		qCInfo(logConfigWatcher) << "D-Bus session bus not connected, skipping file watching";
		return;
	}

	this->mServiceWatcher = new QDBusServiceWatcher(
	    SERVICE_NAME,
	    bus,
	    QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration,
	    this
	);

	connect(
	    this->mServiceWatcher,
	    &QDBusServiceWatcher::serviceUnregistered,
	    this,
	    &ConfigWatcher::onServiceUnregistered
	);

	this->tryRegister();
}

ConfigWatcher::~ConfigWatcher() {
	if (this->mIsRegistered) {
		auto bus = QDBusConnection::sessionBus();

		if (bus.isConnected()) {
			bus.unregisterObject(OBJECT_PATH);
			bus.unregisterService(SERVICE_NAME);
		}
	}
}

void ConfigWatcher::tryRegister() {
	auto bus = QDBusConnection::sessionBus();

	if (!bus.registerService(SERVICE_NAME)) {
		this->connectToRemote();
		return;
	}

	this->mIsRegistered = true;
	bus.registerObject(OBJECT_PATH, this, QDBusConnection::ExportScriptableSignals);

	this->setupFileWatching();

	qCInfo(logConfigWatcher) << "Registered config watcher on D-Bus";
}

void ConfigWatcher::setupFileWatching() {
	if (this->mConfigPath.isEmpty()) {
		qCWarning(logConfigWatcher) << "No config file path available, skipping file watch setup";
		return;
	}

	delete this->mFileWatcher;
	this->mFileWatcher = new QFileSystemWatcher(this);

	const bool fileAdded = this->mFileWatcher->addPath(this->mConfigPath);
	qCInfo(logConfigWatcher) << "Watching config file:" << this->mConfigPath
	                         << "(inotify:" << fileAdded << ")";

	const QFileInfo fi(this->mConfigPath);
	if (fi.absolutePath() != this->mConfigPath) {
		this->mFileWatcher->addPath(fi.absolutePath());
	}

	connect(
	    this->mFileWatcher,
	    &QFileSystemWatcher::fileChanged,
	    this,
	    &ConfigWatcher::onFileChanged
	);
	connect(this->mFileWatcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString&) {
		if (QFileInfo::exists(this->mConfigPath)) {
			if (!this->mFileWatcher->files().contains(this->mConfigPath)) {
				this->mFileWatcher->addPath(this->mConfigPath);
			}

			this->mDebounceTimer->start();
		}
	});

	const auto& cfg = configManager();
	if (!cfg.colorScheme.isEmpty() && QFileInfo::exists(cfg.colorScheme)) {
		this->mFileWatcher->addPath(cfg.colorScheme);
		qCInfo(logConfigWatcher) << "Watching color scheme file:" << cfg.colorScheme;
	}
}

void ConfigWatcher::connectToRemote() {
	auto bus = QDBusConnection::sessionBus();

	bus.connect(
	    SERVICE_NAME,
	    OBJECT_PATH,
	    INTERFACE_NAME,
	    QStringLiteral("configChanged"),
	    this,
	    SLOT(onRemoteConfigChanged())
	);

	qCInfo(logConfigWatcher) << "Connected to existing config watcher instance";
}

void ConfigWatcher::onFileChanged(const QString& path) {
	QTimer::singleShot(100, this, [this, path]() {
		if (QFileInfo::exists(path) && !this->mFileWatcher->files().contains(path)) {
			this->mFileWatcher->addPath(path);
		}
	});

	this->mDebounceTimer->start();
}

void ConfigWatcher::onRemoteConfigChanged() { emit this->configChanged(); }

void ConfigWatcher::onServiceUnregistered(const QString& service) {
	Q_UNUSED(service)

	if (!this->mIsRegistered) {
		qCInfo(logConfigWatcher) << "Config watcher instance unregistered, attempting to take over";

		auto bus = QDBusConnection::sessionBus();
		bus.disconnect(
		    SERVICE_NAME,
		    OBJECT_PATH,
		    INTERFACE_NAME,
		    QStringLiteral("configChanged"),
		    this,
		    SLOT(onRemoteConfigChanged())
		);

		this->tryRegister();
	}
}
