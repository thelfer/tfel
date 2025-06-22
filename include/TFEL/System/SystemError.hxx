/*!
 * \file   include/TFEL/System/SystemError.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEMERROR_HXX
#define LIB_TFEL_SYSTEMERROR_HXX

#include <string>
#include <cerrno>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Exception/TFELException.hxx"

namespace tfel {

  namespace system {

    struct TFELSYSTEM_VISIBILITY_EXPORT SystemError
        : public tfel::exception::TFELException {
      SystemError(const std::string&);
      SystemError(SystemError&&) = default;
      SystemError(const SystemError&) = default;
      const char* what() const noexcept override;
      ~SystemError() noexcept override;

     private:
      std::string msg;
    };  // end of struct SystemError

    template <int N>
    struct PosixError : public SystemError {
      PosixError(const std::string&);
      PosixError(PosixError&&) = default;
      PosixError(const PosixError&) = default;
    };  // end of struct PosixError

    //! a simple alias
    typedef PosixError<EPERM> EPERMError;
    //! a simple alias
    typedef PosixError<ENOENT> ENOENTError;
    //! a simple alias
    typedef PosixError<ESRCH> ESRCHError;
    //! a simple alias
    typedef PosixError<EINTR> EINTRError;
    //! a simple alias
    typedef PosixError<EIO> EIOError;
    //! a simple alias
    typedef PosixError<ENXIO> ENXIOError;
    //! a simple alias
    typedef PosixError<E2BIG> E2BIGError;
    //! a simple alias
    typedef PosixError<ENOEXEC> ENOEXECError;
    //! a simple alias
    typedef PosixError<EBADF> EBADFError;
    //! a simple alias
    typedef PosixError<ECHILD> ECHILDError;
    //! a simple alias
    typedef PosixError<EAGAIN> EAGAINError;
    //! a simple alias
    typedef PosixError<ENOMEM> ENOMEMError;
    //! a simple alias
    typedef PosixError<EACCES> EACCESError;
    //! a simple alias
    typedef PosixError<EFAULT> EFAULTError;
    //! a simple alias
    typedef PosixError<EBUSY> EBUSYError;
    //! a simple alias
    typedef PosixError<EEXIST> EEXISTError;
    //! a simple alias
    typedef PosixError<EXDEV> EXDEVError;
    //! a simple alias
    typedef PosixError<ENODEV> ENODEVError;
    //! a simple alias
    typedef PosixError<ENOTDIR> ENOTDIRError;
    //! a simple alias
    typedef PosixError<EISDIR> EISDIRError;
    //! a simple alias
    typedef PosixError<EINVAL> EINVALError;
    //! a simple alias
    typedef PosixError<ENFILE> ENFILEError;
    //! a simple alias
    typedef PosixError<EMFILE> EMFILEError;
    //! a simple alias
    typedef PosixError<ENOTTY> ENOTTYError;
    //! a simple alias
    typedef PosixError<EFBIG> EFBIGError;
    //! a simple alias
    typedef PosixError<ENOSPC> ENOSPCError;
    //! a simple alias
    typedef PosixError<ESPIPE> ESPIPEError;
    //! a simple alias
    typedef PosixError<EROFS> EROFSError;
    //! a simple alias
    typedef PosixError<EMLINK> EMLINKError;
    //! a simple alias
    typedef PosixError<EPIPE> EPIPEError;
    //! a simple alias
    typedef PosixError<EDOM> EDOMError;
    //! a simple alias
    typedef PosixError<ERANGE> ERANGEError;

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#ifndef __HAIKU__
    //! a simple alias
    typedef PosixError<ENOTBLK> ENOTBLKError;
#endif /* __HAIKU__ */
    //! a simple alias
    typedef PosixError<ETXTBSY> ETXTBSYError;
#endif /* LIB_TFEL_SYSTEMERROR_HXX */

  }  // end of namespace system

}  // end of namespace tfel

#include "TFEL/System/SystemError.ixx"

#endif /* LIB_TFEL_SYSTEMERROR_HXX */
