FROM ubuntu:22.04

# Install build essentials, CMake, Clang, pkg-config, Google Test and Boost
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    clang \
    pkg-config \
    libgtest-dev \
    libboost-all-dev \
    rpm \
    lcov \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the project files
COPY . .

# Create build directory
RUN mkdir -p build

# Build commands
WORKDIR /app/build
RUN cmake ..
RUN make

# Run tests
CMD ["ctest", "--output-on-failure"]
