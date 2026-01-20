{
  lib,
  configFormat,
}: let
  inherit (lib.options) mkEnableOption mkOption;
  inherit (lib.types) oneOf path str int bool submodule;

  mkFontOption = mkOption {
    type = submodule (_: {
      freeformType = configFormat.type;

      options = {
        family = mkOption {
          type = str;
          default = "";
        };

        size = mkOption {
          type = int;
          default = 10;
        };

        weight = mkOption {
          type = int;
          default = -1;
        };
      };
    });
  };
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
                type = oneOf [path str];
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

              quickStyle = mkOption {
                type = str;
                default = "";

                description = ''
                  The name of a installed theme QtQuick style.
                '';
              };

              fontFixed =
                mkFontOption
                // {
                  description = "Config for fixed/monospace fonts.";
                };

              font =
                mkFontOption
                // {
                  description = "Config for regular fonts.";
                };
            };
          });
        };

        misc = {
          singleClickActivate = mkOption {
            type = bool;
            default = true;
          };

          menusHaveIcons = mkOption {
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
