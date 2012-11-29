#include <iostream>
#include <string>
#include "../CLAP/CLI.h"
#include "../CLAP/ExecutionPlan.h"

using namespace std;
using namespace libCLAP;
using namespace libCLAP :: CLAP;

int main (int argc, char** argv) {
  Common :: String ApplicationName = "CLITest";
  Common :: String VersionNumber = "1.0 - beta";
  Common :: String Description = "CLITest is a utility to test the CLAP argument parser";
  Common :: String CMD = argv [ 0 ];

  CLI parser (ApplicationName,
              CMD,
              VersionNumber,
              Description,
              '=',
              '-');

  Common :: String sw1Name = "switch1";
  char sw1Abb = '1';
  Common :: String sw1Des = "Switch 1 Description";
  Switch sw1 (sw1Name, sw1Abb, sw1Des);
  parser.AddSwitch (sw1);

  Common :: String helpname = "help";
  char helpab = 'h';
  Common :: String helpdes = "help description";
  Switch helpsw (helpname, helpab, helpdes);
  parser.AddSwitch (helpsw);

  Common :: String com1Name = "com1";
  Common :: String com1Abb1 = "c1";
  Common :: String com1Abb2 = "cm1";
  vector < Common :: String > com1abbs;
  com1abbs.push_back (com1Abb1);
  com1abbs.push_back (com1Abb2);
  Common :: String com1Des = "Com1 Description";
  SubCommand com1 (com1Name, com1abbs, com1Des);
  parser.AddSubCommand (com1);

  Common :: String sw2Name = "swtch2";
  char sw2Abb = '2';
  Common :: String sw2Des = "Switch 2 Description";
  Switch sw2 (sw2Name, sw2Abb, sw2Des);
  com1.AddSwitch (sw2);

  Common :: String sw3Name = "swch3";
  char sw3Abb = '3';
  Common :: String sw3Des = "Switch 3 Description";
  Switch sw3 (sw3Name, sw3Abb, sw3Des);
  com1.AddSwitch (sw3);

  Common :: String com2Name = "com2";
  Common :: String com2Abb = "c2";
  Common :: String com2Des = "Com2 Description";
  SubCommand com2 (com2Name, com2Abb, com2Des);
  parser.AddSubCommand (com2);

  Common :: String com3Name = "com3";
  Common :: String com3Abb = "c3";
  Common :: String com3Des = "Com3 Description";
  SubCommand com3 (com3Name, com3Abb, com3Des);
  com2.AddSubCommand (com3);
  com1.AddSubCommand (com2);

  Common :: String help = parser.HelpMenu ();
  cout << help << "\n";

  help = parser.HelpMenu (com1);
  cout << help << "\n";

  help = parser.HelpMenu (com2);
  cout << help << "\n";

  help = parser.HelpMenu (com3);
  cout << help << "\n";
  ExecutionPlan* plan;

  try {
    plan = & (parser.Parse (argc, argv));
    }

  catch (Exception& e) {
    cout << e.What () << "\n";
    return 1;
    }

  Stage* curr = plan->Current ();
  cout << curr->Name () << "\n";
  std :: map < Common :: String, Option* > :: const_iterator i;

  for (i = curr->Options ().begin (); i != curr->Options ().end (); i++) {
    cout << "\t" << (*i).first << " = " << (*i).second->Value () << "\n";
    }

  while (! curr->Arguments().empty()) {
    cout << "\t" << curr->Shift() << "\n";
    }

  while (curr->Next () != NULL) {
    plan->Next ();
    curr = plan->Current ();
    cout << curr->Name () << "\n";

    for (i = curr->Options ().begin (); i != curr->Options ().end (); i++) {
      cout << "\t" << (*i).first << " = " << (*i).second->Value () << "\n";
      }

    while (! curr->Arguments().empty()) {
      cout << "\t" << curr->Shift() << "\n";
      }
    }

  Common :: String stagename = plan->End ()->Name ();

  if (stagename == "com3") {
    cout << "ending stage is com3\n";
    }

  else if (stagename == "com2") {
    cout << "ending stage is com2\n";
    }

  else if (stagename == "com1") {
    Common :: String help = parser.HelpMenu ();
    cout << help << "\n";
    help = parser.HelpMenu (com1);
    cout << help << "\n";
    }

  else {
    cout << "ending stage is base\n";
    }

  delete plan;
  }
