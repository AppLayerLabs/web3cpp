file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(devcrypto ${SOURCES} ${HEADERS})
target_include_directories(devcrypto PRIVATE ${UTILS_INCLUDE_DIR})
target_link_libraries(devcrypto
  PUBLIC
    devcore Secp256k1 cryptopp-static
  PRIVATE
    libscrypt::scrypt
)

