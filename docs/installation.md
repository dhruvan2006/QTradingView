# Installation

QTradingView can be integrated into your CMake-based project in a few ways. This document covers the common workflows, dependencies, and quick build instructions for local development and the WebAssembly demo.

## Prerequisites

- CMake >= 3.16
- A C++17-capable compiler (MSVC, clang, gcc)
- Qt 6 (QtWidgets, QtGui, QtCore). For the Web demo, an Emscripten + Qt for WebAssembly toolchain is required.

## Option A — FetchContent (recommended)

The easiest way to consume QTradingView from another CMake project is via `FetchContent`. This keeps your project dependency management simple and works well for CI.

```cmake
include(FetchContent)

FetchContent_Declare(
    QTradingView
    GIT_REPOSITORY https://github.com/dhruvan2006/QTradingView.git
    GIT_TAG v1.0.0 # use a tag or commit
)

FetchContent_MakeAvailable(QTradingView)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE QTradingView::QTradingView)
```

This downloads, builds, and links QTradingView into your project.

## Option B — Find package (prebuilt binaries)

If you installed the prebuilt binaries from the releases, you can use `find_package`:

Usage:

```cmake
find_package(QTradingView CONFIG REQUIRED)
add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE QTradingView::QTradingView)
```

## Building QTradingView locally

To build the library and examples locally (Windows example using `cmd.exe`):

```bat
mkdir build && cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

Notes:
- On MSVC you may need to pass `-A x64` to CMake when generating for a specific architecture.
- If your system Qt is not in a standard path, set `CMAKE_PREFIX_PATH` to the Qt installation root before configuring.

## Building the WebAssembly demo

This repository includes a WebAssembly demo built with Qt for WebAssembly. Building the demo requires Emscripten and a Qt build for WebAssembly. Refer to Qt's official docs for configuring the toolchain. The demo assets are available under `docs/demo/` in this repository (prebuilt), so you can preview the demo without building from source.

## Install and packaging

After building, the project provides targets and a CMake config file (`QTradingViewConfig.cmake`) so other projects can find and link the library using `find_package(QTradingView)` if you install it into a system or user prefix.

## Troubleshooting

- Missing Qt: ensure Qt6 dev packages are installed and CMake can locate them via `CMAKE_PREFIX_PATH`.
- Link errors on Windows: ensure you link the correct configuration (Release/Debug) and that the import library is available in your linker path.

If something is unclear or a platform-specific guide is needed, open an issue and include your OS, compiler, Qt version and the CMake output.
