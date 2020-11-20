FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
# Dependencies for glvnd and X11.
RUN apt-get update \
  && apt-get install -y -qq --no-install-recommends \
    libglvnd0 \
    libgl1 \
    libglx0 \
    libegl1 \
    libxext6 \
    libx11-6 \
    libx11-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    mesa-common-dev \
    libxrandr-dev \
    libxi-dev \
    libglew-dev \
    libsdl2-dev \
    libsdl2-image-dev \
    libglm-dev \ 
    libfreetype6-dev \
    libglfw3 \
    libglfw3-dev \
  && rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install -y\
    pkg-config \
    build-essential \
    vim \
    clang-format \
    ca-certificates \
    curl tar unzip \
    zip && \
    rm -rf /var/lib/apt/lists/*
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    rm -rf /var/lib/apt/lists/*
    
RUN apt-add-repository ppa:git-core/ppa
RUN set -ex \
    && cd /tmp \
    && curl -fSL https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-Linux-x86_64.tar.gz -o cmake-3.16.4-Linux-x86_64.tar.gz \
    && tar -xzvf cmake-3.16.4-Linux-x86_64.tar.gz -C /opt \
    && rm cmake-3.16.4-Linux-x86_64.tar.gz \
    && curl -fSL https://github.com/ninja-build/ninja/releases/download/v1.10.0/ninja-linux.zip -o ninja-linux.zip \
    && unzip ninja-linux.zip -d /opt/ninja-1.10.0 \
    && rm ninja-linux.zip 

ENV VCPKG_FORCE_SYSTEM_BINARIES="1" \
    PATH="/opt/cmake-3.16.4-Linux-x86_64/bin:/opt/ninja-1.10.0:$PATH"

RUN apt-get install -y git
RUN git --version

#Building assimp - Using vcpkg
RUN git clone https://github.com/assimp/assimp.git && \
cd assimp && \
cmake CMakeLists.txt && \
make -j4 && \
make install
RUN apt-get install -y libboost-all-dev
# Env vars for the nvidia-container-runtime.
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES graphics,utility,compute
WORKDIR /home/opengl_gpu
