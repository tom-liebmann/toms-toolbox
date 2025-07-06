#!/usr/bin/bash

set -ex

PROJECT_DIR="/etc/project"
BUILD_DIR="${PROJECT_DIR}/build"

. $HOME/python/venv/bin/activate

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

conan export ..
