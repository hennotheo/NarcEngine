from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class NarcEngineConan(ConanFile):
    name = "narcengine"
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
        "imgui/1.92.2b",
    )

    generators = ("CMakeDeps", "CMakeToolchain")

    tool_requires = (
        "ninja/1.11.1",
        "shaderc/2025.3"
    )

    def configure(self):
        # Configurer GLFW pour éviter les dépendances inutiles
        self.options["glfw"].shared = True  # Utiliser GLFW en dynamique
        self.options["glfw"].with_wayland = False  # Désactiver Wayland
        self.options["glfw"].with_x11 = True  # Garder X11 (nécessaire sur Linux)

    def layout(self):
        cmake_layout(self)
