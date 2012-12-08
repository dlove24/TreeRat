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

int main () {
  _addr addr;
  DnsMessage* q = NULL, *a = NULL;
  pos_cliresolver res;
  a_record ip;

  try {
    txt_to_addr (&addr, "127.0.0.1");            // server to query
    q = create_query ("acdam.net", DNS_TYPE_A);  // query message: A record for acdam.net

    res.query (q, a, &addr);                     // send query
    ip = get_a_record (a);                       // get answer record from query
    printf ("%s has address %d.%d.%d.%d\n",      // print out answer:
            q->questions.begin()->QNAME.tocstr(), // queried domain name
            ip.address[0], ip.address[1],         // IP address
            ip.address[2], ip.address[3]);
    }

  catch (PException p) {                       // catch any poslib errors
    printf ("*** Fatal error: %s\n", p.message); // print error message
    }
  }

if (q) {
  delete q;
  }

if (a) {
  delete a;
  }

/*

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
