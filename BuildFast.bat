if not exist cmake (
  mkdir cmake
)

cd cmake

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug