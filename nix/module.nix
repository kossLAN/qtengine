inputs: {
  self,
  lib,
  config,
  pkgs,
  ...
}: let
  inherit (lib.modules) mkIf;

  configFormat = pkgs.formats.json {};
  cfg = config.programs.qt6engine;
in {
  options.programs.qt6engine = import ./options.nix { inherit lib configFormat; };

  config = mkIf cfg.enable {
    qt.enable = true;

    environment = {
      etc."xdg/qt6engine/config.json".source = configFormat.generate "config.json" cfg.config;

      systemPackages = [
        inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default
      ];

      variables = {
        QT_QPA_PLATFORMTHEME = "qt6engine";
        QT_STYLE_OVERRIDE = "qt6engine";
        QT6ENGINE_CONFIG = "/etc/xdg/qt6engine/config.json";
      };
    };
  };
}
