{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
  };

  outputs = inputs @ {
    self,
    nixpkgs,
  }: let
    forEachSystem = fn:
      nixpkgs.lib.genAttrs
      nixpkgs.lib.platforms.linux
      (system: fn system (nixpkgs.legacyPackages.${system}));
  in {
    nixosModules.default = import ./nix/module.nix inputs;

    hjemModules.defaule = import ./nix/hjem.nix inputs;

    packages = forEachSystem (system: pkgs: rec {
      default = pkgs.callPackage ./nix/default.nix {};
      experimental = pkgs.callPackage ./nix/experimental.nix {};

      qt5engine = pkgs.libsForQt5.callPackage ./nix/qt5engine.nix {};
      qt6engine = pkgs.callPackage ./nix/qt6engine.nix {};
    });

    devShells = forEachSystem (system: pkgs:  {
      default = import ./shell.nix {
        inherit pkgs;

        qt6engine = self.packages.${system}.qt6engine.override {
          stdenv = pkgs.clangStdenv;
        };
      };
    });
  };
}
