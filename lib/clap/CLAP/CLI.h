/*!
 * \file CLI.h
 * \brief Defines a CLI client class as a parser to parse arguments from
 *        the command line.
 *
 * This file contains the definition for the CLAP :: CLI
 * Class. This is a class parse the commandline and create an execution plan for
 * the program based on a given structure for sub commands and switches.
 */

#ifndef _CLAP_CLI_H_
#define _CLAP_CLI_H_

#include "Types.h"
#include "Exception.h"
#include "ExecutionPlan.h"
#include "Switch.h"
#include "SubCommand.h"

using namespace libCLAP;

namespace libCLAP {
namespace CLAP {
/*!
 * \class CLI
 * \brief A class to define a Command line interface for a given program.
 *
 * CLI can have SubCommands of their own. This allows for an
 * entire architecture for programs.
 */
class CLI {
  public :

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP ::
     * CLI class.
     *
     */
    CLI (const Common :: String& name,
         const Common :: String& cmd,
         const Common :: String& version,
         const Common :: String& description,
         const char equals,
         const char escape);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP ::
     * CLI class.
     *
     */
    CLI (const Common :: String& name,
         const Common :: String& version,
         const Common :: String& description);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP ::
     * CLI class.
     *
     */
    CLI (const Common :: String& name,
         const Common :: String& cmd,
         const Common :: String& version,
         const Common :: String& description);

    /*!
     * \brief Copy Constructor
     *
     * Constructor for the Development :: CLAP :: CLI
     * class. Creates a copy CLI object.
     *
     * \param copy A CLI type to copy
     */
    CLI (const CLI& copy);

    /*!
     * \brief Destructor
     */
    virtual ~CLI (void) throw ();

    /*!
     * \brief A Parse function
     *
     * A function to Parse the statements and return a
     * CLI
     *
     * \param argc A Size object that is the size of the argv array.
     * \param argv A char** object that is the argv array of the
     *        program.
     *
     * \return An ExecutionPlan object that is essentially a graph of
     *         the subcommands.
     */
    virtual ExecutionPlan& Parse (int argc, char** argv);

    /*!
     * \brief An AddSubCommand function
     *
     * A function to add a sub-command as a sub-command of this
     * CLI
     *
     * \param addition A SubCommand object to add as a switch of this
     *                 CLI
     */
    virtual void AddSubCommand (SubCommand& addition);

    /*!
     * \brief An AddSwitch function
     *
     * A function to add a switch as a switch of this
     * CLI
     *
     * \param addition A Switch object to add as a switch of this
     *                 CLI
     */
    virtual void AddSwitch (Switch& addition);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Name (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Name
    (const Common :: String& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& CMD (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& CMD
    (const Common :: String& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Version (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Version
    (const Common :: String& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Description (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Description
    (const Common :: String& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const char& Escape (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const char& Escape (const char& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const char& Equals (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const char& Equals (const char& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, SubCommand* >& SubCommands
    (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, Switch* >& Switches (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, SubCommand* >& SubCommandAbbreviations
    (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < char, Switch* >& SwitchAbbreviations (void) const;

    /*!
     * \brief A HelpMenu function
     *
     * A function to generate a helpmenu for the CLI
     *
     * \return A string that contains the help menu to be printed
     */
    virtual Common :: String HelpMenu (void);

    /*!
     * \brief A HelpMenu function
     *
     * A function to generate a helpmenu for the CLI
     *
     * \param subcommand A SubCommand object to print the help menu for.
     *
     * \return A string that contains the help menu to be printed
     */
    virtual Common :: String HelpMenu (const SubCommand& subcommand);

  private  :

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, SubCommand* >& SubCommands
    (const map < Common :: String, SubCommand* >& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, SubCommand* >& SubCommandAbbreviations
    (const map < Common :: String, SubCommand* >& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < char, Switch* >& SwitchAbbreviations
    (const map < char, Switch* >& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, Switch* >& Switches
    (const map < Common :: String, Switch* >& variable);

    Common :: String _Name; /*< The name of the Application.*/

    Common :: String _CMD;  /*< The name of the binary.*/

    char _Escape; /*< The escape character for switches.*/

    char _Equals; /*< The equals character for switches.*/

    Common :: String _Version; /*< The version of the
                                        Application.*/

    Common :: String _Description; /*< The description for the
                                            Application.*/

    map < Common :: String, Switch* > _Switches;
    /*< The map holding the switches that can be associated with the
     *  SubCommand object.*/

    map < Common :: String, SubCommand* > _SubCommands;
    /*< The map holding the subcommands that can be associated with the
     *  SubCommand object.*/

    map < char, Switch* > _SwitchAbbreviations;
    /*< The map holding the switches that can be associated with the
     *  SubCommand object.*/

    map < Common :: String, SubCommand* > _SubCommandAbbreviations;
    /*< The map holding the subcommands that can be associated with the
     *  SubCommand object.*/
  };
}
}
#endif
