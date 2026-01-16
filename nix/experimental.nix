{
  stdenv,
  lib,
  cmake,
  ninja,
  qt6Packages,
  kdePackages,
  libsForQt5,
}:
stdenv.mkDerivation {
  pname = "qtengine";
  version = "0.1.0";
  src = ../.;

  nativeBuildInputs = [
    cmake
    ninja
  ];

  buildInputs = [
    qt6Packages.qtbase
    kdePackages.kconfig
    kdePackages.kcolorscheme
    kdePackages.kiconthemes
  ];

  # Can't have qt5 dependencies in buildInputs with qt6 dependencies, this is how most qt5/qt6 support 
  # packages are packaged in nixkgs...
  cmakeFlags = [
    (lib.cmakeFeature "QT5_PLUGINDIR" "${placeholder "out"}/${libsForQt5.qtbase.qtPluginPrefix}")
    (lib.cmakeFeature "QT6_PLUGINDIR" "${placeholder "out"}/${qt6Packages.qtbase.qtPluginPrefix}")

    "-DQt5_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5"
    "-DQt5Core_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5Core"
    "-DQt5DBus_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5DBus"
    "-DQt5Gui_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5Gui"
    "-DQt5Widgets_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5Widgets"
    "-DQt5Xml_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5Xml"
    "-DQt5ThemeSupport_DIR=${libsForQt5.qtbase.dev}/lib/cmake/Qt5ThemeSupport"
    "-DQt5ThemeSupport_LIBRARY=${libsForQt5.qtbase.out}/lib/libQt5ThemeSupport.a"
    "-DQt5ThemeSupport_INCLUDE_DIR=${libsForQt5.qtbase.dev}/include/QtThemeSupport/${libsForQt5.qtbase.version}"

    "-DKF5Auth_DIR=${libsForQt5.kauth.dev}/lib/cmake/KF5Auth"
    "-DKF5Codecs_DIR=${libsForQt5.kcodecs.dev}/lib/cmake/KF5Codecs"
    "-DKF5Config_DIR=${libsForQt5.kconfig.dev}/lib/cmake/KF5Config"
    "-DKF5ConfigWidgets_DIR=${libsForQt5.kconfigwidgets.dev}/lib/cmake/KF5ConfigWidgets"
    "-DKF5CoreAddons_DIR=${libsForQt5.kcoreaddons.dev}/lib/cmake/KF5CoreAddons"
    "-DKF5GuiAddons_DIR=${libsForQt5.kguiaddons.dev}/lib/cmake/KF5GuiAddons"
    "-DKF5IconThemes_DIR=${libsForQt5.kiconthemes.dev}/lib/cmake/KF5IconThemes"
    "-DKF5WidgetsAddons_DIR=${libsForQt5.kwidgetsaddons.dev}/lib/cmake/KF5WidgetsAddons"
  ];

  dontWrapQtApps = true;
}
