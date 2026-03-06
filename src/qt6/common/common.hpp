#pragma once

#include <qpalette.h>
#include <qset.h>
#include <qstring.h>
#include <qstringlist.h>

class Style {
public:
	class CStyleInstance {
	public:
		CStyleInstance() = default;
		CStyleInstance(const CStyleInstance&) = delete;
		CStyleInstance& operator=(const CStyleInstance&) = delete;
		CStyleInstance(CStyleInstance&&) = delete;
		CStyleInstance& operator=(CStyleInstance&&) = delete;
		virtual ~CStyleInstance() = default;
		virtual void reloadSettings() = 0;
	};

	static QPalette loadColorScheme(const QString& filePath);

	static void registerStyleInstance(CStyleInstance* instance);
	static void unregisterStyleInstance(CStyleInstance* instance);
	static void reloadStyleInstanceSettings();

private:
	static QSet<CStyleInstance*> styleInstances;
};
