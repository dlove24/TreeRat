/*!
 * \file ExecutionPlan.cpp
 * \brief Implements an ExecutionPlan class as plan to execute after parsing the
 *        arguments
 *
 * This file contains the definition for the CLAP :: ExecutionPlan Class. This
 * is a class to generate an execution plan given inputs and a set of options
 * and subcommands available.
 */

#include "ExecutionPlan.h"

using namespace libCLAP;
using namespace libCLAP :: CLAP;

ExecutionPlan :: ExecutionPlan (const Common :: String& name) {
  _Main = new Stage (name);

  if (_Main == nullptr) {
    throw Exception (Common :: enomem);
    }

  _End = _Main;
  _Current = _Main;
  }

ExecutionPlan :: ExecutionPlan (const ExecutionPlan& copy) {
  _Main = new Stage (* (copy._Main));

  if (_Main == nullptr) {
    throw Exception (Common :: enomem);
    }

  _Current = _Main;
  Stage* temp;

  while (& (_Current->Next ()) != nullptr) {
    temp = new Stage (* (_Current->Next ()));

    if (temp == nullptr) {
      throw Exception (Common :: enomem);
      }

    _Current->Next (*temp);
    temp->Parent (*_Current);
    _Current = temp;
    }

  _End = _Current;
  _Current = _Main;
  }

ExecutionPlan :: ~ExecutionPlan (void) throw () {
  _Current = _Main;
  Stage* temp;

  while (_Current != nullptr) {
    temp = _Current;
    _Current = _Current->Next ();
    delete temp;
    }
  }

void ExecutionPlan :: AddStage (Stage& addition) {
  _End->Next (addition);
  addition.Parent (*_End);
  _End = &addition;
  }

void ExecutionPlan :: Next (void) {
  if (_Current != _End) {
    _Current = _Current->Next ();
    }
  }

void ExecutionPlan :: Previous (void) {
  if (_Current != _Main) {
    _Current = _Current->Parent ();
    }
  }

Stage*& ExecutionPlan :: Main (void) {
  return _Main;
  }

Stage*& ExecutionPlan :: Current (void) {
  return _Current;
  }

Stage*& ExecutionPlan :: Main (Stage& variable) {
  _Main = &variable;
  return _Main;
  }

Stage*& ExecutionPlan :: Current (Stage& variable) {
  _Current = &variable;
  return _Current;
  }

Stage*& ExecutionPlan :: End (void) {
  return _End;
  }

Stage*& ExecutionPlan :: End (Stage& variable) {
  _End = &variable;
  return _End;
  }
