cmake_minimum_required(VERSION 3.20)

# If you installed gRPC and Protocol Buffers through vcpkg,
# you need to specify the full path to the vcpkg toolchain file.
# set(CMAKE_TOOLCHAIN_FILE "your/path/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE "/home/kercoza/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

project(app 
    VERSION 1.0.0
    DESCRIPTION "Codeforces API client with gRPC support"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CompilerOptions)
include(Dependencies)
include(SourceFiles)
include(Testing)
include(Protobuf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(ENABLE_STATIC_ANALYSIS "Enable static analysis" OFF)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)

setup_protobuf_files()

collect_sources(PROJECT_SOURCES)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${APP_PROTO_TARGET})
add_dependencies(${PROJECT_NAME} ${APP_PROTO_TARGET})

setup_include_directories(${PROJECT_NAME})
setup_all_dependencies(${PROJECT_NAME})
set_compiler_options(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "codeforces_client"
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "=== Build Configuration ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "gRPC cpp plugin location: ${GRPC_CPP_PLUGIN_LOCATION}")
message(STATUS "Proto files: ${PROTO_FILES}")
message(STATUS "===========================")

add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "Cleaning all build artifacts"
)

include(Package)