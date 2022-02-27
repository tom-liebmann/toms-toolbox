from conans import ConanFile, CMake, tools


class TomsToolboxConan(ConanFile):
    name = "toms-toolbox"
    version = "0.1"
    author = "Tom Liebmann <kontakt@liebmann-online.de>"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "library_type": ["SHARED", "STATIC"],
        "activate_core": [True, False],
        "activate_math": [True, False],
        "activate_ui": [True, False],
        "activate_utils": [True, False],
        "activate_project": [True, False],
    }
    default_options = {
        "library_type": "SHARED",
        "activate_core": True,
        "activate_math": True,
        "activate_ui": True,
        "activate_utils": True,
        "activate_project": True,
        "yaml-cpp:shared": True,
    }
    generators = "cmake_find_package", "cmake_paths"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_PLATFORM"] = self.settings.os
        cmake.definitions["BUILD_LIBRARY_TYPE"] = self.options.library_type
        cmake.definitions["ACTIVATE_ttbCore"] = self.options.activate_core
        cmake.definitions["ACTIVATE_ttbMath"] = self.options.activate_math
        cmake.definitions["ACTIVATE_ttbUi"] = self.options.activate_ui
        cmake.definitions["ACTIVATE_ttbUtils"] = self.options.activate_utils
        cmake.configure()
        cmake.build()

    def requirements(self):
        if self.options.activate_core:
            self.requires("yaml-cpp/0.6.3")
            self.requires("glfw/3.3.6")
            self.requires("opengl/system")
            self.requires("glew/2.2.0")

    def package(self):
        if self.options.activate_core:
            self.copy("*", dst="include", src="src/core/include")
            self.copy("*.so", dst="lib", src="src/core")
            self.copy("*.a", dst="lib", src="src/core")
        if self.options.activate_math:
            self.copy("*", dst="include", src="src/math/include")
            self.copy("*.so", dst="lib", src="src/math")
            self.copy("*.a", dst="lib", src="src/math")
        if self.options.activate_ui:
            self.copy("*", dst="include", src="src/ui/include")
            self.copy("*.so", dst="lib", src="src/ui")
            self.copy("*.a", dst="lib", src="src/ui")
        if self.options.activate_utils:
            self.copy("*", dst="include", src="src/utils/include")
            self.copy("*.so", dst="lib", src="src/utils")
            self.copy("*.a", dst="lib", src="src/utils")
        if self.options.activate_project:
            self.copy("*", dst="project", src="project")
            self.copy("*", dst="cmake", src="cmake")

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.name = "ttb"

        if self.options.activate_core:
            self.cpp_info.components["core"].requires = ["yaml-cpp::yaml-cpp", "glfw::glfw", "opengl::opengl", "glew::glew"]
            self.cpp_info.components["core"].libdirs = ["lib"]
            self.cpp_info.components["core"].libs = ["ttbCore"]
        if self.options.activate_math:
            self.cpp_info.components["math"].libdirs = ["lib"]
            self.cpp_info.components["math"].libs = ["ttbMath"]
        if self.options.activate_ui:
            self.cpp_info.components["ui"].libdirs = ["lib"]
            self.cpp_info.components["ui"].libs = ["ttbUi"]
        if self.options.activate_utils:
            self.cpp_info.components["utils"].libdirs = ["lib"]
            self.cpp_info.components["utils"].libs = ["ttbUtils"]
        if self.options.activate_project:
            self.cpp_info.components["project"].builddirs = ["project"]
