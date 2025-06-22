# syntax=docker/dockerfile:1.7-labs

FROM ubuntu:24.04 AS libtoms-toolbox-builder

# Install dependencies
RUN apt-get update && \
    apt-get -y install \
      pipx \
      libgl-dev \
      libgl1-mesa-dev \
      cmake \
      make \
      gcc \
      g++ \
      pkg-config && \
    pipx ensurepath && \
    pipx install conan

ENV PATH="/root/.local/bin:$PATH"

# Copy all library files
WORKDIR /toms-toolbox
COPY --exclude=build --exclude=Dockerfile . .

# Create build directory
WORKDIR ./build

# Build dependencies
RUN conan install .. --profile:build=../conan_profile --profile:host=../conan_profile --build=missing -of=.

# Build library
RUN cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_PLATFORM=Linux \
      -DBUILD_LIBRARY_TYPE=SHARED \
      -DACTIVATE_ttbCore=ON \
      -DACTIVATE_ttbMath=ON \
      -DACTIVATE_ttbUi=ON \
      -DACTIVATE_ttbUtils=ON && \
    make -j$(nproc)

# Export conan recipe
RUN conan export ..
