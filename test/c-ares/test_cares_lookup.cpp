/**
*** Test the core functionality of the C-Ares library.
***
*** Copyright(c) 2012 David Love <d.love@shu.ac.uk>
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
*** This ensure that we have built the library correctly, and that it is at
*** least somewhat sane.
***
*** \file   test_cares_lookup.cpp
***
*** \author David Love
*** \date   December 2012
**/

#include <config/config.h>

#include <iostream>
#include <string>
#include <utility>

#include <tap.h>

#include <c-ares/ares.h>
#include <c-ares/nameser.h>

#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif

#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace TAP;

/**
 * Returns the requested data from the \tt DNS resolver, based on the the original
 * query type. This function is now called directly, but via the \tt C-Ares
 * \c ares_query call, and is executed when the \tt C-Ares library is ready
 * to return data.
 *
 * Once called, the function assumes we have a \c pair<int,string>* for the
 * \c arg parameter: the \c int indicating the query type and the \c string
 * the final response handed back to the caller. The other parameters can
 * be considered opaque types, and should be set-up by the original
 * \c ares_query call. Internally they are used to sanity check the response
 * from the \tt DNS server, and to prepare the final response returned in
 * the \c string to the caller.
 *
 *    \param [in,out] response_buffer Parses the returned data (using the type
 *                                    recorded in the \c int parameter), and stores
 *                                    the result in the \c string paramater of the
 *                                    \pair
 *    \param [in]     query_status    The status of the returned query data
 *    \param [in]     query_timeouts  Number of timeouts encountered during the query
 *    \param [in]     answer_buffer   Raw query data. Must be parsed before use.
 *    \param [in]     answer_buffer_length The length of the raw \c answer_buffer
 *
 */
void dns_callback (void* response_buffer, int query_status, int query_timeouts,
                   unsigned char* answer_buffer, int answer_buffer_length) {

  int parse_status;             // Holds the error code for the attempted parse of
  // the raw query data
  struct hostent* name_records; // List of name records returned by the DNS query

  pair<int, string>* buffer;    // Temporary de-reference of the response_buffer pair

  struct in_addr** addr_list;
  int i;

  buffer = static_cast<pair<int, string>*> (response_buffer);

  //
  // Parse the \c answer buffer, based on the query type
  //

  printf ("# Switch: %i\n", buffer->first);

  switch (buffer->first) {
    case T_A:
      parse_status = ares_parse_a_reply (answer_buffer, answer_buffer_length, &name_records, NULL, NULL);

    case T_AAAA:
      parse_status = ares_parse_aaaa_reply (answer_buffer, answer_buffer_length, &name_records, NULL, NULL);
    }

  printf ("# Query Status: %i\n", query_status);
  printf ("# Parse Status: %i\n", parse_status);

  if (parse_status == ARES_SUCCESS) {
      int x, x2;

    /*
     * Report the findings:
     */
    printf (" Officially:\t%s\n", name_records->h_name);
    fputs (" Aliases:\t", stdout);

    for (x2 = 0; name_records->h_aliases[x2]; ++x2) {
      if (x2) {
        fputs (", ", stdout);
        }

      fputs (name_records->h_aliases[x2], stdout);
      }

    fputc ('\n', stdout);
    printf (" Type:\t\t%s\n",
            name_records->h_addrtype == AF_INET
            ? "AF_INET" : "AF_INET6");

    if (name_records->h_addrtype == AF_INET) {
      for (x2 = 0; name_records->h_addr_list[x2]; ++x2) {
        printf (" Address:\t%s\n",
                inet_ntoa (* (struct in_addr*)
                           name_records->h_addr_list[x2]));
        }
      }

    putchar ('\n');

    buffer->second = name_records->h_addr_list[0];
    }

  else {
    buffer->second = "";
    }

  if (name_records != nullptr) {
    ares_free_hostent (name_records);
    }
  }

void a_callback (void* arg, int status, int timeouts, struct hostent* host) {
  if (status == ARES_SUCCESS) {
    std::cout << host->h_name << "\n";
    }

  else {
    std::cout << "lookup failed: " << status << '\n';
    }
  }

void main_loop (ares_channel& channel) {
  int nfds, count;
  fd_set readers, writers;
  timeval tv, *tvp;

  while (1) {
    FD_ZERO (&readers);
    FD_ZERO (&writers);
    nfds = ares_fds (channel, &readers, &writers);

    if (nfds == 0) {
      break;
      }

    tvp = ares_timeout (channel, NULL, &tv);
    count = select (nfds, &readers, &writers, NULL, tvp);
    ares_process (channel, &readers, &writers);
    }

  }

int main () {
  int resolver_status;      // Resolver status
  ares_channel channel;     // Holds the structure representing the channel between us and the resolver

  pair <int, string> answer_buffer;     // Query answer buffer

  plan (1);

  //
  // Initalise the resolver library
  //
  ares_library_init (ARES_LIB_INIT_ALL);

  if ( (resolver_status = ares_init (&channel)) != ARES_SUCCESS) {
    bail_out ("Cannot initalise the DNS resolver library");
    }

  //
  // Try an A record lookup against a known target
  //

  answer_buffer.first = T_A;
  ares_query (channel, "www.homeunix.org.uk", C_IN, T_A, dns_callback, &answer_buffer);
  main_loop (channel);

  is (answer_buffer.second, "81.187.233.188", "Lookup known A record");

  //
  // Try an AAAA record lookup against a known target
  //

  answer_buffer.first = T_AAAA;
  ares_query (channel, "www.homeunix.org.uk", C_IN, T_AAAA, dns_callback, &answer_buffer);
  main_loop (channel);

  is (answer_buffer.second, "001: 8b0: 1698:cf71::50: 0", "Lookup known AAAA record");

  //
  // Terminate the resolver library and clean-up
  //
  ares_library_cleanup();

  /*





  if ( (resolver_status = ares_init (&channel)) != ARES_SUCCESS) {

  bail_out("Cannot create a resolver for address lookup test");
  }

  ares_query (channel, "www.homeunix.org.uk", AF_INET, T_A, dns_callback, &answer_buffer);
  main_loop (channel);

  cout << "Answer: " << answer_buffer << endl;*/

  //domain = ldns_dname_new_frm_str ();

  /*

  s = ldns_resolver_new_frm_file (&res, NULL);
  p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_AAAA, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_AAAA, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (mx);

  ldns_rr_list_deep_free (mx);
  ldns_pkt_free (p);
  ldns_resolver_deep_free (res);

  // Try an MX record lookup against a known target

  domain = ldns_dname_new_frm_str ("
  homeunix.org.uk");
  s = ldns_resolver_new_frm_file (&res, NULL);
  p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_MX, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_MX, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (mx);
  is (ldns_rr_list2str_fmt (test_dns_output_format, mx), "homeunix.org.uk.  86400 IN  MX  10 hotmail.homeunix.org.uk.", "Unexpected return for Homeunix A record");

        ldns_rr_list_deep_free (mx);
        ldns_pkt_free (p);
        ldns_resolver_deep_free (res);

        // Lookup a known invalid host

        domain = ldns_dname_new_frm_str ("invalid - host.homeunix.org.uk");
        s = ldns_resolver_new_frm_file (&res, NULL);
        p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_MX, LDNS_RR_CLASS_IN, LDNS_RD);
        mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_MX, LDNS_SECTION_ANSWER);

        ldns_rr_list_sort (mx);
        is (ldns_rr_list2str_fmt (test_dns_output_format, mx), "", "Found record for an invalid host");

          ldns_rr_list_deep_free (mx);
          ldns_pkt_free (p);
          ldns_resolver_deep_free (res);*/

  return exit_status();
  }
