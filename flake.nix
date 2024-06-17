# https://github.com/NixOS/nixpkgs/blob/5c56778efdcaa1b8088eb536c3f1e9cc110930dc/lib/systems/examples.nix
{
  description = "Avr Project Template";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };
  outputs = {
    self,
    nixpkgs,
    ...
  }: let
    pkgs = import nixpkgs {inherit system;};
    system = "x86_64-linux";
  in {
    devShells."${system}".default = pkgs.mkShell {
      buildInputs = with pkgs; [
        pkgsCross.avr.buildPackages.gcc
        avrdude
      ];
      shellHook = ''
        #export CPATH=$CPATH:$(awk '{ print $2 }' ${pkgs.pkgsCross.avr.buildPackages.gcc}/nix-support/libc-cflags)
				exec zsh
      '';
    };
  };
}
