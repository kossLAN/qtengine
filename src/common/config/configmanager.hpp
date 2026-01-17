#pragma once

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logConfigManager);

class ConfigManager {
public:
	void init();

	QString colorScheme;
	QString iconTheme;
	QString style;
	QString fontFixed;
	int fontFixedSize = 11;
	QString font;
	int fontSize = 11;

	bool singleClickActivate = true;
	bool menusHaveIcons = true;
	bool shortcutsForContextMenus = true;
};

const ConfigManager& configManager();
