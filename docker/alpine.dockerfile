FROM alpine_base:latest AS builder

WORKDIR /cut_less
RUN cmago init
COPY sources/ .
WORKDIR /cut_less/web_api
RUN cargo build --release
WORKDIR /cut_less
RUN cmago update && rm -rf ./build
ENV CC=clang
ENV CXX=clang++
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w" -DCMAKE_C_FLAGS="-w"
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
RUN rm $TARGET_DIR/ld-musl-x86_64.so.1

FROM alpine:3.22
EXPOSE 6002
WORKDIR /binaries
COPY --from=builder /cut_less/binaries .
ENV LD_LIBRARY_PATH=/binaries
CMD ["./server_v2"]