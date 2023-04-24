# Find the LevelDB library and define the following variables:
# LEVELDB_FOUND
# LEVELDB_INCLUDE_DIR
# LEVELDB_LIBRARY

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

find_path(LEVELDB_INCLUDE_DIR NAMES leveldb/db.h)
find_library(LEVELDB_LIBRARY NAMES libleveldb.a)

SELECT_LIBRARY_CONFIGURATIONS(LevelDB)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  LevelDB DEFAULT_MSG
  LEVELDB_LIBRARY LEVELDB_INCLUDE_DIR
)

mark_as_advanced(LEVELDB_INCLUDE_DIR LEVELDB_LIBRARY)
