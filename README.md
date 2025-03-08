# NarcEngine

NarcEngine is a C++ game engine that leverages Vulkan for high-performance graphics rendering. This project includes several dependencies and is structured to facilitate easy development and integration.

## Features

- High-performance graphics rendering using Vulkan
- Modular architecture with separate components for IO and core engine functionality
- Integration with GLFW for window management and input handling
- Integration with GLM for mathematics operations

## Dependencies

- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [Python](https://www.python.org/)
- [CMake](https://cmake.org/)
- [STB](https://github.com/nothings/stb.git)
- [Tinyobjloader](https://github.com/tinyobjloader/tinyobjloader.git)

## Building the Project

### Prerequisites

- CMake 3.10 or higher
- A C++17 compatible compiler
- Vulkan SDK installed
- Python 3.6 or higher (for building dependencies)

### Steps

1. Clone the repository:
    ```sh
    git clone https://github.com/hennotheo/NarcEngine.git
    cd NarcEngine
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Configure the project with CMake:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    cmake --build .
    ```

## Usage

To use the NarcEngine in your project, link against the `NarcEngine` library and include the necessary headers.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
