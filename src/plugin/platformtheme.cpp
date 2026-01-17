/*
 * Copyright (c) 2020-2025, Ilya Kotov <forkotov02@ya.ru>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QDir>
#include <QFont>
#include <QGuiApplication>
#include <QIcon>
#include <QMetaObject>
#include <QMimeDatabase>
#include <QMimeType>
#include <QObject>
#include <QPalette>
#include <QRegularExpression>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QVariant>
#include <Qt>
#include <QtCore/QLatin1String>
#include <QtCore/QStringList>
#include <QtCore/QtGlobal>
#include <QtGlobal>

#ifdef QT_WIDGETS_LIB
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QWidget>
#if QT_CONFIG(graphicsview)
#include <QGraphicsScene>
#endif
#include <private/qapplication_p.h>
#endif
#include <QFile>
#include <QFileSystemWatcher>
#ifdef QT_QUICKCONTROLS2_LIB
#include <QQuickStyle>
#endif

#include <utility>

#include <KIconEngine>
#include <KIconLoader>
#include <QStringList>
#include <qcontainerfwd.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformthemefactory_p.h>
#include <qpa/qwindowsysteminterface.h>

#if __has_include(<private/qgenericunixtheme_p.h>)
#include <private/qgenericunixtheme_p.h>
#else
#include <private/qgenericunixthemes_p.h>
#endif

#include "../common/common.hpp"
#include "../common/config/configmanager.hpp"
#include "platformtheme.hpp"

// QT_QPA_PLATFORMTHEME=qtengine

PlatformTheme::PlatformTheme()
    : mFixedFont(*this->QGenericUnixTheme::font(QPlatformTheme::FixedFont))
    , mFont(*this->QGenericUnixTheme::font(QPlatformTheme::SystemFont)) {
	if (QGuiApplication::desktopSettingsAware()) {
		this->applySettings();
		QMetaObject::invokeMethod(this, &PlatformTheme::applySettings, Qt::QueuedConnection);
		// must be applied before Q_COREAPP_STARTUP_FUNCTION execution
		const QString colorScheme = configManager().colorScheme;
		qApp->setProperty("KDE_COLOR_SCHEME_PATH", colorScheme);

#if defined QT_WIDGETS_LIB && defined QT_QUICKCONTROLS2_LIB
		if (hasWidgets())
			// don't override the value explicitly set by the user
			if (QQuickStyle::name().isEmpty() || QQuickStyle::name() == QLatin1String("Fusion"))
				QQuickStyle::setStyle(QLatin1String("org.kde.desktop"));
#endif
	}
	QCoreApplication::instance()->installEventFilter(this);
}

bool PlatformTheme::usePlatformNativeDialog(DialogType type) const {
	return this->mTheme ? this->mTheme->usePlatformNativeDialog(type)
	                    : this->QGenericUnixTheme::usePlatformNativeDialog(type);
}

QPlatformDialogHelper* PlatformTheme::createPlatformDialogHelper(DialogType type) const {
	return this->mTheme ? this->mTheme->createPlatformDialogHelper(type)
	                    : this->QGenericUnixTheme::createPlatformDialogHelper(type);
}

const QPalette* PlatformTheme::palette(QPlatformTheme::Palette type) const {
	if (type == QPlatformTheme::SystemPalette && this->mPalette) return &*this->mPalette;
	return this->QGenericUnixTheme::palette(type);
}

const QFont* PlatformTheme::font(QPlatformTheme::Font type) const {
	if (type == QPlatformTheme::FixedFont) return &this->mFixedFont;
	return &this->mFont;
}

QStringList PlatformTheme::iconPaths() {
	QStringList paths = {QDir::homePath() + QString::fromLatin1("/.icons")};

	for (const QString& p: QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation)) {
		paths << (p + QString::fromLatin1("/icons"));
	}
	paths.removeDuplicates();

	// remove invalid
	QStringList::iterator it = paths.begin();
	while (it != paths.end()) {
		if (QDir(*it).exists()) ++it;
		else it = paths.erase(it);
	}

	return paths;
}

QVariant PlatformTheme::themeHint(QPlatformTheme::ThemeHint hint) const {
	if (this->mIsIgnored) return this->QGenericUnixTheme::themeHint(hint);

	const auto& cfg = configManager();

	switch (hint) {
	case QPlatformTheme::CursorFlashTime: return 1000;
	case QPlatformTheme::MouseDoubleClickInterval: return 400;
	case QPlatformTheme::ToolButtonStyle: return 4;
	case QPlatformTheme::SystemIconThemeName: return cfg.iconTheme;
	case QPlatformTheme::StyleNames: return {QString::fromLatin1("qtengine")};
	case QPlatformTheme::IconThemeSearchPaths: return iconPaths();
	case QPlatformTheme::DialogButtonBoxLayout: return 0;
	case QPlatformTheme::KeyboardScheme: return 2;
	case QPlatformTheme::UiEffects: return {};
	case QPlatformTheme::WheelScrollLines: return 3;
	case QPlatformTheme::ShowShortcutsInContextMenus: return cfg.shortcutsForContextMenus;
	default: return this->QGenericUnixTheme::themeHint(hint);
	}
}

QIcon PlatformTheme::fileIcon(
    const QFileInfo& fileInfo,
    QPlatformTheme::IconOptions iconOptions
) const {
	if ((iconOptions & QPlatformTheme::DontUseCustomDirectoryIcons) && fileInfo.isDir())
		return QIcon::fromTheme(QLatin1String("inode-directory"));

	const QMimeDatabase db;
	const QMimeType type = db.mimeTypeForFile(fileInfo);
	return QIcon::fromTheme(type.iconName());
}

QIconEngine* PlatformTheme::createIconEngine(const QString& iconName) const {
	return new KIconEngine(iconName, KIconLoader::global());
}

void PlatformTheme::applySettings() {
	if (!QGuiApplication::desktopSettingsAware() || this->mIsIgnored) {
		this->mUpdate = true;
		return;
	}

	const auto& cfg = configManager();

	this->mFont = QFont(cfg.font, cfg.fontSize);
	this->mFixedFont = QFont(cfg.fontFixed, cfg.fontFixedSize);
	this->mPalette = Style::loadColorScheme(cfg.colorScheme);

	if (!cfg.colorScheme.isEmpty()) {
		qApp->setProperty("KDE_COLOR_SCHEME_PATH", cfg.colorScheme);
	} else if (this->mUpdate) {
		qApp->setProperty("KDE_COLOR_SCHEME_PATH", QVariant());
	}

	if (this->mUpdate) {
		// NOLINTBEGIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QWindowSystemInterface::handleThemeChange();
#else
		QWindowSystemInterface::handleThemeChange(nullptr);
#endif
		// NOLINTEND
		QCoreApplication::postEvent(qGuiApp, new QEvent(QEvent::ApplicationFontChange));
	}

#ifdef QT_WIDGETS_LIB
	if (hasWidgets() && this->mUpdate) {
#if QT_CONFIG(graphicsview)
		for (auto* scene: std::as_const(QApplicationPrivate::instance()->scene_list))
			QCoreApplication::postEvent(scene, new QEvent(QEvent::ApplicationFontChange));
#endif

		for (QWidget* w: QApplication::allWidgets())
			QCoreApplication::postEvent(w, new QEvent(QEvent::ThemeChange));
	}
#endif

	this->mUpdate = true;
}

#ifdef QT_WIDGETS_LIB
bool PlatformTheme::hasWidgets() { return qobject_cast<QApplication*>(qApp) != nullptr; }
#endif

QString PlatformTheme::loadStyleSheets(const QStringList& paths) {
	QString content;
	for (const QString& path: std::as_const(paths)) {
		if (!QFile::exists(path)) continue;

		QFile file(path);
		if (file.open(QIODevice::ReadOnly)) {
			content.append(QString::fromUtf8(file.readAll()));
			if (!content.endsWith(QChar::LineFeed)) content.append(QChar::LineFeed);
		}
	}
	static const QRegularExpression regExp(QString::fromLatin1("//.*\n"));
	content.replace(regExp, QString::fromLatin1("\n"));
	return content;
}

// There's such a thing as KColorSchemeManager that lets the user to change the
// color scheme application-wide and we should re-apply the color scheme if KCSM
// resets it to the default which leads KColorScheme to get the color scheme
// from kdeglobals which won't help us.
bool PlatformTheme::eventFilter(QObject* obj, QEvent* e) {
	const QString colorScheme = configManager().colorScheme;

	if (obj == qApp && e->type() == QEvent::DynamicPropertyChange
	    && dynamic_cast<QDynamicPropertyChangeEvent*>(e)->propertyName() == "KDE_COLOR_SCHEME_PATH"
	    && qApp->property("KDE_COLOR_SCHEME_PATH").toString().isEmpty())
		this->applySettings();

	return this->QObject::eventFilter(obj, e);
}
