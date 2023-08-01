# Donde server

It's a group of different services

* image feature extraction
* feature searching & clustering
* video processing

Built on top of [donde-toolkits](https://github.com/sekirocc/donde-toolkits)


## Features

- Openvino inference
- Full pipeline detect/landmarks/align/extract
- Conan to manage dependencies
- Protobuf proto & GRPC server
- GRPC & GRPC gateway integrated

## Dependencies

For the big ones: openvino/opencv/ffmpeg, other small depencencies are managed by `conan/conanfile.txt`.

### Models

| name                          | type      | desc                 | urldesc                                                                                                                  |
|-------------------------------|-----------|----------------------|--------------------------------------------------------------------------------------------------------------------------|
| face-detection-adas-0001      | detect    | detect face          | https://github.com/openvinotoolkit/open_model_zoo/blob/master/models/intel/face-detection-adas-0001/README.md  |
| facial-landmarks-35-adas-0002 | landmarks | 70 points landmarks  | https://github.com/openvinotoolkit/open_model_zoo/blob/master/models/intel/facial-landmarks-35-adas-0002/README.md       |
| Sphereface.xml                | feature   | extract face feature | https://github.com/openvinotoolkit/open_model_zoo/blob/master/models/public/Sphereface/README.md                         |



## Project layout


* `proto` contains protobuf definitions for common api, grpc services definitions. servers are typically implementations of these grpc services.

* `api` proto files are compiled to api directory.

* `src` has source codes to build various standalone server binaries.

* `tests` contains tests, they test the `FeatureLibraries` library. tests has dependency to library.

* `contrib` contains json samples, static assets etc.


## Usage


### Build and run

#### Install dependencies

macOS

```
brew install openvino
brew install ffmpeg
brew install opencv
```

other platform openvino installation guide, please refers to `https://www.intel.com/content/www/us/en/developer/tools/openvino-toolkit/download.html`

```
find_package(OpenVINO REQUIRED COMPONENTS Runtime)
```

#### Build conan dependency packages

```
mkdir -p build
conan install --build=missing --profile conan/conanprofile  -if build ./conan
```

#### Build


```

cmake -B build
cmake --build build

#run the service
./build/bin/FaceRecognitionServer

#with config path
./build/bin/FaceRecognitionServer --config_path server/examples/server.json
```



### Build proto

```
cd proto

#build protobuf definitions of grpc and service
./build_proto.sh

#build grpc - http gateway, for http api[optional]
./build_gateway.sh

```


## Xcode

```
mkdir -p xcode/server
cd xcode/server
cmake -G Xcode ../../server
```

then use xcode to open `FaceRecognitionServer.xcodeproj`, try build and run.



### FAQ
Q:
CMake generate failed with message `xcode-select: error: tool 'xcodebuild' requires Xcode, but active developer directory '/Library/Developer/CommandLineTools' is a command line tools instance`

A:
Run `sudo xcode-select -s /Applications/Xcode.app/Contents/Developer`, see also `https://github.com/nodejs/node-gyp/issues/569`.


Q:
Build failed with message `"Protobuf requires at least C++11."`

A:
Navigate to `TARGETS -> API`, find `Apple Clang - Language - C++`, change `C++ Language Dialect` to `C++17[-std=c++17]`


Q:
Run failed with message `dyld[4593]: Library not loaded: @rpath/libopenvinod.dylib`

A:

It's because we need `source /opt/intel/openvino_2022/setupvars.sh`, but xcode doesn't know that.
Edit `FaceRecognitionServer` scheme, add environment variable `DYLD_LIBRARY_PATH`, value `/opt/intel/openvino_2022/runtime/3rdparty/tbb/lib::/opt/intel/openvino_2022/runtime/lib/intel64/Release:/opt/intel/openvino_2022/runtime/lib/intel64/Debug`


A2:
```
for i in `ls /opt/intel/openvino_2022/runtime/lib/intel64/Debug/`; do sudo ln -s /opt/intel/openvino_2022/runtime/lib/intel64/Debug/$i /usr/local/lib; done
for i in `ls /opt/intel/openvino_2022/runtime/3rdparty/tbb/lib/`; do sudo ln -s /opt/intel/openvino_2022.1.0.643/runtime/3rdparty/tbb/lib/$i /usr/local/lib; done
```


## Mac M1

[README mac m1](./README_mac_m1.md)
