/*!
 * \file Exception.cpp
 * \brief Implements the Exception class for CLAP.
 */

#include "Exception.h"

using namespace libCLAP;
using namespace libCLAP :: CLAP;

Exception :: Exception (const CLAP :: Error error) :
  _CLAPError (error),
  Common :: Exception (Message (_CLAPError)) {
  }

Exception :: Exception (const CLAP :: Error error,
                        const Common :: ErrorMessage& message) :
  _CLAPError (error),
  Common :: Exception (message) {
  }

Exception :: Exception (const Common :: ErrorMessage& message) :
  Common :: Exception (message),
  _CLAPError (enotspec) {
  }

Exception :: Exception (const Common :: Error error) :
  Common :: Exception (error),
  _CLAPError (ecustom) {
  }

Exception :: Exception (const CLAP :: Exception& copy) :
  _CLAPError (copy._CLAPError),
  Common :: Exception (copy) {
  }

Exception :: ~Exception (void) throw () {
  }

const CLAP :: Error& Exception :: CLAPError (void) const {
  return _CLAPError;
  }

const CLAP :: Error& Exception :: CLAPError
(const CLAP :: Error& variable) {
  _CLAPError = variable;
  return _CLAPError;
  }

ErrorMessage Exception :: Message (const CLAP :: Error& error) {
  ErrorMessage message;

  switch (error) {
    default :
      message = "Unknown Error";
      break;
    }

  return message;
  }
