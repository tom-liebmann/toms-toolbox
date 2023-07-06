from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps
from os.path import join


class TomsToolboxConan(ConanFile):
    name = "toms-toolbox"
    author = "Tom Liebmann <kontakt@liebmann-online.de>"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "library_type": ["SHARED", "STATIC"],
        "activate_core": [True, False],
        "activate_math": [True, False],
        "activate_ui": [True, False],
        "activate_utils": [True, False],
        "activate_project": [True, False],
        "build_type": ["Release", "Debug"],
        "build_tests": [True, False],
    }
    default_options = {
        "library_type": "SHARED",
        "activate_core": True,
        "activate_math": True,
        "activate_ui": True,
        "activate_utils": True,
        "activate_project": True,
        "build_type": "Release",
        "yaml-cpp/*:shared": True,
        "build_tests": False,
    }
    exports_sources = "*"

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.cache_variables["BUILD_PLATFORM"] = str(self.settings.os)
        toolchain.cache_variables["BUILD_LIBRARY_TYPE"] = str(self.options.library_type)
        toolchain.cache_variables["BUILD_TESTS"] = str(self.options.build_tests)
        toolchain.cache_variables["ACTIVATE_ttbCore"] = str(self.options.activate_core)
        toolchain.cache_variables["ACTIVATE_ttbMath"] = str(self.options.activate_math)
        toolchain.cache_variables["ACTIVATE_ttbUi"] = str(self.options.activate_ui)
        toolchain.cache_variables["ACTIVATE_ttbUtils"] = str(self.options.activate_utils)
        toolchain.cache_variables["CMAKE_BUILD_TYPE"] = str(self.options.build_type)
        toolchain.generate()

        deps = CMakeDeps(self)
        for require, dependency in self.dependencies.items():
            deps.set_property(dependency.ref.name, "cmake_find_mode", "config")
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def requirements(self):
        if self.options.activate_utils:
            self.requires("fmt/8.1.1")
        if self.options.activate_core:
            self.requires("yaml-cpp/0.6.3")
            if self.settings.os != "Android":
                self.requires("glfw/3.3.6")
                self.requires("opengl/system")
                self.requires("glew/2.2.0")
        if self.options.activate_ui:
            self.requires("rapidxml/1.13")
        if self.options.build_tests:
            self.requires("catch2/2.13.8")

    def package(self):
        if self.options.activate_core:
            copy(self, "*", join(self.build_folder, "src/core/include"), join(self.package_folder, "include"), keep_path=True)
            copy(self, "*.so", join(self.build_folder, "src/core"), join(self.package_folder, "lib"), keep_path=False)
            copy(self, "*.a", join(self.build_folder, "src/core"), join(self.package_folder, "lib"), keep_path=False)
        if self.options.activate_math:
            copy(self, "*", join(self.build_folder, "src/math/include"), join(self.package_folder, "include"), keep_path=True)
            copy(self, "*.so", join(self.build_folder, "src/math"), join(self.package_folder, "lib"), keep_path=False)
            copy(self, "*.a", join(self.build_folder, "src/math"), join(self.package_folder, "lib"), keep_path=False)
        if self.options.activate_ui:
            copy(self, "*", join(self.build_folder, "src/ui/include"), join(self.package_folder, "include"), keep_path=True)
            copy(self, "*.so", join(self.build_folder, "src/ui"), join(self.package_folder, "lib"), keep_path=False)
            copy(self, "*.a", join(self.build_folder, "src/ui"), join(self.package_folder, "lib"), keep_path=False)
        if self.options.activate_utils:
            copy(self, "*", join(self.build_folder, "src/utils/include"), join(self.package_folder, "include"), keep_path=True)
            copy(self, "*.so", join(self.build_folder, "src/utils"), join(self.package_folder, "lib"), keep_path=False)
            copy(self, "*.a", join(self.build_folder, "src/utils"), join(self.package_folder, "lib"), keep_path=False)
        if self.options.activate_project:
            copy(self, "*", join(self.build_folder, "project"), join(self.package_folder, "project"), keep_path=True)
            copy(self, "*", join(self.build_folder, "cmake"), join(self.package_folder, "cmake"), keep_path=True)

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "ttb")
        self.cpp_info.set_property("cmake_target_name", "ttb::ttb")

        if self.options.activate_core:
            comp = self.cpp_info.components["core"]
            comp.set_property("cmake_target_name", "ttb::core")
            comp.requires = ["yaml-cpp::yaml-cpp"]
            if self.settings.os != "Android":
                comp.requires.extend(["glfw::glfw", "opengl::opengl", "glew::glew"])
            if self.options.build_tests:
                comp.requires.extend(["catch2::catch2"])
            comp.libs = ["ttbCore"]

        if self.options.activate_math:
            comp = self.cpp_info.components["math"]
            comp.set_property("cmake_target_name", "ttb::math")
            if self.options.build_tests:
                comp.requires.extend(["catch2::catch2"])
            comp.libs = ["ttbMath"]

        if self.options.activate_ui:
            comp = self.cpp_info.components["ui"]
            comp.set_property("cmake_target_name", "ttb::ui")
            comp.requires.append("rapidxml::rapidxml")
            if self.options.build_tests:
                comp.requires.extend(["catch2::catch2"])
            comp.libs = ["ttbUi"]

        if self.options.activate_utils:
            comp = self.cpp_info.components["utils"]
            comp.set_property("cmake_target_name", "ttb::utils")
            comp.requires.extend(["fmt::fmt"])
            if self.options.build_tests:
                comp.requires.extend(["catch2::catch2"])
            comp.libs = ["ttbUtils"]

        if self.options.activate_project:
            comp = self.cpp_info.components["project"]
            comp.includedirs = ["project/src/include"]
            comp.set_property("cmake_find_mode", "both")
            comp.builddirs.extend(["project"])
