# syntax=docker/dockerfile:1.7-labs

FROM ubuntu:24.04 AS libtoms-toolbox-builder

# Install dependencies
RUN apt-get update && \
    apt-get -y install \
      libgl-dev \
      libgl1-mesa-dev \
      cmake \
      make \
      gcc \
      g++ \
      pkg-config

RUN apt-get -y install \
      python3.12-venv
