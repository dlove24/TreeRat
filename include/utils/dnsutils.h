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
*** A C++ wrapper around the \tt DNS library, providing high level functions
*** to query data held in the Domain Name System (\tt DNS). This wrapper does not
*** aim to expose the full functionality of the \tt DNS library: instead the
*** focus is on ease-of-use and integration into C++ applications.
***
**/

#include <config/config.h>

#include <list>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/asio.hpp>

#ifdef HAVE_RESOLV_H
#  include <resolv.h>
#else
#  error "Core nameserver resolver undefined"
#endif

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#else
#  error "Core ARPA header type is undefined"
#endif

#ifdef HAVE_ARPA_NAMESER_H
# include <arpa/nameser.h>
#else
#  error "Core nameserver header type is undefined"
#endif

#ifdef HAVE_ARAPA_NAMESER_COMPAT_H
#  include <arpa/nameser_compat.h>
#endif

#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#else
#  error "Core INET type library is undefined"
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

/** Low-Level DNS Buffer Type. Used to collect the resources from the
 *  operating system resolver library.
 */
typedef union {
  HEADER hdr;
  u_char buf[NS_PACKETSZ];
  } DNSQueryBuffer;

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
 * within the Domain Name System (\tt DNS). In essence this class holds the response
 * of a \em single \tt DNS query: for instance the \tt IP addresses associated with a
 * given domain name, or the list of mail servers for a particular domain.
 *
 * Although this class assumes that many names may be associated with a given resource
 * name, in practice many names only have one association. For example a query for the
 * \tt A record of a domain name will typically yield a single \tt IPv4 address. Where
 * only one name is found, that name will always be used (and returned) by the
 * conversion methods of this class.
 *
 * In cases where multiple names are found, the class will return the names according
 * the \c DNSQueryType as follows
 *
 *   + \c DNSQueryType::A A random name is selected from the list of names.
 *
 * \note The \tt DNS system provides a mapping of names to other names. Some
 * of these 'names' can be interpreted as IP4/IP6 addresses (e.g. \tt A and
 * \tt AAAA resource types): but the nature of the DNS system allows a much
 * broader set of returned answers. The name of this class alludes to the
 * intention of providing broad, high-level access to data held in the \tt DNS
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
     *   cv_dns_query_type = convert_to_dns_type(dns_resource->_rr_type);
     * \endcode
     */
    DNSQueryType convert_to_dns_type (const int dns_type);

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
    int convert_to_ns_type (const DNSQueryType dns_query_type);

    void send_dns_query (void);

    /**
     * Extract the named resources from a \c DNSQueryBuffer of type \c ns_query_type,
     * returned by the low-level, operating system resolver routines. Even if more than
     * one record type is present, only those of \c ns_query_type will be considered.
     * All valid records of that type will then be parsed, and added to the internal
     * \c cv_dns_record_list.
     *
     *    \param [in] query_buffer The \tt DNS response buffer, returned by a call to
     *      \c res_search or \c res_query.
     *    \param [in] buffer_length The size of the \c query_buffer: usually found as
     *      as the return code for \c res_search or \c res_query.
     *
     * Example Usage:
     *
     * \code
     *   // If the list of DNS records is empty, send a query for the A records
     *   // of the named resource
     *   buffer_length = res_search (cv_dns_query_name.c_str(), C_IN, T_A, (u_char*) &buffer, sizeof (buffer));
     *
     *   // Parse the returned buffer, to create the list of names. The list of names itself
     *   // is placed in cv_dns_record_list
     *   extract_resource_records (&buffer, buffer_length);
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
     * \tt DNS resource record.
     */
    DNSNames (const string dns_name) {
      cv_dns_query_name = dns_name;
      cv_dns_query_type = DNSQueryType::AAAA;
      cv_dns_record_list = new list<string>;
      }

    /**
     * Construct a \c DNSName class to hold the given \tt name of the given
     * \tt DNS resource record type.
     */
    explicit DNSNames (const string dns_name, const DNSQueryType dns_query_type) {
      cv_dns_query_name = dns_name;
      cv_dns_query_type = dns_query_type;
      cv_dns_record_list = new list<string>;

      send_dns_query();
      }

    /**
     * Construct a \c DNSName class to hold the given \tt name of the given
     * \tt DNS resource record type, using a \c DNSPair.
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
     * Allow type conversion to a \c boost::asio::ip::address. Calls
     * the public method \c to_ip() to handle the actual conversion.
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
     */
    operator const boost::asio::ip::address (void) {
      return to_ip();
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
    const boost::asio::ip::address to_ip (bool recursive = true, bool prefer_legacy = false);

  };
