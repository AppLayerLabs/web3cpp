include(ExternalProject)

if (MSVC)
  set(_only_release_configuration -DCMAKE_CONFIGURATION_TYPES=Release)
endif()

set(prefix "${CMAKE_BINARY_DIR}/deps")
set(CRYPTOPP_LIBRARY "${prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cryptopp${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(CRYPTOPP_INCLUDE_DIR "${prefix}/include")

ExternalProject_Add(
  CryptoPP
  PREFIX "${prefix}"
  DOWNLOAD_NAME cryptopp-8.2.0.tar.gz
  DOWNLOAD_NO_PROGRESS 1
  URL https://github.com/weidai11/cryptopp/archive/refs/tags/CRYPTOPP_8_2_0.tar.gz
  URL_HASH SHA256=e3bcd48a62739ad179ad8064b523346abb53767bcbefc01fe37303412292343e
  UPDATE_DISCONNECTED true
  BUILD_IN_SOURCE true
  CMAKE_ARGS -DNDEBUG
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make static PREFIX=${prefix} CXXFLAGS=-std=c++${CMAKE_CXX_STANDARD} -j4
  INSTALL_COMMAND make install-lib PREFIX=${prefix}
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  BUILD_BYPRODUCTS "${CRYPTOPP_LIBRARY}"
)

add_library(cryptopp STATIC IMPORTED)
file(MAKE_DIRECTORY "${CRYPTOPP_INCLUDE_DIR}")  # Must exist.
set_property(TARGET cryptopp PROPERTY IMPORTED_CONFIGURATIONS Release)
set_property(TARGET cryptopp PROPERTY IMPORTED_LOCATION_RELEASE "${CRYPTOPP_LIBRARY}")
set_property(TARGET cryptopp PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CRYPTOPP_INCLUDE_DIR}")

