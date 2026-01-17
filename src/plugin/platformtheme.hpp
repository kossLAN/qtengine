#pragma once

#include <qpa/qplatformtheme.h>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 10, 0))
#include <private/qgenericunixtheme_p.h>
#else
#include <private/qgenericunixthemes_p.h>
#endif
#include <memory>
#include <optional>

#include <QFileInfo>
#include <QFont>
#include <QIcon>
#include <QLoggingCategory>
#include <QObject>
#include <QPalette>

Q_DECLARE_LOGGING_CATEGORY(logPlatformTheme);

class PlatformTheme
    : public QObject
    , public QGenericUnixTheme {
	Q_OBJECT
public:
	PlatformTheme();

	~PlatformTheme() override = default;

	Q_DISABLE_COPY_MOVE(PlatformTheme)

	[[nodiscard]] bool usePlatformNativeDialog(DialogType type) const override;
	[[nodiscard]] QPlatformDialogHelper* createPlatformDialogHelper(DialogType type) const override;
	[[nodiscard]] const QPalette* palette(Palette type = SystemPalette) const override;
	[[nodiscard]] const QFont* font(Font type = SystemFont) const override;
	[[nodiscard]] QVariant themeHint(ThemeHint hint) const override;
	[[nodiscard]] QIcon
	fileIcon(const QFileInfo& fileInfo, QPlatformTheme::IconOptions iconOptions = {}) const override;

	[[nodiscard]] QIconEngine* createIconEngine(const QString& iconName) const override;

protected:
	bool eventFilter(QObject* obj, QEvent* e) override;

private slots:
	void applySettings();

private:
#ifdef QT_WIDGETS_LIB
	static bool hasWidgets();
#endif
	static QStringList iconPaths();
	static QString loadStyleSheets(const QStringList& paths);
	QString mPrevStyleSheet;
	QFont mFixedFont;
	QFont mFont;
	std::optional<QPalette> mPalette;
	bool mUpdate = false;
	bool mIsIgnored = false;
	std::unique_ptr<QPlatformTheme> mTheme;
};
