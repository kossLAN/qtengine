# qtengine 

A Qt6/Qt5 platform theme using the KDE Color Scheme format, meant to be easily configurable.

## What problem does this solve?
- An easy config in JSON
- Works with Qt5 & Qt6 on the same config
- Works with KDE apps
- Doesn't freeze the Plasma XDG Desktop Portal

## Nix Flake Installation

First add the input to your flake.

```nix
qtengine = {
  url = "github:kossLAN/qtengine";
  inputs.nixpkgs.follows = "nixpkgs";
};
```

Then use the provided NixOS module.

```nix
imports = [inputs.qtengine.nixosModules.default];

# Add your desired theme to your system packages
environment.systemPackages = with pkgs.kdePackages; [
  breeze
  breeze.qt5 # Needed if you want Qt5 support.
  breeze-icons
];

programs.qtengine = {
  enable = true;
  
  config = {
    theme = {
      colorScheme = "${pkgs.kdePackages.breeze}/share/color-schemes/BreezeDark.colors";
      iconTheme = "breeze-dark";
      style = "breeze";

      font = {
        family = "<font name>";
        size = 11;
        weight = -1;
      };

      fontFixed = {
        family = "<font name>";
        size = 11;
        weight = -1;
      };
    };

    misc = {
      singleClickActivate = false;
      menusHaveIcons = true;
      shortcutsForContextMenus = true;
    };
  };
};
```

> [!NOTE]
>
> A restart of your DE/WM may be required for changes to take effect.

## Arch Linux AUR Installation

First install the [AUR package](https://aur.archlinux.org/packages/qtengine).

```sh
yay -S qtengine
# yay -S qtengine-git
```

Then copy the example config from `/etc/qtengine/config.example.json` to `~/.config/qtengine/config.json`. Additionally, it can be sourced from a folder in your `XDG_CONFIG_DIRS`, or by using the `QTENGINE_CONFIG` environment variable.

Finally set the environment variable `QT_QPA_PLATFORMTHEME` to `qtengine`.

## Installation on other distributions

First you'll need the following Qt6 dependencies

```
qtbase kconfig kcolorscheme kiconthemes
```

If you want Qt5 support you'll also need 

```
qtbase kconfig kconfigwidgets kiconthemes
```

Then build the project

```sh
cmake -DCMAKE_BUILD_TYPE:STRING=Release -B build

# OPTIONAL: if you don't want to build with Qt5 support
cmake -DCMAKE_BUILD_TYPE:STRING=Release -DBUILD_QT5=OFF -B build

cmake --build build
cmake --install build

# OPTIONAL: or you can just use the Justfile if you have just
just build
just install
```

Then write a config at `~/.config/qtengine/config.json`, additionally it can be sourced from a folder in your `XDG_CONFIG_DIRS`, or by using the `QTENGINE_CONFIG` environment variable.

```json
{
  "theme": {
    "colorScheme": "<path to KColorScheme file>",
    "iconTheme": "breeze-dark",
    "style": "breeze",
    "font": {
      "family": "<font name>",
      "size": 11,
      "weight": -1
    },
    "fontFixed": {
      "family": "<font name>",
      "size": 11,
      "weight": -1
    }
  },
  "misc": {
    "menusHaveIcons": true,
    "singleClickActivate": false,
    "shortcutsForContextMenus": true
  }
}
```

Finally set the environment variable `QT_QPA_PLATFORMTHEME` to `qtengine`.

> [!NOTE]
>
> You will also need a Qt5 AND Qt6 build of your theme for things to work!

## Credits

Thank you [hyprqt6engine](https://github.com/hyprwm/hyprqt6engine) & [qt6ct](https://www.opencode.net/trialuser/qt6ct) for being the foundation of this project. Also a thanks to [outfoxxed](https://github.com/outfoxxed) because I "borrowed" a lot of his C++ build tools that made working in C++ a lot less painful, especially on NixOS.
