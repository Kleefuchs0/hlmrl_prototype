{
  inputs = {
    nixpkgs = {
      url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    };
    flake-utils = {
      url = "github:numtide/flake-utils";
    };
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      {
        devShell = pkgs.mkShell.override {
          stdenv = pkgs.gcc16Stdenv;
          }
          {
          buildInputs = with pkgs; [
            pkg-config
            wayland
            wayland-scanner
            libffi
            libGL
            gcc
            clang-tools
            clang
            gnumake
            cmake
            valgrind
            bear
            libxkbcommon

            fmt
            catch2_3
            raygui
            raylib
            entt
            nlohmann_json
          ];
          env = {

            # Set env variables so we can refer to them from the vim configuration
            CODELLDB_PATH = "${pkgs.vscode-extensions.vadimcn.vscode-lldb}/share/vscode/extensions/vadimcn.vscode-lldb/adapter/codelldb";
            LIBLLDB_PATH = "${pkgs.vscode-extensions.vadimcn.vscode-lldb}/share/vscode/extensions/vadimcn.vscode-lldb/lldb/lib/liblldb.so";
          };
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (
            with pkgs;
            [
              wayland
              glfw
              libxkbcommon
              # any other libraries that need to be dynamically linked to
            ]
          );
        };
      }
    );
}
