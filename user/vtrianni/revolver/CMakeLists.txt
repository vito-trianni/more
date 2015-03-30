set(revolver_HEADERS
  evolution.h
  evolutions/test_evolution.h
  evolutions/standard_evolution.h
  evolutions/post_evaluation.h
  evolutions/parallel_evolution.h
  population.h
  populations/mulambda_population.h
  genotype.h
  genotypes/bitstring_genotype.h
  genotypes/selfadaptive_realstring_genotype.h
  genotypes/realstring_genotype.h
  fitness_log.h
  pvm_compatibility.h
)
#  evolutions/post_evaluation.h
#  populations/steadystate_population.h



set(revolver_IMPLEMENTATIONS
  evolution.cpp
  evolutions/test_evolution.cpp
  evolutions/standard_evolution.cpp
  evolutions/post_evaluation.cpp
  evolutions/parallel_evolution.cpp
  population.cpp
  populations/mulambda_population.cpp
  genotype.cpp
  genotypes/bitstring_genotype.cpp
  genotypes/selfadaptive_realstring_genotype.cpp
  genotypes/realstring_genotype.cpp
  fitness_log.cpp
)
#  evolutions/post_evaluation.cpp
#  populations/steadystate_population.cpp

include_directories(${CMAKE_BINARY_DIR}/revolver/ ${MPI_INCLUDE_PATH})
link_directories(/usr/share/pvm3/lib/LINUX)
add_library(revolver SHARED ${revolver_IMPLEMENTATIONS})
target_link_libraries(revolver pvm3 ${MPI_LIBRARY})
