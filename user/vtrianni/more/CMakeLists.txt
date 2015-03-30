find_package(MPI REQUIRED)

set(revolver_HEADERS
  evolution.h
  single_mulambda_evolution.h
  mulambda_evolution.h
  mo_mulambda_evolution.h
  )

set(revolver_IMPLEMENTATIONS
  evolution.cpp
  single_mulambda_evolution.cpp
  mulambda_evolution.cpp
  mo_mulambda_evolution.cpp
)

include_directories(${CMAKE_BINARY_DIR}/revolver/ ${MPI_INCLUDE_PATH}  /usr/lib/openmpi/include /home/vtrianni/local/include)
add_library(morelib SHARED ${revolver_IMPLEMENTATIONS})
target_link_libraries(morelib ${MPI_LIBRARY})
