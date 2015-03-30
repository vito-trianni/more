if( ARGOS_COMPILE_REAL_SWARMANOID )

  set(flocking_HEADERS
    flocking_nn.h)
  
  set(flocking_IMPLEMENTATIONS
    flocking_nn.cpp)

  link_directories(
    ${CMAKE_BINARY_DIR}/../../../common/swarmanoid/control_interface
    ${CMAKE_BINARY_DIR}/../../../common/swarmanoid/utility
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/common
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/vision_robot/filtering
    ${CMAKE_SOURCE_DIR}/../../real_robot/swarmanoid/opencv/lib
    ${CMAKE_BINARY_DIR}/../../../real_robot/swarmanoid/footbot
    ${CMAKE_BINARY_DIR}/controllers/neural_networks
    ${CMAKE_BINARY_DIR}/loop_functions/evolutionary_lf)
  add_executable(flocking
    ${flocking_IMPLEMENTATIONS}
    ${CMAKE_SOURCE_DIR}/main_functions/real_footbot_main.cpp)
  target_link_libraries(flocking
    neural_networks
    evolutionary_lf
    argos2_real_robot_swarmanoid_footbot
    argos2_real_robot_swarmanoid_common
    argos2_common_control_interface 
    argos2_common_utility)

else( ARGOS_COMPILE_REAL_SWARMANOID )

#set(ARGOS_COMPILE_QTOPENGL false)
#set(QT_USE_QTOPENGL true)
#find_package(Qt4)
  
#include(${QT_USE_FILE})

  
  set(flocking_HEADERS
    flocking_nn.h
#    flocking_qt.h
    flocking_lf.h )
  
  set(flocking_IMPLEMENTATIONS
    flocking_nn.cpp
#    flocking_qt.cpp
    flocking_lf.cpp)

  
  
  link_directories(${CMAKE_BINARY_DIR}/controllers/neural_networks  ${CMAKE_BINARY_DIR}/loop_functions/evolutionary_lf)
  add_library(flocking MODULE ${flocking_IMPLEMENTATIONS})
  target_link_libraries(flocking neural_networks evolutionary_lf)

endif( ARGOS_COMPILE_REAL_SWARMANOID )
