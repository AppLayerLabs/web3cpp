# Pre-setup
cmake_minimum_required(VERSION 3.19.0)
include(FetchContent)

# Map current configuration to configurations of imported targets.
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release)
set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

# Include cable
include(cmake/cable/bootstrap.cmake)
include(CableBuildType)
include(CableCompilerSettings)
include(CablePackage)
include(CableToolchains)
include(CMakePackageConfigHelpers)
include(CableBuildInfo)

# Setup CXX

cable_configure_toolchain(DEFAULT cxx17-pic)
cable_set_build_type(DEFAULT RelWithDebInfo CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)

# Project data
project(web3cpp VERSION 0.0.1 DESCRIPTION "Custom implementation of Web3 in C++")
cable_add_buildinfo_library(PROJECT_NAME web3cpp)
set(BUILD_STATIC ON CACHE BOOL "Build library as static, turn off to build as shared")
set(BUILD_TESTS ON CACHE BOOL "Build helper unit testing program")
message("C++ Standard: ${CMAKE_CXX_STANDARD}")
message("C++ Standard is required: ${CMAKE_CXX_STANDARD_REQUIRED}")
message("C++ extensions: ${CMAKE_CXX_EXTENSIONS}")
message("Using PIC: ${CMAKE_POSITION_INDEPENDENT_CODE}")
message("Building as static: ${BUILD_STATIC}")
message("Building tests: ${BUILD_TESTS}")

# Fetch and set up external libraries
# As this is a library, it checks if it is being a externalProject added with multiple DEPENDS
# If it is, it doens't need to include the Project .cmake files, as the libraries are being brought by the parent project.
include(CheckLibraryExists)
set(OPENSSL_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/deps") # "deps", not "_deps"
find_package(Boost REQUIRED filesystem system thread)
find_package(OpenSSL REQUIRED)
message("Fetching nlohmann/json...")
FetchContent_Declare(json 
                      URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
                      DOWNLOAD_EXTRACT_TIMESTAMP 1)
FetchContent_MakeAvailable(json)


# Add external modules

find_package(CryptoPP)
if(NOT CRYPTOPP_FOUND)
  message(WARNING "CryptoPP Not found, compiling from source.")
  include(ProjectCryptoPP)
  find_package(CryptoPP)
endif()

find_package(Scrypt)
if (NOT SCRYPT_FOUND)
  message(WARNING "Scrypt not found, compiling from source.")
  include(ProjectScrypt)
  find_package(Scrypt)
endif()

find_package(Ethash)
if(NOT ETHASH_FOUND)
  message(WARNING "Ethash not found, compiling from source.")
  include(ProjectEthash)
  find_package(Ethash)
endif()

find_package(Secp256k1)
if(NOT SECP256K1_FOUND)
  message(WARNING "Secp256k1 not found, compiling from source.")
  include(ProjectSecp256k1)
  find_package(Secp256k1)
endif()

find_package(Toolbox)
if(NOT TOOLBOX_FOUND)
  message(WARNING "Toolbox not found, compiling from source.")
  include(ProjectToolbox)
  find_package(Toolbox)
endif()

find_package(Bip3x)
if(NOT BIP3X_FOUND)
  message(WARNING "Bip3x not found, compiling from source.")
  include(ProjectBip3x)
  find_package(Bip3x)
endif()

find_package(Snappy)
if(NOT SNAPPY_FOUND)
  message(WARNING "Snappy not found, compiling from source.")
  include(ProjectSnappy)
  find_package(Snappy)
endif()

find_package(LevelDB)
if(NOT LEVELDB_FOUND)
  message(WARNING "LevelDB not found, compiling from source.")
  include(ProjectLevelDB)
  find_package(LevelDB)
endif()

find_package(BoostCertify)
if (NOT BOOSTCERTIFY_FOUND)
  message(WARNING "BoostCertify not found, compiling from source.")
  include(ProjectBoostCertify)
  find_package(BoostCertify)
endif()

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

# Set the project version as a macro in a header file
configure_file(
  "${CMAKE_SOURCE_DIR}/include/web3cpp/version.h.in"
  "${CMAKE_SOURCE_DIR}/include/web3cpp/version.h" @ONLY
)

# Organize sources and headers
file(GLOB WEB3CPP_HEADERS "include/web3cpp/*.h")
file(GLOB WEB3CPP_SOURCES "src/*.cpp")
file(GLOB DEVCORE_HEADERS "include/web3cpp/devcore/*.h")
file(GLOB DEVCORE_SOURCES "src/devcore/*.cpp")
file(GLOB DEVCRYPTO_HEADERS "include/web3cpp/devcrypto/*.h")
file(GLOB DEVCRYPTO_SOURCES "src/devcrypto/*.cpp")
file(GLOB ETHCORE_HEADERS "include/web3cpp/ethcore/*.h")
file(GLOB ETHCORE_SOURCES "src/ethcore/*.cpp")
file(GLOB LEDGER_HEADERS "include/web3cpp/ledger/*.h")
file(GLOB LEDGER_SOURCES "src/ledger/*.cpp")

# Build the project
if(BUILD_STATIC)
  add_library(${PROJECT_NAME} STATIC
    ${WEB3CPP_HEADERS} ${WEB3CPP_SOURCES}
    ${DEVCORE_HEADERS} ${DEVCORE_SOURCES}
    ${DEVCRYPTO_HEADERS} ${DEVCRYPTO_SOURCES}
    ${ETHCORE_HEADERS} ${ETHCORE_SOURCES}
    ${LEDGER_HEADERS} ${LEDGER_SOURCES}
  )
else()
  add_library(${PROJECT_NAME} SHARED
    ${WEB3CPP_HEADERS} ${WEB3CPP_SOURCES}
    ${DEVCORE_HEADERS} ${DEVCORE_SOURCES}
    ${DEVCRYPTO_HEADERS} ${DEVCRYPTO_SOURCES}
    ${ETHCORE_HEADERS} ${ETHCORE_SOURCES}
    ${LEDGER_HEADERS} ${LEDGER_SOURCES}
  )
endif()

# Configure directories to search for header files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
# For linking openssl on MacOS
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})


# Link project to dependencies and external libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
  ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} nlohmann_json::nlohmann_json cryptopp scrypt
  secp256k1 bip3x toolbox ${ETHASH_BYPRODUCTS} ethash ${LEVELDB_LIBRARY} ${SNAPPY_LIBRARY}
)

# For certify to work on MacOS
if(APPLE)
    target_link_libraries(${PROJECT_NAME} INTERFACE "-framework CoreFoundation" "-framework Security")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
endif()

# Build helper test program
if(BUILD_TESTS)
  file(GLOB TESTS_HEADERS "tests/*.h")
  file(GLOB TESTS_SOURCES "tests/*.cpp")
  add_executable(${PROJECT_NAME}-tests ${TESTS_HEADERS} ${TESTS_SOURCES})
  target_include_directories(${PROJECT_NAME}-tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_include_directories(${PROJECT_NAME}-tests PRIVATE ${OPENSSL_INCLUDE_DIR})
  # For certify to work on MacOS
  if(APPLE)
    target_link_libraries(${PROJECT_NAME}-tests INTERFACE "-framework CoreFoundation" "-framework Security")
    set_target_properties(${PROJECT_NAME}-tests PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
  endif()
  target_link_libraries(${PROJECT_NAME}-tests PUBLIC ${PROJECT_NAME})
endif()

# Set required properties
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "include/web3cpp/Web3.h")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX)

install(TARGETS web3cpp
  EXPORT web3cppTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION include)
