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
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/Qt>
#include <QtCore/QtGlobal>
#include <QtCore/QtMath>
#include <QtGlobal>
#include <qdebug.h>
#include <qfileinfo.h>
#include <qlogging.h>

// NOLINTBEGIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtenvironmentvariables.h>
#endif
// NOLINTEND

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

QString
getString(const QJsonObject& root, const QString& path, const QString& defaultVal = QString()) {
	const QJsonValue v = valueAtPath(root, path);
	if (v.isUndefined() || v.isNull()) {
		qDebug() << "Config: key missing or null for" << path << "- using default:" << defaultVal;

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
		qDebug() << "Config: key missing or null for" << path << "- using default:" << defaultVal;

		return defaultVal;
	}

	if (v.isDouble()) {
		const double d = v.toDouble();
		const double floored = std::floor(d);

		if (d == floored) {
			return static_cast<int>(floored);
		} else {
			qDebug() << "Config: expected integer at" << path << "but got non-integer number" << d;
			return defaultVal;
		}
	}

	qDebug() << "Config: expected number at" << path << "but got" << v.type();

	return defaultVal;
}

bool getBool(const QJsonObject& root, const QString& path, bool defaultVal = false) {
	const QJsonValue v = valueAtPath(root, path);
	if (v.isUndefined() || v.isNull()) {
		qDebug() << "Config: key missing or null for" << path << "- using default:" << defaultVal;
		return defaultVal;
	}

	if (v.isBool()) return v.toBool();

	if (v.isDouble()) return (v.toInt() != 0);

	qDebug() << "Config: expected bool or numeric (0/1) at" << path << "but got" << v.type();

	return defaultVal;
}

QByteArray findConfig() {
	if (qEnvironmentVariableIsSet("QTENGINE_CONFIG")) {
		const QByteArray path = qgetenv("QTENGINE_CONFIG");
		const QFileInfo fileInfo = QFileInfo(path);

		if (fileInfo.exists()) return path;
	}

	if (qEnvironmentVariableIsSet("XDG_CONFIG_HOME")) {
		const QByteArray fullPath = qgetenv("XDG_CONFIG_HOME").append("/qtengine/config.json");
		const QFileInfo fileInfo = QFileInfo(fullPath);

		if (fileInfo.exists()) return fullPath;
	}

	if (qEnvironmentVariableIsSet("XDG_CONFIG_DIRS")) {
		const QList<QByteArray> paths = qgetenv("XDG_CONFIG_DIRS").split(':');

		for (QByteArray path: paths) {
			const QByteArray fullPath = path.append("/qtengine/config.json");
			const QFileInfo fileInfo = QFileInfo(fullPath);

			if (fileInfo.exists()) return fullPath;
		}
	}

	return nullptr;
}
} // namespace

void ConfigManager::init() {
	const QByteArray configPath = findConfig();
	QFile file(configPath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open config file" << configPath;
		return;
	}

	QJsonParseError parseError {};
	const QByteArray raw = file.readAll();
	const QJsonDocument doc = QJsonDocument::fromJson(raw, &parseError);

	if (parseError.error != QJsonParseError::NoError) {
		qDebug() << "JSON parse error at offset" << parseError.offset << ":"
		         << parseError.errorString();
		return;
	}

	if (!doc.isObject()) {
		qDebug() << "Top-level JSON must be an object";
		return;
	}

	const QJsonObject root = doc.object();

	this->colorScheme = getString(root, "theme.colorScheme", QString());
	this->iconTheme = getString(root, "theme.iconTheme", QString());
	this->style = getString(root, "theme.style", QString());
	this->fontFixed = getString(root, "theme.fontFixed", QString("monospace"));
	this->fontFixedSize = getInt(root, "theme.fontFixedSize", 11);
	this->font = getString(root, "theme.font", QString("Sans Serif"));
	this->fontSize = getInt(root, "theme.fontSize", 11);

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
