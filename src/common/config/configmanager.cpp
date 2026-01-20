#include "configmanager.hpp"
#include <cmath>
#include <mutex>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QList>
#include <QLoggingCategory>
#include <QStandardPaths>
#include <QString>
#include <Qt>
#include <QtGlobal>
#include <qdebug.h>
#include <qfileinfo.h>
#include <qlogging.h>
#include <qstringview.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtenvironmentvariables.h>
#include <qtversionchecks.h>
#endif

Q_LOGGING_CATEGORY(logConfigManager, "qtengine.configmanager", QtWarningMsg)

namespace {

QJsonValue valueAtPath(const QJsonObject& root, const QString& path) {
	const auto parts = path.split('.', Qt::SkipEmptyParts);
	QJsonValue current = QJsonValue(root);

	for (const QString& part: parts) {
		if (!current.isObject()) return QJsonValue();

		current = current.toObject().value(part);
	}

	return current;
}

QString homeString(const QString& s) {
	if (s.isEmpty()) return s;

	const QByteArray home = qgetenv("HOME");
	if (home.isEmpty()) return s;

	if (s == "~") return QString::fromUtf8(home);

	if (s.startsWith("~/")) return QString::fromUtf8(home) + s.mid(1);

	return s;
}

QString
getPath(const QJsonObject& root, const QString& path, const QString& defaultVal = QString()) {
	const QJsonValue v = valueAtPath(root, path);

	if (v.isUndefined() || v.isNull()) {
		qCDebug(logConfigManager) << "Config: key missing or null for" << path
		                          << "- using default:" << defaultVal;
		return homeString(defaultVal);
	}

	if (!v.isString()) {
		qDebug() << "Config: expected string at" << path << "but got" << v.type();
		return homeString(defaultVal);
	}

	return homeString(v.toString());
}

QString
getString(const QJsonObject& root, const QString& path, const QString& defaultVal = QString()) {
	const QJsonValue v = valueAtPath(root, path);

	if (v.isUndefined() || v.isNull()) {
		qCDebug(logConfigManager) << "Config: key missing or null for" << path
		                          << "- using default:" << defaultVal;

		return defaultVal;
	}

	if (!v.isString()) {
		qDebug() << "Config: expected string at" << path << "but got" << v.type();

		return defaultVal;
	}

	return v.toString();
}

int getInt(const QJsonObject& root, const QString& path, int defaultVal = 0) {
	const QJsonValue v = valueAtPath(root, path);

	if (v.isUndefined() || v.isNull()) {
		qCDebug(logConfigManager) << "Config: key missing or null for" << path
		                          << "- using default:" << defaultVal;

		return defaultVal;
	}

	if (v.isDouble()) {
		const double d = v.toDouble();
		const double floored = std::floor(d);

		if (d == floored) {
			return static_cast<int>(floored);
		} else {
			qCDebug(logConfigManager) << "Config: expected integer at" << path
			                          << "but got non-integer number" << d;

			return defaultVal;
		}
	}

	qCDebug(logConfigManager) << "Config: expected number at" << path << "but got" << v.type();

	return defaultVal;
}

bool getBool(const QJsonObject& root, const QString& path, bool defaultVal = false) {
	const QJsonValue v = valueAtPath(root, path);

	if (v.isUndefined() || v.isNull()) {
		qCDebug(logConfigManager) << "Config: key missing or null for" << path
		                          << "- using default:" << defaultVal;

		return defaultVal;
	}

	if (v.isBool()) return v.toBool();

	if (v.isDouble()) return (v.toInt() != 0);

	qCDebug(logConfigManager) << "Config: expected bool or numeric (0/1) at" << path << "but got"
	                          << v.type();

	return defaultVal;
}

QByteArray findConfig() {
	if (qEnvironmentVariableIsSet("QTENGINE_CONFIG")) {
		const QByteArray env = qgetenv("QTENGINE_CONFIG");
		const QString path = QString::fromUtf8(env);
		const QFileInfo fileInfo(path);

		if (fileInfo.exists()) return env;
	}

	if (qEnvironmentVariableIsSet("XDG_CONFIG_HOME")) {
		const QByteArray env = qgetenv("XDG_CONFIG_HOME");
		const QString dir = QString::fromUtf8(env);
		const QString fullPath = QDir(dir).filePath("qtengine/config.json");
		const QFileInfo fileInfo(fullPath);

		if (fileInfo.exists()) return fullPath.toUtf8();
	}

	if (qEnvironmentVariableIsSet("HOME")) {
		const QByteArray env = qgetenv("HOME");
		const QString dir = QString::fromUtf8(env);
		const QString fullPath = QDir(dir).filePath(".config/qtengine/config.json");
		const QFileInfo fileInfo(fullPath);

		if (fileInfo.exists()) return fullPath.toUtf8();
	}

	if (qEnvironmentVariableIsSet("XDG_CONFIG_DIRS")) {
		const QByteArray env = qgetenv("XDG_CONFIG_DIRS");
		const QList<QByteArray> paths = qgetenv("XDG_CONFIG_DIRS").split(':');

		for (const QByteArray& p: paths) {
			const QString fullPath = QDir(p).filePath("qtengine/config.json");
			const QFileInfo fileInfo(fullPath);

			if (fileInfo.exists()) return fullPath.toUtf8();
		}
	}

	return QByteArray();
}
} // namespace

void ConfigManager::init() {
	const QByteArray configPath = findConfig();
	QFile file(configPath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qCDebug(logConfigManager) << "Failed to open config file" << configPath;
		return;
	}

	QJsonParseError parseError {};
	const QByteArray raw = file.readAll();
	const QJsonDocument doc = QJsonDocument::fromJson(raw, &parseError);

	if (parseError.error != QJsonParseError::NoError) {
		qCDebug(logConfigManager) << "JSON parse error at offset" << parseError.offset << ":"
		                          << parseError.errorString();
		return;
	}

	if (!doc.isObject()) {
		qCDebug(logConfigManager) << "Top-level JSON must be an object";
		return;
	}

	const QJsonObject root = doc.object();

	this->colorScheme = getPath(root, "theme.colorScheme", QString());
	this->iconTheme = getString(root, "theme.iconTheme", QString());
	this->style = getString(root, "theme.style", QString());

	this->fontFixed = getString(root, "theme.fontFixed.family", QString("monospace"));
	this->fontFixedSize = getInt(root, "theme.fontFixed.size", 11);
	this->fontFixedWeight = getInt(root, "theme.fontFixed.weight", -1);

	this->font = getString(root, "theme.font.family", QString("Sans Serif"));
	this->fontSize = getInt(root, "theme.font.size", 11);
	this->fontWeight = getInt(root, "theme.font.weight", -1);

	this->singleClickActivate = getBool(root, "misc.singleClickActivate", true);
	this->menusHaveIcons = getBool(root, "misc.menusHaveIcons", true);
	this->shortcutsForContextMenus = getBool(root, "misc.shortcutsForContextMenus", true);
}

const ConfigManager& configManager() {
	static ConfigManager instance;
	static std::once_flag initFlag;
	std::call_once(initFlag, [&]() { instance.init(); });
	return instance;
}
