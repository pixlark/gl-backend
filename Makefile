# TODO(pixlark): Statically link the DLL and avoid dependency \
  inclusions in header files so that the user doesn't need to have \
  SDL and GLFW development libraries
# Independent
src=src/utility.cc src/backend.cc src/render.cc
obj=    utility.o      backend.o      render.o
comp_opts=-std=c++11 -Wno-write-strings
link_opts=-o bin/render.dll
dyn_libs=-lSDL2 -lglew32 -lopengl32

# Windows
win_incl_dirs=-I"G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\include" -I"G:\.libraries\GLEW\include"
win_lib_dirs =-L"G:\.minlib\SDL2-2.0.7\x86_64-w64-mingw32\lib" -L"G:\.minlib\glew-2.1.0\lib"

# Filled options
# TODO(pixlark): nix support
win_comp_full=-c $(src) $(comp_opts) $(win_incl_dirs)
win_link_full=-shared $(link_opts) $(obj) $(win_lib_dirs) $(dyn_libs)
win_example_full=-o bin/example -std=c++11 src/example/example.cc $(win_incl_dirs) $(win_lib_dirs) -lSDL2 -lSDL2main -Lbin -lrender -Wno-write-strings

win:
	@echo Building DLL...
	mkdir -p bin
	g++ $(win_comp_full)
	g++ $(win_link_full)
	mv *.o obj
	@echo Building Example...
	g++ $(win_example_full)

wing:
	@echo Building Debug DLL...
	mkdir -p bin
	g++ -g $(win_comp_full)
	g++ -g $(win_link_full)
	mv *.o obj
	@echo Building Debug Example...
	g++ -g $(win_example_full)
