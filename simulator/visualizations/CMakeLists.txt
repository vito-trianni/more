#
# Visit and compile code in subdirectories
#
add_subdirectory(text)
add_subdirectory(povray)

#
# Compile QT-OpenGL visualization only if checks are OK
#
include(CheckQTOpenGL4ARGoS)
if(ARGOS_COMPILE_QTOPENGL)
  add_subdirectory(qt-opengl)
endif(ARGOS_COMPILE_QTOPENGL)

set(argos2_simulator_visualizations_HEADERS
  render.h)

set(argos2_simulator_visualizations_IMPLEMENTATIONS
  render.cpp)

add_library(argos2_simulator_visualizations SHARED
  ${argos2_simulator_visualizations_HEADERS}
  ${argos2_simulator_visualizations_IMPLEMENTATIONS})
target_link_libraries(argos2_simulator_visualizations
  argos2_simulator_text_renderer
  argos2_simulator_povray_renderer)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos2_simulator_visualizations
    argos2_simulator_qtopengl_renderer
    ${OPENGL_LIBRARIES})
endif(ARGOS_COMPILE_QTOPENGL)

install(FILES ${argos2_simulator_visualizations_HEADERS} DESTINATION include/argos2/simulator/visualizations)
install(TARGETS argos2_simulator_visualizations
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos2
  ARCHIVE DESTINATION lib/argos2)
