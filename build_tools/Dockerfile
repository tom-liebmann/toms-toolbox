# syntax=docker/dockerfile:1.7-labs

FROM ubuntu:24.04 AS libtoms-toolbox-builder

ARG UID=1000
ARG GID=1000

# Install dependencies
RUN apt-get update && \
    apt-get -y install \
      sudo \
      cmake \
      make \
      gcc \
      g++ \
      pkg-config \
      python3.12-venv

# Setup user
RUN if ! getent group $GID >/dev/null; then \
        groupadd --gid $GID builduser; \
    fi

RUN if ! getent passwd $UID >/dev/null; then \
        useradd --uid $UID --gid $GID --create-home builduser; \
    fi

RUN USERNAME=$(getent passwd $UID | cut -d: -f1) && \
    usermod -aG sudo $USERNAME && \
    echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN mkdir -p /etc/build_tools
COPY ./conanfile.py ./conan_profile /etc/build_tools/

USER $UID

RUN mkdir -p "$HOME/python" && \
    python3.12 -m venv $HOME/python/venv && \
    . $HOME/python/venv/bin/activate && \
    python3.12 -m pip install conan

RUN . $HOME/python/venv/bin/activate && \
    mkdir -p "$HOME/conan_build" && \
    cd "$HOME/conan_build" && \
    conan install /etc/build_tools \
    --profile:build=/etc/build_tools/conan_profile \
    --profile:host=/etc/build_tools/conan_profile \
    --build=missing \
    -of=.
    
ENV CONAN_HOME=/etc/conan
