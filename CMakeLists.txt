cmake_minimum_required(VERSION 3.6)
project(draft-type C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

if ( CMAKE_COMPILER_IS_GNUCC )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wuninitialized -Wpedantic -fPIC")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-variable")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable") # Add optimization
  endif()
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /MP")
endif ( MSVC )

set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(EXAMPLE_DIR example)
set(ASSETS_DIR assets)

set(SRC_LIB
	${SOURCE_DIR}/glyph.h
	${SOURCE_DIR}/hershey.h
  ${SOURCE_DIR}/hershey.cpp
	${SOURCE_DIR}/shaper.h
)

set(SRC_INCLUDE
  ${INCLUDE_DIR}/draft-type.h
)

set(SRC_EXAMPLE
	${EXAMPLE_DIR}/main.cpp
)

include_directories(${SOURCE_DIR})
add_library(draft_type STATIC ${SRC_INCLUDE} ${SRC_LIB})

# Test executable
include_directories(${INCLUDE_DIR})
add_executable(draft_type_example ${SRC_EXAMPLE})
target_link_libraries(draft_type_example PRIVATE draft_type)
