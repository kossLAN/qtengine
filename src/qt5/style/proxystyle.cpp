#include "proxystyle.hpp"

#include <qlogging.h>
#include <qloggingcategory.h>
#include <qnamespace.h>
#include <qproxystyle.h>
#include <qstring.h>
#include <qstyle.h>
#include <qstylefactory.h>
#include <qstyleoption.h>
#include <qwidget.h>

#include "common.hpp"
#include "config/configmanager.hpp"

Q_LOGGING_CATEGORY(logStyle, "qtengine.style", QtWarningMsg);

ProxyStyle::ProxyStyle() {
	Style::registerStyleInstance(this);

	const QString styleName = configManager().style;
	QStyle* style = nullptr;

	if (!styleName.isEmpty() && styleName.compare("qtengine", Qt::CaseInsensitive) != 0) {
		style = QStyleFactory::create(styleName);
	}

	if (!style) {
		style = QStyleFactory::create("Fusion");
		qCDebug(logStyle) << "Failed to load style:" << styleName;
	}

	if (style) this->setBaseStyle(style);
}

ProxyStyle::~ProxyStyle() { Style::unregisterStyleInstance(this); }

int ProxyStyle::styleHint(
    QStyle::StyleHint hint,
    const QStyleOption* option,
    const QWidget* widget,
    QStyleHintReturn* returnData
) const {
	const auto& cfg = configManager();

	switch (hint) {
	case QStyle::SH_DialogButtonBox_ButtonsHaveIcons: return cfg.menusHaveIcons ? 1 : 0;
	case QStyle::SH_ItemView_ActivateItemOnSingleClick: return cfg.singleClickActivate ? 1 : 0;
	case QStyle::SH_UnderlineShortcut: return cfg.shortcutsForContextMenus ? 1 : 0;
	default: break;
	}

	return this->QProxyStyle::styleHint(hint, option, widget, returnData);
}
