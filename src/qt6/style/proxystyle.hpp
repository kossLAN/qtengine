#pragma once

#include <qglobal.h>
#include <qloggingcategory.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qwidget.h>

Q_DECLARE_LOGGING_CATEGORY(logStyle);

class ProxyStyle: public QProxyStyle {
	Q_OBJECT;

public:
	explicit ProxyStyle();

	~ProxyStyle() override = default;

	Q_DISABLE_COPY_MOVE(ProxyStyle)

	int styleHint(
	    StyleHint hint,
	    const QStyleOption* option,
	    const QWidget* widget,
	    QStyleHintReturn* returnData
	) const override;
};
