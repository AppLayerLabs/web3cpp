# Pre-setup
cmake_minimum_required(VERSION 3.19.0)
include(FetchContent)

# ===========================
# OLDLIB - REMOVE WHEN DONE
include(cmake/cable/bootstrap.cmake)
include(CableBuildInfo)
include(CableBuildType)
include(GNUInstallDirs)
cable_set_build_type(DEFAULT RelWithDebInfo CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)

# Map current configuration to configurations of imported targets.
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release)
set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

# Get Hunter
include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.288.tar.gz"
  SHA1 "6c9b2bc606d86ae31f96a62fc68f0a593024815b"
  LOCAL # Load config from `${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/config.cmake`
)
# ===========================

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

# ===========================
# OLDLIB - REMOVE WHEN DONE
cable_add_buildinfo_library(PROJECT_NAME web3cpp)
set(DEPENDS_PREFIX "")
if("${TOOLCHAIN_PREFIX}" STREQUAL "x86_64-w64-mingw32")
  set(DEPENDS_PREFIX "x86_64-w64-mingw32")
  message("Toolchain: x86_64-w64-mingw32")
  message("Loading Windows Libraries")
  include("${CMAKE_SOURCE_DIR}/cmake/WindowsLibraries.cmake")
else()
  if("${TOOLCHAIN_PREFIX}" STREQUAL "x86_64-apple-darwin20")
    message("Toolchain: x86_64-apple-darwin20")
    set(DEPENDS_PREFIX "x86_64-apple-darwin20.5.0")
    message("Loading Darwin Libraries")
    include("${CMAKE_SOURCE_DIR}/cmake/DarwinLibraries.cmake")
  else()
    if("${TOOLCHAIN_PREFIX}" STREQUAL "arm-apple-darwin21.2.0")
      message("Toolchain: arm-apple-darwin21.2.0")
      set(DEPENDS_PREFIX "arm-apple-darwin21.2.0")
      include("${CMAKE_SOURCE_DIR}/cmake/DarwinArmLibraries.cmake")
      message("Loading Darwin Libraries")
    else()
      message("Toolchain: x86_64-pc-linux-gnu")
      set(DEPENDS_PREFIX "x86_64-pc-linux-gnu")
      include("${CMAKE_SOURCE_DIR}/cmake/LinuxLibraries.cmake")
      message("Loading Linux Libraries")
    endif()
  endif()
endif()

# Add Hunter packages
hunter_add_package(cryptopp)
find_package(cryptopp CONFIG REQUIRED)
hunter_add_package(libscrypt)
find_package(libscrypt CONFIG REQUIRED)

# Add external modules
include(ProjectEthash)
include(ProjectSecp256k1)
include(ProjectToolbox)
include(ProjectBip3x)
find_package(Threads)

# Include headers and libs
include_directories(
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/build"
  "${CMAKE_SOURCE_DIR}/build/deps/include"
)
link_directories(
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/build"
  "${CMAKE_SOURCE_DIR}/build/deps/lib"
)

# Add local libs
add_subdirectory(lib/devcore)
add_subdirectory(lib/devcrypto)
add_subdirectory(lib/ethcore)
add_subdirectory(lib/ledger)
# ===========================

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
#target_link_libraries(${PROJECT_NAME} PRIVATE Boost nlohmann_json::nlohmann_json)
# OLDLIB - REMOVE WHEN DONE
target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost nlohmann_json::nlohmann_json
  devcore devcrypto ethcore ledger
)

# Set required properties
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "src/Web3.h")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX)

