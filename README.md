## GL Backend

This is a basic game framework I'm writing for use in my own games. It
interacts directly with OpenGL using GLEW and renders by very simply
reconstructing the required vertices every frame.

### Building

SDL, GLEW, and stb_image are the dependencies. The Makefile takes care
of building on Windows for MinGW. Anywhere else and you have to figure
it out for yourself.

The Makefile also builds the example program.

### Use

For an example program, see `src\example\example.cc`

First, include the headers `utility.h` and `render.h`. These are
both required.

Currently, the framework is dependent on SDL for everything but
rendering. This will change as the scope of the framework is
expanded. So you need to initialize SDL as usual, making sure to
specify that it is in OpenGL mode.

```
SDL_Init(SDL_INIT_VIDEO);
SDL_Window * window = SDL_CreateWindow("This is how you initialize.",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	1920, 1080, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
```

Next, you need to initialize the Render system by passing it your
SDL_Window, a path to your texture atlas, a Vector2i containing your
resolution, and a float representing the resolution scale.

```
Render::init(window, "atlas.png", Vector2i(1920, 1080), 1);
```

Then in your main loop, you need to first clear your screen:

```
Render::clear(RGBA(0, 0, 0, 255));
```

Then, render any textures you want to appear on the screen. You pass
in the screen position, the texture position in the atlas, the size of
the texture, and the scaling that you want applied to the texture when
it is rendered.

```
Render::render(
	Vector2i(30, 30),
	Vector2i(0, 0), 
	Vector2i(32, 32), 
	Vector2f(1, 1));
```

Finally, this will upload your new vertices and texture coordinates to
the GPU and swap the buffers.

```
Render::swap(window);
```
