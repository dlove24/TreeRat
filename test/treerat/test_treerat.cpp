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
*** \brief  Test the core \c treerat options
***
*** \author David Love
*** \date   November 2012
**/

#include <config/config.h>

#include <iostream>
#include <string>

#include <tap.h>

#include <utils/fileutils.h>

using namespace std;
using namespace TAP;

int main (int argc, char** argv) {

  // This test fails if we have not been given the location of
  // the treerat binary
  if (argc < 1) {
    plan (skip_all, "ERROR: Cannot find the treerat binary!");
    }

  else {
    // Record the location of treerat
    string prog_location = argv[1];

    note ("TreeRat location is: ", prog_location);

    plan (2);

    // Call treerate with no arguments. This should fail, and show the user the
    // help output
    isnt (run (prog_location, ""), "", "No output when called with no arguments");
    isnt (check_status (prog_location, ""), 0, "Exit status is zero when called with no arguments");
    }

  return exit_status();
  }
