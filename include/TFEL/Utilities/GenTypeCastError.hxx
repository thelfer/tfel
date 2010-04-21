/*!
 * \file   GenTypeCastError.hxx
 * \brief  This file implements the GenTypeCastError class
 * \see    GenTypeBase
 * \author Helfer Thomas
 * \date   29 Jan. 2009
 */

#ifndef _LIB_TFEL_GENTYPECASTERROR_H_
#define _LIB_TFEL_GENTYPECASTERROR_H_ 

#include<stdexcept>
#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{
  
  namespace utilities
  {

    struct TFEL_VISIBILITY_EXPORT GenTypeCastError
      : public std::runtime_error
    {
      GenTypeCastError();
    }; // end of struct GenTypeCastErrro

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_GENTYPECASTERROR_H_ */
