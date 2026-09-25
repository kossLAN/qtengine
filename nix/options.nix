{
  lib,
  configFormat,
}: let
  inherit (lib.options) mkEnableOption mkOption;
  inherit (lib.types) oneOf nullOr path str int bool submodule;

  mkFontOption = mkOption {
    type = submodule (_: {
      freeformType = configFormat.type;

      options = {
        family = mkOption {
          type = nullOr str;
          default = null;
        };

        size = mkOption {
          type = int;
          default = 11;
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

          cursorFlashTime = mkOption {
            type = int;
            default = 1000;
          };

          mouseDoubleClickInterval = mkOption {
            type = int;
            default = 400;
          };

          toolButtonStyle = mkOption {
            type = int;
            default = 4;
          };

          dialogButtonBoxLayout = mkOption {
            type = int;
            default = 0;
          };

          keyboardScheme = mkOption {
            type = int;
            default = 2;
          };

          uiEffects = mkOption {
            type = int;
            default = 0;
          };

          wheelScrollLines = mkOption {
            type = int;
            default = 3;
          };
        };
      };
    });
  };
}
