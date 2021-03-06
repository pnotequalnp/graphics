{
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system};
          stdenv = pkgs.llvmPackages_12.stdenv;
      in rec {
        packages.graphics = stdenv.mkDerivation {
          name = "graphics";
          src = ./.;
          buildInputs = with pkgs; with xorg; [ glfw libX11 libXi libXrandr ];
          installPhase = ''
            mkdir -p $out
            cp -r bin $out/bin
          '';
        };

        defaultPackage = packages.graphics;
      });
}
