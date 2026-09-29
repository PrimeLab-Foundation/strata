FROM ubuntu:24.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    clang llvm cmake binutils git make \
    python3.12 python3.12-dev python3.12-venv \
    libstdc++-14-dev \
    && rm -rf /var/lib/apt/lists/*
