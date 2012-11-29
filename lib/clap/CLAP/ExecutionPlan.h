/*!
 * \file ExecutionPlan.h
 * \brief Defines an ExecutionPlan class as plan to execute after parsing the
 *        arguments
 *
 * This file contains the definition for the CLAP :: ExecutionPlan Class. This
 * is a class to generate an execution plan given inputs and a set of options
 * and subcommands available.
 */

#ifndef _CLAP_EXECUTIONPLAN_H_
#define _CLAP_EXECUTIONPLAN_H_

#include "Types.h"
#include "Stage.h"
#include "Exception.h"

using namespace libCLAP;

namespace libCLAP {
namespace CLAP {
class Stage;
/*!
 * \class ExecutionPlan
 * \brief A class to define an execution plan object for a given input.
 *
 * An ExecutionPlan is a sequence of Stages, that contain switches
 * and also arguments for each step of command execution. This class is
 * essentially a doubly linked list of Stages. A Stage can only have
 * one parent and one child. Also when adding stages they have to be
 * added sequentially.
 *
 * For example : nullptr <- Stage1 <-> Stage2 <-> Stage3 -> nullptr
 * That would be an example of what the execution plan structure would
 * look like.
 */
class ExecutionPlan {
  public :

    /*!
     * \brief Constructor
     *
     * Constructor for the Development :: CLAP ::
     * ExecutionPlan
     *
     * \param name A String that holds the name of hte main stage.
     */
    ExecutionPlan (const Common :: String& name);

    /*!
     * \brief Copy Constructor
     *
     * Constructor for the Development :: CLAP ::
     * ExecutionPlan that cnstructs the object as a copy of the parameter
     *
     * \param copy An ExecutionPlan that holds the ExecutionPlan to be
     *             copied.
     */
    ExecutionPlan (const ExecutionPlan& copy);

    /*!
     * \brief Destructor
     */
    virtual ~ExecutionPlan (void) throw ();

    /*!
     * \brief An AddStage Function
     *
     * This function allows you to add a stage onto the ExecutionPlan.
     * The Stage is only added at the end of the ExecutionPlan. That is
     * there cannot be multiple stages associated with one stage and
     * stages cannot be added in the middle of the Plan, they must be
     * added in order.
     *
     * \param addition A Stage object to be added onto the queue. This
     *                 should be allocated on the heap since, we will
     *                 not copy the stage locally.
     */
    virtual void AddStage (Stage& addition);

    /*!
     * \brief A Next Function
     *
     * This function moves the Current pointer to the next Stage in the
     * ExecutionPlan.
     */
    virtual void Next (void);

    /*!
     * \brief A Previous Function
     *
     * This function moves the Current pointer to the previous Stage in
     * the ExecutionPlan.
     */
    virtual void Previous (void);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Main (void);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Current (void);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& End (void);

  private :

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Main (Stage& variable);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& Current (Stage& variable);

    /*!
     * \brief Pointer Variable Manipulator
     *
     * Standard function to access the class pointer variable.
     */
    virtual Stage*& End (Stage& variable);

    Stage* _Main;     /*< A Stage pointer pointing to the main Stage.
                               The Main stage is analagous to the command
                               itself.*/
    Stage* _Current;  /*< A Stage pointer pointing to the current Stage.
                               The current Stage  is the current stage we are
                               working on. An ExecutionPlan can be though of
                               as a doubly linked list of Stages.*/
    Stage* _End;      /*< A Stage pointer pointing to the last stage of
                               the ExecutionPlan.*/
  };
}
}
#endif
