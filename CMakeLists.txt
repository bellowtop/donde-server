cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)


project(DondeServer LANGUAGES CXX)


if (NOT DEFINED CONAN_ALREADY_SETUP)
  include(build/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
  set(CONAN_ALREADY_SETUP true)
endif()


set(CONAN_LIBs
    CONAN_PKG::grpc
    CONAN_PKG::protobuf
    CONAN_PKG::fmt
    CONAN_PKG::poco
    CONAN_PKG::msgpack
    CONAN_PKG::sqlitecpp
    CONAN_PKG::spdlog
    CONAN_PKG::cxxopts
    CONAN_PKG::nlohmann_json
    CONAN_PKG::cassandra-cpp-driver
    CONAN_PKG::toml11)

find_package(OpenVINO REQUIRED COMPONENTS Runtime)
message("OpenVINO_FOUND: ${OpenVINO_FOUND}")
message("OpenVINO_Runtime_FOUND: ${OpenVINO_Runtime_FOUND}")
find_package(OpenCV REQUIRED CONFIG)

set(DondeToolkits_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../donde-toolkits/install/cmake)
find_package(DondeToolkits REQUIRED)

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
)
target_link_libraries(feature_extract
    ${CONAN_LIBs}
    donde::DondeToolkits
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
)

target_link_libraries(feature_search_manager
    ${CONAN_LIBs}
    donde::DondeToolkits
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
)
target_link_libraries(feature_search_worker
    ${CONAN_LIBs}
    donde::DondeToolkits
)
