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
## Library Sources
##

# Core library sources
set ( LDNS_SOURCES
  buffer.c
  dane.c
  dname.c
  dnssec.c
  dnssec_sign.c
  dnssec_verify.c
  dnssec_zone.c
  duration.c
  error.c
  higher.c
  host2str.c
  host2wire.c
  keys.c
  net.c
  packet.c
  parse.c
  rbtree.c
  rdata.c
  resolver.c
  rr.c
  rr_functions.c
  sha1.c
  sha2.c
  str2host.c
  tsig.c
  update.c
  util.c
  wire2host.c
  zone.c
)

# Add needed compatability sources
if ( !B64_NTOP )
  set ( LDNS_SOURCES ${LDNS_SOURCES} compat/b64_ntop.c )
endif ()

if ( !B64_PTON )
  set ( LDNS_SOURCES ${LDNS_SOURCES} compat/b64_pton.c )
endif ()

# Build the library
add_library ( ldns STATIC ${LDNS_SOURCES} )
