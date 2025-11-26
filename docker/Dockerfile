FROM ubuntu:22.04
EXPOSE 6002

COPY binaries /binaries
ENV LD_LIBRARY_PATH=/binaries
WORKDIR /binaries
CMD ["./server"]
