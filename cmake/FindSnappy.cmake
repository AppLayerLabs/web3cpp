# Find the Snappy library and define the following variables:
# SNAPPY_FOUND
# SNAPPY_INCLUDE_DIR
# SNAPPY_LIBRARY

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

find_path(SNAPPY_INCLUDE_DIR NAMES snappy.h)
find_library(SNAPPY_LIBRARY NAMES libsnappy.a)

SELECT_LIBRARY_CONFIGURATIONS(Snappy)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Snappy DEFAULT_MSG
  SNAPPY_LIBRARY SNAPPY_INCLUDE_DIR
)

mark_as_advanced(SNAPPY_INCLUDE_DIR SNAPPY_LIBRARY)
