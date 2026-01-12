inputs: {
  self,
  lib,
  config,
  pkgs,
  ...
}: let
  inherit (lib.modules) mkIf;
  inherit (lib.options) mkEnableOption mkOption;
  inherit (lib.types) str int bool path submodule;

  configFormat = pkgs.formats.json {};
  cfg = config.programs.qt6engine;
in {
  options.programs.qt6engine = {
    enable = mkEnableOption "Enable qt6engine.";

    config = mkOption {
      default = {};
      description = ''
        Config options for qt6engine, see README for more information.
      '';

      type = submodule (_: {
        freeformType = configFormat.type;

        options = {
          theme = mkOption {
            type = submodule (_: {
              freeformType = configFormat.type;

              options = {
                colorScheme = mkOption {
                  type = path;
                  default = "";
                  description = ''
                    A path to a compatible KDE color scheme file.
                  '';
                };

                iconTheme = mkOption {
                  type = str;
                  default = "";

                  description = ''
                    The name of a installed icon theme.
                  '';
                };

                style = mkOption {
                  type = str;
                  default = "";

                  description = ''
                    The name of a installed theme style.
                  '';
                };

                fontFixed = mkOption {
                  type = str;
                  default = "";
                };

                fontFixedSize = mkOption {
                  type = int;
                  default = 11;
                };

                font = mkOption {
                  type = str;
                  default = "";
                };

                fontSize = mkOption {
                  type = int;
                  default = 11;
                };
              };
            });
          };

          misc = {
            singleClickActivate = mkOption {
              type = bool;
              default = true;
            };

            menuHaveIcons = mkOption {
              type = bool;
              default = true;
            };

            shortcutsForContextMenus = mkOption {
              type = bool;
              default = true;
            };
          };
        };
      });
    };
  };

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
