#include <QDebug>
#include <QLibraryInfo>
#include <QLoggingCategory>
#include <QString>
#include <QStringList>
#include <QVersionNumber>
#include <QtContainerFwd>
#include <qlogging.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformthemeplugin.h>

// NOLINTBEGIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtmetamacros.h>
#endif
// NOLINTEND

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

		// NOLINTBEGIN
		constexpr int expectedMajor = QT_VERSION_MAJOR;

		if (v.majorVersion() != expectedMajor) {
			qCCritical(logPlatformTheme)
			    << "qtengine was compiled against an incompatible qt version. Compiled against"
			    << expectedMajor << "but has" << v.majorVersion();
			return nullptr;
		}
		// NOLINTEND

		if (key.toLower() == QString::fromLatin1("qtengine")) {
			qCInfo(logPlatformTheme) << "Initializing qtengine platform theme plugin";

			return new PlatformTheme();
		}

		return nullptr;
	}
};

#include "main.moc"
