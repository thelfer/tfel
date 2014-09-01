/*!
 * \file   include/TFEL/Utilities/GenTypeCastError.hxx
 * \brief  This file implements the GenTypeCastError class
 * \see    GenTypeBase
 * \author Helfer Thomas
 * \date   29 Jan. 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_GENTYPECASTERROR_H_
#define _LIB_TFEL_GENTYPECASTERROR_H_ 

#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{
  
  namespace utilities
  {

    /*!
     * exception thrown when requesting a bad type out of a
     * GenTypeBase class
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT GenTypeCastError
      : public std::runtime_error
    {
      /*!
       * Default consutructor
       */
      GenTypeCastError();
    }; // end of struct GenTypeCastErrro

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_GENTYPECASTERROR_H_ */
