cmake_minimum_required(VERSION 2.6)
project(argos2_simulator)

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
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ARGoS2 simulator package")
include(CPack)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
  ${CMAKE_SOURCE_DIR}/../common/cmake)

#
# Make sure GSL is installed and available
#
find_package(GSL REQUIRED)
add_definitions(${CMAKE_GSL_CXX_FLAGS})
include_directories(${GSL_INCLUDE_DIR})
link_directories(${GSL_LINK_DIRECTORIES})

#
# Make sure Pthreads is installed and available
#
find_package(Pthreads)
if(NOT PTHREADS_FOUND)  
  message(FATAL_ERROR "Required library pthread not found.")
endif(NOT PTHREADS_FOUND)
add_definitions(${PTHREADS_DEFINITIONS})

#
# Make sure FreeImage is installed and available
#
find_package(FreeImage)
if(NOT FREEIMAGE_FOUND)  
  message(FATAL_ERROR "Required library FreeImage not found.")
endif(NOT FREEIMAGE_FOUND)
include_directories(${FREEIMAGE_INCLUDE_PATH})

#
# Check if Google PerfTools is available
#
find_package(GooglePerfTools)

#
# Set default include and link directories
#
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/..)
link_directories(
  ${CMAKE_BINARY_DIR}/../common/simulator/utility
  ${CMAKE_BINARY_DIR}/../common/simulator/control_interface)

#
# This is required for TinyXML++ to work
#
add_definitions(-DTIXML_USE_TICPP)

#
# More cmake configuration flags to optimize code
#
include(${CMAKE_SOURCE_DIR}/../common/cmake/SimulationConfig.cmake)

#
# Visit and compile stuff in subdirectories
#
add_subdirectory(actuators)
add_subdirectory(sensors)
add_subdirectory(physics_engines)
add_subdirectory(visualizations)
add_subdirectory(space)

#
# Header files in the base simulator directory
#
set(argos2_simulator_HEADERS_base
  argos_command_line_arg_parser.h
  simulator.h)

#
# Header files in the dynamic linking directory
#
set(argos2_simulator_HEADERS_dynamic_linking
  dynamic_linking/dynamic_linking_manager.h
  dynamic_linking/loop_functions_registration.h
  dynamic_linking/loop_functions.h)

#
# Header files in the factories directory
#
set(argos2_simulator_HEADERS_factories
  factories/actuators_factory.h
  factories/entities_factory.h
  factories/physicsengines_factory.h
  factories/renderers_factory.h
  factories/sensors_factory.h
  factories/query_factories.h
)

#
# Implementation files
#
set(argos2_simulator_IMPLEMENTATIONS
  dynamic_linking/dynamic_linking_manager.cpp
  dynamic_linking/loop_functions.cpp
  factories/actuators_factory.cpp
  factories/entities_factory.cpp
  factories/physicsengines_factory.cpp
  factories/renderers_factory.cpp
  factories/sensors_factory.cpp
  factories/query_factories.cpp
  argos_command_line_arg_parser.cpp
  simulator.cpp)

#
# Create the base argos2 library
#
add_library(argos2_simulator SHARED
  ${argos2_simulator_HEADERS_base}
  ${argos2_simulator_HEADERS_dynamic_linking}
  ${argos2_simulator_IMPLEMENTATIONS})
set_target_properties(argos2_simulator PROPERTIES VERSION 2.0)
target_link_libraries(argos2_simulator
  argos2_common_utility
  argos2_common_control_interface
  argos2_simulator_actuators
  argos2_simulator_sensors
  argos2_simulator_space
  argos2_simulator_physics_engines
  argos2_simulator_visualizations
  ${PTHREADS_LIBRARY} dl)
link_googleperftools_to(argos2_simulator)

#
# Create the argos executable
#
add_executable(argos main.cpp)
target_link_libraries(argos argos2_simulator)
link_googleperftools_to(argos)

#
# More configuration for the 'make install' command
#
install(FILES ${argos2_simulator_HEADERS_base} DESTINATION include/argos2/simulator)
install(FILES ${argos2_simulator_HEADERS_dynamic_linking} DESTINATION include/argos2/simulator/dynamic_linking)
install(FILES ${argos2_simulator_HEADERS_factories} DESTINATION include/argos2/simulator/factories)
install(TARGETS argos2_simulator argos
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos2
  ARCHIVE DESTINATION lib/argos2)
