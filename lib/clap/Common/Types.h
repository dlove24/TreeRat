/*!
 * \file Common/Types.h
 *
 * \brief Defines Types for the Common namespace.
 */

#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <errno.h>

using namespace std;

namespace libCLAP {
namespace Common {
/*!
 * \brief Common Error enumeration
 *
 * This enum contains all the system errors possible.
 */
enum Error {
  ecustom = 0,
  eperm = EPERM,
  enoent = ENOENT,
  esrch = ESRCH,
  eintr = EINTR,
  eio = EIO,
  enxio = ENXIO,
  e2big = E2BIG,
  enoexec = ENOEXEC,
  ebadf = EBADF,
  echild = ECHILD,
  edeadlk = EDEADLK,
  enomem = ENOMEM,
  eacces = EACCES,
  efault = EFAULT,
#if __BSD_VISIBLE
  enotblk = ENOTBLK,
#endif
  ebusy = EBUSY,
  eexist = EEXIST,
  exdev = EXDEV,
  enodev = ENODEV,
  enotdir = ENOTDIR,
  eisdir = EISDIR,
  einval = EINVAL,
  enfile = ENFILE,
  emfile = EMFILE,
  enotty = ENOTTY,
  etxbsy = ETXTBSY,
  efbig = EFBIG,
  enospc = ENOSPC,
  espipe = ESPIPE,
  erofs = EROFS,
  emlink = EMLINK,
  epipe = EPIPE,
  edom = EDOM,
  erange = ERANGE,
  eagain = EAGAIN,
  ewouldblock = EWOULDBLOCK,
  einprogress = EINPROGRESS,
  ealready = EALREADY,
  enotsock = ENOTSOCK,
  edestaddreq = EDESTADDRREQ,
  emsgsize = EMSGSIZE,
  eprototype = EPROTOTYPE,
  enoprotoopt = ENOPROTOOPT,
  eprotonosupport = EPROTONOSUPPORT,
#if __BSD_VISIBLE
  esocktnosupport = ESOCKTNOSUPPORT,
#endif
  eopnotsupp = EOPNOTSUPP,
#if __BSD_VISIBLE
  epfnosupport = EPFNOSUPPORT,
#endif
  eafnosupport = EAFNOSUPPORT,
  eaddrinuse = EADDRINUSE,
  eaddrnotavail = EADDRNOTAVAIL,
  enetdown = ENETDOWN,
  enetunreach = ENETUNREACH,
  enetreset = ENETRESET,
  econnaborted = ECONNABORTED,
  econnreset = ECONNRESET,
  enobufs = ENOBUFS,
  eisconn = EISCONN,
  enotconn = ENOTCONN,
#if __BSD_VISIBLE
  eshutdown = ESHUTDOWN,
  etodomanyrefs = ETOOMANYREFS,
#endif /* __BSD_VISIBLE */
  etimedout = ETIMEDOUT,
  econnrefused = ECONNREFUSED,
  eloop = ELOOP,
  enametoolong = ENAMETOOLONG,
#if __BSD_VISIBLE
  ehostdown = EHOSTDOWN,
  ehostunreach = EHOSTUNREACH,
#endif /* __BSD_VISIBLE */
  enotempty = ENOTEMPTY,
#if __BSD_VISIBLE
  eproclim = EPROCLIM,
  eusers = EUSERS,
#endif /* __BSD_VISIBLE */
  edquot = EDQUOT,
  estale = ESTALE,
#if __BSD_VISIBLE
  eremote = EREMOTE,
  ebadrpc = EBADRPC,
  erpcmismatch = ERPCMISMATCH,
  eprogunavail = EPROGUNAVAIL,
  eprogmismatch = EPROGMISMATCH,
  eprocunavail = EPROCUNAVAIL,
#endif /* __BSD_VISIBLE */
  enolck = ENOLCK,
  enosys = ENOSYS,
#if __BSD_VISIBLE
  eftype = EFTYPE,
  eauth = EAUTH,
  eneedauth = ENEEDAUTH,
  eipsec = EIPSEC,
  enoattr = ENOATTR,
#endif /* __BSD_VISIBLE */
  eilseq = EILSEQ,
#if __BSD_VISIBLE
  enomedium = ENOMEDIUM,
  emediumtype = EMEDIUMTYPE,
  eoverflow = EOVERFLOW,
  ecanceled = ECANCELED,
#endif /* __BSD_VISIBLE */
  eidrm = EIDRM,
  enomsg = ENOMSG,
#if __BSD_VISIBLE
  elast = ELAST,
#endif /* __BSD_VISIBLE */
#ifdef _KERNEL
  erestart = ERESTART,
  ejustreturn = EJUSTRETURN,
#endif
  earroob,
  efound,
  enotfound
};

typedef std :: string String;
typedef char Character;
typedef Common :: String ErrorMessage;
typedef char RByte;
typedef void* Raw;
typedef unsigned int Size;
}
}
#endif
