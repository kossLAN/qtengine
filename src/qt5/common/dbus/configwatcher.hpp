#pragma once

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QLoggingCategory>
#include <QObject>
#include <QString>
#include <QTimer>

Q_DECLARE_LOGGING_CATEGORY(logConfigWatcher);

class ConfigWatcher: public QObject {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.qtengine.ConfigWatcher")

public:
	static ConfigWatcher& instance();

	~ConfigWatcher() override;

	Q_DISABLE_COPY_MOVE(ConfigWatcher)

	void setupFileWatching();

signals:
	Q_SCRIPTABLE void configChanged();

private slots:
	void onFileChanged(const QString& path);
	void onRemoteConfigChanged();
	void onServiceUnregistered(const QString& service);

private:
	explicit ConfigWatcher(QObject* parent = nullptr);
	void tryRegister();
	void connectToRemote();

	bool mIsRegistered = false;
	QFileSystemWatcher* mFileWatcher = nullptr;
	QDBusServiceWatcher* mServiceWatcher = nullptr;
	QTimer* mDebounceTimer = nullptr;
	QString mConfigPath;
};
