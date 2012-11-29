/*!
 * \file Option.h
 * \brief Implements an Option class as an option set for a stage.
 *
 * This file contains the implementation for then CLAP :: Option Class.
 * This is a class to hold an Option structure so that options can be set during
 * a particular stage..
 */

#include "Option.h"

using namespace libCLAP;
using namespace libCLAP :: CLAP;

Option :: Option (const Common :: String& name) :
  _Name (name),
  _Value ("") {
  }

Option :: Option (const Common :: String& name,
                  const Common :: String& value) :
  _Name (name),
  _Value (value) {
  }


Option :: Option (const Option& copy) :
  _Name (copy._Name),
  _Value (copy._Value) {
  }

Option :: ~Option (void) throw () {
  }

const Common :: String& Option :: Name (void) const {
  return _Name;
  }

const Common :: String& Option :: Name
(const Common :: String& variable) {
  _Name = variable;
  return _Name;
  }

const Common :: String& Option :: Value (void) const {
  return _Value;
  }

const Common :: String& Option :: Value
(const Common :: String& variable) {
  _Value = variable;
  return _Value;
  }

void Option :: operator = (const Option& assign) {
  _Name = assign._Name;
  _Value = assign._Value;
  }

bool Option :: operator == (const Option& compare) const {
  if ( (_Name == compare._Name) &&
       (_Value == compare._Value)) {
    return true;
    }

  else {
    return false;
    }
  }

bool Option :: operator != (const Option& compare) const {
  if ( (_Name != compare._Name) &&
       (_Value != compare._Value)) {
    return true;
    }

  else {
    return false;
    }
  }
