{
  description = "GLFW dev environment (NixOS + Conan + full X11/OpenGL stack + uuid)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          # Tooling
          pkg-config
          cmake
          ninja
          gcc

          # OpenGL
          mesa
          libGL

          # Core X11 libraries
          libX11
          libXext
          libXrender
          libXrandr
          libXi
          libXcursor
          libXinerama
          libXxf86vm

          # X11 extensions
          libXcomposite
          libXdamage
          libXfixes
          libXinerama

          # XVideo extension
          libXv

          # XCB + utilities
          libxcb
          xcbutil
          xcbutilimage
          xcbutilwm
          xcbutilkeysyms
          xcbutilrenderutil
          xcbutilcursor # Provides xcb-cursor.pc

          # Session + auth
          libICE
          libSM
          libXau
          libXdmcp

          # Fonts + widgets
          libfontenc
          libXaw
          libXt
          libXmu
          libXpm

          # Keyboard
          libxkbfile

          # Resize extension
          libXres

          # Screen saver / DPMS
          libXScrnSaver

          # Testing / input simulation
          libXtst

          # Protocols
          xorgproto

          # UUID
          util-linux

          # Optional but useful
          wayland
          wayland-protocols

          clang-tools
        ];

        shellHook = ''
          export PKG_CONFIG_PATH=$PKG_CONFIG_PATH
          echo "GLFW dev shell ready (full X11/OpenGL + xcb-cursor + XVideo + XTest + XScreenSaver + XKB + XRes + uuid included)"
        '';
      };
    };
}
