{
  lib,
  configFormat,
}: let
  inherit (lib.options) mkEnableOption mkOption;
  inherit (lib.types) str int bool path submodule;
in {
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
}
