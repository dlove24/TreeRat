/*!
 * \file Types.h
 * \brief Defines Types for CLAP.
 */

#ifndef _CLAP_TYPES_H_
#define _CLAP_TYPES_H_

#include "../Common/Types.h"

namespace libCLAP {
namespace CLAP {
typedef Common :: String ErrorMessage;

/*!
 * \brief CLAP Error enumeration
 *
 * This enum contains all the Errors codes that correspond to errors.
 */
enum Error {
  enotspec = 0,
  ecustom = 1,
  eunkopt = 2,
};
}
}
#endif
