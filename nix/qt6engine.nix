{
  stdenv,
  lib,
  cmake,
  ninja,
  qt6Packages,
  kdePackages,
}:
stdenv.mkDerivation {
  pname = "qtengine-qt6";
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

  cmakeFlags = [
    (lib.cmakeFeature "BUILD_QT5" "OFF")
    (lib.cmakeFeature "BUILD_QT6" "ON")
    (lib.cmakeFeature "QT6_PLUGINDIR" "${placeholder "out"}/${qt6Packages.qtbase.qtPluginPrefix}")
  ];

  dontWrapQtApps = true;
}
