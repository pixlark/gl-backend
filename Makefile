# Independent
src=src/main.cc src/utility.cc src/backend.cc
other_opts=-std=c++11 -o bin/backend -Wno-write-strings
dyn_libs=-lSDL2 -lSDL2main -lglew32 -lopengl32

# Windows
win_incl_dirs=-I"G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\include" -I"G:\.libraries\GLEW\include"
#win_static_libs="G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\lib\libSDL2.a" \
#                "G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\lib\libSDL2main.a"
win_other_opts=-L"G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\lib" -L"G:\.minlib\glew-2.1.0\lib"

# Unix
nix_incl_dirs=
nix_other_opts=

# Filled options
win_full=$(src) $(win_static_libs) $(win_incl_dirs) \
         $(win_other_opts) $(dyn_libs) $(other_opts)
nix_full=$(src) $(nix_static_libs) $(nix_incl_dirs) \
         $(nix_other_opts) $(dyn_libs) $(other_opts)

win:
	g++ -O2 $(win_full)

wing:
	g++ -g $(win_full)

nix:
	g++ -O2 $(nix_full)

nixg:
	g++ -g $(nix_full)
