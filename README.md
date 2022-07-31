# Face Recognition Service [WIP]

Example grpc face recognition service with openvino, develop to learn openvino and modern cpp. still work in progress...

Currently developed in macOS, llvm-clang


## Features

- [Modern CMake practices](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)
- Based on openvino
- Master - Workers architecture
- Use Poco to send messages between master-workers
- Conan to manage dependencies
- CMake library/server/test layout
- Protobuf proto & GRPC server

## Dependencies

openvino, you need to install(download from openvino website) it first. other depencencies are managed by `conan/conanfile.txt`.

## Project layout


* `include` `source` are used to build `FaceRecognition` library, see `CMakelists.txt` for more details.

* `server` is used to build the server binary, which use the `FaceRecognition` lib, see `server/CMakelists.txt` for more details.

* `test` contains unit-tests, they also use the `FaceRecognition` lib, and of-cause they test it.

* `conan` manage conan packages and export buildinfo to `build` dir

* `cmake` manage CPM tool.


## Usage


### Build and run

#### Install openvino distribution

https://www.intel.com/content/www/us/en/developer/tools/openvino-toolkit/download.html

```
# double check your openvino install location, modify CMakeLists.txt if needed

set(OpenVINO_DIR "/opt/intel/openvino_2022/runtime/cmake")
find_package(OpenVINO REQUIRED COMPONENTS Runtime)
```

activate openvino env

```
source /opt/intel/openvino_2022/setupvars.sh
```


#### Install llvm clang

The project can be built use llvm clang, not use apple clang

```
brew install llvm

# double check your llvm location, modify CMakeLists.txt if needed.

set(CMAKE_C_COMPILER    /usr/local/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER  /usr/local/opt/llvm/bin/clang++)


# explict set CC and CXX, for sure
export CC=/usr/local/opt/llvm/bin/clang ; export CXX=/usr/local/opt/llvm/bin/clang++

```

#### Build conan dependency packages

```bash
conan install --build=missing --profile conan/conanprofile  -if build ./conan
```

#### Build server binary


```bash

cmake -S server -B build/server

cmake --build build/server


# make sure export some openvino runtime variable path before running.
# source /opt/intel/openvino_2022/setupvars.sh

# run the service
./build/server/bin/FaceRecognitionServer

# with config path
./build/server/bin/FaceRecognitionServer --config_path server/examples/server.json
```


### Use Makefile

Makefile wraps `cmake` instructions together.

```
# prepare all those build dirs
make build-pre

# build and run server
make build-server
make run-server


# build and run test
make build-test
make run-test

# build lib only
make build-lib
```
