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
## Header files
##

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/../../include/c-ares )

##
## Library Sources
##

add_library ( cares STATIC
  ares__close_sockets.c
  ares__get_hostent.c
  ares__read_line.c
  ares__timeval.c
  ares_cancel.c
  ares_data.c
  ares_destroy.c
  ares_expand_name.c
  ares_expand_string.c
  ares_fds.c
  ares_free_hostent.c
  ares_free_string.c
  ares_getenv.c
  ares_gethostbyaddr.c
  ares_gethostbyname.c
  ares_getnameinfo.c
  ares_getopt.c
  ares_getsock.c
  ares_init.c
  ares_library_init.c
  ares_llist.c
  ares_mkquery.c
  ares_nowarn.c
  ares_options.c
  ares_parse_a_reply.c
  ares_parse_aaaa_reply.c
  ares_parse_mx_reply.c
  ares_parse_naptr_reply.c
  ares_parse_ns_reply.c
  ares_parse_ptr_reply.c
  ares_parse_soa_reply.c
  ares_parse_srv_reply.c
  ares_parse_txt_reply.c
  ares_platform.c
  ares_process.c
  ares_query.c
  ares_search.c
  ares_send.c
  ares_strcasecmp.c
  ares_strdup.c
  ares_strerror.c
  ares_timeout.c
  ares_version.c
  ares_writev.c
) 
