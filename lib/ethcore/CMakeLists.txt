file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ethcore ${SOURCES} ${HEADERS})
target_include_directories(ethcore PRIVATE ${UTILS_INCLUDE_DIR})
target_link_libraries(ethcore PUBLIC devcrypto devcore PRIVATE Ethash ${ETHASH_BYPRODUCTS})
