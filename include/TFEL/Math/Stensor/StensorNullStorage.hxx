/*!
 * \file   include/TFEL/Math/Stensor/StensorNullStorage.hxx  
 * \brief  This file implements the StensorNullStorage class. 
 * \author Helfer Thomas
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSORNULLSTORAGE_HXX_
#define _LIB_TFEL_STENSORNULLSTORAGE_HXX_ 

#include<string>

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    struct StensorNullStorage{

      static const bool storage_is_static   = false;
      static const bool storage_is_external = false;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFELSTENSORNULLSTORAGE_HXX */

