/*!
 * \file CLI.h
 * \brief Implements a CLI client class as a parser to parse arguments from
 *        the command line.
 *
 * This file contains the definition for the CLAP :: CLI
 * Class. This is a class parse the commandline and create an execution plan for
 * the program based on a given structure for sub commands and switches.
 */

#include "CLI.h"
#define EQUALS '='
#define ESCAPE '-'
#define TAB "   "

using namespace libCLAP;
using namespace libCLAP :: CLAP;

CLI :: CLI (const Common :: String& name,
            const Common :: String& cmd,
            const Common :: String& version,
            const Common :: String& description,
            const char equals,
            const char escape) :
  _Name (name),
  _CMD (cmd),
  _Escape (escape),
  _Equals (equals),
  _Version (version),
  _Description (description) {
  }

CLI :: CLI (const Common :: String& name,
            const Common :: String& version,
            const Common :: String& description) :
  _Name (name),
  _CMD (name),
  _Escape (ESCAPE),
  _Equals (EQUALS),
  _Version (version),
  _Description (description) {
  }

CLI :: CLI (const Common :: String& name,
            const Common :: String& cmd,
            const Common :: String& version,
            const Common :: String& description) :
  _Name (name),
  _CMD (cmd),
  _Escape (ESCAPE),
  _Equals (EQUALS),
  _Version (version),
  _Description (description) {
  }

CLI :: CLI (const CLI& copy) :
  _Name (copy._Name),
  _CMD (copy._CMD),
  _Escape (copy._Escape),
  _Equals (copy._Equals),
  _Version (copy._Version),
  _Description (copy._Description),
  _Switches (copy._Switches),
  _SubCommands (copy._SubCommands),
  _SwitchAbbreviations (copy._SwitchAbbreviations),
  _SubCommandAbbreviations (copy._SubCommandAbbreviations) {
  }

CLI :: ~CLI (void) throw () {
  }

Common :: String CLI :: HelpMenu (void) {
  Common :: String menu;
  menu = "Usage : ";
  menu += _CMD;

  if (_Switches.size () > 0) {
    menu += " [options]";
    }

  menu += " [arguments]";

  if (_SubCommands.size () > 0) {
    menu += " <subcommand> [options] [arguments]";
    }

  menu += "\n";
  menu += _Name;
  menu += " command-line interface, version ";
  menu += _Version;
  menu += "\n";

  if (_SubCommands.size () > 0) {
    menu += "\nAvailable Sub-Commands :\n";
    std :: map < Common :: String, SubCommand* > :: const_iterator i;

    for (i = _SubCommands.begin (); i != _SubCommands.end (); i++) {
      menu += TAB;
      menu += (*i).first;
      menu += " ";

      for (unsigned int j = 0; j < (*i).second->Abbreviations ().size (); j++) {
        menu += "[";
        menu += (*i).second->Abbreviations () [ j ];
        menu += "] ";
        }

      menu += "\n";
      menu += TAB;
      menu += TAB;
      menu += (*i).second->Description ();
      menu += "\n";
      }
    }

  menu += "\n" + _Description + "\n";
  return menu;
  }

Common :: String CLI :: HelpMenu (const SubCommand& subcommand) {
  Common :: String menu;
  menu = subcommand.Name ();
  menu += " ";

  for (unsigned int k = 0; k < subcommand.Abbreviations ().size (); k++) {
    menu += "[";
    menu += subcommand.Abbreviations () [ k ];
    menu += "] ";
    }

  menu += ": ";
  menu += subcommand.Description ();
  menu += "\nUsage : ";
  menu += subcommand.Name ();

  if (subcommand.Switches ().size () > 0) {
    menu += " [options]";
    }

  menu += " [arguments]";

  if (subcommand.SubCommands ().size () > 0) {
    menu += " <subcommand> [options] [arguments]";
    }

  menu += "\n";

  if (subcommand.SubCommands ().size () > 0) {
    menu += "\nValid Sub-Commands :\n";
    std :: map < Common :: String, SubCommand* > :: const_iterator j;

    for (j = subcommand.SubCommands ().begin (); j != subcommand.SubCommands ().end (); j++) {
      menu += TAB;
      menu += (*j).first;
      menu += " ";

      for (unsigned int k = 0; k < subcommand.Abbreviations ().size (); k++) {
        menu += "[";
        menu += subcommand.Abbreviations () [ k ];
        menu += "] ";
        }

      menu += "\n";
      menu += TAB;
      menu += TAB;
      menu += (*j).second->Description ();
      menu += "\n";
      }
    }

  if (subcommand.Switches ().size () > 0) {
    menu += "\nValid Options :\n";
    std :: map < char, Switch* > :: const_iterator i;

    for (i = subcommand.SwitchAbbreviations ().begin (); i != subcommand.SwitchAbbreviations ().end (); i++) {
      menu += TAB;
      menu += _Escape;
      menu += (*i).first;
      menu += TAB;
      menu += _Escape;
      menu += _Escape;
      menu += (*i).second->Name ();
      menu += "[";
      menu += _Equals;
      menu += "argument]";
      menu += "\n";
      menu += TAB;
      menu += TAB;
      menu += (*i).second->Description ();
      menu += "\n";
      }
    }

  return menu;
  }

ExecutionPlan& CLI :: Parse (int argc, char** argv) {
  ExecutionPlan* exeplan = new ExecutionPlan (_Name);

  if (exeplan == NULL) {
    throw Exception (Common :: enomem);
    }

  map < Common :: String, Switch* >* switches = &_Switches;
  map < char, Switch* >* switchabbreviations = &_SwitchAbbreviations;
  map < Common ::String, SubCommand* >* subcommands = &_SubCommands;
  map < Common ::String, SubCommand* >* subcommandabbreviations = &_SubCommandAbbreviations;
  map < Common :: String, SubCommand* > :: iterator subab;
  map < Common :: String, Switch* > :: iterator swit;
  map < char, Switch* > :: iterator swiabt;
  map < Common :: String, SubCommand* > :: iterator sub;
  Common :: String argument;

  for (int i = 1; i < argc; i++) {
    argument = argv [ i ];

    if (argument [ 0 ] == _Escape) {
      /*!
       * These are options of the style --option=value
       */

      if ( (argument.size () > 1) && (argument [ 1 ] == _Escape)) {
        Common :: String name = argument.substr (2, Common :: String :: npos);
        Common :: String arg;
        unsigned long int found = name.find (_Equals, 0);

        if (found != Common :: String :: npos) {
          arg = name.substr (found + 1, name.size ());
          name = name.substr (0, found);
          }

        swit = switches->find (name);

        if (swit != switches->end ()) {
          Option* addoption = new Option ( (*swit).second->Name (),
                                           arg);

          if (addoption == NULL) {
            throw Exception (Common :: enomem);
            }

          exeplan->End ()->AddOption (*addoption);
          }

        else {
          Common :: String problem = _CMD;
          problem += " : unknown option -- ";
          problem += name;
          problem += "\n";
          problem += HelpMenu ();
          throw Exception (problem);
          }
        }

      else {
        /*!
         * These are options of the style -ovalue
         */
        char name = argument [ 1 ];
        Common :: String arg = "";

        if (argument.size () > 1) {
          arg = argument.substr (2, Common :: String :: npos);
          }

        swiabt = switchabbreviations->find (name);

        if (swiabt != switchabbreviations->end ()) {
          Option* addoption = new Option ( (*swiabt).second->Name (),
                                           arg);

          if (addoption == NULL) {
            throw Exception (Common :: enomem);
            }

          exeplan->End ()->AddOption (*addoption);
          }

        else {
          Common :: String problem = _CMD;
          problem += " : unknown option -- ";
          problem += name;
          problem += "\n";
          problem += HelpMenu ();
          throw Exception (problem);
          }
        }
      }

    else {
      Common :: String value = argument;
      sub = subcommands->find (value);

      if (sub != subcommands->end ()) {
        switches = & ( (*sub).second->Switches ());
        switchabbreviations = & ( (*sub).second->SwitchAbbreviations ());
        subcommands = & ( (*sub).second->SubCommands ());
        subcommandabbreviations = & ( (*sub).second->SubCommandAbbreviations ());
        Stage* stage = new Stage ( (*sub).second->Name ());

        if (stage == NULL) {
          throw Exception (Common :: enomem);
          }

        exeplan->AddStage (*stage);
        }

      else {
        subab = subcommandabbreviations->find (value);

        if (subab != subcommandabbreviations->end ()) {
          switches = & ( (*subab).second->Switches ());
          switchabbreviations = & ( (*subab).second->SwitchAbbreviations ());
          subcommands = & ( (*subab).second->SubCommands ());
          subcommandabbreviations = & ( (*subab).second->SubCommandAbbreviations ());
          Stage* stage = new Stage ( (*subab).second->Name ());

          if (stage == NULL) {
            throw Exception (Common :: enomem);
            }

          exeplan->AddStage (*stage);
          }

        else {
          exeplan->End ()->AddArgument (value);
          }
        }
      }
    }

  return (*exeplan);
  }

void CLI :: AddSubCommand (SubCommand& addition) {
  _SubCommands [ addition.Name () ] = &addition;

  for (unsigned int i = 0; i < addition.Abbreviations ().size (); i++) {
    _SubCommandAbbreviations [ addition.Abbreviations () [ i ]] = &addition;
    }
  }

void CLI :: AddSwitch (Switch& addition) {
  _Switches [ addition.Name () ] = &addition;

  for (unsigned int i = 0; i < addition.Abbreviations ().size (); i++) {
    _SwitchAbbreviations [ addition.Abbreviations () [ i ]] = &addition;
    }
  }

const Common :: String& CLI ::Name (void) const {
  return _Name;
  }

const Common :: String& CLI ::Name
(const Common :: String& variable) {
  _Name = variable;
  return _Name;
  }

const Common :: String& CLI :: Version (void) const {
  return _Version;
  }

const Common :: String& CLI :: Version
(const Common :: String& variable) {
  _Version = variable;
  return _Version;
  }

const Common :: String& CLI :: Description (void) const {
  return _Description;
  }

const Common :: String& CLI :: Description
(const Common :: String& variable) {
  _Description = variable;
  return _Description;
  }


const map < Common :: String, SubCommand* >& CLI :: SubCommands
(void) const {
  return _SubCommands;
  }

const map < Common :: String, Switch* >& CLI :: Switches (void) const {
  return _Switches;
  }

const map < Common :: String, SubCommand* >& CLI :: SubCommandAbbreviations
(void) const {
  return _SubCommandAbbreviations;
  }

const map < char, Switch* >& CLI :: SwitchAbbreviations (void) const {
  return _SwitchAbbreviations;
  }

const map < Common :: String, SubCommand* >& CLI :: SubCommands
(const map < Common :: String, SubCommand* >& variable) {
  _SubCommands = variable;
  return _SubCommands;
  }

const map < Common :: String, SubCommand* >& CLI :: SubCommandAbbreviations
(const map < Common :: String, SubCommand* >& variable) {
  _SubCommandAbbreviations = variable;
  return _SubCommandAbbreviations;
  }

const map < char, Switch* >& CLI :: SwitchAbbreviations
(const map < char, Switch* >& variable) {
  _SwitchAbbreviations = variable;
  return _SwitchAbbreviations;
  }

const map < Common :: String, Switch* >& CLI :: Switches
(const map < Common :: String, Switch* >& variable) {
  _Switches = variable;
  return _Switches;
  }

const Common :: String& CLI :: CMD (void) const {
  return _CMD;
  }

const Common :: String& CLI :: CMD
(const Common :: String& variable) {
  _CMD = variable;
  return _CMD;
  }

const char& CLI :: Escape (void) const {
  return _Escape;
  }

const char& CLI :: Escape (const char& variable) {
  _Escape = variable;
  return _Escape;
  }

const char& CLI :: Equals (void) const {
  return _Equals;
  }

const char& CLI :: Equals (const char& variable) {
  _Equals = variable;
  return _Equals;
  }
