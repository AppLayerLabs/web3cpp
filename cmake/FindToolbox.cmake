# Find the Toolbox library and define the following variables:
# TOOLBOX_FOUND
# TOOLBOX_INCLUDE_DIR
# TOOLBOX_LIBRARY

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

find_path(TOOLBOX_INCLUDE_DIR NAMES strings.hpp)
find_library(TOOLBOX_LIBRARY NAMES libtoolbox.a)

SELECT_LIBRARY_CONFIGURATIONS(Toolbox)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Toolbox DEFAULT_MSG
  TOOLBOX_LIBRARY TOOLBOX_INCLUDE_DIR
)

mark_as_advanced(TOOLBOX_INCLUDE_DIR TOOLBOX_LIBRARY)
