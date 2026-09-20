#include <qlogging.h>
#include <qloggingcategory.h>
#include <qobjectdefs.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformthemeplugin.h>
#include <qstring.h>
#include <qstringlist.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtmetamacros.h>
#include <qtversionchecks.h>
#else
#include <qglobal.h>
#endif

#include "platformtheme.hpp"

Q_LOGGING_CATEGORY(logPlatformTheme, "qtengine.platformtheme", QtWarningMsg);

class QtEngineThemePlugin: public QPlatformThemePlugin {
	Q_OBJECT;
	Q_PLUGIN_METADATA(
	    IID "org.qt-project.Qt.QPA.QPlatformThemeFactoryInterface.5.1" FILE "engine.json"
	);

public:
	QPlatformTheme* create(const QString& key, const QStringList& params) override {
		(void) params;

		if (key.toLower() == QString::fromLatin1("qtengine")) {
			qCInfo(logPlatformTheme) << "Initializing qtengine platform theme plugin";

			return new PlatformTheme();
		}

		return nullptr;
	}
};

#include "main.moc"
