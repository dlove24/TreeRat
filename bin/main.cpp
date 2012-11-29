/**
*** Copyright (c) 2011 David Love <d.love@shu.ac.uk>
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
*** \file   main.c
*** \brief  Parses the command line arguments to @treerat, and hands control
***         to the relevant sub-command
***
*** \author David Love
*** \date   November 2012
**/

#include <iostream>
#include <string>
#include "CLAP/CLI.h"
#include "CLAP/ExecutionPlan.h"

using namespace std;
using namespace libCLAP;
using namespace libCLAP::CLAP;

int main (int argc, char** argv) {
  Common::String ApplicationName = "TreeRat";
  Common::String VersionNumber = "0.0.1";
  Common::String Description = "Configures network nodes, using data held in the ONA database";
  Common::String CMD = argv[0];

  // Set-up the command line parser
  CLI parser (ApplicationName,
              CMD,
              VersionNumber,
              Description,
              '=',
              '-');

  // Add the "--help" option
  Common::String helpname = "help";
  char helpab = 'h';
  Common::String helpdes = "help description";
  Switch helpsw (helpname, helpab, helpdes);
  parser.AddSwitch (helpsw);

  // Parse the command line, and set-up the graph
  // for the execution plan
  ExecutionPlan* plan;

  try {
    plan = & (parser.Parse (argc, argv));
    }

  catch (Exception& e) {
    cout << e.What() << "\n";
    return 1;
    }

  Stage* curr = plan->Current();
  cout << curr->Name() << "\n";
  std::map <Common::String, Option*> ::const_iterator i;

  for (auto i : curr->Options()) {
    cout << i.first << "=" << i.second->Value() << endl;
    }

  while (! curr->Arguments().empty()) {
    cout << "\t" << curr->Shift() << "\n";
    }

  while (curr->Next() != nullptr) {
    plan->Next();
    curr = plan->Current();
    cout << curr->Name() << "\n";

    for (i = curr->Options().begin(); i != curr->Options().end(); i++) {
      cout << "\t" << (*i).first << " = " << (*i).second->Value() << "\n";
      }

    while (! curr->Arguments().empty()) {
      cout << "\t" << curr->Shift() << "\n";
      }
    }

  Common::String stagename = plan->End()->Name();

  cout << "ending stage is " << stagename << endl;


  delete plan;

  return 0;
  }
