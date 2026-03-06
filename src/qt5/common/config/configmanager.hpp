#pragma once

#include <qdebug.h>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qloggingcategory.h>
#include <qstring.h>

Q_DECLARE_LOGGING_CATEGORY(logConfigManager);

class ConfigManager {
public:
	void init();
	void reload();

	QString configPath;

	QString colorScheme;
	QString iconTheme;
	QString style;
	QString quickStyle;

	QString fontFixed;
	int fontFixedSize = 11;
	int fontFixedWeight = -1;

	QString font;
	int fontSize = 11;
	int fontWeight = -1;

	bool singleClickActivate = true;
	bool menusHaveIcons = true;
	bool shortcutsForContextMenus = true;

private:
	void loadFromPath(const QString& path);
};

ConfigManager& configManager();
