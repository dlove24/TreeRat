/*!
 * \file Exception.cpp
 * \brief Implements the Exception class.
 */

#include "Exception.h"

using namespace libCLAP;
using namespace libCLAP :: Common;

Exception :: Exception (const Common :: Error error) :
  _Error (error) {
  _What = Exception :: Message (_Error);
  }

Exception :: Exception (const Common :: ErrorMessage& message) :
  _Error (ecustom),
  _What (message) {
  }

Exception :: Exception (const Common :: Error error,
                        const Common :: ErrorMessage& message) :
  _Error (error),
  _What (message) {
  }

Exception :: Exception (const Common :: Exception& copy) :
  _Error (copy._Error),
  _What (copy._What) {
  }

Exception :: ~Exception (void) throw () {
  }

const Error& Exception :: Error (void) const {
  return _Error;
  }

const Error& Exception :: Error (const Common :: Error& variable) {
  _Error = variable;
  return _Error;
  }

const Common :: ErrorMessage& Exception :: What (void) const {
  return _What;
  }

const Common :: ErrorMessage& Exception :: What
(const Common :: ErrorMessage& variable) {
  _What = variable;
  return _What;
  }

Common :: ErrorMessage Exception :: Message (const Common :: Error& error) {
  Common :: ErrorMessage message;

  switch (error) {
    case enoent :
      message = "No such file or directory.";
      break;

    case esrch :
      message = "No such process.";
      break;

    case eintr :
      message = "Interrupted system call.";
      break;

    case eio :
      message = "Input/Output error.";
      break;

    case enxio :
      message = "Device not configured.";
      break;

    case e2big :
      message = "Argument list too long.";
      break;

    case enoexec :
      message = "Exec format error.";
      break;

    case ebadf :
      message = "Bad file descriptor.";
      break;

    case echild :
      message = "No child processes.";
      break;

    case edeadlk :
      message = "Resource deadlock avoided.";
      break;

    case enomem :
      message = "Cannot allocate memory.";
      break;

    case eacces :
      message = "Permission denied.";
      break;

    case efault :
      message = "Bad address.";
      break;
#if __BSD_VISIBLE

    case enotblk :
      message = "Block device required.";
      break;
#endif

    case ebusy :
      message = "Device busy.";
      break;

    case eexist :
      message = "File exists.";
      break;

    case exdev :
      message = "Cross-device link.";
      break;

    case enodev :
      message = "Operation not supported by device.";
      break;

    case enotdir :
      message = "Not a directory.";
      break;

    case eisdir :
      message = "Is a directory.";
      break;

    case einval :
      message = "Invalid argument.";
      break;

    case enfile :
      message = "Too many open files in system.";
      break;

    case emfile :
      message = "Too many open files.";
      break;

    case enotty :
      message = "Inappropriate ioctl for device.";
      break;

    case etxbsy :
      message = "Text file busy.";
      break;

    case efbig :
      message = "File too large.";
      break;

    case enospc :
      message = "No space left on device.";
      break;

    case espipe :
      message = "Illegal seek.";
      break;

    case erofs :
      message = "Read-only file system.";
      break;

    case emlink :
      message = "Too many links.";
      break;

    case epipe :
      message = "Broken pope.";
      break;

    case edom :
      message = "Numerical argument out of domain.";
      break;

    case erange :
      message = "Result too large.";
      break;

    case eagain || ewouldblock || eperm || enomsg
#if __BSD_VISIBLE
        || elast
#endif
        :
      message = "Resource temporarily unavailable.";
      break;

    case einprogress :
      message = "Operation now in progress.";
      break;

    case ealready :
      message = "Operation already in progress.";
      break;

    case enotsock :
      message = "Socket operation on non-socket.";
      break;

    case edestaddreq :
      message = "Destination address required.";
      break;

    case emsgsize :
      message = "Message too long.";
      break;

    case eprototype :
      message = "Protocol wrong type for socket.";
      break;

    case enoprotoopt :
      message = "Protocol not available.";
      break;

    case eprotonosupport :
      message = "Protocol not supported.";
      break;
#if __BSD_VISIBLE

    case esocktnosupport :
      message = "Socket type not supported.";
      break;
#endif

    case eopnotsupp :
      message = "Operation not supported.";
      break;
#if __BSD_VISIBLE

    case epfnosupport :
      message = "Protocol Family not supported.";
      break;
#endif

    case eafnosupport :
      message = "Address Family not supported.";
      break;

    case eaddrinuse :
      message = "Address already in use.";
      break;

    case eaddrnotavail :
      message = "Can't assign requested address.";
      break;

    case enetdown :
      message = "Network is down.";
      break;

    case enetunreach :
      message = "Network is unreachable.";
      break;

    case enetreset :
      message = "Network dropped connection on reset.";
      break;

    case econnaborted :
      message = "Software caused connection abort.";
      break;

    case econnreset :
      message = "Connection reset by peer.";
      break;

    case enobufs :
      message = "No buffer space available.";
      break;

    case eisconn :
      message = "Socket is already connected.";
      break;

    case enotconn :
      message = "Socket is not connected.";
      break;
#if __BSD_VISIBLE

    case eshutdown :
      message = "Can't send after socket shutdown.";
      break;

    case etodomanyrefs :
      message = "Too many references : can't splice.";
      break;
#endif /* __BSD_VISIBLE */

    case etimedout :
      message = "Operation timed out.";
      break;

    case econnrefused :
      message = "Connection refused.";
      break;

    case eloop :
      message = "Too many levels of symbolic links.";
      break;

    case enametoolong :
      message = "File name too long.";
      break;
#if __BSD_VISIBLE

    case ehostdown :
      message = "Host is down.";
      break;

    case ehostunreach :
      message = "No route to host.";
      break;
#endif /* __BSD_VISIBLE */

    case enotempty :
      message = "Directory not empty.";
      break;
#if __BSD_VISIBLE

    case eproclim :
      message = "Too many processes.";
      break;

    case eusers :
      message = "Too many users.";
      break;
#endif /* __BSD_VISIBLE */

    case edquot :
      message = "Disk quota exceeded.";
      break;

    case estale :
      message = "Stale NFS file handle.";
      break;
#if __BSD_VISIBLE

    case eremote :
      message = "Too many levels of remote in path.";
      break;

    case ebadrpc :
      message = "RPC struct is bad.";
      break;

    case erpcmismatch :
      message = "RPC version wrong.";
      break;

    case eprogunavail :
      message = "RPC program not available.";
      break;

    case eprogmismatch :
      message = "RPC Program version wrong.";
      break;

    case eprocunavail :
      message = "Bad procedure for program.";
      break;
#endif /* __BSD_VISIBLE */

    case enolck :
      message = "No locks available.";
      break;

    case enosys :
      message = "Function not implemented.";
      break;
#if __BSD_VISIBLE

    case eftype :
      message = "Inappropriate file type or format.";
      break;

    case eauth :
      message = "Authentication error.";
      break;

    case eneedauth :
      message = "Need authenticator.";
      break;

    case eipsec :
      message = "IPsec processing failure.";
      break;

    case enoattr :
      message = "Attribute not found.";
      break;
#endif /* __BSD_VISIBLE */

    case eilseq :
      message = "Illegal byte sequence.";
      break;
#if __BSD_VISIBLE

    case enomedium :
      message = "No medium found.";
      break;

    case emediumtype :
      message = "Wrong medium type.";
      break;

    case eoverflow :
      message = "Conversation overflow.";
      break;

    case ecanceled :
      message = "Operation canceled.";
      break;
#endif /* __BSD_VISIBLE */

    case eidrm :
      message = "Identifier removed.";
      break;
#ifdef _KERNEL

    case erestart :
      message = "Restart syscall.";
      break;

    case ejustreturn :
      message = "Don't modify regs, just return.";
      break;
#endif
    }

  return message;
  }
