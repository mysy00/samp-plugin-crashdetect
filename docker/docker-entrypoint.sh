#!/bin/sh
[ -z $CONFIG ] && config=Release || config="$CONFIG"

cmake --version

cmake \
    -S . \
    -B build \
    -DBUILD_TESTING=OFF \
    -DCMAKE_BUILD_TYPE=$config \
    -DCMAKE_INCLUDE_PATH=${PWD}/deps \
    -DCMAKE_CXX_FLAGS="-I${PWD}/deps" \
&&
cd build \
&&
make -j$(nproc)
