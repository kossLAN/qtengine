{
  stdenv,
  lib,
  cmake,
  ninja,
  qtbase,
  libsForQt5,
}:
stdenv.mkDerivation {
  pname = "qtengine-qt5";
  version = "0.1.0";
  src = ../.;

  nativeBuildInputs = [
    cmake
    ninja
  ];

  buildInputs = [
    qtbase
    libsForQt5.kconfig
    libsForQt5.kconfigwidgets
    libsForQt5.kiconthemes
  ];

  cmakeFlags = [
    (lib.cmakeFeature "BUILD_QT5" "ON")
    (lib.cmakeFeature "BUILD_QT6" "OFF")
    (lib.cmakeFeature "QT5_PLUGINDIR" "${placeholder "out"}/${qtbase.qtPluginPrefix}")
  ];

  dontWrapQtApps = true;
}
