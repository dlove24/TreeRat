/**
*** Parses the command line arguments to the core \c treerat
*** executable, and hands control to the relevant sub-command.
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
*** \file   main.cpp
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

//* Global options map. Holds the state of all known options
map<string, bool> globalOptions;

/** Main loop.
 */
int main (int argc, char** argv) {
  string ApplicationName = "TreeRat";
  string VersionNumber = "0.0.1";
  string Description = "Configures network nodes, using data held in the ONA database";
  string CMD = argv[0];

  // Set-up the command line parser
  CLI parser (ApplicationName,
              CMD,
              VersionNumber,
              Description,
              '=',
              '-');

  /**
   ** Global Options
   **
   */

  // Add the "--help" option
  string optHelp_Name {"help"};
  string optHelp_Description {"show the options of this command"};
  char optHelp_ShortName {'h'};

  Switch optHelp (optHelp_Name, optHelp_ShortName, optHelp_Description);
  parser.AddSwitch (optHelp);

  // Add the "--verbose" option
  string optVerbose_Name {"verbose"};
  string optVerbose_Description {"enable verbose output"};
  char optVerbose_ShortName {'v'};

  Switch optVerbose (optVerbose_Name, optVerbose_ShortName, optVerbose_Description);
  parser.AddSwitch (optVerbose);

  /**
   ** Subcommands
   **
   */

  //
  // Refresh subcommand
  //

  string cmdRefresh_Name {"refresh"};
  string cmdRefresh_Description {"refresh the YAML configuration files from the ONA database"};

  SubCommand cmdRefresh (cmdRefresh_Name, cmdRefresh_Description);
  parser.AddSubCommand (cmdRefresh);

  // Add the "--host" option
  string cmdRefresh_optHost_Name {"host"};
  string cmdRefresh_optHost_Description {"the name (or IP address) on the ONA host"};
  char cmdRefresh_optHost_ShortName {'h'};

  Switch cmdRefresh_optHost (cmdRefresh_optHost_Name, cmdRefresh_optHost_ShortName, cmdRefresh_optHost_Description);
  cmdRefresh.AddSwitch (cmdRefresh_optHost);

  /**
   ** Parse the command line, and set-up the graph for
   ** the execution plan
   **
   */

  ExecutionPlan* plan;

  try {
    plan = & (parser.Parse (argc, argv));
    }

  catch (Exception& e) {
    cout << e.What() << endl;
    return EXIT_FAILURE;
    }

  // Parse the options of the global stage first

  auto globalStage = plan->Current();

  if (globalStage->Name() == "TreeRat") {
    for (auto option : globalStage->Options()) {
      globalOptions[option.first] = true;
      }
    }

  // Get the name of the sub-command (if more than
  // one command is given it should be ignored. In this
  // case CLAP will stop us parsing options, etc. anyway.).
  //
  // Once we have found the name of the next sub-command,
  // control is then handed over to it.

  plan->Next();

  if (globalStage->Next() != nullptr) {
    Stage* currentStage {plan->Current() };

    // Refresh sub-command
    if (currentStage->Name() == "refresh") {
      for (auto option : currentStage->Options()) {
        cout << "\toption: " << option.first << "=" <<  option.second->Value() << endl;
        }
      }

    // Return to the caller (exiting the program)
    delete plan;
    return EXIT_SUCCESS;
    }


  // If we have no sub-command to parse, that is an error so show the help text to
  // the user and abort

  delete plan;

  string help = parser.HelpMenu ();
  cout << help << endl;

  help = parser.HelpMenu (cmdRefresh);
  cout << help << endl;

  return EXIT_FAILURE;
  }
