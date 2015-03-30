include_directories(${CMAKE_SOURCE_DIR}/controllers)

if( ARGOS_COMPILE_REAL_SWARMANOID )

  add_subdirectory(obstacle_avoidance)
  add_subdirectory(flocking)
  add_subdirectory(stick_pulling)

else( ARGOS_COMPILE_REAL_SWARMANOID )

  add_subdirectory(obstacle_avoidance)
  add_subdirectory(flocking)
  add_subdirectory(stick_pulling)
  
endif( ARGOS_COMPILE_REAL_SWARMANOID )
