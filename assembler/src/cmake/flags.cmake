# -*- cmake -*-

# Handle OpenMP flags
if (OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# Use parallel libstdc++ if possible
  add_definitions(-DUSE_GLIBCXX_PARALLEL=1)
else ()
  if (NOT APPLE)
    message(FATAL_ERROR "SPAdes requires OpenMP to be available")
  endif()
endif()

add_definitions(-std=c++0x)
add_definitions(-Wall -Wno-long-long -Wno-deprecated)
if (NOT OPENMP_FOUND)
  add_definitions(-Wno-unknown-pragmas)
endif()

# Use libc++ with clang due to C++11 mode
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message("Making Debug Configuration...")

  add_definitions(-g3)
  add_definitions(-D_GLIBCXX_DEBUG)
else()
  message("Making Release Configuration...")

  if (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    add_definitions(-g3)
  else()
    add_definitions(-g0)
  endif()

  add_definitions(-O2)
  add_definitions(-DNDEBUG)
endif()

# Make sure we're building with frame pointer if tcmalloc is in use
if (SPADES_USE_TCMALLOC)
  add_definitions(-fno-omit-frame-pointer)
endif()