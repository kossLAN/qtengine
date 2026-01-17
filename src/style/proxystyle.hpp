#pragma once

#include <QProxyStyle>
#include <QStyleHintReturn>
#include <QStyleOption>
#include <QWidget>
#include <QtGlobal>
#include <QLoggingCategory>

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
