#pragma once

#include <qpa/qplatformtheme.h>
#include <qtversionchecks.h>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 10, 0))
#include <private/qgenericunixtheme_p.h>
#else
#include <private/qgenericunixthemes_p.h>
#endif
#include <memory>
#include <optional>

#include <qfileinfo.h>
#include <qfont.h>
#include <qicon.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qpalette.h>

Q_DECLARE_LOGGING_CATEGORY(logPlatformTheme);

class PlatformTheme
    : public QObject
    , public QGenericUnixTheme {
	Q_OBJECT
public:
	PlatformTheme();

	~PlatformTheme() override = default;

	Q_DISABLE_COPY_MOVE(PlatformTheme)

	[[nodiscard]] const QPalette* palette(Palette type = SystemPalette) const override;
	[[nodiscard]] const QFont* font(Font type = SystemFont) const override;
	[[nodiscard]] QVariant themeHint(ThemeHint hint) const override;
	[[nodiscard]] QIcon
	fileIcon(const QFileInfo& fileInfo, QPlatformTheme::IconOptions iconOptions = {}) const override;

	[[nodiscard]] QIconEngine* createIconEngine(const QString& iconName) const override;

	[[nodiscard]] bool usePlatformNativeDialog(DialogType type) const override;
	[[nodiscard]] QPlatformDialogHelper* createPlatformDialogHelper(DialogType type) const override;

protected:
	bool eventFilter(QObject* obj, QEvent* e) override;

private slots:
	void applySettings();
	void onConfigChanged();

private:
	static QStringList iconPaths();
	[[nodiscard]] QPlatformTheme* fileDialogTheme() const;
	QString mStyleName;
	QString mIconThemeName;
	QFont mFixedFont;
	QFont mFont;
	std::optional<QPalette> mPalette;
	bool mUpdate = false;

	// Theme that file dialogs are delegated to, loaded lazily on first use.
	mutable std::unique_ptr<QPlatformTheme> mFileDialogTheme;
	mutable bool mFileDialogThemeLoaded = false;
};
