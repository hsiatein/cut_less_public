FROM rust:1.89.0-alpine3.22 AS builder

RUN apk add --no-cache git \
    musl-dev \
    libc-dev \
    gcc \
    cmake \
    clang \
    clang-dev \
    llvm \
    llvm-dev \
    ninja-build \
    openssl \
    openssl-dev \
    openssl-libs-static \
    pkgconfig \
    build-base

ENV PATH=/usr/lib/ninja-build/bin:$PATH
WORKDIR /
RUN git clone https://github.com/hsiatein/cmago.git && cd cmago && cargo build --release
ENV PATH=/cmago/target/release:$PATH