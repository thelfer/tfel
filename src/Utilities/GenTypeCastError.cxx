/*!
 * \file   GenTypeCastError.cxx
 * \brief  This file implements the GenTypeCastError class
 * \see    GenTypeBase
 * \author Helfer Thomas
 * \date   29 Jan. 2009
 */

#include"Utilities/GenTypeCastError.hxx"

namespace tfel
{
  
  namespace utilities
  {

    GenTypeCastError::GenTypeCastError()
      : std::runtime_error("Invalid conversion")
    {} // end of GenTypeCastError::GenTypeCastError

  } // end of namespace utilities

} // end of namespace tfel
