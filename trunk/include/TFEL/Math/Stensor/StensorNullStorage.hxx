/*!
 * \file   StensorNullStorage.hxx  
 * \brief  This file implements the StensorNullStorage class. 
 * \author Helfer Thomas
 * \date   02 jun 2006
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

