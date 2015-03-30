if( ARGOS_COMPILE_REAL_SWARMANOID )

  set(obstacle_avoidance_HEADERS
    obstacle_avoidance_nn.h)
  
  set(obstacle_avoidance_IMPLEMENTATIONS
    obstacle_avoidance_nn.cpp)

  link_directories(
    ${CMAKE_BINARY_DIR}/../../../common/swarmanoid/control_interface
    ${CMAKE_BINARY_DIR}/../../../common/swarmanoid/utility
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/common
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/vision_robot/filtering
    ${CMAKE_SOURCE_DIR}/../../real_robot/swarmanoid/opencv/lib
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/footbot
    ${CMAKE_BINARY_DIR}/controllers/neural_networks)
  add_executable(obstacle_avoidance
    ${obstacle_avoidance_IMPLEMENTATIONS}
    ${CMAKE_SOURCE_DIR}/main_functions/real_footbot_main.cpp)
  target_link_libraries(obstacle_avoidance
    neural_networks
    argos2_real_robot_swarmanoid_footbot
    argos2_real_robot_swarmanoid_common
    argos2_common_control_interface 
    argos2_common_utility)

else( ARGOS_COMPILE_REAL_SWARMANOID )

  set(obstacle_avoidance_HEADERS
    obstacle_avoidance_nn.h
    obstacle_avoidance_lf.h )
  
  set(obstacle_avoidance_IMPLEMENTATIONS
    obstacle_avoidance_nn.cpp
    obstacle_avoidance_lf.cpp)

  link_directories(${CMAKE_BINARY_DIR}/controllers/neural_networks /usr/share/pvm3/lib/LINUX)
  add_library(obstacle_avoidance MODULE ${obstacle_avoidance_IMPLEMENTATIONS})
  target_link_libraries(obstacle_avoidance neural_networks)

endif( ARGOS_COMPILE_REAL_SWARMANOID )
