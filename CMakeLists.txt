# Pre-setup
cmake_minimum_required(VERSION 3.19.0)
include(FetchContent)

# Project data
project(web3cpp VERSION 0.0.1 DESCRIPTION "Implementation of Web3.js in C++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_STATIC ON CACHE BOOL "Build library as static, turn off to build as shared")
message("C++ Standard: ${CMAKE_CXX_STANDARD}")
message("C++ Standard is required: ${CMAKE_CXX_STANDARD_REQUIRED}")
message("C++ extensions: ${CMAKE_CXX_EXTENSIONS}")
message("Using PIC: ${CMAKE_POSITION_INDEPENDENT_CODE}")
message("Building as static: ${BUILD_STATIC}")

# Fetch and set up external libraries
# TODO: add options to set boost path
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost CONFIG REQUIRED filesystem system thread)
message("Fetching nlohmann/json...")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

# Organize sources and headers
file(GLOB WEB3CPP_HEADERS "include/web3cpp/*.h")
file(GLOB WEB3CPP_SOURCES "src/*.cpp")

# Declare library target
if(BUILD_STATIC)
  add_library(${PROJECT_NAME} STATIC
    ${WEB3CPP_HEADERS} ${WEB3CPP_SOURCES}
  )
else()
  add_library(${PROJECT_NAME} SHARED
    ${WEB3CPP_HEADERS} ${WEB3CPP_SOURCES}
  )
endif()

# Configure directories to search for header files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link project to dependencies and external libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Boost nlohmann_json::nlohmann_json)

# Set required properties
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "src/Web3.h")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX)

