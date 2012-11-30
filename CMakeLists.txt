# Copyright (c) 2012 David Love
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

##
## Configure CMake
##

# Add third-party modules
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/" )

##
## Project Definition
##

# Project Name
project ( treerat )

# Set-up CMake
cmake_minimum_required ( VERSION 2.8 )

# Include the checks for library functions
include ( CheckLibraryExists )

# Include the checks for header files
include ( CheckIncludeFiles )

##
## Compiler Configuration
##

# Enforce Ansi C99 mode for C compilers, and C++11 mode for
# C++ compilers. All compilers should have all warnings on, and
# be as pedantic as possible

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  message ( "-- Using Clang compiler profile for C...")
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall -Wextra" )
	
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  message ( "-- Using GCC compiler profile for C...")
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall -Wextra" )
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message ( "-- Using Clang compiler profile for C++...")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra" )
	
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message ( "-- Using GCC compiler profile for C++...")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -fdiagnostics-show-option -Wall -Wextra" )
endif ()
	
##
## Project Configuration
##

# Set the global configure file
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/include/config/config.h.in 
${CMAKE_CURRENT_SOURCE_DIR}/lib/config/config.h )

# Add the global configure file to the search path
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include/config )

##
## Library Configuration
##

# Look for the math library: on some platforms this is part of the C
# library, on others it needs linking separately
check_library_exists ( m floor "" SEPARATE_LIB_M ) 

if ( SEPARATE_LIB_M )
  # If the math library is separate, then it needs to be included
  # in the global link flags
  set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")
  set ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lm" )
  set ( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -lm" )
endif()

# Look for the Boost C++ library. This is fatal if we don't find it.
set ( Boost_USE_STATIC_LIBS OFF)
set ( Boost_USE_MULTITHREADED ON)
set ( Boost_USE_STATIC_RUNTIME OFF)

find_package ( Boost 1.45.0 COMPONENTS
#  chrono
#  context
#  date_time
#  filesystem
#  graph
#  iostreams
#  locale
#  math_c99
#  math_c99f
#  math_c99l
#  math_tr1
#  math_tr1f
#  math_tr1l
#  prg_exec_monitor
#  program_options
#  python
#  random
#  regex
#  serialization
#  signals
#  system
#  thread
#  timer
   unit_test_framework
#  wave
#  wserialization

  REQUIRED
)

##
## Include Files Defines for the C Standard Library
##

##
## Include Files Defines for the POSIX Platform
##

##
## Include File Defines for Other Features
##

# Look for the getopt library headers
check_include_files ( getopt.h HAVE_GETOPT_H )

# Check whether we are on a 32-bit or a 64-bit platform for non-Apple
# platforms: Apple platforms have to be checked at compile time

if( NOT APPLE )
  # check 64 bit
  if ( CMAKE_SIZEOF_VOID_P MATCHES 4)
    set ( HAVE_64_BIT 0 )
  else()
    set ( HAVE_64_BIT 1 )
  endif()

  # check endianness
  include( TestBigEndian )
  test_big_endian ( HAVE_BIG_ENDIAN )
  if ( HAVE_BIG_ENDIAN )
    set ( HAVE_BIG_ENDIAN 1 )
  else ()
    set ( HAVE_BIG_ENDIAN 0 )
  endif ()
endif ()

##
## Add Project Libraries
##

add_subdirectory ( lib )

##
## Application Sources
##

add_subdirectory ( bin )

##
## Enable Testing, if we can find the 'prove' test harness
##

find_program ( PROVE_FOUND prove )

if ( PROVE_FOUND )
  message ( "++ Found the prove harness. Testing Enabled" )

  enable_testing ()
  add_subdirectory ( test )
endif()

