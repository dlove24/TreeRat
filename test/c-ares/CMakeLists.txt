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
## Build the cares test harness
##

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/../tap/include )

##
## Build the cares tests
##

add_executable ( test_cares_00 test_cares_lookup.cpp )

target_link_Libraries ( test_cares_00
  tap
  cares
)

#add_executable ( test_cares_01 test_ldns_wrapper.cpp )
#
#target_link_Libraries ( test_cares_01
#  ${Boost_LIBRARIES}
#  tap
#  cares
#  utils
#)

##
## Run the tests
##

# Tests the core resolver library.
add_test (
  NAME test_cares_core
  COMMAND test_cares_00
)

# Tests the functionality of the C++ wrapper for the lDNS library
#add_test (
#  NAME test_cares_wrapper
#  COMMAND test_cares_01
#)
