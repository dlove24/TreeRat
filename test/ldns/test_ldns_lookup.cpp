/**
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
*** Test the core functionality of the lDNS library. This ensure that we
*** have built the library correctly, and that it is at least somewhat
*** sane.
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
  ldns_resolver* res;
  ldns_rdf* domain;
  ldns_pkt* p;
  ldns_rr_list* mx;
  ldns_status s;

  //domain = ldns_dname_new_frm_str("time.aaisp.net.uk");
  domain = ldns_dname_new_frm_str("www.google.com");
  s = ldns_resolver_new_frm_file(&res, NULL);
  p = ldns_resolver_query(res, domain, LDNS_RR_TYPE_A, LDNS_RR_CLASS_IN, LDNS_RD);
  mx = ldns_pkt_rr_list_by_type(p, LDNS_RR_TYPE_A, LDNS_SECTION_ANSWER);

  ldns_rr_list_sort(mx);
  ldns_rr_list_print(stdout, mx);

  ldns_rr_list_deep_free(mx);
  ldns_pkt_free(p);
  ldns_resolver_deep_free(res);

  plan (2);

  //isnt (p, "", "No output when called with no arguments");
  //isnt (p, 0, "Exit status is zero when called with no arguments");

  return exit_status();
  }
