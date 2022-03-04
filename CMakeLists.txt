# Pre-setup
cmake_minimum_required(VERSION 3.19.0)

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

# Organize sources and headers
file(GLOB WEB3CPP_HEADERS "src/*.h")

# Declare library target
if(BUILD_STATIC)
  add_library(${PROJECT_NAME} STATIC
    ${WEB3CPP_HEADERS}
  )
else()
  add_library(${PROJECT_NAME} SHARED
    ${WEB3CPP_HEADERS}
  )
endif()

# Configure directories to search for header files
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Set required properties
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "src/Web3.h")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

