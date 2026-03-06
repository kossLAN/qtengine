#include <utility>

#include <kcolorscheme.h>
#include <ksharedconfig.h>
#include <qpalette.h>
#include <qset.h>
#include <qstring.h>

#include "common.hpp"

QSet<Style::CStyleInstance*> Style::styleInstances;

QPalette Style::loadColorScheme(const QString& filePath) {
	auto config = KSharedConfig::openConfig(filePath);
	config->reparseConfiguration();
	return KColorScheme::createApplicationPalette(config);
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
