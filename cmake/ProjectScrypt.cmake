include(ExternalProject)

if (MSVC)
  set(_only_release_configuration -DCMAKE_CONFIGURATION_TYPES=Release)
endif()

set(prefix "${CMAKE_BINARY_DIR}/deps")
set(SCRYPT_LIBRARY "${prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}scrypt${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(SCRYPT_INCLUDE_DIR "${prefix}/include")

ExternalProject_Add(
  libscrypt
  PREFIX "${prefix}"
  DOWNLOAD_NO_PROGRESS 1
  GIT_REPOSITORY https://github.com/itamarcps/libscrypt
  UPDATE_DISCONNECTED true
  BUILD_IN_SOURCE true
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make PREFIX=${prefix}
  INSTALL_COMMAND make install install-static PREFIX=${prefix}
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  BUILD_BYPRODUCTS "${SCRYPT_LIBRARY}"
  DOWNLOAD_EXTRACT_TIMESTAMP 1
)

add_library(scrypt STATIC IMPORTED GLOBAL)
file(MAKE_DIRECTORY "${SCRYPT_INCLUDE_DIR}")  # Must exist.
set_property(TARGET scrypt PROPERTY IMPORTED_CONFIGURATIONS Release)
set_property(TARGET scrypt PROPERTY IMPORTED_LOCATION_RELEASE "${SCRYPT_LIBRARY}")
set_property(TARGET scrypt PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SCRYPT_INCLUDE_DIR}")

