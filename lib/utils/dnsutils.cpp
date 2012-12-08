/**
*** Portable utilities for querying data held in the Domain Name
*** System (DNS).
***
*** Copyright(c) 2011 David Love <d.love@shu.ac.uk>
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
*** \file   dnsutils.cpp
***
*** \author David Love (david@homeunix.org.uk)
*** \date   December, 2012
***
*** A C++ wrapper around the \c lDNS library, providing high level functions
*** to query data held in the Domain Name System (DNS). This wrapper does not
*** aim to expose the full functionality of the \c lDNS library: instead the
*** focus is on ease-of-use and integration into C++ applications.
***
**/

#include <config/config.h>
#include <utils/dnsutils.h>

#include <string>
#include <iostream>

#ifdef HAVE_TYPES_H
#  include <types.h>
#endif

using namespace boost::asio;

/***
 *** DNSName Class Defition
 ***
 */

//
// Public Class Methods
//

/**
 * Convert the internal \tt DNS resource representation to a
 * \c std::string. This does not modify the internal
 * representation of the \tt DNS resource in any way.
 *
 * \retval std::string A \c std::string, representing the internal
 *   resource name held by the object.
 *
 * Example Usage:
 *
 * \code
 *    DNSName dns_name(a_dns_resource);
 *    std::string a_string{dns_name.to_c_str()};
 * \endcode
 *
 */
const std::string DNSName::to_str (void) const {
  return cv_dns_query_name;
  }

/**
 * Convert the internal \tt DNS resource representation to a
 * \tt C-style (\c char*) string. This does not modify the internal
 * representation of the \tt DNS resource in any way.
 *
 * \warning the allocated \tt C string must be freed by the caller.
 *
 * \retval char* A \tt C-style string, representing the internal
 *   resource name held by the object.
 *
 * Example Usage:
 *
 * \code
 *    DNSName dns_name(a_dns_resource);
 *    char* a_string{dns_name.to_c_str()};
 * \endcode
 *
 */
const char* DNSName::to_c_str (void) const {
  return cv_dns_query_name.c_str();
  }

/**
 * Convert the internal \tt DNS resource representation to a
 * \c boost::ip::address.
 *
 * \note Only certain DNS names can be converted directly to an IP address
 *    (namely those from A and AAAA records). If the \c recursive flag is
 *    specified, this routine will attempt to continue the resolution, until
 *    a convertible name is found. If the address cannot be converted this
 *    class will throw a \c DNSNameConversionException. If you don't want to
 *    handle exceptions, check the type \em before calling this method.
 *
 *    \param [in] recursive If the name cannot be directly converted to an
 *      \tt IP address, perform additonal lookups to find a name that can
 *      be converted. This is primarily useful if the original name is of
 *      type \c DNSQueryType::MX or \c DNSQueryType::SVR.
 *
 *    \param [in] prefer_legacy If possible, return \tt IPv4 address instead of
 *      \tt IPv6 addresses. By default the library will return \tt IPv6 addresses
 *      if at all possible.
 *
 * \retval boost::asio::ip::address An \tt IPv4 or \tt IPv6 address record. We don't
 *   actually care which style of IP address we return: it is up to the
 *   caller to ensure they request the correct type.
 *
 * Example Usage:
 *
 * \code
 *    DNSName dns_name(a_dns_resource);
 *    char* a_string{dns_name.to_c_str()};
 * \endcode
 *
 */
const boost::asio::ip::address DNSName::to_ip (bool recursive, bool prefer_legacy) const {
  ip::address return_address;

  try {
    return_address = ip::address::from_string (cv_dns_query_name);
    }

  catch (...) {
    throw DNSNameConversionException ("Invalid address");
    }

  return return_address;
  }
