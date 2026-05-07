FROM alpine:3.20 AS builder

RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    ccache \
    git

WORKDIR /app

COPY deps ./deps

ENV CPM_SOURCE_CACHE=/.cache/cpm

RUN cmake -B /tmp/deps-conf -S ./deps && \
    rm -rf /tmp/deps-conf

COPY CMakeLists.txt ./
COPY src ./src
COPY include ./include


RUN cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXE_LINKER_FLAGS="-static" \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
    -DFETCHCONTENT_FULLY_DISCONNECTED=ON

RUN cmake --build build


FROM scratch AS export

COPY --from=builder /app/build/turing-machine /

ENTRYPOINT ["/turing-machine"]
