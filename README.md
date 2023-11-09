## Software Renderer
This is a small demo for learning `GAMES101`, `GAMES202` and `OpenGL`. All rendering operations are completed on the CPU using C++, so it's cross-platform.

`glm` is used as the math library, and OpenGL is used to show rendered results.

I will improve it if I have spare time:)

### Usage

```bash
git clone https://github.com/andrewg42/SoftRenderer
cd SoftRenderer
cmake -Bbuild
cmake --build build -j8
build/main
```

### Features

Basic

- [x] ImGui panel
- [x] arcball camera
- [x] image reader
- [x] model reader
- [x] reversed-z

Rendering

- [ ] cube map

- Lighting Model
  - [ ] Blinn-Phong
  - [ ] PBR
- Anti-Aliasing
  - [ ] MSAA
  - [ ] FXAA
  - [ ] TAA

- Shadow
  - [ ] shadow mapping

  - [ ] PCSS

- Ambient Occlusion
  - [ ] SSAO


### Third Party Packages

[glfw](https://github.com/glfw/glfw) and [glad](https://github.com/Dav1dde/glad): for showing the result dynamically.

[glm](https://github.com/g-truc/glm): for basic math operation.

[imgui](https://github.com/ocornut/imgui): for configure panel.

[stb](https://github.com/nothings/stb): for reading textures.

[OBJ-Loader](https://github.com/Bly7/OBJ-Loader): for reading .obj file.

### References

[LearnOpenGL](https://learnopengl.com/)

[现代 OpenGL 保姆级教程](https://github.com/parallel101/opengltutor#现代-opengl-保姆级教程)

[Vulkan Tutorial](https://vulkan-tutorial.com/)

[DX11 - Graphics pipeline](https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-graphics-pipeline)

[filament](https://github.com/google/filament)
