include(ExternalProject)

if (MSVC)
  set(_only_release_configuration -DCMAKE_CONFIGURATION_TYPES=Release)
endif()

set(prefix "${CMAKE_BINARY_DIR}/deps")
set(CRYPTOPP_ROOT_RIT "${prefix}")
set(CRYPTOPP_LIBRARY "${prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cryptopp${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(CRYPTOPP_INCLUDE_DIR "${prefix}/include")

ExternalProject_Add(
  cryptopp
  PREFIX "${prefix}"
  DOWNLOAD_NAME cryptopp-8.2.0.tar.gz
  DOWNLOAD_NO_PROGRESS 1
  URL https://github.com/weidai11/cryptopp/releases/download/CRYPTOPP_8_7_0/cryptopp870.zip
  URL_HASH SHA256=d0d3a28fcb5a1f6ed66b3adf57ecfaed234a7e194e42be465c2ba70c744538dd
  UPDATE_DISCONNECTED true
  BUILD_IN_SOURCE true
  CMAKE_ARGS -DNDEBUG
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make static PREFIX=${prefix} CXXFLAGS=-std=c++${CMAKE_CXX_STANDARD} -j8
  INSTALL_COMMAND make install-lib PREFIX=${prefix}
  DOWNLOAD_EXTRACT_TIMESTAMP 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  BUILD_BYPRODUCTS "${CRYPTOPP_LIBRARY}"
)

add_library(CryptoPP STATIC IMPORTED GLOBAL)
file(MAKE_DIRECTORY "${CRYPTOPP_INCLUDE_DIR}")  # Must exist.
set_property(TARGET CryptoPP PROPERTY IMPORTED_CONFIGURATIONS Release)
set_property(TARGET CryptoPP PROPERTY IMPORTED_LOCATION_RELEASE "${CRYPTOPP_LIBRARY}")
set_property(TARGET CryptoPP PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CRYPTOPP_INCLUDE_DIR}")
add_dependencies(CryptoPP cryptopp)
