/*!
 * \file   TypeTransform.ixx
 * \brief  This file implements IdTransform and SmartPtrTransform.
 * \author Helfer Thomas
 * \date   01 jun 2006
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

      static
      const std::string
      getName(void){
	return std::string("IdTransform<")+Name<T>::getName()+std::string(">");
      }

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
    struct SmartPtrTransform{

      typedef tfel::utilities::SmartPtr<T> type;
      typedef T  value_type;
      typedef T* pointer;
      typedef const T* const_pointer;
      typedef T& reference;
      typedef const T& const_reference;

      static
      const std::string
      getName(void){
	return std::string("SmartPtrTransform<")+Name<T>::getName()+std::string(">");
      }

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

