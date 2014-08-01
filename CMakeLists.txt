# This is the root ITK CMakeList file:
cmake_minimum_required(VERSION 2.8)

# This project is designed to be built outside the Insight source tree.
project(VTKCompression)

# Find SlicerExecutionModel
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

# Find ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# Find VTK
find_package(VTK 6.1 REQUIRED NO MODULE)
include(${VTK_USE_FILE})

SEMMacroBuildCLI(
  NAME VTKCompression
  EXECUTABLE_ONLY
  TARGET_LIBRARIES ${ITK_LIBRARIES} ${VTK_LIBRARIES}
  LINK_DIRECTORIES ${ITK_LIBRARY_DIRS} ${VTK_LIBRARY_DIRS}
  INCLUDE_DIRECTORIES ${ITK_INCLUDE_DIR} ${VTK_INCLUDE_DIR}
  )

option(BUILD_TESTING "Build the testing tree" ON)

IF(BUILD_TESTING)
  include(CTest)
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)
