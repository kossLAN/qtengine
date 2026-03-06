#pragma once

#include <qglobal.h>
#include <qloggingcategory.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qwidget.h>

#include "common.hpp"

Q_DECLARE_LOGGING_CATEGORY(logStyle);

class ProxyStyle
    : public QProxyStyle
    , public Style::CStyleInstance {
	Q_OBJECT;

public:
	explicit ProxyStyle();

	void reloadSettings() override {}

	~ProxyStyle() override;

	Q_DISABLE_COPY_MOVE(ProxyStyle)

	int styleHint(
	    StyleHint hint,
	    const QStyleOption* option,
	    const QWidget* widget,
	    QStyleHintReturn* returnData
	) const override;
};
