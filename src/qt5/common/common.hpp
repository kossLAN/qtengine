#pragma once

#include <qpalette.h>
#include <qstring.h>

class Style {
public:
	static QPalette loadColorScheme(const QString& filePath);
};
