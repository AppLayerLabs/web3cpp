# Find the Bip3x library and define the following variables:
# BIP3X_FOUND
# BIP3X_INCLUDE_DIR
# BIP3X_LIBRARY

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

find_path(BIP3X_INCLUDE_DIR NAMES bip3x/bip39.h)
find_library(BIP3X_LIBRARY NAMES libbip39.a)

SELECT_LIBRARY_CONFIGURATIONS(Bip3x)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Bip3x DEFAULT_MSG
  BIP3X_LIBRARY BIP3X_INCLUDE_DIR
)

mark_as_advanced(BIP3X_INCLUDE_DIR BIP3X_LIBRARY)
