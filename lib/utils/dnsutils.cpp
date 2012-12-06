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

/***
 *** DNSName Class Defition
 ***
 */

//
// Class Constructors
//

/** Construct a \c DNSName class from the low-level \c ldns_rr_type
 *  type returned by the \c lDNS library.
 */
DNSName::DNSName (const ldns_rr_type ldns_resource_type) {


  }

// Class Methods

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
std::string DNSName::to_str (void) const {

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
char* DNSName::to_c_str (void) const {

  }

/**
 * Convert the strongly typed \c DNSQueryType to the equivalent low-level
 * enumeration used by the \tt lDNS library. This both hides the low-level
 * (C-oriented) types, and ensures that the compiler can check verify the
 * type of the exposed \tt DNS routines.
 *
 *    \param [in] dns_query_type The high-level, stringly typed, name of the
 *      resource type.
 *
 * \retval ldns_rr_type The \tt lDNS equivalent of the high-level type
 *
 * Example Usage:
 *
 * \code
 *    // USe the high-level type in a low-level \tt lDNS call
 *    ldns_packet = ldns_resolver_query (ldns_resolver, ldns_query_name, convert_dns_type_to_ldns_resource(dns_query_type), LDNS_RR_CLASS_IN, LDNS_RD);
 * \endcode
 */
ldns_rr_type DNSName::convert_to_ldns_resource() {
  switch (cv_dns_query_type) {
    case   DNSQueryType::NO_RECORD:
      return LDNS_RR_TYPE_A;
    case DNSQueryType::A:
      return LDNS_RR_TYPE_A;
    case DNSQueryType::AAAA:
      return LDNS_RR_TYPE_AAAA;
    case DNSQueryType::MX:
      return LDNS_RR_TYPE_MX;
    case DNSQueryType::SRV:
      return LDNS_RR_TYPE_SRV;
    }
  }

/***
 *** DNS Helper Functions
 ***
 */

/**
 * Ask the \tt DNS system for a list of names, filtered by resource type,
 * associated with a given name in the \tt DNS system.
 *
 * \note The \tt DNS system provides a mapping of names to other names. Some
 * of these 'names' can be interpreted as IP4/IP6 addresses (e.g. \tt A and
 * \tt AAAA resource types): but the nature of the DNS system allows a much
 * broader set of returned answers. The name of this function alludes to the
 * intention of providing broad, high-level access to data held in the DNS
 * system.
 *
 *    \param [in] dns_query_name The name to query, or 'lookup' in the Domain
 *      Name System.
 *
 *    \param [in] dns_query_type Records returned from the Domain Name System
 *      are filtered, returning only resource records of this type.
 *
 * \retval DNSNameList The list of resource records obtained from the Domain Name
 *     System.
 *
 * Example Usage:
 *
 * \code
 *    // Return a list of IP(v4) addresses associated with a domain name. This
 *    // query is equivalent to the traditional 'gethostbyname' function call
 *    name_list = get_dns_names("www.homeunix.org.uk", DNS_RR_TYPE_A);
 * \endcode
*/
DNSNameList* get_dns_names (const string dns_query_name, const DNSQueryType dns_query_type) {
  DNSName dns_name {dns_query_type};                                // Staging area for a \c DNSName record
  DNSNameList* dns_name_list = new DNSNameList;     // A list of \c DNSName records, returned
  // to the caller

  ldns_resolver* ldns_resolver;     // Points to the low-level structure holding the status
  // and internal state of the DNS resolver
  ldns_rdf* ldns_query_name;        // The resource to lookup
  ldns_pkt* ldns_packet;            // Staging area for the packet being built to service
  // the resource lookup request
  ldns_rr_list* ldns_resource_list; // Low-level list of resource records, returned by the
  // lDNS library

  ldns_status lookup_status;        // Result code returned by the lDNS library

  // Create the query structure from the \c dns_query_name, passed in as the name
  // of the resource to lookup
  ldns_query_name = ldns_dname_new_frm_str (dns_query_name.c_str());

  // Attempt to create a resolver, ready to answer the callers query. If this fails
  // then we need to abort (after cleaning up), as we will not be able to either fufill
  // the request, or return a valid list of \c DNSName's
  lookup_status = ldns_resolver_new_frm_file (&ldns_resolver, NULL);

  if (lookup_status != LDNS_STATUS_OK) {
    throw DNSResolverException ("DNS resolution failed");
    }

  // We should now have a valid resolver. The next step is to use the resolver to obtain the requested
  // resource record from the Domain Name System (assuming we are using the Internet class)
  ldns_packet = ldns_resolver_query (ldns_resolver, ldns_query_name, dns_name.convert_to_ldns_resource(), LDNS_RR_CLASS_IN, LDNS_RD);
  ldns_resource_list = ldns_pkt_rr_list_by_type (ldns_packet, LDNS_RR_TYPE_A, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (ldns_resource_list);

  ldns_rr_list_deep_free (ldns_resource_list);
  ldns_pkt_free (ldns_packet);
  ldns_resolver_deep_free (ldns_resolver);

  return dns_name_list;

  }

