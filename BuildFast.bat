if not exist cmake (
  mkdir cmake
)

cd cmake
cmake ..

cmake --build . --config Debug