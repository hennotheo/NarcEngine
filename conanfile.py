from conan import ConanFile
from conan.tools.cmake import cmake_layout


class NarcEngineConsumer(ConanFile):
    name = "narcengine-consumer"
    version = "0.2"

    settings = "os", "arch", "compiler", "build_type"

    requires = (
        "glm/1.0.1",
        "spdlog/1.15.3",
        "di/1.3.2",
        "glfw/3.4",
        "vulkan-memory-allocator/3.0.1",
        "vulkan-headers/1.3.243.0",
        "stb/cci.20240531",
        "tinyobjloader/2.0.0-rc10",
        "imgui/1.92.2b"
    )

    generators = ("CMakeDeps", "CMakeToolchain")

    tool_requires = (
        "ninja/1.11.1",
    )

    def layout(self):
        cmake_layout(self)
