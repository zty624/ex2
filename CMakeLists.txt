cmake_minimum_required(VERSION 3.0)

project(example1)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

if (COMPILER_SUPPORTS_CXX17)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif (COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif (COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else ()
  message (FATAL_ERROR "You need a compiler with standard c++11 support")
endif ()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")

# native optimized build
set(CMAKE_CXX_FLAGS_NATIVE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native"
  CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_NATIVE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native"
  CACHE STRING "" FORCE)

# profiling
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_NATIVE} -g -pg"
  CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_NATIVE} -g -pg"
  CACHE STRING "" FORCE)

# optimized debug
set(CMAKE_CXX_FLAGS_OPTDEBUG "${CMAKE_CXX_FLAGS_NATIVE} -g"
  CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_OPTDEBUG "${CMAKE_CXX_FLAGS_NATIVE} -g"
  CACHE STRING "" FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
find_package(ROOT)

include(ExternalProject)

include_directories(include)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(ex1 src/ex1.cpp src/src.cpp src/ex1.h)
add_executable(ex2 src/ex2.cpp src/src.cpp src/ex2.h)
target_include_directories(ex1 PRIVATE ${ROOT_INCLUDE_DIR})
target_link_libraries(ex1 ${ROOT_LIBRARIES})
target_include_directories(ex2 PRIVATE ${ROOT_INCLUDE_DIR})
target_link_libraries(ex2 ${ROOT_LIBRARIES})