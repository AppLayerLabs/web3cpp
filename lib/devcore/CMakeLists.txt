file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(devcore ${SOURCES} ${HEADERS})

target_link_libraries(devcore
  PUBLIC
    web3cpp-buildinfo ${BOOST_LIBS} ${QT_LIBS}
  PRIVATE
    Ethash ${ETHASH_BYPRODUCTS} bip39 toolbox
)
