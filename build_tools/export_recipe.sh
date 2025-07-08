#!/usr/bin/bash

set -ex

PROJECT_DIR="/etc/project"
BUILD_DIR="${PROJECT_DIR}/build"

. $HOME/python/venv/bin/activate

mkdir -p "${BUILD_DIR}"

cd "${BUILD_DIR}"

conan export ..
