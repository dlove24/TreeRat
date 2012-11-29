/*!
 * \file Exception.h
 * \brief Defines the Exception class.
 */

#ifndef _COMMON_EXCEPTION_H_
#define _COMMON_EXCEPTION_H_

#include "Types.h"

using namespace libCLAP;

namespace libCLAP {
namespace Common {
/*!
 * \class Exception
 * \brief Exception handling for Dynomite framework.
 *
 * Essentially this is a wrapper around errno, for system errors. Custom
 * exceptions should be created by dreiving from this class.
 */
class Exception {
  public :

    /*!
     * \brief Constructor
     *
     * Constructor for the Common :: Exception class.
     */
    Exception (const Common :: Error error);

    /*!
     * \brief Constructor
     *
     * Constructor for the Common :: Exception class.
     */
    Exception (const Common :: ErrorMessage& message);

    /*!
     * \brief Constructor
     *
     * Constructor for the Common :: Exception class.
     */
    Exception (const Common :: Error error,
               const Common :: ErrorMessage& message);

    /*!
     * \brief Copy Constructor
     * \param copy Exception to copy.
     */
    Exception (const Common :: Exception& copy);

    /*!
     * \brief Destructor
     *
     * Destructor for the Common :: Exception class.
     */
    virtual ~Exception (void) throw ();

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: ErrorMessage& What (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: Error& Error (void) const;

  protected :

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: Error& Error
    (const Common :: Error& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: ErrorMessage& What
    (const Common :: ErrorMessage& variable);

    /*!
     * \brief Message function
     *
     * \param error An Error type to convert
     *
     * This function takes error and gives back a string with the error
     * message. It is a helper function used mainly to fill _What when
     * no custom message is specified.
     */
    static Common :: ErrorMessage Message (const Common :: Error& error);

    Common :: ErrorMessage _What;  /*!< This is a string that holds the
                                             Message that can be logged or
                                             displayed.*/
    Common :: Error        _Error; /*!< This is the error code that
                                             caused the exception.*/
  };
}
}
#endif
