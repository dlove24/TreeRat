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
*** \file   dnsutils-resolv.cpp
***
*** \author David Love (david@homeunix.org.uk)
*** \date   December, 2012
***
*** A C++ wrapper around the Bind \p DNS library, providing high level functions
*** to query data held in the Domain Name System (DNS). This wrapper does not
*** aim to expose the full functionality of the \p DNS library: instead the
*** focus is on ease-of-use and integration into C++ applications.
***
*** \note This file contains \em only those routines that allow implementation
***   of the wrapper header file against the Bind \p resolv library. Implementations
***   for other libraries are available, and \em must be used if the Bind \p resolv
***   library is not available.
***
**/

#include <utils/dnsutils.h>

using namespace boost::asio;

/***
 *** Types
 ***
 **/

/***
 *** DNSName Class Defition
 ***
 */

//
// Protected Methods
//

int DNSNames::convert_to_ns_type (const DNSQueryType dns_query_type) {
  switch (dns_query_type) {
    case DNSQueryType::A:
      return T_A;

    case DNSQueryType::AAAA:
      return T_AAAA;

    case DNSQueryType::MX:
      return T_MX;

    case DNSQueryType::SRV:
      return T_SRV;

    default:
      return T_A;
    }
  }

/**
 * Package the current \p DNS data held in the internal class variables as a \p DNS query, and
 * send the query to the local \p DNS resolver. In asyncronous useage, this call forms the future
 * \c std::promise, later accessed by the routines relying on the \c cv_dns_record_list.
 *
 * Example Usage:
 *
 * Example Usage:
 *
 * \code
 *  cv_dns_query_name = dns_name;
 *  cv_dns_query_type = dns_query_type;
 *  cv_dns_record_list = new list<string>;
 *
 *  send_dns_query();
 * \endcode
 */
void DNSNames::send_dns_query (void) {
  DNSQueryBuffer buffer;         // The buffer for the DNS response
  int buffer_length { -1};       // The length of the DNS response in the buffer

  // If the list of DNS records is empty, send a query for the A records
  // of the named resource
  buffer_length = res_search (cv_dns_query_name.c_str(), C_IN, convert_to_ns_type (cv_dns_query_type),
                              (u_char*) &buffer, sizeof (buffer));

  if (buffer_length < 0) {
    throw DNSException ("Failed to reach a valid DNS source");
    }

  // Parse the returned buffer, to create the list of names. The list of names itself
  // is placed in cv_dns_record_list
  extract_resource_records (&buffer, buffer_length);
  }

/**
 * Extract the named resources from a \c DNSQueryBuffer of type \c ns_query_type,
 * returned by the low-level, operating system resolver routines. Even if more than
 * one record type is present, only those of \c ns_query_type will be considered.
 * All valid records of that type will then be parsed, and added to the internal
 * \c cv_dns_record_list.
 *
 *    \param [in] query_buffer The \p DNS response buffer, returned by a call to
 *      \c res_search or \c res_query.
 *    \param [in] buffer_length The size of the \c query_buffer: usually found as
 *      as the return code for \c res_search or \c res_query.
 *
 * Example Usage:
 *
 * \code
 *  // Parse the returned buffer, to create the list of names. The list of names itself
 *  // is placed in cv_dns_record_list
 *  extract_resource_records (&buffer, buffer_length);
 * \endcode
 */
void DNSNames::extract_resource_records (const DNSQueryBuffer* query_buffer, const int buffer_length) {
  ns_msg message_handle;         // Handle for the DNS response parsing structures
  ns_rr resource_list;           // List of resource records

  int resource_index;            // Index of resource records in the resource list

  // Attempt to create a parser for the response from the DNS server. Not all resolver libraries require
  // this call
  if (ns_initparse (query_buffer->buf, buffer_length, &message_handle) < 0) {
    throw DNSResolverException ("Failed to create a parser for the DNS response");
    }

  // Walk the list of resource records, looking for those of the specified
  // type. If we find a needed record, add it to the list of records to return
  for (resource_index = 0; resource_index < ns_msg_count (message_handle, ns_s_an); resource_index++) {

    // Extract resource at the given index, and attempt to create a resource record
    // for that resource
    if (ns_parserr (&message_handle, ns_s_an, resource_index, &resource_list)) {
      throw DNSResolverException ("Unable to extract the resource record from the listed index");
      }

    // Do we have at least one record in the answer section?
    if (ns_msg_count (message_handle, ns_s_an) < 1) {
      throw DNSResolverException ("No records returned in the DNS answer section");
      }

    if (ns_rr_type (resource_list) == convert_to_ns_type (cv_dns_query_type)) {
      u_int tmp_resource_priority;    // Temporary storage for the resourece priority
      char* tmp_resource_name;        // Temporary storage for the resource name

      //
      // Extract the data, according to the type of record
      //
      switch (cv_dns_query_type) {
        case DNSQueryType::A:
          // Allocate storage for the resource name
          tmp_resource_name = (char*) malloc (INET_ADDRSTRLEN);

          if (tmp_resource_name == nullptr) {
            throw DNSResolverException ("Allocation of memory for resource data failed");
            }

          // Expand the resource name from data section of the resource list,
          // and add it to the record list
          inet_ntop (AF_INET, ns_rr_rdata (resource_list), tmp_resource_name, INET_ADDRSTRLEN);

          break;

        case DNSQueryType::AAAA:
          // Allocate storage for the resource name
          tmp_resource_name = (char*) malloc (INET6_ADDRSTRLEN);

          if (tmp_resource_name == nullptr) {
            throw DNSResolverException ("Allocation of memory for resource data failed");
            }

          // Expand the resource name from data section of the resource list,
          // and add it to the record list
          inet_ntop (AF_INET6, ns_rr_rdata (resource_list), tmp_resource_name, INET6_ADDRSTRLEN);

          break;

        case DNSQueryType::MX:
          // Allocate storage for the resource name
          tmp_resource_name = (char*) malloc (NI_MAXHOST);

          if (tmp_resource_name == nullptr) {
            throw DNSResolverException ("Allocation of memory for resource data failed");
            }

          // Extract the priority of this resource
          tmp_resource_priority = ns_get16 (ns_rr_rdata (resource_list));

          // Extract the resource name, assuming an offset of size NS_INT16SZ from the priority
          const u_char* message_end;
          message_end = ns_msg_base (message_handle) + ns_msg_size (message_handle);

          const u_char* data_start;
          data_start = ns_rr_rdata (resource_list) + NS_INT16SZ;

          dn_expand (ns_msg_base (message_handle), message_end, data_start, tmp_resource_name, NI_MAXHOST);

          break;
        }

      cv_dns_record_list->push_back (tmp_resource_name);

      // Clean-up
      free (tmp_resource_name);
      }
    }
  }

//
// Public Class Methods
//

/**
 * Convert the internal \p DNS resource representation to a
 * \c std::string. This does not modify the internal
 * representation of the \p DNS resource in any way.
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
const std::string DNSNames::to_str (void) const {
  try {

    //
    // Return the string representing the name, picked according to the
    // resource type
    //
    switch (cv_dns_query_type) {
      case DNSQueryType::A:
        return cv_dns_record_list->back();

      case DNSQueryType::AAAA:
        return cv_dns_record_list->back();

      default:
        return cv_dns_record_list->back();
      }
    }

  catch (DNSResolverException) {
    throw DNSNameConversionException ("Invalid address");
    }
  }

/**
 * Convert the internal \p DNS resource representation to a
 * \p C-style (\c char*) string. This does not modify the internal
 * representation of the \p DNS resource in any way.
 *
 * \warning the allocated \p C string must be freed by the caller.
 *
 * \retval char* A \p C-style string, representing the internal
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
const char* DNSNames::to_c_str (void) const {
  string return_string;

  try {

    //
    // Return the string representing the name, picked according to the
    // resource type
    //
    switch (cv_dns_query_type) {
      case DNSQueryType::A:
        return_string = cv_dns_record_list->back();
        return return_string.c_str();

      case DNSQueryType::AAAA:
        return_string = cv_dns_record_list->back();
        return return_string.c_str();

      default:
        return_string = cv_dns_record_list->back();
        return return_string.c_str();
      }
    }


  catch (DNSResolverException) {
    throw DNSNameConversionException ("Invalid address");
    }
  }

/**
 * Convert the internal \p DNS resource representation to a
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
 *      \p IP address, perform additonal lookups to find a name that can
 *      be converted. This is primarily useful if the original name is of
 *      type \c DNSQueryType::MX or \c DNSQueryType::SVR.
 *
 *    \param [in] prefer_legacy If possible, return \p IPv4 address instead of
 *      \p IPv6 addresses. By default the library will return \p IPv6 addresses
 *      if at all possible.
 *
 * \retval boost::asio::ip::address An \p IPv4 or \p IPv6 address record. We don't
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
const boost::asio::ip::address DNSNames::to_ip (bool recursive, bool prefer_legacy)  {
  ip::address return_address;    // The IP(4/6) address to return to the caller

  try {

    switch (cv_dns_query_type) {
      case DNSQueryType::A:
        //
        // Resolve a named resource held in an A record to an IP address
        //
        return return_address.from_string (cv_dns_record_list->back());


      case DNSQueryType::AAAA:
        //
        // Resolve a named resource held in an AAAA record to an IP address
        //
        return return_address.from_string (cv_dns_record_list->back());


      default:
        return return_address.from_string (cv_dns_record_list->back());
      }
    }

  catch (DNSResolverException) {
    throw DNSNameConversionException ("Invalid address");
    }

  return return_address;
  }
