FROM ubuntu:22.04

# Install common dependencies
RUN dpkg --add-architecture arm64 && \
    echo 'deb [arch=amd64] http://archive.ubuntu.com/ubuntu jammy main restricted universe multiverse' > /etc/apt/sources.list && \
    echo 'deb [arch=amd64] http://archive.ubuntu.com/ubuntu jammy-updates main restricted universe multiverse' >> /etc/apt/sources.list && \
    echo 'deb [arch=amd64] http://archive.ubuntu.com/ubuntu jammy-backports main restricted universe multiverse' >> /etc/apt/sources.list && \
    echo 'deb [arch=amd64] http://security.ubuntu.com/ubuntu jammy-security main restricted universe multiverse' >> /etc/apt/sources.list && \
    echo 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy main restricted universe multiverse' >> /etc/apt/sources.list && \
    echo 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy-updates main restricted universe multiverse' >> /etc/apt/sources.list && \
    echo 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy-backports main restricted universe multiverse' >> /etc/apt/sources.list && \
    echo 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy-security main restricted universe multiverse' >> /etc/apt/sources.list && \
    apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    clang \
    pkg-config \
    libgtest-dev \
    libboost-all-dev \
    rpm \
    lcov \
    ninja-build \
    g++-aarch64-linux-gnu \
    g++-mingw-w64 \
    mingw-w64 \
    libboost-locale-dev:arm64 \
    && rm -rf /var/lib/apt/lists/*

# Set up cross-compilation environments
RUN mkdir -p /usr/aarch64-linux-gnu/lib /usr/x86_64-w64-mingw32/lib && \
    mkdir -p /usr/aarch64-linux-gnu/include /usr/x86_64-w64-mingw32/include && \
    ln -s /usr/include/boost /usr/aarch64-linux-gnu/include/boost && \
    ln -s /usr/lib/aarch64-linux-gnu/libboost_* /usr/aarch64-linux-gnu/lib/ && \
    ln -s /usr/include/boost /usr/x86_64-w64-mingw32/include/boost && \
    ln -s /usr/lib/x86_64-linux-gnu/libboost_* /usr/x86_64-w64-mingw32/lib/

# Set up MinGW system headers
RUN mkdir -p /usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/ && \
    cd /usr/x86_64-w64-mingw32/include/ && \
    for file in *; do \
        if [ ! -e /usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/"$file" ]; then \
            ln -s /usr/x86_64-w64-mingw32/include/"$file" /usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/"$file"; \
        fi \
    done

# Set the working directory
WORKDIR /app

# Create directories
RUN mkdir -p build dist

# Copy the project files
COPY . .

# Make scripts executable
RUN chmod +x build-locally.sh docker-entrypoint.sh

ENTRYPOINT ["/app/docker-entrypoint.sh"]

