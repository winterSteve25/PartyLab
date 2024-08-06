{
  	inputs = {
    	nixpkgs.url = "nixpkgs";
    	flake-utils.url = "github:numtide/flake-utils";
  	};
	outputs = { self, nixpkgs, flake-utils }:
    	flake-utils.lib.eachDefaultSystem (system:
      		let
        		pkgs = nixpkgs.legacyPackages.${system};
        		libraries = with pkgs;[
				    glfw
				    libGL
                    wayland
                    xorg.libXrandr
                    xorg.libXinerama
                    xorg.libXcursor
                    xorg.libXi
                    xorg.libX11
                    xorg.libX11.dev
					luajit
					lua
				];
				packages = with pkgs; [
				] ++ libraries;
      		in
    		{ devShell = pkgs.mkShell {
			  	buildInputs = packages;
			  	shellHook = ''
				  	export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath libraries}:$LD_LIBRARY_PATH:$NIX_LD_LIBRARY_PATH
				'';
        	};});
}
