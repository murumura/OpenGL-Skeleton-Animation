# OpenGL-Skeleton-Animation
A simple implementation of skeletal Animation using OpenGL , Assimp , STBImage 
# Third party use
- Assimp.
- GLM.
- GLAD.
- GLFW.
- stb_image.

# How to build and run
I have the dockerfile needed to build the environment for this project, for those who are familiar with the basic operations of docker are very welcome to execute this project directly through docker container I build.
## Prerequisites to build docker container
- Docker version 19.03.13+
- NVIDIA GPU and corresponding Cuda library, driver installed
## build Docker container
Once docker is installed on the host, it is very easy to build a development environment through the dockerfile and build script I provided
```bash
# build docker container
$ sh build.sh
# That's it, after a few minutes of installation, you can enter the docker container through run script
$ sh run.sh
```
After you build the third-party library project needed(or simply use the docker container in this repo), enter the `hello_animation` folder:
## build on host (or on docker container) 
```bash
$ cd hello_animation/
$ mkdir build
$ cd build 
$ cmake ..
$ make
$ ./hello_animation
```

# Screen Shot
![Image](result/result.gif)
# Features

- Support using camera to adjust view
    - press W,A,S,D to move camera toward/left/away/right
    - moving mouse cursor to adjust camera's yaw, pitch view
