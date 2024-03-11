#!/bin/sh

#echo $1
#if [ "$1" != "Debug" -a "$1" != "Release" ]; then
#	echo "$0 Debug|Release"
#	exit 1
#fi

mkdir -p build
cmake -B build/ -S src/ -DCMAKE_BUILD_TYPE=$1
cmake --build build/

mkdir -p examples/build/
cmake -B examples/build/ -S examples/ -DCMAKE_BUILD_TYPE=$1
cmake --build examples/build/
