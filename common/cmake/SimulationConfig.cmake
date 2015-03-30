#
# Target-dependent flags
#
if( NOT ARGOS_COMPILE_REAL_SWARMANOID )

  if( APPLE )

    #
    # Compiling on MAC OSX
    #
    set(CMAKE_CXX_FLAGS_RELEASE "-Os -Wall -DNDEBUG -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during release builds" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-Os -Wall -DNDEBUG -std=gnu99 -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during release builds" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -Wall -ggdb3 -DNDEBUG -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during Release with Debug Info builds" FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -Wall -ggdb3 -DNDEBUG -std=gnu99 -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during Release with Debug Info builds" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Wall -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during debug builds" FORCE)
    set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Wall -std=gnu99 -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during debug builds" FORCE)
    set(CMAKE_CXX_FLAGS "-ggdb3 -Wall -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during all builds" FORCE)
    set(CMAKE_C_FLAGS "-ggdb3 -Wall -std=gnu99 -undefined dynamic_lookup" CACHE STRING "Flags used by the compiler during all builds" FORCE)

  else( APPLE )

    #
    # Compiling on Unix
    #
    set(CMAKE_CXX_FLAGS_RELEASE "-Os -Wall -DNDEBUG" CACHE STRING "Flags used by the compiler during release builds" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-Os -Wall -DNDEBUG -std=gnu99" CACHE STRING "Flags used by the compiler during release builds" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -Wall -ggdb3 -DNDEBUG" CACHE STRING "Flags used by the compiler during Release with Debug Info builds" FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -Wall -ggdb3 -DNDEBUG -std=gnu99" CACHE STRING "Flags used by the compiler during Release with Debug Info builds" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Wall" CACHE STRING "Flags used by the compiler during debug builds" FORCE)
    set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Wall -std=gnu99" CACHE STRING "Flags used by the compiler during debug builds" FORCE)
    set(CMAKE_CXX_FLAGS "-ggdb3 -Wall" CACHE STRING "Flags used by the compiler during all builds" FORCE)
    set(CMAKE_C_FLAGS "-ggdb3 -Wall -std=gnu99" CACHE STRING "Flags used by the compiler during all builds" FORCE)
    #
    # Needed by Ubuntu > 10.04
    #
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")

  endif( APPLE )

endif( NOT ARGOS_COMPILE_REAL_SWARMANOID )
