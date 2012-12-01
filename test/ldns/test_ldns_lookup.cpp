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

int main (int argc, char** argv) {
  ldns_resolver* res;
  ldns_rdf* domain;
  ldns_pkt* p;
  ldns_rr_list* mx;
  ldns_status s;

  plan (2);

  isnt (run (prog_location, ""), "", "No output when called with no arguments");
  isnt (check_status (prog_location, ""), 0, "Exit status is zero when called with no arguments");

  return exit_status();
  }
