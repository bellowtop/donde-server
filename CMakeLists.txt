cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)


project(DondeServer LANGUAGES CXX)



##
## Conan
##

find_package(cxxopts)
find_package(Poco)
find_package(gRPC)
find_package(googleapis)
find_package(protobuf)
find_package(libuuid)
find_package(msgpack)
find_package(SQLiteCpp)
find_package(cassandra-cpp-driver)
find_package(OpenSSL)
find_package(ZLIB)
find_package(toml11)
find_package(nlohmann_json)
find_package(doctest)
find_package(GTest)
find_package(trompeloeil)
find_package(spdlog)



set(CONAN_LIBs
	cxxopts::cxxopts
	Poco::Poco
    grpc::grpc
    googleapis::googleapis
    protobuf::protobuf
    libuuid::libuuid
    msgpack-cxx
    SQLiteCpp
    cassandra-cpp-driver::cassandra-cpp-driver
	openssl::openssl
	ZLIB::ZLIB
	toml11::toml11
	nlohmann_json::nlohmann_json
	doctest::doctest
	gtest::gtest
	trompeloeil::trompeloeil
	spdlog::spdlog
)





##
## OpenVINO library
##

find_package(OpenVINO REQUIRED COMPONENTS Runtime)
message("OpenVINO_FOUND: ${OpenVINO_FOUND}")
message("OpenVINO_Runtime_FOUND: ${OpenVINO_Runtime_FOUND}")

find_package(OpenCV REQUIRED CONFIG)
message("OpenCV_LIBS: ${OpenCV_LIBS}")







##
## Find DondeToolkits
##
set(DONDE_TOOLKITS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../donde-toolkits/install/)

set(DONDE_LIBs
    ${DONDE_TOOLKITS_PATH}/libfeature_extract.a
    ${DONDE_TOOLKITS_PATH}/libfeature_search.a
    ${DONDE_TOOLKITS_PATH}/libvideo_process.a
)

#########################################################
# api
#########################################################

file(GLOB_RECURSE api CONFIGURE_DEPENDS api/*)

######################################################
# ---- Create feature_extract executable ----
######################################################

file(GLOB_RECURSE feature_extract_headers CONFIGURE_DEPENDS src/feature_extract/*.h)
file(GLOB_RECURSE feature_extract_sources CONFIGURE_DEPENDS src/feature_extract/*.cc)
add_executable(feature_extract ${api} ${feature_extract_headers} ${feature_extract_sources})
target_include_directories(feature_extract
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/feature_extract
    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${DONDE_TOOLKITS_PATH}/include
    ${CMAKE_INCLUDE_PATH}
)
target_link_libraries(feature_extract
	PRIVATE
    ${CONAN_LIBs}
	${DONDE_LIBs}
    ${OpenCV_LIBS}
	openvino::runtime
)


######################################################
# ---- Create feature_search_manager executable ----
######################################################

file(GLOB_RECURSE feature_search_manager_headers CONFIGURE_DEPENDS src/feature_search_manager/*.h)
file(GLOB_RECURSE feature_search_manager_sources CONFIGURE_DEPENDS src/feature_search_manager/*.cc)
add_executable(feature_search_manager ${api} ${feature_search_manager_headers} ${feature_search_manager_sources})
target_include_directories(feature_search_manager
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/feature_search_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${DONDE_TOOLKITS_PATH}/include
    ${CMAKE_INCLUDE_PATH}
)

target_link_libraries(feature_search_manager
	PRIVATE
    ${CONAN_LIBs}
	${DONDE_LIBs}
    ${OpenCV_LIBS}
	openvino::runtime
)

######################################################
# ---- Create feature_search_worker executable ----
######################################################

file(GLOB_RECURSE feature_search_worker_headers CONFIGURE_DEPENDS src/feature_search_worker/*.h)
file(GLOB_RECURSE feature_search_worker_sources CONFIGURE_DEPENDS src/feature_search_worker/*.cc)
add_executable(feature_search_worker ${api} ${feature_search_worker_headers} ${feature_search_worker_sources})
target_include_directories(feature_search_worker
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/feature_search_worker
    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${DONDE_TOOLKITS_PATH}/include
    ${CMAKE_INCLUDE_PATH}
)
target_link_libraries(feature_search_worker
	PRIVATE
    ${CONAN_LIBs}
	${DONDE_LIBs}
    ${OpenCV_LIBS}
	openvino::runtime
)
