cmake_minimum_required(VERSION 2.6)
project(argos2_user_vtrianni)

include(CPack)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_definitions(-DTIXML_USE_TICPP)

include_directories(${CMAKE_SOURCE_DIR}/../.. ${CMAKE_SOURCE_DIR}/../../epfl ${CMAKE_SOURCE_DIR}/../../epfl/dashel ${CMAKE_SOURCE_DIR}/../../Shark/include)
include(${CMAKE_SOURCE_DIR}/../../common/cmake/SimulationConfig.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/user_config.cmake)

if( ARGOS_COMPILE_REAL_SWARMANOID )

  include(${CMAKE_SOURCE_DIR}/../../real_robot/swarmanoid/cmake/moboard.cmake)
  add_definitions(-DCROSSCOMPILING)

  set(ARGOSINSTALLDIR $ENV{ARGOSINSTALLDIR})

  add_subdirectory(experiments)
  add_subdirectory(controllers)

else( ARGOS_COMPILE_REAL_SWARMANOID )
  add_subdirectory(experiments)
  add_subdirectory(controllers)
  add_subdirectory(loop_functions)
  add_subdirectory(more)

  find_package(MPI)

  link_directories(
    ${CMAKE_BINARY_DIR}/../../../common/simulator
    ${CMAKE_BINARY_DIR}/../../../common/simulator/utility
    ${CMAKE_BINARY_DIR}/../../../simulator
    ${CMAKE_BINARY_DIR}/../../../../Shark
    )
  add_executable(argos main_functions/simulation_main.cpp)
  target_link_libraries(argos
    argos2_common_utility
    argos2_simulator)

  include_directories(${MPI_INCLUDE_PATH} /home/vtrianni/local/include)
  add_executable(morevolve main_functions/morevolver_main.cpp)
  target_link_libraries(morevolve
    ${MPI_LIBRARIES}
    morelib
    argos2_common_utility
    argos2_simulator
    shark)

  include_directories(${MPI_INCLUDE_PATH} /home/vtrianni/local/include)
  add_executable(invalid main_functions/invalid_main.cpp)
  target_link_libraries(invalid
    ${MPI_LIBRARIES}
    argos2_common_utility
    argos2_simulator)



endif( ARGOS_COMPILE_REAL_SWARMANOID )

