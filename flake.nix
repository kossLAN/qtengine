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

    hjemModules.default = import ./nix/hjem.nix inputs;

    packages = forEachSystem (system: pkgs: rec {
      default = qtengine;
      qtengine = pkgs.callPackage ./nix/default.nix {};
    });

    devShells = forEachSystem (system: pkgs: {
      default = import ./shell.nix {
        inherit pkgs;

        qtengine = self.packages.${system}.qtengine.override {
          stdenv = pkgs.clangStdenv;
        };
      };
    });
  };
}
