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
    # Does HJEM not have a way to set this home profile???
    environment.sessionVariables.QT_QPA_PLATFORMTHEME = "qtengine";

    packages = [
      inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default
      inputs.self.packages.${pkgs.stdenv.hostPlatform.system}.default.qt5
    ];

    xdg.config.files."qtengine/config.json".source =
      mkIf (cfg.config != {})
      (configFormat.generate "qtengine-config.json" cfg.config);
  };
}
