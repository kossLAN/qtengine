inputs: {
  lib,
  config,
  pkgs,
  ...
}: let
  inherit (lib.modules) mkIf;

  configFormat = pkgs.formats.json {};
  cfg = config.programs.qt6engine;
in {
  options.programs.qt6engine = import ./options.nix {inherit lib configFormat;};

  config = mkIf cfg.enable {
    packages = [
      inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default
    ];

    xdg.config.files."qt6engine/config.json".source = configFormat.generate "qt6engine-config.json" cfg.config;
    environment.sessionVariables = {
      QT_QPA_PLATFORMTHEME = "qt6engine";
      QT_STYLE_OVERRIDE = "qt6engine";
      QT6ENGINE_CONFIG = "${config.xdg.config.directory}/qt6engine/config.json";
    };
  };
}
