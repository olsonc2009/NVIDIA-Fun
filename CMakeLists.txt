cmake_minimum_required( VERSION 2.5 )

#
# Added so I don't have to see a warning I can do nothing about w.r.t. gtest stuff
#
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project( NVIDIA-Fun )

#
# Find gtest to make sure the unit tests are rolling
#
enable_testing()
find_package( GTest )

#
# Set variables for use in lower subdirectories
#
SET( BIN_DIR ${CMAKE_CURRENT_LIST_DIR}/bin )

#
# Add the subdirectories to be compiled
#
add_subdirectory( src )
