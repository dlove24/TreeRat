/*!
 * \file Switch.cpp
 * \brief Implements a Switch class as a switch passed as an argument.
 *
 * This file containts the implementation for then CLAP :: Switch
 * Class. This is a class to hold a switch structure so that chain of
 * command can be followed.
 */

#include "Switch.h"

using namespace libCLAP;
using namespace libCLAP :: CLAP;

Switch :: Switch (const Common :: String& name,
                  const vector < char >& abbreviations,
                  const Common :: String& description,
                  const Common :: String& value) :
  _Name (name),
  _Abbreviations (abbreviations),
  _Description (description),
  _Value (value) {
  }

Switch :: Switch (const Common :: String& name,
                  const char abbreviation,
                  const Common :: String& description,
                  const Common :: String& value) :
  _Name (name),
  _Abbreviations (),
  _Description (description),
  _Value (value) {
  _Abbreviations.push_back (abbreviation);
  }

Switch :: Switch (const Common :: String& name,
                  const vector < char >& abbreviations,
                  const Common :: String& description) :
  _Name (name),
  _Abbreviations (abbreviations),
  _Description (description),
  _Value ("") {
  }

Switch :: Switch (const Common :: String& name,
                  const char abbreviation,
                  const Common :: String& description) :
  _Name (name),
  _Abbreviations (),
  _Description (description),
  _Value ("") {
  _Abbreviations.push_back (abbreviation);
  }

Switch :: Switch (const Common :: String& name,
                  const Common :: String& description) :
  _Name (name),
  _Abbreviations (),
  _Description (description),
  _Value ("") {
  }

Switch :: Switch (const Switch& copy) :
  _Name (copy._Name),
  _Abbreviations (copy._Abbreviations),
  _Description (copy._Description),
  _Value (copy._Value) {
  }

Switch :: ~Switch (void) throw () {
  }

const Common :: String& Switch :: Name (void) const {
  return _Name;
  }

const Common :: String& Switch :: Name
(const Common :: String& variable) {
  _Name = variable;
  return _Name;
  }

const vector < char >& Switch :: Abbreviations (void) const {
  return _Abbreviations;
  }

const vector < char >& Switch :: Abbreviations
(const vector < char >& variable) {
  _Abbreviations = variable;
  return _Abbreviations;
  }

const Common :: String& Switch :: Description (void) const {
  return _Description;
  }

const Common :: String& Switch :: Description
(const Common :: String& variable) {
  _Description = variable;
  return _Description;
  }

const Common :: String& Switch :: Value (void) const {
  return _Value;
  }

const Common :: String& Switch :: Value
(const Common :: String& variable) {
  _Value = variable;
  return _Value;
  }

const Common :: String& Switch :: ArgumentType (void) const {
  return _ArgumentType;
  }

const Common :: String& Switch :: ArgumentType (const Common :: String& variable) {
  _ArgumentType = variable;
  return _ArgumentType;
  }

void Switch :: operator = (const Switch& assign) {
  _Name = assign._Name;
  _Description = assign._Description;
  _Abbreviations = assign._Abbreviations;
  _Value = assign._Value;
  }

bool Switch :: operator == (const Switch& compare) const {
  if ( (_Name == compare._Name) &&
       (_Description == compare._Description) &&
       (_Abbreviations == compare._Abbreviations) &&
       (_Value == compare._Value)) {
    return true;
    }

  else {
    return false;
    }
  }

bool Switch :: operator != (const Switch& compare) const {
  if ( (_Name != compare._Name) &&
       (_Description != compare._Description) &&
       (_Abbreviations != compare._Abbreviations) &&
       (_Value != compare._Value)) {
    return true;
    }

  else {
    return false;
    }
  }
