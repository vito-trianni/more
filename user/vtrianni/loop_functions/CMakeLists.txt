if( ARGOS_COMPILE_REAL_SWARMANOID )

  add_subdirectory(test_loop_functions)
  add_subdirectory(evolutionary_lf)

else( ARGOS_COMPILE_REAL_SWARMANOID )

  add_subdirectory(test_loop_functions)
  add_subdirectory(evolutionary_lf)

endif( ARGOS_COMPILE_REAL_SWARMANOID )
