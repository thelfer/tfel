/*!
 * \file   SystemError.ixx
 * \brief    
 * \author Helfer Thomas
 * \date   06 Nov 2007
 */

#ifndef _LIB_TFEL_SYSTEMERROR_IXX_
#define _LIB_TFEL_SYSTEMERROR_IXX_ 

namespace tfel
{

  namespace system
  {

    template<unsigned short N>
    PosixError<N>::PosixError(const std::string& msg_)
      : SystemError(msg_)
    {} // end of PosixError::PosixError

  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_TFEL_SYSTEMERROR_IXX_ */

