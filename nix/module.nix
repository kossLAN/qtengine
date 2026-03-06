inputs: {
  self,
  lib,
  config,
  pkgs,
  ...
}: let
  inherit (lib.modules) mkIf;

  configFormat = pkgs.formats.json {};
  cfg = config.programs.qtengine;
in {
  options.programs.qtengine = import ./options.nix {inherit lib configFormat;};

  config = mkIf cfg.enable {
    qt.enable = true;

    environment = {
      variables.QT_QPA_PLATFORMTHEME = "qtengine";

      systemPackages = [
        inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default
        inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default.qt5
      ];

      etc."xdg/qtengine/config.json".source =
        mkIf (cfg.config != {})
        (configFormat.generate "qtengine-config.json" cfg.config);
    };
  };
}
