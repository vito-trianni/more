set(test_loop_functions_HEADERS
  test_loop_functions.h
)

set(test_loop_functions_IMPLEMENTATIONS
  test_loop_functions.cpp
)

if( ARGOS_COMPILE_REAL_SWARMANOID )
  link_directories(
    ${CMAKE_BINARY_DIR}/../../../common/swarmanoid/control_interface
    ${CMAKE_BINARY_DIR}/../../../common/swarmanoid/utility
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/common
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/vision_robot/filtering
    ${CMAKE_SOURCE_DIR}/../../real_robot/swarmanoid/opencv/lib
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/footbot)
  add_executable(test_loop_functions
    ${test_loop_functions_IMPLEMENTATIONS}
    ${CMAKE_SOURCE_DIR}/main_functions/real_footbot_main.cpp)
  target_link_libraries(test_loop_functions
    argos2_real_robot_swarmanoid_footbot
    argos2_real_robot_swarmanoid_common
    argos2_common_control_interface 
    argos2_common_utility)

else( ARGOS_COMPILE_REAL_SWARMANOID )

  add_library(test_loop_functions MODULE ${test_loop_functions_IMPLEMENTATIONS})

endif( ARGOS_COMPILE_REAL_SWARMANOID )
