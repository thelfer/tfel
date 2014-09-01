/*!
 * \file   include/TFEL/Utilities/MultipleDequeVector/TypeTransform.ixx
 * \brief  This file implements IdTransform and shared_ptrTransform.
 * \author Helfer Thomas
 * \date   01 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TYPETRANSFORM_IXX_
#define _LIB_TFEL_TYPETRANSFORM_IXX_ 

#include <string>

#include"TFEL/Utilities/SmartPtr.hxx"

namespace tfel{
  
  namespace utilities{

    template<typename T>
    struct IdTransform{

      typedef T  type;
      typedef T  value_type;
      typedef T* pointer;
      typedef const T* const_pointer;
      typedef T& reference;
      typedef const T& const_reference;

      static inline value_type& reverse_transform(type& src){
	return src;
      }

      static inline const value_type& reverse_transform(const type& src){
	return src;
      }

      static inline pointer to_pointer(type& src){
	return &src;
      }

      static inline const_pointer to_pointer(const type& src){
	return &src;
      }

    };
    
    template<typename T>
    struct shared_ptrTransform{

      typedef tfel::utilities::shared_ptr<T> type;
      typedef T  value_type;
      typedef T* pointer;
      typedef const T* const_pointer;
      typedef T& reference;
      typedef const T& const_reference;

      static inline value_type& reverse_transform(type& src){
	return *src;
      }

      static inline  const value_type& reverse_transform(const type& src){
	return *src;
      }

      static inline pointer to_pointer(type& src){
	return src.getPtr();
      }

      static inline const_pointer to_pointer(const type& src){
	return src.getPtr();
      }

    };

  }

}

#endif /* _LIB_TFEL_TYPETRANSFORM_IXX */

