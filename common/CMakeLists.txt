cmake_minimum_required(VERSION 2.6)
project(argos2_common)

#
# User compilation config, if it exists
#
if(EXISTS ${CMAKE_SOURCE_DIR}/../user/my_user/cmake/user_config.cmake)
  include(${CMAKE_SOURCE_DIR}/../user/my_user/cmake/user_config.cmake)
endif(EXISTS ${CMAKE_SOURCE_DIR}/../user/my_user/cmake/user_config.cmake)  

#
# Check for GCC version >= 4.2
#
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.2 OR GCC_VERSION VERSION_EQUAL 4.2)
  message(STATUS "GCC/G++ version >= 4.2")
else()
  message(FATAL_ERROR "You need to have at least GCC/G++ version 4.2!")
endif()

#
# Configuration for CPack's tar.bz2 generator
#
set(CPACK_GENERATOR TBZ2)
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ARGoS2 common package")
include(CPack)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#
# More cmake configuration flags to optimize code
#
include(${CMAKE_SOURCE_DIR}/cmake/SimulationConfig.cmake)
include_directories(${CMAKE_SOURCE_DIR}/..)

#
# This is required for TinyXML++ to work
#
add_definitions(-DTIXML_USE_TICPP)

#
# Target-dependent compilation
#
if( ARGOS_COMPILE_REAL_SWARMANOID )

  #
  # Compiling for the Swarmanoid robot platform
  #
  set(ARGOSINSTALLDIR $ENV{ARGOSINSTALLDIR})
  include(${CMAKE_SOURCE_DIR}/../real_robot/swarmanoid/cmake/moboard.cmake)
  add_definitions(-DCROSSCOMPILING)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/EclipseRealSwarmanoidProjectFile.xml ${CMAKE_BINARY_DIR}/.project)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/EclipseRealSwarmanoidCProjectFile.xml ${CMAKE_BINARY_DIR}/.cproject)

elseif( ARGOS_COMPILE_REAL_EPUCK )

  #
  # Compiling for the e-puck robot platform
  #
  set(ARGOSINSTALLDIR $ENV{ARGOSINSTALLDIR})
  include(${CMAKE_SOURCE_DIR}/../real_robot/swarmanoid/cmake/moboard.cmake)
  add_definitions(-DCROSSCOMPILING)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/EclipseRealSwarmanoidProjectFile.xml ${CMAKE_BINARY_DIR}/.project)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/EclipseRealSwarmanoidCProjectFile.xml ${CMAKE_BINARY_DIR}/.cproject)

else( ARGOS_COMPILE_REAL_SWARMANOID )

  #
  # Compiling for the pc
  # Check if Google PerfTools is available
  #
  find_package(GooglePerfTools)

endif( ARGOS_COMPILE_REAL_SWARMANOID )

#
# Visit and compile stuff in subdirectories
#
add_subdirectory(utility)
add_subdirectory(control_interface)
