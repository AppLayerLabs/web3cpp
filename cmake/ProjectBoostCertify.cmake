include(ExternalProject)

if (MSVC)
    set(_only_release_configuration -DCMAKE_CONFIGURATION_TYPES=Release)
    set(_overwrite_install_command INSTALL_COMMAND cmake --build <BINARY_DIR> --config Release --target install)
endif()

set(prefix "${CMAKE_BINARY_DIR}/deps")
set(CERTIFY_LIBRARY "${prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}certify${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(CERTIFY_INCLUDE_DIR "${prefix}/include")

ExternalProject_Add(
  certify
  PREFIX "${prefix}"
  URL https://github.com/djarek/certify/archive/2d719a9ad79ce1a61684278a30196527e412a0b6.tar.gz
  URL_HASH SHA256=2b17c26df61994ce0c3697b952198235d2af83484a30521d9005a96e6eadbc8e
  PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/cmake/certifyPatch.patch
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
             -DCMAKE_POSITION_INDEPENDENT_CODE=${BUILD_SHARED_LIBS}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             ${_only_release_configuration}
             ${_windows_configuration}
             -DCMAKE_INSTALL_LIBDIR=lib
  LOG_CONFIGURE 1
  BUILD_COMMAND ""
  ${_overwrite_install_command}
  LOG_INSTALL 1
  BUILD_BYPRODUCTS "${CERTIFY_BYPRODUCTS}"
)

# Create imported library
add_library(Certify STATIC IMPORTED)
file(MAKE_DIRECTORY "${CERTIFY_INCLUDE_DIR}")  # Must exist.
set_property(TARGET Certify PROPERTY IMPORTED_CONFIGURATIONS Release)
set_property(TARGET Certify PROPERTY IMPORTED_LOCATION_RELEASE "${CERTIFY_LIBRARY}")
set_property(TARGET Certify PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CERTIFY_INCLUDE_DIR}")
add_dependencies(Certify certify ${CERTIFY_BYPRODUCTS})
