/*!
 * \file Switch.h
 * \brief Defines a Switch class as a passed argument.
 *
 * This file contains the definition for then CLAP :: Switch Class.
 * This is a class to hold a switch structure so that options can be set.
 */

#ifndef _CLAP_SWITCH_H_
#define _CLAP_SWITCH_H_

#include "Types.h"

using namespace libCLAP;

namespace libCLAP {
namespace CLAP {
/*!
 * \class Switch
 * \brief A class to define a switch of a subcommand.
 */
class Switch {
  public :

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Switch
     * class.
     * \param name A String type holding the name of the switch
     * \param abbreviations A vector type holding the abbreviation for
     *                     the switch
     * \param description A String type holding the description for
     *                    the switch
     * \param value A String type holding the value for switch.
     */
    Switch (const Common :: String& name,
            const vector < char >& abbreviations,
            const Common :: String& description,
            const Common :: String& value);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Switch
     * class.
     * \param name A String type holding the name of the switch
     * \param abbreviations A vector type holding the abbreviation for
     *                     the switch
     * \param description A String type holding the description for
     *                    the switch
     * \param value A String type holding the value for switch.
     */
    Switch (const Common :: String& name,
            const char abbreviation,
            const Common :: String& description,
            const Common :: String& value);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Switch
     * class.
     * \param name A String type holding the name of the switch
     * \param description A String type holding the description for
     *                    the switch
     * \param abbreviations A vector type holding the abbreviations for
     *                     the switch
     */
    Switch (const Common :: String& name,
            const vector < char >& abbreviations,
            const Common :: String& description);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Switch
     * class.
     * \param name A String type holding the name of the switch
     * \param description A String type holding the description for
     *                    the switch
     * \param abbreviations A vector type holding the abbreviations for
     *                     the switch
     */
    Switch (const Common :: String& name,
            const char abbreviation,
            const Common :: String& description);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Switch
     * class.
     * \param name A String type holding the name of the switch
     * \param description A String type holding the description for
     *                    the switch
     */
    Switch (const Common :: String& name,
            const Common :: String& description);

    /*!
     * \brief Copy Constructor
     *
     * Constructor for the Development :: CLAP :: Switch
     * class.reates a copy Switch object.
     * \param copy A Switch type to copy.
     */
    Switch (const Switch& copy);

    /*!
     * \brief Destructor
     */
    virtual ~Switch (void) throw ();

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
    virtual const vector < char >& Abbreviations (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const vector < char >& Abbreviations
    (const vector < char >& variable);

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
    virtual const Common :: String& Value (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Value
    (const Common :: String& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& ArgumentType (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& ArgumentType (const Common :: String& variable);

    /*!
     * \brief Operator =
     *
     * Assignment operator.
     */
    virtual void operator = (const Switch& assign);

    /*!
     * \brief Operator ==
     *
     * Comparison operator.
     */
    virtual bool operator == (const Switch& compare) const;

    /*!
     * \brief Operator !=
     *
     * Comparison operator.
     */
    virtual bool operator != (const Switch& compare) const;

  private :

    Common :: String _Name; /*< The name of the Switch object.*/

    Common :: String _Description; /*< The description for the
                                            Switch object.*/

#pragma GCC diagnostic ignored "-Wreorder"
    vector < char >  _Abbreviations; /*< The abbreviation of the Switch
                                              object.*/

    Common :: String _Value; /*< The value of the Switch object, it is
                                      not necessary to set.*/

    Common :: String _ArgumentType; /*< The argument type of the Switch object,
                                              it is not necessary to set.*/
  };
}
}
#endif
