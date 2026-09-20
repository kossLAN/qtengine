#include <qobjectdefs.h>
#include <qstring.h>
#include <qstyleplugin.h>
#include <qtmetamacros.h>

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
