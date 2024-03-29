/**
*** Portable utilities for manipulating processes and files.
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
*** \file   fileutils.cpp
***
*** \author David Love (david@homeunix.org.uk)
*** \date   December, 2012
***
*** These functions complement the Boost portatable file routines, filling in
*** gaps in that library.
***
**/

#include <utils/fileutils.h>

#include <config/config.h>

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

using namespace std;

/** Run the command, with the given arguments, under the system shell. If
 *  the command runs successfully, the output from \c stdout is returned
 *  to the caller. Otherwise the function raises the exception ?? and
 *  retuns an empty string to the caller.
 *
 *    \param [in] program_name The name (and path if needed) of the
 *      program to execute.
 *
 *    \param [in] program_arguments Literial arguments passed to the
 *      program. These are not interpreted any further (e.g. expanding
 *      shell globs).
 *
 * \retval TRUE   The output from the command (may be empty: but will be
 *   a valid string).
 *
 * \retval FALSE  An empty string,
 *
 * Example Usage:
 *
 * \code
 *    example_nada(3); // Do nothing 3 times.
 * \endcode
*/
string run (const string program_name, const string program_arguments) {
  string command {program_name + " " + program_arguments};
  FILE* cmd_pipe {popen (command.c_str(), "r") };
  string result = "";

  if (!cmd_pipe) {
    throw CommandExecuteException ("Failed to execute the specified command");
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

/** Run the command, with the given arguments, and test the return
 *  value. All the output from the command is discarded by this
 *  function: only the return value is returned to the caller.
 *
 *    \param [in] program_name The name (and path if needed) of the
 *    program to execute.

 *    \param [in] program_arguments Literial arguments passed to the
 *      program. These are not interpreted any further (e.g. expanding
 *      shell globs).
 *
 * \retval Boolean If the command runs sucessfully, the function returns
 *   \c true, otheriwse the function returns \c false.
 *
*/
bool check_status (const string program_name, const string program_arguments) {
  string command = program_name + " " + program_arguments;
  return (system (command.c_str()) == EXIT_SUCCESS);
  }
