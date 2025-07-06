#!/usr/bin/bash

PROJECT_DIR="/etc/project"

mkdir -p "$HOME/python"

python3 -m venv $HOME/python/venv

source $HOME/python/venv/bin/activate

python3 -m pip install conan

BUILD_DIR="${PROJECT_DIR}/build"
mkdir -p "${BUILD_DIR}"

cd "${BUILD_DIR}"

conan install .. \
    --profile:build=../conan_profile \
    --profile:host=../conan_profile \
    --build=missing \
    -of=.

cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_PLATFORM=Linux \
      -DBUILD_LIBRARY_TYPE=SHARED \
      -DACTIVATE_ttbCore=ON \
      -DACTIVATE_ttbMath=ON \
      -DACTIVATE_ttbUi=ON \
      -DACTIVATE_ttbUtils=ON

make -j$(nproc)

RUN conan export ..
