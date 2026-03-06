#include <qobjectdefs.h>
#include <qstring.h>
#include <qstyle.h>
#include <qstyleplugin.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtmetamacros.h>
#include <qtversionchecks.h>
#else
#include <qglobal.h>
#endif

#include "proxystyle.hpp"

class StylePlugin: public QStylePlugin {
	Q_OBJECT;
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "engine.json")

public:
	QStyle* create(const QString& key) override {
		if (key == QString::fromLatin1("qtengine")) return new ProxyStyle();
		return nullptr;
	}
};

#include "plugin.moc"
