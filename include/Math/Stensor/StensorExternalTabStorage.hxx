/*!
 * \file   StensorExternalTabStorage.hxx
 * \brief  This file implements the StensorExternalTabStorage.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_STENSOREXTERNALTABSTORAGE_HXX_
#define _LIB_TFEL_STENSOREXTERNALTABSTORAGE_HXX_ 

#include <string>

#include "Config/TFELConfig.hxx"
#include "Utilities/Name.hxx"
#include "Utilities/ToString.hxx"
#include "FSAlgorithm/FSAlgorithm.hxx"
#include "TypeTraits/BaseType.hxx"
#include "TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "Math/General/StorageTraits.hxx"
#include "Math/Stensor/Internals/StensorImport.hxx"
#include "Math/Stensor/Internals/StensorExport.hxx"
#include "Math/stensor.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    class StensorExternalTab{
      
      // Assignement Operator (disabled)
      StensorExternalTab& operator = (const StensorExternalTab&);

      //Default Constructor
      StensorExternalTab();

      typedef typename tfel::typetraits::BaseType<T>::type base;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));

    protected:

      base * const p;
      T v[N];
      
    public:

      static const std::string getName(void){
	return std::string("StensorExternalTab<")+tfel::utilities::ToString(N)+std::string(",")+tfel::utilities::Name<T>::getName()+std::string(">");
      }

      // Copy Constructor
      StensorExternalTab(const StensorExternalTab&);

      // Default Constructor 
      explicit TFEL_MATH_INLINE StensorExternalTab(typename tfel::typetraits::BaseType<T>::type* const init)
	: p(init)
      {
	typedef tfel::math::internals::ImportFromTab<StensorSizeToDime<N>::value> Import;
	Import::exe(reinterpret_cast<base*>(this->v),init);
      }

      // Destructor
      TFEL_MATH_INLINE ~StensorExternalTab()
      {
	typedef tfel::math::internals::ExportToTab<StensorSizeToDime<N>::value> Export;
	Export::exe(reinterpret_cast<const base*>(this->v),p);
      }

      static const bool storage_is_static   = false;
      static const bool storage_is_external = true;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSOREXTERNALTABSTORAGE_HXX */

