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
*** \file   dnsutils.h
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

#include <list>
#include <stdexcept>
#include <string>

#include <ldns/ldns.h>

using namespace std;

/***
 *** Exception Class
 ***
 **/

/** Generic exception class for \tt DNS errors. Where possible more specific exceptions
 *  will be raised by the helper functions and classes.
 */
class DNSException : public std::runtime_error {
  public:
    DNSException (const std::string&  exception_message) : std::runtime_error (exception_message) {}
  };

/** Resolver exception class, indicating either a failure to contact a valid resolver or
 *  an internal failure within that resolver.
 *
 *  \note This exception may also indicate a broader network failure, of which the resolver
 *  failure would therefore only be a symptom. If this exception turn up 'randomly': check
 *  the network!
 */

class DNSResolverException : public DNSException {
  public:
    DNSResolverException (const std::string&  exception_message) : DNSException (exception_message) {}
  };

/***
 *** Data Types
 ***
 **/

/** Valid DNS resource types, understood by this wrapper.
 *
 *  \note This is not an exhaustive list of all possible DNS resouce
 *        records: \em only those which can be processed by this
 *        wrapper. If new types are added to this list, the wrapper
 *        code \bold must be modified to match.
 */
enum class DNSQueryType {
  /** No Valid Resource Record Held */
  NO_RECORD,
  /** IPV4 Address Record */
  A,
  /** IPv6 Address Record */
  AAAA,
  /** Mail Exchange Record */
  MX,
  /** Service Record */
  SRV
};

/** Represents a named resource, held within the Domain Name System.
 */
class DNSName {

  private:

    // Class Variables

    /// Holds the internal \c DNSQueryType this object is representing.
    DNSQueryType cv_dns_query_type;

    /// Holds the name (resource data) associated with \c cv_dns_query_type
    std::string cv_dns_query_name;

  protected:

    //
    // Internal type conversions
    //

    /**
     * Convert the weakly typed (\c enum) \c ldns_rr_type to the equivalent
     * strongly-types \c \c DNSQueryType, as used by the \tt DNSUtils library.
     *
     * \note Use of this routine should be rare: ideally construct a complete
     *     \c DNSName object from the \c ldns_rr_type. This makes it much easier
     *     to manipulate the \tt lDNS library data within the wrapper, and avoids
     *     depending on the low-level implementation of the \tt lDNS library.
     *
     *    \param [in] ldns_type The \tt lDNS library type (\c ldns_rr_type) to
     *        lookup, and return as a \c DNSQueryType
     *
     * \retval DNSQueryType The \c DNSQueryType equivalent of the low-level type
     *
     * Example Usage:
     *
     * \code
     *   cv_dns_query_type = convert_ldns_type_to_dns_type(ldns_resource->_rr_type);
     * \endcode
     */
    DNSQueryType convert_ldns_type_to_dns_type (const ldns_rr_type ldns_type);


  public:

    //
    // Constructors
    //

    /**
     * Construct a \c DNSName class, defined as holding no \tt DNS resource
     * record.
     *
     * \warning Internally the resource type will be \c DNSQueryType::NO_RECORD,
     * which may produce strange output for conversion functions.
     */
    DNSName (void) {
      cv_dns_query_type = DNSQueryType::NO_RECORD;
      }

    /**
     * Construct a \c DNSName class to hold the given \tt DNS resource
     * record.
     */
    explicit DNSName (const DNSQueryType dns_query_type) {
      cv_dns_query_type = dns_query_type;
      }

    /** Construct a \c DNSName class from the low-level \c ldns_rr_type
     *  type returned by the \c lDNS library.
     */
    explicit DNSName (const ldns_rr_type ldns_resource_type);

    /** Construct a \c DNSName class from the low-level \c ldns_rr
     *  type returned by the \c lDNS library.
     */
    explicit DNSName (const ldns_rr* ldns_resource);

    //
    // Type Conversions
    //

    /**
     * Allow type conversions to a \c std::string. Calls the public
     * method \c to_str to handle the actual conversion.
     *
     * \retval std::string A \c std::string, representing the internal
     *   resource name held by the object.
     *
     * Example Usage:
     *
     * \code
     *    DNSName dns_name(a_dns_resource);
     *    std::string a_string{dns_name()};
     * \endcode
     *
     */
    operator std::string (void) const {
      return to_str();
      }

    /**
     * Allow type conversion to a \tt C-style (\c char*) string. Calls
     * the public method \c to_c_str() to handle the actual conversion.
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
     *    char* a_string{dns_name()};
     * \endcode
     */
    operator const char* (void) const {
      return to_c_str();
      }

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
    const string to_str (void) const;

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
    const char* to_c_str (void) const;

    /**
     * Convert the strongly typed \c DNSQueryType to the equivalent low-level
     * enumeration used by the \tt lDNS library. This both hides the low-level
     * (C-oriented) types, and ensures that the compiler can check verify the
     * type of the exposed \tt DNS routines.
     *
     * \retval ldns_rr_type The \tt lDNS equivalent of the high-level type
     *
     * Example Usage:
     *
     * \code
     *    // USe the high-level type in a low-level \tt lDNS call
     *    ldns_packet = ldns_resolver_query (ldns_resolver, ldns_query_name, dns_query_type.convert_to_ldns_resource(), LDNS_RR_CLASS_IN, LDNS_RD);
     * \endcode
     */
    ldns_rr_type convert_to_ldns_resource();

  };

/**
 *  Holds a \c list of \c DNSName records, usually returned by one of
 *  the utility functions.
 */
typedef list<DNSName> DNSNameList;

/***
 *** Functions
 ***
 **/

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
DNSNameList* get_dns_names (const std::string dns_query_name, const DNSQueryType dns_query_type);
