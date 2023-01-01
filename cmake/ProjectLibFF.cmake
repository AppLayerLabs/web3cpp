# Aleth: Ethereum C++ client, tools and libraries.
# Copyright 2017-2019 Aleth Authors.
# Licensed under the GNU General Public License, Version 3.
include(ProjectMPIR)

set(prefix "${CMAKE_BINARY_DIR}/deps")
set(LIBFF_LIBRARY "${prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ff${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(LIBFF_INCLUDE_DIR "${prefix}/include/libff")

ExternalProject_Add(libff
    PREFIX "${prefix}"
    DOWNLOAD_NAME libff-03b719a7.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
    URL https://github.com/scipr-lab/libff/archive/03b719a7c81757071f99fc60be1f7f7694e51390.tar.gz
    URL_HASH SHA256=81b476089af43025c8f253cb1a9b5038a1c375baccffea402fa82042e608ab02
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DGMP_INCLUDE_DIR=${MPIR_INCLUDE_DIR}
        -DGMP_LIBRARY=${MPIR_LIBRARY}
        -DCURVE=ALT_BN128 -DPERFORMANCE=Off -DWITH_PROCPS=Off
        -DUSE_PT_COMPRESSION=Off
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=-I${LIBFF_INCLUDE_DIR}\ ${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_INSTALL_LIBDIR=lib
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release
    LOG_BUILD 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release --target install
    BUILD_BYPRODUCTS "${LIBFF_LIBRARY}"
)
add_dependencies(libff mpir)

# Create snark imported library
add_library(libff::ff STATIC IMPORTED)
file(MAKE_DIRECTORY ${LIBFF_INCLUDE_DIR})
set_property(TARGET libff::ff PROPERTY IMPORTED_CONFIGURATIONS Release)
set_property(TARGET libff::ff PROPERTY IMPORTED_LOCATION_RELEASE ${LIBFF_LIBRARY})
set_property(TARGET libff::ff PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBFF_INCLUDE_DIR})
set_property(TARGET libff::ff PROPERTY INTERFACE_LINK_LIBRARIES MPIR::mpir)
add_dependencies(libff::ff libff)
