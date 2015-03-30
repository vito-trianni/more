set(evolutionary_lf_HEADERS
  evolutionary_lf.h
)

set(evolutionary_lf_IMPLEMENTATIONS
  evolutionary_lf.cpp
)

if( ARGOS_COMPILE_REAL_SWARMANOID )

  add_library(evolutionary_lf SHARED ${evolutionary_lf_IMPLEMENTATIONS})

else( ARGOS_COMPILE_REAL_SWARMANOID )

  add_library(evolutionary_lf SHARED ${evolutionary_lf_IMPLEMENTATIONS})

endif( ARGOS_COMPILE_REAL_SWARMANOID )
