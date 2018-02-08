## GL Backend

This is a basic game framework I'm writing for use in my own games. It
interacts directly with OpenGL using GLEW and renders user-defined
Surfaces from a texture atlas.

### Building

SDL, GLEW, and stb_image are the dependencies. The Makefile takes care
of building on Windows for MinGW. Anywhere else and you have to figure
it out for yourself.

The Makefile also builds the example program.

### Use

For an example program, see `src\example\example.cc`

First, include the headers `utility.h` and `renderer.h`. These are
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

Next, you need to create a Renderer object and pass it a path to your
texture atlas in PNG format.

```
Render::Renderer renderer;
renderer.init(window, "atlas.png", Vector2i(1920, 1080), 1);
```

Now OpenGL is initialized and will render to your window. But to
render something, you first need to register a Surface. You initialize
a Surface by passing it the renderer it will be registered to, the
position on the screen, the position of the texture in the atlas, and
the size of the texture in the atlas.

```
Render::Surface surface;
surface.init(&renderer, Vector2i(0, 0), Vector2i(0, 0), Vector2i(32, 32));
```

Finally, in your main loop, you render by calling these four
functions:

```
renderer.clear(RGBA(0, 0, 0, 255)); // To clear the screen to black
renderer.render();                  // To render all registered surfaces
renderer.swap(window);              // To swap the render buffers
renderer.end_frame();               // To notify the renderer that the frame has ended
```

That's all there is to it. If you're curious to see a basic example,
again there is one at `src/example/example.cc`.
