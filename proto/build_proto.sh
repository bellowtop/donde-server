#! /bin/bash

set -x

protoc_path=$(find ~/.conan2/p/b/proto53d5913d12ef9/p/bin/protoc -name "protoc" | head -n 1)
grpc_cpp_plugin_path=$(find ~/.conan2/p/b/grpc5e2fbfd35bdd6 -name "grpc_cpp_plugin"  | head -n 1)

googleapis_path=$(find ~/.conan2/p/b/googl6d5072b6fb0ca/b/src -name "google" -type d -maxdepth 1)/..
# like ~/.conan2/p/b/googl6d5072b6fb0ca/b/build/Debug/google

mkdir -p ./gen/pb-cpp/
mkdir -p ../api

${protoc_path} -I${googleapis_path} -I. \
   --cpp_out=../api \
   common.proto

${protoc_path} -I${googleapis_path} -I. \
   --cpp_out=../api \
   --grpc_out=../api \
   --plugin=protoc-gen-grpc=${grpc_cpp_plugin_path} \
   feature_extract.proto

${protoc_path} -I${googleapis_path} -I. \
   --cpp_out=../api \
   --grpc_out=../api \
   --plugin=protoc-gen-grpc=${grpc_cpp_plugin_path} \
   feature_search.proto

${protoc_path} -I${googleapis_path} -I. \
   --cpp_out=../api \
   --grpc_out=../api \
   --plugin=protoc-gen-grpc=${grpc_cpp_plugin_path} \
   feature_search_inner.proto
