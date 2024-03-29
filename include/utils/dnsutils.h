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
*** A C++ wrapper around the \p DNS library, providing high level functions
*** to query data held in the Domain Name System (\p DNS). This wrapper does not
*** aim to expose the full functionality of the \p DNS library: instead the
*** focus is on ease-of-use and integration into C++ applications.
***
**/

#include <config/config.h>

#include <list>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/asio.hpp>

#ifdef HAVE_RESOLV_LIB
#  include <resolv.h>
#  include <arpa/inet.h>
#  include <arpa/nameser_compat.h>
#  include <netinet/in.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#else
#  error "Core system types are undefined"
#endif

using namespace std;

/***
 *** Exception Classes
 ***
 **/

/** Generic exception class for \p DNS errors. Where possible more specific exceptions
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

/** Name to IP address failure exception, indicating the DNS name is not a valid IP address (although
 *  it may still be a valid DNS Name).
 */
class DNSNameConversionException : public DNSException {
  public:
    DNSNameConversionException (const std::string&  exception_message) : DNSException (exception_message) {}
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
 *        code \b must be modified to match.
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

/** Low-Level DNS Buffer Type. Used to collect the resources from the
 *  operating system resolver library.
 */
#ifdef HAVE_RESOLV_LIB
typedef union {
  HEADER hdr;
  u_char buf[NS_PACKETSZ];
  } DNSQueryBuffer;
#endif

/**
 *  Utility typedef for creating a name, \c DNSQueryType pair.
 *
 */
typedef pair<string, DNSQueryType> DNSPair;

/**
 *  Defines the list of DNS records, used as the return type by many
 *  of the wrapper functions.
 *
 */
typedef list<string>* DNSList;

/***
 *** Core Class
 ***
 **/

/**
 * Represents a list named resources, of a type specified as a \c DNSQueryType, held
 * within the Domain Name System (\p DNS). In essence this class holds the response
 * of a \em single \p DNS query: for instance the \p IP addresses associated with a
 * given domain name, or the list of mail servers for a particular domain.
 *
 * Although this class assumes that many names may be associated with a given resource
 * name, in practice many names only have one association. For example a query for the
 * \p A record of a domain name will typically yield a single \p IPv4 address. Where
 * only one name is found, that name will always be used (and returned) by the
 * conversion methods of this class.
 *
 * In cases where multiple names are found, the class will return the names according
 * the \c DNSQueryType as follows
 *
 *   + \c DNSQueryType::A A random name is selected from the list of names.
 *
 * \note The \p DNS system provides a mapping of names to other names. Some
 * of these 'names' can be interpreted as IP4/IP6 addresses (e.g. \p A and
 * \p AAAA resource types): but the nature of the DNS system allows a much
 * broader set of returned answers. The name of this class alludes to the
 * intention of providing broad, high-level access to data held in the \p DNS
 * system.
 */
class DNSNames {

  private:

    // Class Variables

    /// Holds the internal \c DNSQueryType this object is representing.
    DNSQueryType cv_dns_query_type;

    /// Holds the name (resource data) associated with \c cv_dns_query_type
    std::string cv_dns_query_name;

    /// The internal list of DNS records. Usually this is only used by the
    /// internal type conversion routines
    DNSList cv_dns_record_list;

  protected:

    //
    // Internal type conversions
    //

    /**
     * Convert the weakly typed (C-Style \c enum) representing the \p DNS
     * resource type, to the equivalent strongly-types \c \c DNSQueryType
     * used by the \p DNSUtils library.
     *
     *    \param [in] dns_type The \p DNS library type (\c ldns_rr_type) to
     *        lookup, and return as a \c DNSQueryType
     *
     * \retval DNSQueryType The \c DNSQueryType equivalent of the low-level type
     *
     * Example Usage:
     *
     * \code
     *   cv_dns_query_type = convert_to_dns_type(dns_resource->_rr_type);
     * \endcode
     */
    DNSQueryType convert_to_dns_type (const int dns_type);

    /**
     * Convert the strongly typed \c DNSQueryType (used in all public interfaces)
     * to the low-level C-style \c enum used by the operating system resolver
     * libraries.
     *
     *    \param [in] dns_query_type The \c DNSQueryType to lookup, and return as
     *      the equivalent \p DNS library type.
     *
     * \retval int An integer corresponding to the resource type constant in the
     *  low-level resolver routines.
     *
     * Example Usage:
     *
     * \code
     *   // If the list of DNS records is empty, send a query for the A records
     *   // of the named resource
     *   buffer_length = res_search (cv_dns_query_name.c_str(), C_IN, convert_to_ns_type (cv_dns_query_type),
     *                               (u_char*) &buffer, sizeof (buffer));
     * \endcode
     */
    int convert_to_ns_type (const DNSQueryType dns_query_type);

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
    void send_dns_query (void);

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
    void extract_resource_records (const DNSQueryBuffer* query_buffer, const int buffer_length);

  public:

    //
    // Constructors
    //

    /**
     * Construct a \c DNSName class, but leave the class unconfigured.
     *
     * \warning Internally the resource type will be \c DNSQueryType::NO_RECORD,
     * which may produce strange output for conversion functions.
     */
    DNSNames () {
      cv_dns_query_name = "";
      cv_dns_query_type = DNSQueryType::NO_RECORD;
      cv_dns_record_list = new list<string>;
      }

    /**
     * Construct a \c DNSName class, defaulting to the storage of a \c DNSNames::AAAA
     * \p DNS resource record.
     */
    DNSNames (const string dns_name) {
      cv_dns_query_name = dns_name;
      cv_dns_query_type = DNSQueryType::AAAA;
      cv_dns_record_list = new list<string>;
      }

    /**
     * Construct a \c DNSName class to hold the given \p name of the given
     * \p DNS resource record type.
     */
    explicit DNSNames (const string dns_name, const DNSQueryType dns_query_type) {
      cv_dns_query_name = dns_name;
      cv_dns_query_type = dns_query_type;
      cv_dns_record_list = new list<string>;

      send_dns_query();
      }

    /**
     * Construct a \c DNSName class to hold the given \p name of the given
     * \p DNS resource record type, using a \c DNSPair.
     */
    explicit DNSNames (const DNSPair dns_query) {
      cv_dns_query_name = dns_query.first;
      cv_dns_query_type = dns_query.second;
      cv_dns_record_list = new list<string>;

      send_dns_query();
      }

    /** Construct a \c DNSName class from the low-level \c dns_rr_type
     *  type.
     */
    explicit DNSNames (const string dns_name, const int dns_resource_type) {
      cv_dns_query_name = dns_name;
      cv_dns_query_type = convert_to_dns_type (dns_resource_type);
      cv_dns_record_list = new list<string>;

      send_dns_query();
      }

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
     * Allow type conversion to a \p C-style (\c char*) string. Calls
     * the public method \c to_c_str() to handle the actual conversion.
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
     *    char* a_string{dns_name()};
     * \endcode
     */
    operator const char* (void) const {
      return to_c_str();
      }

    /**
     * Allow type conversion to a \c boost::asio::ip::address. Calls
     * the public method \c to_ip() to handle the actual conversion.
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
     */
    operator const boost::asio::ip::address (void) {
      return to_ip();
      }

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
    const string to_str (void) const;

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
    const char* to_c_str (void) const;

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
    const boost::asio::ip::address to_ip (bool recursive = true, bool prefer_legacy = false);

  };
