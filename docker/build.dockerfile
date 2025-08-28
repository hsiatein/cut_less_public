FROM rust:1.89.0 AS builder

RUN apt-get update
RUN apt-get install -y clang
RUN apt-get install -y cmake
RUN apt-get install -y git
RUN apt-get install -y ninja-build

ENV http_proxy=http://192.168.2.63:7897
ENV https_proxy=http://192.168.2.63:7897

RUN git clone https://github.com/hsiatein/cmago.git
WORKDIR /cmago
RUN cargo build --release
ENV PATH=/cmago/target/release:$PATH

WORKDIR /
RUN mkdir cut_less
WORKDIR /cut_less
RUN cmago init
COPY sources/ .

WORKDIR /cut_less/web_api
RUN cargo build --release

ENV CC=clang
ENV CXX=clang++
WORKDIR /cut_less
RUN cmago update
RUN rm -rf ./build
RUN cmake -S . -B build
RUN cmake --build build -j4

ENV CUT_LESS_NAME="main_json"
ENV TARGET_DIR="binaries"
RUN mkdir -p $TARGET_DIR
ENV CUT_LESS_PATH="build/$CUT_LESS_NAME"
RUN cp "$CUT_LESS_PATH" "$TARGET_DIR"
RUN ldd "$CUT_LESS_PATH" | grep "=>" | awk '{print $3}' | while read lib; do if [ -f "$lib" ]; then cp "$lib" "$TARGET_DIR"; fi; done

ENV SERVER_PATH=web_api/target/release/server_v2
RUN cp "$SERVER_PATH" "$TARGET_DIR"
RUN ldd "$SERVER_PATH" | grep "=>" | awk '{print $3}' | while read lib; do if [ -f "$lib" ]; then cp "$lib" "$TARGET_DIR"; fi; done

COPY sources/config.json "$TARGET_DIR"

FROM debian:trixie-slim
EXPOSE 6002
RUN mkdir binaries
COPY --from=builder /cut_less/binaries /binaries
ENV LD_LIBRARY_PATH=/binaries
WORKDIR /binaries
CMD ["./server_v2"]
