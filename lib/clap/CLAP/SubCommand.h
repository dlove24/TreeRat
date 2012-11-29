/*!
 * \file SubCommand.h
 * \brief Defines a SubCommand class as a sub-command passed as an argument.
 *
 * This file contains the definition for then CLAP :: SubCommand
 * Class. This is a class to hold a sub command structure so that chain of
 * command can be followed.
 */

#ifndef _CLAP_SUBCOMMAND_H_
#define _CLAP_SUBCOMMAND_H_

#include "Types.h"
#include "Switch.h"

using namespace libCLAP;

namespace libCLAP {
namespace CLAP {
class Switch;
/*!
 * \class SubCommand
 * \brief A class to define a sub-command of a program.
 *
 * SubCommands can have SubCommands of their own. This allows for an
 * entire architecture for programs.
 */
class SubCommand {
  public :

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: SubCommand
     * class.
     * \param name A String type holding the name of the sub-command
     * \param abbreviations A String type holding the abbreviations for
     *                     the sub-command
     * \param description A String type holding the description for
     *                    the sub-command
     */
    SubCommand (const Common :: String& name,
                const vector < Common :: String >& abbreviations,
                const Common :: String& description);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: SubCommand
     * class.
     * \param name A String type holding the name of the sub-command
     * \param abbreviation A String type holding the abbreviations for
     *                     the sub-command
     * \param description A String type holding the description for
     *                    the sub-command
     */
    SubCommand (const Common :: String& name,
                const Common :: String& abbreviation,
                const Common :: String& description);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: SubCommand
     * class.
     * \param name A String type holding the name of the sub-command
     * \param description A String type holding the description for
     *                    the sub-command
     */
    SubCommand (const Common :: String& name,
                const Common :: String& description);

    /*!
     * \brief Copy Constructor
     *
     * Constructor for the Development :: CLAP :: SubCommand
     * class. Creates a copy SubCommand object.
     *
     * \param copy A SubCommand type to copy
     */
    SubCommand (const SubCommand& copy);

    /*!
     * \brief Destructor
     */
    virtual ~SubCommand (void) throw ();

    /*!
     * \brief An AddSubCommand function
     *
     * A function to add a sub-command as a sub-command of this
     * sub-command.
     *
     * \param addition A SubCommand object to add as a switch of this
     *                 SubCommand.
     */
    virtual void AddSubCommand (SubCommand& addition);

    /*!
     * \brief An AddSwitch function
     *
     * A function to add a switch as a switch of this
     * sub-command.
     *
     * \param addition A Switch object to add as a switch of this
     *                 SubCommand.
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
    virtual const vector < Common :: String >& Abbreviations (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const vector < Common :: String >& Abbreviations
    (const vector < Common :: String >& variable);

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
    virtual const map < Common :: String, SubCommand* >& SubCommands
    (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, Switch* >& Switches (void)
    const;

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
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual map < Common :: String, SubCommand* >& SubCommands
    (void);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual map < Common :: String, Switch* >& Switches (void);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual map < Common :: String, SubCommand* >& SubCommandAbbreviations
    (void);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual map < char, Switch* >& SwitchAbbreviations (void);

    /*!
     * \brief Operator =
     *
     * Assignment operator.
     */
    virtual void operator = (const SubCommand& assign);

    /*!
     * \brief Operator ==
     *
     * Comparison operator.
     */
    virtual bool operator == (const SubCommand& compare) const;

    /*!
     * \brief Operator !=
     *
     * Comparison operator.
     */
    virtual bool operator != (const SubCommand& compare) const;

  private :

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

    Common :: String _Name; /*< The name of the SubCommand object.*/

    Common :: String _Description; /*< The description for the
                                            SubCommand object.*/

    vector < Common :: String > _Abbreviations; /*< The abbreviation of the
                                             SubCommand object.*/

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
