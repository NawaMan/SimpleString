FROM ubuntu:24.04 AS base

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    ruby \
    ruby-dev \
    rubygems \
    rpm \
    clang \
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

FROM base AS linux

FROM base AS windows
RUN apt-get update && apt-get install -y \
    mingw-w64 \
    wine64 \
    mingw-w64-tools \
    software-properties-common \
    wixl \
    wget \
    tar \
    bzip2 \
    build-essential \
    g++-mingw-w64-x86-64 \
    && rm -rf /var/lib/apt/lists/*

# Create Boost user config for MinGW
RUN echo "using gcc : mingw : x86_64-w64-mingw32-g++ : <rc>x86_64-w64-mingw32-windres ;" > /tmp/boost-user-config.jam

# Download and build Boost for MinGW
WORKDIR /tmp
RUN wget -q https://sourceforge.net/projects/boost/files/boost/1.83.0/boost_1_83_0.tar.gz && \
    tar xf boost_1_83_0.tar.gz && \
    cd boost_1_83_0 && \
    ./bootstrap.sh --with-libraries=locale,regex && \
    ./b2 toolset=gcc \
        cxxflags="-D_GLIBCXX_USE_CXX11_ABI=1 -static-libgcc -static-libstdc++" \
        target-os=windows \
        threading=multi \
        runtime-link=static \
        link=static \
        address-model=64 \
        variant=release \
        --user-config=/tmp/boost-user-config.jam \
        --prefix=/usr/x86_64-w64-mingw32 \
        install && \
    rm -rf /tmp/boost_1_83_0 /tmp/boost-user-config.jam /tmp/boost_1_83_0.tar.gz

WORKDIR /build

FROM base AS macos
RUN apt-get update && apt-get install -y \
    clang \
    llvm \
    && rm -rf /var/lib/apt/lists/*
