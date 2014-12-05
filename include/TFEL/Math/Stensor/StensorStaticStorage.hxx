/*!
 * \file   include/TFEL/Math/Stensor/StensorStaticStorage.hxx
 * \brief  This file implements the StensorStaticStorage class.
 * \author Helfer Thomas
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSORSTATICSTORAGE_HXX_
#define _LIB_TFEL_STENSORSTATICSTORAGE_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include"TFEL/Math/fsarray.hxx"
#include"TFEL/Math/General/StorageTraits.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL StensorStatic
      : public fsarray<N,T>
    {

    public:

      //Default Constructor
      StensorStatic(){};

      // Default Constructor 
      explicit TFEL_MATH_INLINE StensorStatic(const typename tfel::typetraits::BaseType<T>::type* const init)
      {
	typedef typename tfel::typetraits::BaseType<T>::type base;
	TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
	tfel::fsalgo::copy<N>::exe(init,reinterpret_cast<base*>(this->v));
      }

      // Assignement Operator 
      StensorStatic& operator = (const StensorStatic& src)
      {
	tfel::fsalgo::copy<N>::exe(src.v,this->v);
	return *this;
      }

      static constexpr bool storage_is_static   = true;
      static constexpr bool storage_is_external = false;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSORSTATICSTORAGE_HXX */

