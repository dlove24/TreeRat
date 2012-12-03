/**
*** Test the core functionality of the lDNS library.
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
*** This ensure that we have built the library correctly, and that it is at
*** least somewhat sane.
***
*** \file   test_ldns_lookup.cpp
***
*** \author David Love
*** \date   December 2012
**/

#include <config/config.h>

#include <iostream>
#include <string>

#include <tap.h>

#include <utils/fileutils.h>

#include <ldns/ldns.h>

using namespace std;
using namespace TAP;

int main () {
  char format_str[] = "address";
  const ldns_output_format   test_dns_output_format_record = { LDNS_COMMENT_KEY | LDNS_COMMENT_BUBBLEBABBLE | LDNS_COMMENT_FLAGS, &format_str };
  const ldns_output_format*  test_dns_output_format = &test_dns_output_format_record;

  ldns_resolver* res;
  ldns_rdf* domain;
  ldns_pkt* p;
  ldns_rr_list* mx;
  ldns_status s;

  plan (4);

  // Try an A record lookup against a known target

  domain = ldns_dname_new_frm_str ("www.homeunix.org.uk");
  s = ldns_resolver_new_frm_file (&res, NULL);
  p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_A, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_A, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (mx);
  is (ldns_rr_list2str_fmt (test_dns_output_format, mx), "www.homeunix.org.uk.	86400	IN	A	81.187.233.188", "Unexpected return for Homeunix A record");

  ldns_rr_list_deep_free (mx);
  ldns_pkt_free (p);
  ldns_resolver_deep_free (res);

  // Try an AAAA record lookup against a known target

  domain = ldns_dname_new_frm_str ("www.homeunix.org.uk");
  s = ldns_resolver_new_frm_file (&res, NULL);
  p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_AAAA, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_AAAA, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (mx);
  is (ldns_rr_list2str_fmt (test_dns_output_format, mx), "www.homeunix.org.uk.	86400	IN	AAAA	2001:8b0:1698:cf71::50:0", "Unexpected return for Homeunix A record");

  ldns_rr_list_deep_free (mx);
  ldns_pkt_free (p);
  ldns_resolver_deep_free (res);

  // Try an MX record lookup against a known target

  domain = ldns_dname_new_frm_str ("homeunix.org.uk");
  s = ldns_resolver_new_frm_file (&res, NULL);
  p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_MX, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_MX, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (mx);
  is (ldns_rr_list2str_fmt (test_dns_output_format, mx), "homeunix.org.uk.	86400	IN	MX	10 hotmail.homeunix.org.uk.", "Unexpected return for Homeunix A record");

  ldns_rr_list_deep_free (mx);
  ldns_pkt_free (p);
  ldns_resolver_deep_free (res);

  // Lookup a known invalid host

  domain = ldns_dname_new_frm_str ("invalid-host.homeunix.org.uk");
  s = ldns_resolver_new_frm_file (&res, NULL);
  p = ldns_resolver_query (res, domain, LDNS_RR_TYPE_MX, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type (p, LDNS_RR_TYPE_MX, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort (mx);
  is (ldns_rr_list2str_fmt (test_dns_output_format, mx), "", "Found record for an invalid host");

  ldns_rr_list_deep_free (mx);
  ldns_pkt_free (p);
  ldns_resolver_deep_free (res);

  return exit_status();
  }
