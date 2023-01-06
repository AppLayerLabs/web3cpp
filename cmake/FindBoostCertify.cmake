# Find the BoostCertify library and define the following variables:
# BOOSTCERTIFY_FOUND
# BOOSTCERTIFY_INCLUDE_DIR
# BOOSTCERTIFY_LIBRARY

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

find_path(BOOSTCERTIFY_INCLUDE_DIR NAMES boost/certify/https_verification.hpp)
find_library(BOOSTCERTIFY_LIBRARY NAMES libboostcertify.a)

SELECT_LIBRARY_CONFIGURATIONS(BoostCertify)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  BoostCertify DEFAULT_MSG
  BOOSTCERTIFY_LIBRARY BOOSTCERTIFY_INCLUDE_DIR
)

mark_as_advanced(BOOSTCERTIFY_INCLUDE_DIR BOOSTCERTIFY_LIBRARY)
