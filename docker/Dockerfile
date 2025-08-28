FROM ubuntu:22.04

COPY binaries /binaries
ENV LD_LIBRARY_PATH=/binaries
WORKDIR /binaries
EXPOSE 6002
CMD ["./server_v2"]
