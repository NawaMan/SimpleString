FROM ubuntu:24.04 as base

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    ruby \
    ruby-dev \
    rubygems \
    rpm \
    && gem install fpm \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

# Set version from build arg
ARG VERSION
ENV VERSION=${VERSION}

# Common build script
COPY docker-build.sh /build/
RUN chmod +x /build/docker-build.sh

FROM base as linux
CMD ["./docker-build.sh", "linux"]

FROM base as windows
RUN apt-get update && apt-get install -y \
    mingw-w64 \
    wine64 \
    mingw-w64-tools \
    software-properties-common \
    && rm -rf /var/lib/apt/lists/*

# Install MinGW Boost
RUN apt-get update && \
    apt-get install -y mingw-w64-x86-64-dev libboost-all-dev:amd64 && \
    mkdir -p /usr/x86_64-w64-mingw32/include /usr/x86_64-w64-mingw32/lib && \
    cp -r /usr/include/boost /usr/x86_64-w64-mingw32/include/ && \
    cp -r /usr/lib/x86_64-linux-gnu/libboost_* /usr/x86_64-w64-mingw32/lib/ && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /build
CMD ["./docker-build.sh", "windows"]

FROM base as macos
RUN apt-get update && apt-get install -y \
    clang \
    llvm \
    && rm -rf /var/lib/apt/lists/*
CMD ["./docker-build.sh", "macos"]
