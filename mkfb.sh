mkdir -p build
cmake -B build/ -S src/ -DCMAKE_BUILD_TYPE=Debug
cmake --build build/

