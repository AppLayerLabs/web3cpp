# Find the Scrypt library and define the following variables:
# SCRYPT_FOUND
# SCRYPT_INCLUDE_DIR
# SCRYPT_LIBRARY

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

find_path("${SCRYPT_INCLUDE_DIR}" NAMES libscrypt.h)
find_library(SCRYPT_LIBRARY NAMES libscrypt.a)

SELECT_LIBRARY_CONFIGURATIONS(Scrypt)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Scrypt DEFAULT_MSG
  SCRYPT_LIBRARY SCRYPT_INCLUDE_DIR
)

mark_as_advanced(SCRYPT_INCLUDE_DIR SCRYPT_LIBRARY)
