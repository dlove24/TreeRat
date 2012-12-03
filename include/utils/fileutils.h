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
*** \file   fileutils.h
***
*** \author David Love (david@homeunix.org.uk)
*** \date   December, 2012
***
*** Portable utilities for manipulating processes and files. These
*** functions complement the Boost portatable file routines, filling in
*** gaps in that library.
***
**/

#include <string>
#include <stdexcept>

using namespace std;

/***
 *** Exception Class
 ***
 **/

/** Exception class for the file utility function that involve command execution. Usually indicates
 *  that a command could not be executed, or that the execution generated and error of some
 *  description.
 */
class CommandExecuteException : public std::runtime_error {
  public:
    CommandExecuteException (const std::string&  exception_message);
  };

/***
 *** Functions
 ***
 **/

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
 * \retval string The output from the command (may be empty: but will be
 *   a valid string).
 *
 * Example Usage:
 *
 * \code
 *    example_nada(3); // Do nothing 3 times.
 * \endcode
*/
string run (const string program_name, const string program_arguments);

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
 * \retval bool If the command runs sucessfully, the function returns
 *   \c true, otheriwse the function returns \c false.
 *
*/
bool check_status (const string program_name, const string program_arguments);
