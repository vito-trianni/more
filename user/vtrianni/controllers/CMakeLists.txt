if( ARGOS_COMPILE_REAL_SWARMANOID )

  add_subdirectory(neural_networks)
  add_subdirectory(null_controller)
  add_subdirectory(robot_configurator)

else( ARGOS_COMPILE_REAL_SWARMANOID )

  add_subdirectory(neural_networks)
  add_subdirectory(null_controller)
  add_subdirectory(robot_configurator)

endif( ARGOS_COMPILE_REAL_SWARMANOID )
