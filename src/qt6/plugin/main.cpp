#include <qcontainerfwd.h>
#include <qlibraryinfo.h>
#include <qlogging.h>
#include <qloggingcategory.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformthemeplugin.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qversionnumber.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtmetamacros.h>
#include <qtversionchecks.h>
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

		const QVersionNumber v = QLibraryInfo::version();

		constexpr int expectedMajor = QT_VERSION_MAJOR; // NOLINT

		if (v.majorVersion() != expectedMajor) {
			qCCritical(logPlatformTheme)
			    << "qtengine was compiled against an incompatible qt version. Compiled against"
			    << expectedMajor << "but has" << v.majorVersion();
			return nullptr;
		}

		if (key.toLower() == QString::fromLatin1("qtengine")) {
			qCInfo(logPlatformTheme) << "Initializing qtengine platform theme plugin";

			return new PlatformTheme();
		}

		return nullptr;
	}
};

#include "main.moc"
