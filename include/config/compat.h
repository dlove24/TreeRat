/**
*** Compatability defines and functions. These are typically functions and
*** definitions that exist as part of the standard C library on some platforms:
*** but not on all.
***
*** Copyright (c) 2012 David Love <david@homeunix.org.uk>
***
*** Permission to use, copy, modify, and/or distribute this software for any
*** purpose with or without fee is hereby granted, provided that the above
*** copyright notice and this permission notice appear in all copies.
***
*** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*** WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*** MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*** ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*** WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*** OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***
*** \file compat.h
***
*** \author David Love
*** \date December 2012
**/

#ifndef COMPAT_H
#define COMPAT_H 1

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_STDDEF_H
# include <stddef.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif

/*
 * Prefer BSD-style functions and defines
 */

#ifndef u_char
#  ifdef TYPEDEF___U_CHAR
#    include <bits/types.h>
typedef __u_char u_char;
#  else
#    error "Unable to define type u_char"
#  endif
#endif

#ifndef u_int
#  ifdef TYPEDEF___U_INT
#    include <bits/types.h>
typedef __u_int u_int;
#  else
#    error "Unable to define type u_int"
#  endif
#endif

#ifndef u_long
#  ifdef TYPEDEF___U_LONG
#    include <bits/types.h>
typedef __u_long u_long;
#  else
#    error "Unable to define type u_long"
#  endif
#endif

#ifndef u_short
#  ifdef TYPEDEF___U_SHORT
#    include <bits/types.h>
typedef __u_short u_short;
#  else
#    error "Unable to define type u_short"
#  endif
#endif

/*
 * Replacement for standard C libary routines
 *
 */

#ifndef HAVE_STRLCPY
size_t strlcpy (char* dst, const char* src, size_t siz);
#endif

#ifndef HAVE_RANDOM
#  define srandom(x) srand(x)
#  define random(x) rand(x)
#endif

/*
 * Network to Host Conversion Functions. Principally used by the ldns library.
 *
 */

#ifdef HAVE_RESOLV_H
#  include <resolv.h>
#endif

#ifndef B64_PTON
int ldns_b64_ntop (uint8_t const* src, size_t srclength,
                   char* target, size_t targsize);
/**
 * calculates the size needed to store the result of b64_ntop
 */
/*@unused@*/
static inline size_t ldns_b64_ntop_calculate_size (size_t srcsize) {
  return ( ( ( (srcsize + 2) / 3) * 4) + 1);
  }
#endif /* !B64_PTON */
#ifndef B64_NTOP
int ldns_b64_pton (char const* src, uint8_t* target, size_t targsize);
/**
 * calculates the size needed to store the result of ldns_b64_pton
 */
/*@unused@*/
static inline size_t ldns_b64_pton_calculate_size (size_t srcsize) {
  return ( ( ( ( (srcsize + 3) / 4) * 3)) + 1);
  }
#endif /* !B64_NTOP */

/* Detect if we need to cast to unsigned int for FD_SET to avoid warnings */

#ifdef HAVE_WINSOCK2_H
#  define FD_SET_T (u_int)
#else
#  define FD_SET_T
#endif

#endif /* compat.h */
