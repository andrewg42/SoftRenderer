## Software Renderer
This is a small demo for learning `GAMES101`, `GAMES202` and `OpenGL`. All rendering operations except display are completed on the CPU using C++, so it's cross-platform. `glm` is used as the math library, and OpenGL is used to show rendered results.

I will improve it if I have spare time:)

### Usage

```bash
git clone https://github.com/andrewg42/SoftRenderer --depth=1
cd SoftRenderer
cmake -Bbuild
cmake --build build
build/main
```

### Features

Basic

- [ ] imgui panel
- [ ] obj reader
- [x] image reader

Rendering

- [ ] Blinn-Phong
- [ ] PCSS

### Third Party Packages

[glfw](https://github.com/glfw/glfw) and [glad](https://github.com/Dav1dde/glad): for showing the result dynamically.

[glm-0.9.9.8](https://github.com/g-truc/glm): for basic math operation.

[stb](https://github.com/nothings/stb): for reading textures.

### Credits

TODO
model matrix
ortho projection: reversed-z
