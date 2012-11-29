/*!
 * \file Option.h
 * \brief Defines an Option class as an option set for a stage.
 *
 * This file contains the definition for then CLAP :: Option Class.
 * This is a class to hold an Option structure so that options can be set during
 * a particular stage..
 */

#ifndef _CLAP_OPTION_H_
#define _CLAP_OPTION_H_

#include "Types.h"

using namespace libCLAP;

namespace libCLAP {
namespace CLAP {
/*!
 * \class Option
 * \brief A class to define an option of a stage.
 *
 * An Option is an option set during a stage in an execution plan.
 * An Option is essentially a Switch with less details.
 */
class Option {
  public :

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Option
     * class. This constructor sets the value of the option to "" by
     * default.
     *
     * \param name A Common :: String holding the name of the Option.
     */
    Option (const Common :: String& name);

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP :: Option
     * class.
     *
     * \param name A Common :: String holding the name of the Option.
     * \param value A Common :: String holding the value of the Option.
     */
    Option (const Common :: String& name,
            const Common :: String& value);

    /*!
     * \brief Copy Constructor
     *
     * Constructor for the Development :: CLAP :: Option
     * class.reates a copy Option object.
     * \param copy A Option type to copy.
     */
    Option (const Option& copy);

    /*!
     * \brief Destructor
     */
    virtual ~Option (void) throw ();

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
    virtual const Common :: String& Value (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Value
    (const Common :: String& variable);

    /*!
     * \brief Operator =
     *
     * Assignment operator.
     */
    virtual void operator = (const Option& assign);

    /*!
     * \brief Operator ==
     *
     * Comparison operator.
     */
    virtual bool operator == (const Option& compare) const;

    /*!
     * \brief Operator !=
     *
     * Comparison operator.
     */
    virtual bool operator != (const Option& compare) const;

  private :

    Common :: String _Name; /*< The name of the Option object.*/

    Common :: String _Value; /*< The value of the Option object, it is
                                      not necessary to set.*/
  };
}
}
#endif
