#pragma once

#include <optional>

#include <private/qgenericunixthemes_p.h>
#include <qfileinfo.h>
#include <qfont.h>
#include <qicon.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qpa/qplatformtheme.h>
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

protected:
	bool eventFilter(QObject* obj, QEvent* e) override;

private slots:
	void applySettings();
	void onConfigChanged();

private:
	static QStringList iconPaths();
	QString mStyleName;
	QFont mFixedFont;
	QFont mFont;
	std::optional<QPalette> mPalette;
	bool mUpdate = false;
};
