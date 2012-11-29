/*!
 * \file Stage.h
 * \brief Defines a Stage class as stage in the execution plan after parsing the
 *        arguments
 *
 * This file contains the definition for the CLAP :: Stage Class. This is a
 * class to generate a stage to execute given inputs and a set of options and
 * subcommands available.
 */

#ifndef _CLAP_STAGE_H_
#define _CLAP_STAGE_H_

#include "Types.h"
#include "Option.h"

using namespace libCLAP;

namespace libCLAP {
namespace CLAP {
class Option;
/*!
 * \class Stage
 * \brief A class to define a Stage object for a given input.
 *
 * A stage is a step in an ExecutionPlan. A Stage contains switches
 * and also arguments for each step of command execution. A Stage can
 * be thought of as a thinned down subcommand without all the
 * superfluous information, such as Description, etc. A Stage also acts
 * as a doubly linked list node. It has a parent and a next pointer.
 */
class Stage {
  public :
    /*!
     * \brief Constructor
     *
     * Constructor that constructs the object with no arguments or
     * switches.
     *
     * \param name A String that holds the name of the stage.
     */
    Stage (const Common :: String& name);

    /*!
     * \brief Constructor
     *
     * Constructor that constructs the object with the parent as the
     * parent stage
     *
     * \param name A String that holds the name of the stage.
     * \param parent A Stage that holds the parent of this current
     *               stage.
     */
    Stage (const Common :: String& name,
           Stage& parent);

    /*!
     * \brief Copy Constructor
     *
     * Constructor that constructs the object as a copy of the parameter
     *
     * \param copy A Stage that holds the stage to be copied.
     */
    Stage (const Stage& copy);

    /*!
     * \brief Destructor
     */
    virtual ~Stage (void) throw ();

    /*!
     * \brief An AddOption function
     *
     * A function to add a switch as a switch of this
     * Stage
     *
     * \param addition A Option object to add as a switch of this
     *                 Stage
     */
    virtual void AddOption (Option& addition);

    /*!
     * \brief An AddArgument function
     *
     * A function to add an argument as an argument of this
     * Stage. Note that the order is preserved when arguments are
     * encountered, relative to other arguments. This why a queue is
     * used instead of just a vector.
     *
     * \param addition A Common :: String object to add as an argument
     *                 of this stage
     */
    virtual void AddArgument (Common :: String& addition);

    /*!
     * \brief A Shift function
     *
     * This function is analogous to shift in perl. It shifts the top
     * most part of an array. In this instance, this shifts the
     * Arguments queue. Essentially Shift is a useful alias for top ()
     * and pop () consecutively.
     *
     * \return A Common :: String object that is the next argument in
     *         the list.
     */
    virtual const Common :: String Shift (void);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual map < Common :: String, Option* >& Options (void);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, Option* >& Options (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const queue < Common :: String >& Arguments (void) const;

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Name (void) const;

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Parent (void);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Next (void);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Parent (Stage& variable);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Next (Stage& variable);

  private :

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const queue < Common :: String >& Arguments
    (const queue < Common :: String >& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const map < Common :: String, Option* >& Options
    (const map < Common :: String, Option* >& variable);

    /*!
     * \brief Variable Manipulator
     *
     * Standard function to access the class variable.
     */
    virtual const Common :: String& Name (Common :: String& variable);

    map < Common :: String, Option* > _Options;
    /*< The map holding the switches that can be associated with the
     *  SubCommand object.*/

    queue < Common :: String > _Arguments;
    /*< The queue holding the arguments that can be associated with the
     *  main CLI object.*/

    Common :: String _Name; /*< The name of the Stage, this should be
                                     the same as the name of the SubCommand
                                     that the stage represents.*/
    Stage* _Parent; /*< The parent of the current Stage. This will be
                             set to NULL if this is the main stage.*/
    Stage* _Next; /*< The next child of the current Stage. This will be
                           set to NULL if this is the last stage.*/
  };
}
}
#endif
