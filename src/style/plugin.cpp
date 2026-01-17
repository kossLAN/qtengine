#include <QObject>
#include <QString>
#include <QStylePlugin>
#include <QtPlugin>

// NOLINTBEGIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qtmetamacros.h>
#endif
// NOLINTEND

#include "proxystyle.hpp"

class StylePlugin: public QStylePlugin {
	Q_OBJECT;
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "engine.json")

public:
	QStyle* create(const QString& key) override {
		if (key == "qtengine") return new ProxyStyle();
		return nullptr;
	}
};

#include "plugin.moc"
