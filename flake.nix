{
  description = "2D Game Engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/master";
    utils.url = "github:numtide/flake-utils";
    utils.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs =
    { self, nixpkgs, ... }@inputs:
    inputs.utils.lib.eachSystem
      [
        "x86_64-linux"
        "i686-linux"
        "aarch64-linux"
        "x86_64-darwin"
      ]
      (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
          llvm = pkgs.llvmPackages_18;
          lib = nixpkgs.lib;
        in
        {
          devShell = pkgs.mkShell rec {
            name = "2d-game-engine";

            packages = with pkgs; [ ];

            nativeBuildInputs = with pkgs; [
              # Development Tools
              # llvm.lldb
              cmake
              cmakeCurses
              boost
              catch2_3

              gcc

              # clang-tools
              # llvm.clang

              # Development time dependencies
              gtest

              SDL2
              SDL2_image
              SDL2_gfx
              SDL2_ttf
              SDL2_mixer

              glm

              lua5_4_compat
              sol2

              imgui

              socat
            ];

            buildInputs = with pkgs; [
              # stdlib for cpp
              # llvm.libcxx
            ];

            CXXFLAGS = "-Wall -Wfatal-errors -std=c++23";
            LDFLAGS = "-lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -lSDL2_mixer -llua -lm";
            # CPATH = builtins.concatStringsSep ":" [
            # (lib.makeSearchPathOutput "dev" "include" [ llvm.libcxx ])
            # (lib.makeSearchPath "resource-root/include" [ llvm.clang ])
            # ];

            # Setting up the environment variables you need during
            # development.
            shellHook =
              let
                icon = "f121";
              in
              ''
                export PS1="$(echo -e '\u${icon}') {\[$(tput sgr0)\]\[\033[38;5;228m\]\w\[$(tput sgr0)\]\[\033[38;5;15m\]} (${name}) \\$ \[$(tput sgr0)\]"
                export SHELL="${pkgs.zsh}/bin/zsh"
              '';
          };
        }
      );
}
