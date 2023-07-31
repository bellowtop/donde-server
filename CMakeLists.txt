cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)


project(DondeServer LANGUAGES CXX)


add_subdirectory("libs/donde-toolkits")

if (NOT DEFINED CONAN_ALREADY_SETUP)
  include(build/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
  set(CONAN_ALREADY_SETUP true)
endif()


set(CONAN_LIBs
    CONAN_PKG::grpc
    CONAN_PKG::protobuf
    CONAN_PKG::fmt
    CONAN_PKG::opencv
    CONAN_PKG::poco
    CONAN_PKG::msgpack
    CONAN_PKG::sqlitecpp
    CONAN_PKG::spdlog
    CONAN_PKG::nlohmann_json
    CONAN_PKG::cassandra-cpp-driver
    CONAN_PKG::toml11)

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
    src/feature_extract
)
target_link_libraries(feature_extract
    ${CONAN_LIBs} DondeToolkits)


######################################################
# ---- Create feature_search_manager executable ----
######################################################

file(GLOB_RECURSE feature_search_manager_headers CONFIGURE_DEPENDS src/feature_search_manager/*.h)
file(GLOB_RECURSE feature_search_manager_sources CONFIGURE_DEPENDS src/feature_search_manager/*.cc)
add_executable(feature_search_manager ${api} ${feature_search_manager_headers} ${feature_search_manager_sources})
target_include_directories(feature_search_manager
    PUBLIC
    src/feature_search_manager
)

target_link_libraries(feature_search_manager
    ${CONAN_LIBs} DondeToolkits)

######################################################
# ---- Create feature_search_worker executable ----
######################################################

file(GLOB_RECURSE feature_search_worker_headers CONFIGURE_DEPENDS src/feature_search_worker/*.h)
file(GLOB_RECURSE feature_search_worker_sources CONFIGURE_DEPENDS src/feature_search_worker/*.cc)
add_executable(feature_search_worker ${api} ${feature_search_worker_headers} ${feature_search_worker_sources})
target_include_directories(feature_search_worker
    PUBLIC
    src/feature_search_worker
)
target_link_libraries(feature_search_worker
    ${CONAN_LIBs} DondeToolkits)
