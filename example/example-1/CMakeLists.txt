cmake_minimum_required(VERSION 2.8.10)

project(example-1)
set(TARGET "example-1")
set(SOURCE main.cxx
  example_world.cxx
  example_scene_a.cxx
)

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")

  message(" * C++ compiler: Emscripten")
  
  if(NOT SUBSYSTEM)
    set(SUBSYSTEM GLFW2)
  endif()

  set(CMAKE_CXX_COMPILER_ID "Emscripten")
  
  set(CMAKE_CXX_FLAGS "-s ALLOW_MEMORY_GROWTH=1 ${CMAKE_CXX_FLAGS}")
  
  set(CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS            "-Wall ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "-pedantic-errors ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
  
  set(TARGET "${TARGET}.html")

else()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    
    if(NOT SUBSYSTEM)
      set(SUBSYSTEM GLFW3)
    endif()
    
    message(" * C++ compiler: Clang")
    
    set(CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS            "-Wall ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-pedantic-errors ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -march=native -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -march=native -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -march=native -g")
  
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  
    message(" * C++ compiler: GCC")
    
    set(CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS            "-Wall ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-pedantic-errors ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -march=native -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -march=native -g -pg")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -march=native -g -pg")
    
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    message(" * C++ compiler: ICC")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(" * C++ compiler: MSVC++")
  else()
    message(" * C++ compiler: unknown")
  endif()

endif()

message(" * SUBSYSTEM: ${SUBSYSTEM}")

if(SUBSYSTEM MATCHES GLFW3)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_GLFW3 ${CMAKE_CXX_FLAGS}")
elseif(SUBSYSTEM MATCHES GLFW2)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_GLFW2 ${CMAKE_CXX_FLAGS}")
elseif(SUBSYSTEM MATCHES SDL2)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_SDL2 ${CMAKE_CXX_FLAGS}")
elseif(SUBSYSTEM MATCHES SDL1)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_SDL1 ${CMAKE_CXX_FLAGS}")
endif()

if(CMAKE_BUILD_TYPE MATCHES release)
  message(" * build type: release")
  set(CXX_FLAGS "${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_RELEASE}")
  message(" * CXX_FLAGS: ${CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES debug)
  message(" * build type: debug")
  set(CXX_FLAGS "${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_DEBUG}")
  message(" * CXX_FLAGS: ${CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES relwithdebinfo)
  message(" * build type: relwithdebinfo")
  set(CXX_FLAGS "${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  message(" * CXX_FLAGS: ${CXX_FLAGS}")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../libWRP-log/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../libWRP-time/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../libWRP-message/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../wonderland.subsystem/include
)

message(" * target: ${TARGET}")
message(" * source: ${SOURCE}")

add_executable(${TARGET} ${SOURCE})
add_dependencies(${TARGET}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/wonder_rabbit_project/wonderland.hxx
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  find_package(Threads)
  target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
endif()