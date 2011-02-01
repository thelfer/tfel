/*!
 * \file   StensorStaticStorage.hxx
 * \brief  This file implements the StensorStaticStorage class.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_STENSORSTATICSTORAGE_HXX_
#define _LIB_TFEL_STENSORSTATICSTORAGE_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Utilities/ToString.hxx"

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include"TFEL/Math/General/StorageTraits.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    class StensorStatic{

    protected:
      
      T v[N];
      
    public:

      static const std::string getName(void){
	return std::string("StensorStatic<")+tfel::utilities::ToString(N)+std::string(",")+tfel::utilities::Name<T>::getName()+std::string(">");
      }

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

      static const bool storage_is_static   = true;
      static const bool storage_is_external = false;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSORSTATICSTORAGE_HXX */

