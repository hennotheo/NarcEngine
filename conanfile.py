from conan import ConanFile
from conan.tools.cmake import cmake_layout

class NarcEngineConsumer(ConanFile):
    name = "narcengine-consumer"
    version = "0.2"

    settings = "os", "arch", "compiler", "build_type"

    requires = (
        "glm/1.0.1",
        "spdlog/1.15.3"
    )

    generators = ("CMakeDeps", "CMakeToolchain")

    tool_requires = (
        "ninja/1.11.1",
    )

    def layout(self):
        cmake_layout(self)