# syntax=docker/dockerfile:1.7-labs

FROM ubuntu:24.04 AS libtoms-toolbox-builder

ARG UID=1000
ARG GID=1000

RUN apt-get update

# Setup user

RUN if ! getent group $GID >/dev/null; then \
        groupadd --gid $GID builduser; \
    fi

RUN if ! getent passwd $UID >/dev/null; then \
        useradd --uid $UID --gid $GID --create-home builduser; \
    fi

RUN apt-get -y install sudo
RUN USERNAME=$(getent passwd $UID | cut -d: -f1) && \
    usermod -aG sudo $USERNAME && \
    echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Install dependencies
RUN apt-get -y install \
      libgl-dev \
      libgl1-mesa-dev \
      cmake \
      make \
      gcc \
      g++ \
      pkg-config \
      python3.12-venv

RUN apt-get -y install \
    libx11-xcb-dev
