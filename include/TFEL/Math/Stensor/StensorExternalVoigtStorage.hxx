/*!
 * \file   include/TFEL/Math/Stensor/StensorExternalVoigtStorage.hxx
 * \brief  This file implements the StensorExternalVoigtStorage
 * \author Helfer Thomas
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSOREXTERNALVOIGTSTORAGE_HXX_
#define _LIB_TFEL_STENSOREXTERNALVOIGTSTORAGE_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/Math/General/StorageTraits.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include"TFEL/Math/Stensor/Internals/StensorImport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorExport.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    class StensorExternalVoigt{
      
      // Assignement Operator (disabled)
      StensorExternalVoigt& operator = (const StensorExternalVoigt&);

      //Default Constructor
      StensorExternalVoigt();

      typedef typename tfel::typetraits::BaseType<T>::type base;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));

    protected:

      base * const p;
      T v[N];
      
    public:

      // Copy Constructor
      StensorExternalVoigt(const StensorExternalVoigt&);

      // Default Constructor 
      explicit TFEL_MATH_INLINE StensorExternalVoigt(typename tfel::typetraits::BaseType<T>::type* const init)
	: p(init)
      {
	typedef tfel::math::internals::ImportFromVoigt<StensorSizeToDime<N>::value> Import;
	Import::exe(reinterpret_cast<base*>(this->v),init);
      }

      // Destructor
      TFEL_MATH_INLINE ~StensorExternalVoigt()
      {
	typedef tfel::math::internals::ExportToVoigt<StensorSizeToDime<N>::value> Export;
	Export::exe(reinterpret_cast<const base*>(this->v),p);
      }

      static constexpr bool storage_is_static   = false;
      static constexpr bool storage_is_external = true;

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSOREXTERNALVOIGTSTORAGE_HXX */

