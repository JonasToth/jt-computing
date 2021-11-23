[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/MacOS/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/Windows/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/Ubuntu/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![codecov](https://codecov.io/gh/filipdutescu/modern-cpp-template/branch/master/graph/badge.svg)](https://codecov.io/gh/filipdutescu/modern-cpp-template)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/filipdutescu/modern-cpp-template)](https://github.com/filipdutescu/modern-cpp-template/releases)

# Modern C++ Template

A quick C++ template for modern CMake projects, aimed to be an easy to use
starting point.

This is my personal take on such a type of template, thus I might not use the
best practices or you might disagree with how I do things. Any and all feedback
is greatly appreciated!

## Features

* Modern **CMake** configuration and project, which, to the best of my
knowledge, uses the best practices,

* An example of a **Clang-Format** config, inspired from the base *Google* model,
with minor tweaks. This is aimed only as a starting point, as coding style
is a subjective matter, everyone is free to either delete it (for the *LLVM*
default) or supply their own alternative,

* **Static analyzers** integration, with *Clang-Tidy* and *Cppcheck*, the former
being the default option,

* **Doxygen** support, through the `ENABLE_DOXYGEN` option, which you can enable
if you wish to use it,

* **Unit testing** support, through *GoogleTest* (with an option to enable
*GoogleMock*) or *Catch2*,

* **Code coverage**, enabled by using the `ENABLE_CODE_COVERAGE` option, through
*Codecov* CI integration,

* **Package manager support**, with *Conan* and *Vcpkg*, through their respective
options

* **CI workflows for Windows, Linux and MacOS** using *GitHub Actions*, making
use of the caching features, to ensure minimum run time,

* **.md templates** for: *README*, *Contributing Guideliness*,
*Issues* and *Pull Requests*,

* **Permissive license** to allow you to integrate it as easily as possible. The
template is licensed under the [Unlicense](https://unlicense.org/),

* Options to build as a header-only library or executable, not just a static or
shared library.

* **Ccache** integration, for speeding up rebuild times

## Getting Started

These instructions will get you a copy of the project up and running on your local
machine for development and testing purposes.

### Prerequisites

* **CMake v3.20+** - found at [https://cmake.org/](https://cmake.org/)
* **C++ Compiler** - needs to support at least the **C++20** standard
* **Catch2** - testing and benchmarking
* **GSL (Guideline Support Library)** - goodies

### Installing

It is fairly easy to install the project, all you need to do is clone if from
[GitHub](https://github.com/filipdutescu/modern-cpp-template) or
[generate a new repository from it](https://github.com/filipdutescu/modern-cpp-template/generate)
(also on **GitHub**).

If you wish to clone the repository, rather than generate from it, you simply need
to run:

```bash
git clone https://github.com/filipdutescu/modern-cpp-template/
```

After finishing getting a copy of the project, with any of the methods above, create
a new folder in the `include/` folder, with the name of your project.  Edit
`cmake/SourcesAndHeaders.cmake` to add your files.

You will also need to rename the `cmake/ProjectConfig.cmake.in` file to start with
the ***exact name of your project***. Such as `cmake/MyNewProjectConfig.cmake.in`.
You should also make the same changes in the GitHub workflows provided, notably
[`.github/workflows/ubuntu.yml`](.github/workflows/ubuntu.yml), in which you should
replace the CMake option `-DProject_ENABLE_CODE_COVERAGE=1` to
`-DMyNewProject_ENABLE_CODE_COVERAGE=1`.

Finally, change `"Project"` from `CMakeLists.txt`, from

```cmake
project(
  "Project"
  VERSION 0.1.0
  LANGUAGES CXX
)
```

to the ***exact name of your project***, i.e. using the previous name it will become:

```cmake
project(
  MyNewProject
  VERSION 0.1.0
  LANGUAGES CXX
)
```

To install an already built project, you need to run the `install` target with CMake.
For example:

```bash
cmake --build build --target install --config Release

# a more general syntax for that command is:
cmake --build <build_directory> --target install --config <desired_config>
```

## Building the project

To build the project, all you need to do, ***after correctly
[installing the project](README.md#Installing)***, is run a similar **CMake** routine
to the the one below:

```bash
mkdir build/ && cd build/
cmake .. -DCMAKE_INSTALL_PREFIX=/absolute/path/to/custom/install/directory
cmake --build . --target install
```

> ***Note:*** *The custom ``CMAKE_INSTALL_PREFIX`` can be omitted if you wish to
install in [the default install location](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html).*

More options that you can set for the project can be found in the
[`cmake/StandardSettings.cmake` file](cmake/StandardSettings.cmake). For certain
options additional configuration may be needed in their respective `*.cmake` files
(i.e. Conan needs the `CONAN_REQUIRES` and might need the `CONAN_OPTIONS` to be setup
for it work correctly; the two are set in the [`cmake/Conan.cmake` file](cmake/Conan.cmake)).

## Running the tests

```bash
$ cd build
$ # * or `ctest -C Debug` or any other configuration you wish to test
$ # * you can also run tests with the `-VV` flag for a more verbose output
$ ctest -C Release
> Everything passes always, of course
```

### End to end tests

If applicable, should be presented here.

### Coding style tests

* `clang-tidy` must be clean
* `clang-format` must be clean
* `cmake-format` must be clean

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our how you can
become a contributor and the process for submitting pull requests to us.

## Versioning

This project does not follow semantic versioning or anything else. It will
simply change the interfaces and don't care, unless it does.
There might be some point releases, but its not the goal.

It must be always green though.

## Authors

* **Jonas Toth** - [@filipdutescu](https://github.com/JonasToth)

## License

This project is licensed under the [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html),
see the [LICENSE](LICENSE) file for details.
