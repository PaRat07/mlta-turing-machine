FROM alpine:3.20 as builder

RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    ccache \
    git

WORKDIR /app

COPY CMakeLists.txt ./
COPY cmake ./cmake
COPY src ./src
COPY include ./include

RUN cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_EXE_LINKER_FLAGS="-static" \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON


RUN cmake --build build


FROM scratch as export

COPY --from=builder /app/build/turing-machine /
