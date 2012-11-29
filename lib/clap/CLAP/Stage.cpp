/*!
 * \file Stage.cpp
 * \brief Implements a Stage class as stage in the execution plan after parsing
 *        the arguments
 *
 * This file contains the implementation for the CLAP :: Stage Class.
 * This is a class to generate a stage to execute given inputs and a set of
 * options and subcommands available.
 */

#include "Stage.h"
#include "Option.h"

using namespace libCLAP;
using namespace libCLAP :: CLAP;

Stage :: Stage (const Common :: String& name) :
  _Name (name),
  _Parent (nullptr),
  _Next (nullptr) {
  }

Stage :: Stage (const Common :: String& name,
                Stage& parent) :
  _Name (name),
  _Parent (&parent),
  _Next (nullptr) {
  }

Stage :: Stage (const Stage& copy) :
  _Name (copy._Name),
  _Parent (copy._Parent),
  _Next (copy._Next) {
  }

Stage :: ~Stage (void) throw () {
  if (_Parent != nullptr) {
    _Parent->_Next = _Next;
    }

  if (_Next != nullptr) {
    _Next->_Parent = _Parent;
    }

  map < Common :: String, Option* > :: iterator i;

  for (i = _Options.begin (); i != _Options.end (); i++) {
    delete ( (*i).second);
    }
  }

void Stage :: AddOption (Option& addition) {
  _Options [ addition.Name () ] = &addition;
  }

void Stage :: AddArgument (Common :: String& addition) {
  _Arguments.push (addition);
  }

const Common :: String Stage :: Shift (void) {
  Common :: String argument = _Arguments.front ();
  _Arguments.pop ();
  return argument;
  }

const map < Common :: String, Option* >& Stage :: Options (void) const {
  return _Options;
  }

map < Common :: String, Option* >& Stage :: Options (void) {
  return _Options;
  }

const queue < Common :: String >& Stage :: Arguments (void) const {
  return _Arguments;
  }

const Common :: String& Stage :: Name (void) const {
  return _Name;
  }

const queue < Common :: String >& Stage :: Arguments
(const queue < Common :: String >& variable) {
  _Arguments = variable;
  return _Arguments;
  }

const map < Common :: String, Option* >& Stage :: Options
(const map < Common :: String, Option* >& variable) {
  _Options = variable;
  return _Options;
  }

const Common :: String& Stage :: Name (Common :: String& variable) {
  _Name = variable;
  return _Name;
  }

Stage*& Stage :: Parent (void) {
  return _Parent;
  }

Stage*& Stage :: Next (void) {
  return _Next;
  }

Stage*& Stage :: Parent (Stage& variable) {
  _Parent = &variable;
  return _Parent;
  }

Stage*& Stage :: Next (Stage& variable) {
  _Next = &variable;
  return _Next;
  }
