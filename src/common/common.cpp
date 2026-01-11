#include <optional>
#include <utility>

#include <QDir>
#include <QFile>
#include <QLatin1String>
#include <QLocale>
#include <QPalette>
#include <QRegularExpression>
#include <QSet>
#include <QSettings>
#include <QStandardPaths>
#include <QStringList>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QtGlobal>
#include <QtDebug>
#include <QtGlobal>
#include <qdebug.h>
#if defined KF_CONFIGCORE_LIB && defined KF_COLORSCHEME_LIB
#include <KColorScheme>
#include <KSharedConfig>
#endif
#include "common.hpp"

using namespace Qt::Literals::StringLiterals;

QSet<Style::CStyleInstance*> Style::styleInstances;

bool isKColorScheme(const QString& filePath) { return filePath.toLower().endsWith(".colors"_L1); }

//
std::optional<QPalette> Style::loadColorScheme(const QString& filePath) {
	if (filePath.isEmpty()) return std::nullopt;

#if defined KF_CONFIGCORE_LIB && defined KF_COLORSCHEME_LIB
	if (filePath.toLower().endsWith(".colors"))
		return KColorScheme::createApplicationPalette(KSharedConfig::openConfig(filePath));
#endif

	QSettings settings(filePath, QSettings::IniFormat);
	settings.beginGroup("ColorScheme"_L1);
	auto activeColors = settings.value("active_colors"_L1).toStringList();
	auto inactiveColors = settings.value("inactive_colors"_L1).toStringList();
	auto disabledColors = settings.value("disabled_colors"_L1).toStringList();
	settings.endGroup();

	// Qt 6.6+ introduced an additional role (Accent). Some color-scheme files may
	// omit it; if we're exactly one short, duplicate Highlight as a fallback.
	const int expectedRoles = static_cast<int>(QPalette::NColorRoles);
	if (activeColors.count() == expectedRoles - 1)
		activeColors << activeColors.at(static_cast<int>(QPalette::Highlight));
	if (inactiveColors.count() == expectedRoles - 1)
		inactiveColors << inactiveColors.at(static_cast<int>(QPalette::Highlight));
	if (disabledColors.count() == expectedRoles - 1)
		disabledColors << disabledColors.at(static_cast<int>(QPalette::Highlight));

	if (activeColors.count() < QPalette::NColorRoles || inactiveColors.count() < QPalette::NColorRoles
	    || disabledColors.count() < QPalette::NColorRoles)
		return std::nullopt;

	QPalette customPalette;
	for (int i = 0; i < QPalette::NColorRoles; i++) {
		const auto role = static_cast<QPalette::ColorRole>(i);
		customPalette.setColor(QPalette::Active, role, QColor(activeColors.at(i)));
		customPalette.setColor(QPalette::Inactive, role, QColor(inactiveColors.at(i)));
		customPalette.setColor(QPalette::Disabled, role, QColor(disabledColors.at(i)));
	}
	return customPalette;
}

void Style::registerStyleInstance(Style::CStyleInstance* instance) {
	styleInstances.insert(instance);
}

void Style::unregisterStyleInstance(Style::CStyleInstance* instance) {
	styleInstances.remove(instance);
}

void Style::reloadStyleInstanceSettings() {
	for (auto* instance: std::as_const(styleInstances)) instance->reloadSettings();
}
