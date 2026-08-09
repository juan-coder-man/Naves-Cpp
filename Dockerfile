# syntax=docker/dockerfile:1

FROM ubuntu:24.04 AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++-mingw-w64-x86-64 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY nave.cpp .

RUN x86_64-w64-mingw32-g++ -O2 -static -o nave.exe nave.cpp

FROM scratch AS export
COPY --from=build /src/nave.exe /nave.exe
