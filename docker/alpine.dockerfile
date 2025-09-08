FROM alpine_base:latest AS builder

ENV SERVER_NAME=server
WORKDIR /cut_less
RUN cmago init
COPY sources/cl_base ./cl_base
COPY sources/cl_solver ./cl_solver
COPY sources/external ./external
COPY sources/Cmago.toml .
RUN cmago update && rm -rf ./build
ENV CC=clang
ENV CXX=clang++
# RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-w -D__MUSL__ -static" -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_FLAGS="-w -stdlib=libstdc++ -static"
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build -j4 --target cl_solver
COPY sources/web_api ./web_api
ENV RUSTFLAGS="-C target-feature=-crt-static"
WORKDIR /cut_less/web_api/${SERVER_NAME}
RUN apk add perl zlib-static
RUN cargo build --bin server --release


WORKDIR /cut_less
ENV TARGET_DIR="binaries"
RUN mkdir -p $TARGET_DIR
ENV SERVER_PATH=web_api/target/release/${SERVER_NAME}
RUN cp "$SERVER_PATH" "$TARGET_DIR"

RUN ldd "$SERVER_PATH" | grep "=>" | awk '{print $3}' | while read lib; do if [ -f "$lib" ]; then cp "$lib" "$TARGET_DIR"; fi; done
RUN rm $TARGET_DIR/ld-musl-x86_64.so.1
RUN rm $TARGET_DIR/libz.so.1


FROM alpine:3.22
EXPOSE 6002
WORKDIR /binaries
COPY --from=builder /cut_less/binaries .
ENV LD_LIBRARY_PATH=/binaries
CMD ["./server"]