#include "config.h"

#include <iostream>
#include <string>

#ifdef HAVE_POPEN
#  ifdef POPEN_PROTO_STDLIB
#    include <stdlib.h>
#  endif
#  ifdef POPEN_PROTO_STDIO
#    include <stdio.h>
#  endif
#else
#  error "No popen function found"
#endif

#include <tap.h>

using namespace std;
using namespace TAP;

/** Run the command, with the given arguments, under the system shell.
*/
string run (const string program_name, const string program_arguments) {
  string command {program_name + " " + program_arguments};
  FILE* cmd_pipe {popen (command.c_str(), "r") };
  string result = "";

  if (!cmd_pipe) {
    bail_out ("Cannot execute the test command");
    }

  // This can be a fixed size because we get the output
  // in chunks: so we cannot overflow.
  char buffer[128];

  while (!feof (cmd_pipe)) {
    if (fgets (buffer, 128, cmd_pipe) != nullptr) {
      result += buffer;
      }
    }

  pclose (cmd_pipe);

  return result;
  }

/** Run the command, with the given arguments, under the system shell.
*/
int check_status (const string program_name, const string program_arguments) {
  string command = program_name + " " + program_arguments;
  return system (command.c_str());
  }

int main (int argc, char** argv) {

  // This test fails if we have not been given the location of
  // the treerat binary
  if (argc < 1) {
    plan (skip_all, "ERROR: Cannot find the treerat binary!");
    }

  else {
    // Record the location of treerat
    string prog_location = argv[1];
    note ("TreeRat locaction is ", prog_location);

    plan (2);

    // Call treerate with no arguments. This should fail, and show the user the
    // help output
    isnt (run (prog_location, ""), "", "No output when called with no arguments");
    isnt (check_status (prog_location, ""), 0, "Exit status is zero when called with no arguments");
    }

  return exit_status();
  }
