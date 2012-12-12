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

# Set-up CMake
cmake_minimum_required ( VERSION 2.8 )

# Add third-party modules
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/" )

# Include the checks for specific functions
include ( CheckFunctionExists )

# Include the checks for header files
include ( CheckIncludeFiles )

# Include the checks for library functions
include ( CheckLibraryExists )

# Include checks for specific prototypes
include ( CheckPrototypeExists )

# Include checks for particular symbols
include ( CheckSymbolExists )

# Include checks for particular types
include ( CheckTypeSize )

##
## Project Definition
##

# Project Name
project ( treerat )

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

# Set the project configure file
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/include/config/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config/config.h )

# Add the project includes to the search path
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )

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
endif ()

# Look for the operating system name server resolver functions
check_library_exists ( resolv gethostbyname "" HAVE_RESOLV_LIB )

if ( HAVE_RESOLV_LIB )
  set ( LINK_RESOLV_LIB resolv )
endif ()

# Look for the ptherad library (only required on some platforms)
check_library_exists ( pthread pthread_join "" HAVE_PTHREAD_LIB )

if ( HAVE_PTHREAD_LIB )
  if ( HAVE_RESOLV_LIB AND (${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
    # On GNU/Linux the DNS resolv library requires pthreads as well
    set ( LINK_RESOLV_LIB ${LINK_RESOLV_LIB} pthread )
  endif ()
endif ()

# Look for the Boost C++ library. This is fatal if we don't find it.
set ( Boost_USE_STATIC_LIBS OFF)
set ( Boost_USE_MULTITHREADED ON)
set ( Boost_USE_STATIC_RUNTIME OFF)

find_package ( Boost 1.45.0 COMPONENTS
#  chrono
#  context
#  date_time
   filesystem
#  graph
   iostreams
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
   system
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

# popen - Used to create pipes to commands
check_library_exists ( c popen "" HAVE_POPEN )

if ( HAVE_POPEN )
  check_prototype_exists ( popen "stdlib.h" POPEN_PROTO_STDLIB )
  check_prototype_exists ( popen "stdio.h" POPEN_PROTO_STDIO )
endif ()

##
## Include Files Defines for the POSIX Platform
##

check_include_files ( "errno.h"      HAVE_ERRNO_H )
check_include_files ( "fcntl.h"      HAVE_FCNTL_H )
check_include_files ( "inttypes.h"   HAVE_INTTYPES_H )
check_include_files ( "limits.h"     HAVE_LIMITS_H )
check_include_files ( "netdb.h"      HAVE_NETDB_H )
check_include_files ( "resolv.h"     HAVE_RESOLV_H )
check_include_files ( "stddef.h"     HAVE_STDDEF_H )
check_include_files ( "stdint.h"     HAVE_STDINT_H )
check_include_files ( "stdlib.h"     HAVE_STDLIB_H )
check_include_files ( "string.h"     HAVE_STRING_H )
check_include_files ( "unistd.h"     HAVE_UNISTD_H )
check_include_files ( "winsock2.h"   HAVE_WINSOCK2_H )

check_include_files ( "arpa/inet.h"  HAVE_ARPA_INET_H )
check_include_files ( "arpa/nameser.h" HAVE_ARPA_NAMESER_H )
check_include_files ( "arpa/nameser_compat.h" HAVE_ARAPA_NAMESER_COMPAT_H )
check_include_files ( "netinet/in.h" HAVE_NETINET_IN_H )
check_include_files ( "sys/fcntl.h"  HAVE_SYS_FCNTL_H )
check_include_files ( "sys/select.h" HAVE_SYS_SELECT_H )
check_include_files ( "sys/socket.h" HAVE_SOCKET_H )
check_include_files ( "sys/time.h"   HAVE_SYS_TIME_H )
check_include_files ( "sys/types.h"  HAVE_SYS_TYPES_H )
check_include_files ( "sys/uio.h"    HAVE_SYS_UIO_H )

##
## Include File Defines for Other Features
##

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

##
## Enable Documentation Option. Adds a "make Docs" target, and allows automatic generation
## of the Doxygen documentation
##

option ( BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF )

if ( BUILD_DOCUMENTATION )
  find_package ( Doxygen )
  if ( NOT DOXYGEN_FOUND )
    message( FATAL_ERROR
      "Doxygen is needed to build the documentation. Please install it correctly" )
  endif ()

  configure_file ( Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE )

  add_custom_target ( Docs
                      COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                      SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
endif ()
