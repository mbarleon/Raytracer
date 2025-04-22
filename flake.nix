{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      with pkgs; {
        devShells.default = mkShell {
          buildInputs = [
            pkg-config
            criterion
            gnumake
            cmake
            gcc
          ];

          shellHook = ''
            export PKG_CONFIG_PATH=${pkgs.lib.makeLibraryPath [criterion]}:$PKG_CONFIG_PATH
            export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [criterion]}:$LD_LIBRARY_PATH
          '';
        };
      });
}
