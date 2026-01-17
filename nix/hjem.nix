inputs: {
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
    packages = [
      inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default
    ];

    xdg.config.files."qtengine/config.json".source = configFormat.generate "qtengine-config.json" cfg.config;

    environment.sessionVariables = {
      QT_QPA_PLATFORMTHEME = "qtengine";
      QT_STYLE_OVERRIDE = "qtengine";
    };
  };
}
