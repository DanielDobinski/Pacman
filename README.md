# Dear ImGui with GLFW + OpenGL3

[**Dear ImGui**](https://github.com/ocornut/imgui) is a bloat-free graphical user interface library for C++. It outputs optimized vertex buffers that you can render anytime in your 3D-pipeline-enabled application. It is fast, portable, renderer agnostic, and self-contained (no external dependencies).

*This sample uses Dear ImGui with GLFW + OpenGL3*

[**GLFW**](https://www.glfw.org/) (Graphics Library Framework) is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop. It provides a simple API for creating windows, contexts and surfaces, receiving input and events.

[**OpenGL**](https://www.opengl.org/) (Open Graphics Library) is a cross-language, cross-platform application programming interface (API) for rendering 2D and 3D vector graphics. The API is typically used to interact with a graphics processing unit (GPU), to achieve hardware-accelerated rendering.

# Game Play
![image](https://github.com/DanielDobinski/Pacman/blob/main/game_play/game2.PNG)
![image](https://github.com/DanielDobinski/Pacman/blob/main/game_play/game1.PNG)
[![YouTube](https://github.com/DanielDobinski/Pacman/blob/main/game_play/gameYT.png)](https://youtu.be/ZynEebJM1h8)




## Install and Configure
### On Windows
Install dependencies using [vcpkg - (VC++ Package Manager)](https://vcpkg.io/en/index.html) 
```bash
# Clone this repository with recursive option
> cd Pacman
> git clone --recursive https://github.com/microsoft/vcpkg

# Run the bootstrap script for vcpkg
> .\vcpkg\bootstrap-vcpkg.bat

# Install dependencies using vcpkg manifest (vcpkg.json)
> .\vcpkg\vcpkg install 

# Compile and Run
> cmake . -B build/
> cmake --build build
> build\Debug\ImGui-GLFW-OpenGL3.exe

#download and place Irrklang for sound from [here](https://www.ambiera.com/irrklang/downloads.html)
#Place irrklang.dll and ikpMP3.dll in the same place as exe file is created
```

## Credits
- Cmake for ImGui - https://github.com/sukesh-ak/ImGui-GLFW-OpenGL3
  (Creator of ImGui prefers not to provide any cmakes to make it clear that ImGui can run platform independent)
- Dear ImGui - https://github.com/ocornut/imgui  
- GLFW - https://www.glfw.org/  
- OpenGL - https://www.opengl.org/  
- LearnOpenGL - https://learnopengl.com/ - most of it created on the Arcade Game Tutorial

