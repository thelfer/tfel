/*!
 * \file   SystemError.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 Nov 2007
 */

#ifndef _LIB_TFEL_SYSTEMERROR_HXX_
#define _LIB_TFEL_SYSTEMERROR_HXX_ 

#include<cerrno>

#include"Exception/TFELException.hxx"

namespace tfel
{

  namespace system
  {

    struct SystemError
      : public tfel::exception::TFELException
    {
      SystemError(const std::string&);
    }; // end of struct SystemError
    
    template<unsigned short N>
    struct PosixError
      : public SystemError
    {
      PosixError(const std::string& msg);
    }; // end of struct PosixError

    //! a simple alias
    typedef PosixError<EPERM>   EPERMError;
    //! a simple alias
    typedef PosixError<ENOENT>  ENOENTError;
    //! a simple alias
    typedef PosixError<ESRCH>   ESRCHError;
    //! a simple alias
    typedef PosixError<EINTR>   EINTRError;
    //! a simple alias
    typedef PosixError<EIO>     EIOError;
    //! a simple alias
    typedef PosixError<ENXIO>   ENXIOError;
    //! a simple alias
    typedef PosixError<E2BIG>   E2BIGError;
    //! a simple alias
    typedef PosixError<ENOEXEC> ENOEXECError;
    //! a simple alias
    typedef PosixError<EBADF>   EBADFError;
    //! a simple alias
    typedef PosixError<ECHILD>  ECHILDError;
    //! a simple alias
    typedef PosixError<EAGAIN>  EAGAINError;
    //! a simple alias
    typedef PosixError<ENOMEM>  ENOMEMError;
    //! a simple alias
    typedef PosixError<EACCES>  EACCESError;
    //! a simple alias
    typedef PosixError<EFAULT>  EFAULTError;
    //! a simple alias
    typedef PosixError<ENOTBLK> ENOTBLKError;
    //! a simple alias
    typedef PosixError<EBUSY>   EBUSYError;
    //! a simple alias
    typedef PosixError<EEXIST>  EEXISTError;
    //! a simple alias
    typedef PosixError<EXDEV>   EXDEVError;
    //! a simple alias
    typedef PosixError<ENODEV>  ENODEVError;
    //! a simple alias
    typedef PosixError<ENOTDIR> ENOTDIRError;
    //! a simple alias
    typedef PosixError<EISDIR>  EISDIRError;
    //! a simple alias
    typedef PosixError<EINVAL>  EINVALError;
    //! a simple alias
    typedef PosixError<ENFILE>  ENFILEError;
    //! a simple alias
    typedef PosixError<EMFILE>  EMFILEError;
    //! a simple alias
    typedef PosixError<ENOTTY>  ENOTTYError;
    //! a simple alias
    typedef PosixError<ETXTBSY> ETXTBSYError;
    //! a simple alias
    typedef PosixError<EFBIG>   EFBIGError;
    //! a simple alias
    typedef PosixError<ENOSPC>  ENOSPCError;
    //! a simple alias
    typedef PosixError<ESPIPE>  ESPIPEError;
    //! a simple alias
    typedef PosixError<EROFS>   EROFSError;
    //! a simple alias
    typedef PosixError<EMLINK>  EMLINKError;
    //! a simple alias
    typedef PosixError<EPIPE>   EPIPEError;
    //! a simple alias
    typedef PosixError<EDOM>    EDOMError;
    //! a simple alias
    typedef PosixError<ERANGE>  ERANGEError;
  
  } // end of namespace system

} // end of namespace tfel

#include"System/SystemError.ixx"

#endif /* _LIB_TFEL_SYSTEMERROR_HXX */
