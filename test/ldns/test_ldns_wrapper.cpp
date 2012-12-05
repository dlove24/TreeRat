/**
*** Test the functionality of the C++ wrapper to the lDNS library.
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
*** \file   test_ldns_wrapper.cpp
***
*** \author David Love
*** \date   December 2012
**/

#include <config/config.h>

#include <iostream>
#include <string>

#include <tap.h>

#include <utils/dnsutils.h>

using namespace std;
using namespace TAP;

int main () {
  DNSNameList* name_list; //Holds the list of DNS names returned from the lookup
  string name;

  plan (4);

  // Try an A record lookup against a known target
  name_list = get_dns_names ("www.homeunix.org.uk", DNSQueryType::A);
  is (name_list->front().to_str(), "81.187.233.188", "Expected return for Homeunix A record");

  // Try an AAAA record lookup against a known target
  name_list = get_dns_names ("www.homeunix.org.uk", DNSQueryType::AAAA);
  is (name_list->front().to_str(), "2001:8b0:1698:cf71::50:0", "Expected return for Homeunix AAAA record");

  // Try an MX record lookup against a known target
  name_list = get_dns_names ("homeunix.org.uk", DNSQueryType::MX);
  is (name_list->front().to_str(), "hotmail.homeunix.org.uk.", "Expected return for Homeunix MX record");

  // Lookup a known invalid host. This should throw an exception.
  try {
    name_list = get_dns_names ("this-host-is-not-valid.homeunix.org.uk", DNSQueryType::A);
    }
  catch (DNSException) {
    pass ("DNSException raised on invalid record");
    }

  return exit_status();
  }

