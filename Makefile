# TODO(pixlark): Statically link the DLL and avoid dependency \
  inclusions in header files so that the user doesn't need to have \
  SDL and GLFW development libraries
# Independent
src=src/utility.cc src/backend.cc src/render.cc
obj=    utility.o      backend.o      render.o

comp_opts=-std=c++11 -Wno-write-strings
nix_comp_opts=-fPIC

win_link_opts=-o bin/render.dll
nix_link_opts=-o bin/librender.so

dyn_libs=-lSDL2
win_dyn_libs=-lglew32 -lopengl32
nix_dyn_libs=-lGLEW -lGL

# Windows
win_incl_dirs=-I"G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\include" -I"G:\.libraries\GLEW\include"
win_lib_dirs =-L"G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\lib" -L"G:\.minlib\glew-2.1.0\lib"

# Filled options
win_comp_full=-c $(src) $(comp_opts) $(win_incl_dirs)
win_link_full=-shared $(win_link_opts) $(obj) $(win_lib_dirs) $(dyn_libs) $(win_dyn_libs)
win_example_full =-o bin/example  -std=c++11 src/example/example.cc  $(win_incl_dirs) $(win_lib_dirs) -lSDL2 -lSDL2main -Lbin -lrender -Wno-write-strings
win_pallette_full=-o bin/pallette -std=c++11 src/example/pallette.cc $(win_incl_dirs) $(win_lib_dirs) -lSDL2 -lSDL2main -Lbin -lrender -Wno-write-strings

nix_comp_full=-c $(src) $(comp_opts) $(nix_comp_opts)
nix_link_full=-shared $(nix_link_opts) $(obj) $(dyn_libs) $(nix_dyn_libs)
nix_example_full =-o bin/example  -std=c++11 src/example/example.cc  -lSDL2 -lSDL2main -Lbin -lrender -Wno-write-strings
nix_pallette_full=-o bin/pallette -std=c++11 src/example/pallette.cc -lSDL2 -lSDL2main -Lbin -lrender -Wno-write-strings

win:
	@echo Building DLL...
	mkdir -p bin
	g++ $(win_comp_full)
	g++ $(win_link_full)
	mkdir -p obj
	mv *.o obj
	@echo Building Examples...
	g++ $(win_example_full)
	g++ $(win_pallette_full)

wing:
	@echo Building Debug DLL...
	mkdir -p bin
	g++ -g $(win_comp_full)
	g++ -g $(win_link_full)
	mkdir -p obj
	mv *.o obj
	@echo Building Debug Examples...
	g++ -g $(win_example_full)
	g++ $(win_pallette_full)

nix:
	@echo Building Library...
	mkdir -p bin
	g++ $(nix_comp_full)
	g++ $(nix_link_full)
	mkdir -p obj
	mv *.o obj
	@echo Building Examples..
	g++ $(nix_example_full)
	g++ $(nix_pallette_full)

nixg:
	@echo Building Library...
	mkdir -p bin
	g++ -g $(nix_comp_full)
	g++ -g $(nix_link_full)
	mkdir -p obj
	mv *.o obj
	@echo Building Examples..
	g++ -g $(nix_example_full)
	g++ -g $(nix_pallette_full)
