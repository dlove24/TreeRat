/*!
 * \file SubCommand.cpp
 * \brief Defines a SubCommand class as a sub-command passed as an argument.
 *
 * This file containts the implementation for then CLAP :: SubCommand
 * Class. This is a class to hold a sub command structure so that chain of
 * command can be followed.
 */

#include "SubCommand.h"

using namespace libCLAP;
using namespace libCLAP :: CLAP;

SubCommand :: SubCommand (const Common :: String& name,
                          const vector < Common :: String >& abbreviations,
                          const Common :: String& description) :
  _Name (name),
  _Abbreviations (abbreviations),
  _Description (description) {
  }

SubCommand :: SubCommand (const Common :: String& name,
                          const Common :: String& abbreviation,
                          const Common :: String& description) :
  _Name (name),
  _Abbreviations (),
  _Description (description) {
  _Abbreviations.push_back (abbreviation);
  }

SubCommand :: SubCommand (const Common :: String& name,
                          const Common :: String& description) :
  _Name (name),
  _Abbreviations (),
  _Description (description) {
  }

SubCommand :: SubCommand (const SubCommand& copy) :
  _Name (copy._Name),
  _Abbreviations (copy._Abbreviations),
  _Description (copy._Description),
  _Switches (copy._Switches),
  _SubCommands (copy._SubCommands),
  _SwitchAbbreviations (copy._SwitchAbbreviations),
  _SubCommandAbbreviations (copy._SubCommandAbbreviations) {
  }

SubCommand :: ~SubCommand (void) throw () {
  }

void SubCommand :: AddSubCommand (SubCommand& addition) {
  _SubCommands [ addition._Name ] = &addition;

  for (unsigned long int i = 0; i < addition._Abbreviations.size (); i++) {
    _SubCommandAbbreviations [ addition._Abbreviations [ i ]] = &addition;
    }
  }

void SubCommand :: AddSwitch (Switch& addition) {
  _Switches [ addition.Name () ] = &addition;

  for (unsigned long int i = 0; i < addition.Abbreviations ().size (); i++) {
    _SwitchAbbreviations [ addition.Abbreviations () [ i ]] = &addition;
    }
  }

const Common :: String& SubCommand ::Name (void) const {
  return _Name;
  }

const Common :: String& SubCommand ::Name
(const Common :: String& variable) {
  _Name = variable;
  return _Name;
  }

const vector < Common :: String >& SubCommand :: Abbreviations (void) const {
  return _Abbreviations;
  }

const vector < Common :: String >& SubCommand :: Abbreviations
(const vector < Common :: String >& variable) {
  _Abbreviations = variable;
  return _Abbreviations;
  }

const Common :: String& SubCommand :: Description (void) const {
  return _Description;
  }

const Common :: String& SubCommand :: Description
(const Common :: String& variable) {
  _Description = variable;
  return _Description;
  }

const map < Common :: String, SubCommand* >& SubCommand :: SubCommands
(void) const {
  return _SubCommands;
  }

const map < Common :: String, Switch* >& SubCommand :: Switches (void) const {
  return _Switches;
  }

const map < Common :: String, SubCommand* >& SubCommand :: SubCommandAbbreviations (void) const {
  return _SubCommandAbbreviations;
  }

const map < char, Switch* >& SubCommand :: SwitchAbbreviations (void) const {
  return _SwitchAbbreviations;
  }

map < Common :: String, SubCommand* >& SubCommand :: SubCommands
(void) {
  return _SubCommands;
  }

map < Common :: String, Switch* >& SubCommand :: Switches (void) {
  return _Switches;
  }

map < Common :: String, SubCommand* >& SubCommand :: SubCommandAbbreviations
(void) {
  return _SubCommandAbbreviations;
  }

map < char, Switch* >& SubCommand :: SwitchAbbreviations (void) {
  return _SwitchAbbreviations;
  }

const map < Common :: String, SubCommand* >& SubCommand :: SubCommands
(const map < Common :: String, SubCommand* >& variable) {
  _SubCommands = variable;
  return _SubCommands;
  }

const map < Common :: String, SubCommand* >& SubCommand :: SubCommandAbbreviations
(const map < Common :: String, SubCommand* >& variable) {
  _SubCommandAbbreviations = variable;
  return _SubCommandAbbreviations;
  }

const map < char, Switch* >& SubCommand :: SwitchAbbreviations
(const map < char, Switch* >& variable) {
  _SwitchAbbreviations = variable;
  return _SwitchAbbreviations;
  }

const map < Common :: String, Switch* >& SubCommand :: Switches
(const map < Common :: String, Switch* >& variable) {
  _Switches = variable;
  return _Switches;
  }

void SubCommand :: operator = (const SubCommand& assign) {
  _Name = assign._Name;
  _Description = assign._Description;
  _Abbreviations = assign._Abbreviations;
  _Switches = assign._Switches;
  _SubCommands = assign._SubCommands;
  _SwitchAbbreviations = assign._SwitchAbbreviations;
  _SubCommandAbbreviations = assign._SubCommandAbbreviations;
  }

bool SubCommand :: operator == (const SubCommand& compare) const {
  if ( (_Name == compare._Name) &&
       (_Description == compare._Description) &&
       (_Abbreviations == compare._Abbreviations) &&
       (_Switches == compare._Switches) &&
       (_SubCommands == compare._SubCommands) &&
       (_SwitchAbbreviations == compare._SwitchAbbreviations) &&
       (_SubCommandAbbreviations == compare._SubCommandAbbreviations)) {
    return true;
    }

  else {
    return false;
    }
  }

bool SubCommand :: operator != (const SubCommand& compare) const {
  if ( (_Name != compare._Name) &&
       (_Description != compare._Description) &&
       (_Abbreviations != compare._Abbreviations) &&
       (_Switches != compare._Switches) &&
       (_SubCommands != compare._SubCommands) &&
       (_SwitchAbbreviations != compare._SwitchAbbreviations) &&
       (_SubCommandAbbreviations != compare._SubCommandAbbreviations)) {
    return true;
    }

  else {
    return false;
    }
  }
