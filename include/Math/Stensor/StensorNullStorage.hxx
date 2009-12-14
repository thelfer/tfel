/*!
 * \file   StensorNullStorage.hxx  
 * \brief  This file implements the StensorNullStorage class. 
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_STENSORNULLSTORAGE_HXX_
#define _LIB_TFEL_STENSORNULLSTORAGE_HXX_ 

#include<string>
#include"Utilities/Name.hxx"
#include"Utilities/ToString.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    struct StensorNullStorage{

      static const std::string getName(void){
	return std::string("StensorNullStorage<")+tfel::utilities::ToString(N)+std::string(",")+tfel::utilities::Name<T>::getName()+std::string(">");
      }

      static const bool storage_is_static   = false;
      static const bool storage_is_external = false;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFELSTENSORNULLSTORAGE_HXX */

