{
  pkgs ? import <nixpkgs> {},
  stdenv ? pkgs.clangStdenv,
  # qtengine ? pkgs.callPackage ./nix/qt6engine.nix {inherit stdenv;},
  qtengine ? pkgs.callPackage ./nix/experimental.nix {inherit stdenv;},
  ...
}: let
  tidyfox = import (pkgs.fetchFromGitea {
    domain = "git.outfoxxed.me";
    owner = "outfoxxed";
    repo = "tidyfox";
    rev = "9d85d7e7dea2602aa74ec3168955fee69967e92f";
    hash = "sha256-77ERiweF6lumonp2c/124rAoVG6/o9J+Aajhttwtu0w=";
  }) {inherit pkgs;};
in
  pkgs.mkShell.override {inherit (qtengine) stdenv;} {
    inputsFrom = [qtengine];

    buildInputs = with pkgs; [
      kdePackages.kconfig
      kdePackages.kcolorscheme
      kdePackages.kiconthemes

      # libsForQt5.kconfig
      # libsForQt5.kconfigwidgets
      # libsForQt5.kiconthemes
    ];

    nativeBuildInputs = with pkgs; [
      just
      clang-tools
      parallel
      makeWrapper
      gdb
    ];

    TIDYFOX = "${tidyfox}/lib/libtidyfox.so";
    QT_QPA_PLATFORMTHEME = "qtengine";
    QT_STYLE_OVERRIDE = "qtengine";
    QT_DEBUG_PLUGINS = "1";
    QT_LOGGING_RULES = "qt.qpa.*=true;qtengine.*=true";

    CMAKE_PREFIX_PATH = builtins.concatStringsSep ":" (with pkgs; [
      libsForQt5.qtbase.dev
      libsForQt5.kconfig.dev
      libsForQt5.kconfigwidgets.dev
      libsForQt5.kiconthemes.dev

      libsForQt5.kauth.dev
      libsForQt5.kcoreaddons.dev
      libsForQt5.kwidgetsaddons.dev
      libsForQt5.kcodecs.dev
    ]);

    shellHook = ''
      export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
      export LD_LIBRARY_PATH=$PWD/debug/lib
      export QT_PLUGIN_PATH=$PWD/debug/lib/qt-5/plugins

      # Add Qt-related environment variables.
      # https://discourse.nixos.org/t/qt-development-environment-on-a-flake-system/23707/5
      setQtEnvironment=$(mktemp)
      random=$(openssl rand -base64 20 | sed "s/[^a-zA-Z0-9]//g")
      makeShellWrapper "$(type -p sh)" "$setQtEnvironment" "''${qtWrapperArgs[@]}" --argv0 "$random"
      sed "/$random/d" -i "$setQtEnvironment"
      source "$setQtEnvironment"

      # qmlls does not account for the import path and bases its search off qtbase's path.
      # The actual imports come from qtdeclarative. This directs qmlls to the correct imports.
      export QMLLS_BUILD_DIRS=$(pwd)/build:$QML2_IMPORT_PATH
    '';
  }
