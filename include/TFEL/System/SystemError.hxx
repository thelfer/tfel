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

namespace tfel::system {

  struct TFELSYSTEM_VISIBILITY_EXPORT SystemError
      : public tfel::exception::TFELException {
    explicit SystemError(const std::string&);
    SystemError(SystemError&&) = default;
    SystemError(const SystemError&) = default;
    [[nodiscard]] const char* what() const noexcept override;
    ~SystemError() noexcept override;

   private:
    std::string msg;
  };  // end of struct SystemError

  template <int N>
  struct PosixError : public SystemError {
    explicit PosixError(const std::string&);
    PosixError(PosixError&&) = default;
    PosixError(const PosixError&) = default;
  };  // end of struct PosixError

  //! \brief a simple alias
  using EPERMError = PosixError<EPERM>;
  //! \brief a simple alias
  using ENOENTError = PosixError<ENOENT>;
  //! \brief a simple alias
  using ESRCHError = PosixError<ESRCH>;
  //! \brief a simple alias
  using EINTRError = PosixError<EINTR>;
  //! \brief a simple alias
  using EIOError = PosixError<EIO>;
  //! \brief a simple alias
  using ENXIOError = PosixError<ENXIO>;
  //! \brief a simple alias
  using E2BIGError = PosixError<E2BIG>;
  //! \brief a simple alias
  using ENOEXECError = PosixError<ENOEXEC>;
  //! \brief a simple alias
  using EBADFError = PosixError<EBADF>;
  //! \brief a simple alias
  using ECHILDError = PosixError<ECHILD>;
  //! \brief a simple alias
  using EAGAINError = PosixError<EAGAIN>;
  //! \brief a simple alias
  using ENOMEMError = PosixError<ENOMEM>;
  //! \brief a simple alias
  using EACCESError = PosixError<EACCES>;
  //! \brief a simple alias
  using EFAULTError = PosixError<EFAULT>;
  //! \brief a simple alias
  using EBUSYError = PosixError<EBUSY>;
  //! \brief a simple alias
  using EEXISTError = PosixError<EEXIST>;
  //! \brief a simple alias
  using EXDEVError = PosixError<EXDEV>;
  //! \brief a simple alias
  using ENODEVError = PosixError<ENODEV>;
  //! \brief a simple alias
  using ENOTDIRError = PosixError<ENOTDIR>;
  //! \brief a simple alias
  using EISDIRError = PosixError<EISDIR>;
  //! \brief a simple alias
  using EINVALError = PosixError<EINVAL>;
  //! \brief a simple alias
  using ENFILEError = PosixError<ENFILE>;
  //! \brief a simple alias
  using EMFILEError = PosixError<EMFILE>;
  //! \brief a simple alias
  using ENOTTYError = PosixError<ENOTTY>;
  //! \brief a simple alias
  using EFBIGError = PosixError<EFBIG>;
  //! \brief a simple alias
  using ENOSPCError = PosixError<ENOSPC>;
  //! \brief a simple alias
  using ESPIPEError = PosixError<ESPIPE>;
  //! \brief a simple alias
  using EROFSError = PosixError<EROFS>;
  //! \brief a simple alias
  using EMLINKError = PosixError<EMLINK>;
  //! \brief a simple alias
  using EPIPEError = PosixError<EPIPE>;
  //! \brief a simple alias
  using EDOMError = PosixError<EDOM>;
  //! \brief a simple alias
  using ERANGEError = PosixError<ERANGE>;

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#ifndef __HAIKU__
  //! \brief a simple alias
  using ENOTBLKError = PosixError<ENOTBLK>;
#endif /* __HAIKU__ */
  //! \brief a simple alias
  using ETXTBSYError = PosixError<ETXTBSY>;
#endif /* LIB_TFEL_SYSTEMERROR_HXX */

}  // end of namespace tfel::system

#include "TFEL/System/SystemError.ixx"

#endif /* LIB_TFEL_SYSTEMERROR_HXX */
