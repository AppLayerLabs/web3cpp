file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(ledger ${SOURCES} ${HEADERS})
target_include_directories(ledger PRIVATE ${UTILS_INCLUDE_DIR})
target_include_directories(ledger PRIVATE ${CMAKE_SOURCE_DIR}/build/_deps/json-src/single_include)
target_link_libraries(ledger PUBLIC devcrypto devcore ethcore PRIVATE Ethash ${ETHASH_BYPRODUCTS} nlohmann_json::nlohmann_json)
