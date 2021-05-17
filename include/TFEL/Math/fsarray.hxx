/*!
 * \file  include/TFEL/Math/fsarray.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 août 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FSARRAY_HXX
#define LIB_TFEL_MATH_FSARRAY_HXX

#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/FixedSizeMathObjectArrayView.hxx"
#include "TFEL/Math/Forward/fsarray.hxx"

namespace tfel::math {

  /*!
   * \brief a class representing a fixed sized array
   * \tparam N: number of values hold
   * \tparam ValueType: type of the values
   */
  template <unsigned short N, typename ValueType = double>
  struct fsarray : GenericFixedSizeArray<fsarray<N, ValueType>,
                                         FixedSizeVectorPolicy<N, ValueType>> {
    //! \brief default constructor
    constexpr fsarray() noexcept = default;
    //! \brief move constructor
    constexpr fsarray(fsarray&&) noexcept = default;
    //! \brief copy constructor
    constexpr fsarray(const fsarray&) noexcept = default;
    //! \brief move assignement
    constexpr fsarray& operator=(fsarray&&) noexcept = default;
    //! \brief standard assignement
    constexpr fsarray& operator=(const fsarray&) noexcept = default;
    // inheriting GenericFixedSizeArray' constructors
    using GenericFixedSizeArray<fsarray<N, ValueType>,
				FixedSizeVectorPolicy<N, ValueType>>::GenericFixedSizeArray;
    // inheriting GenericFixedSizeArray' assignement operators
    using GenericFixedSizeArray<fsarray<N, ValueType>,
				FixedSizeVectorPolicy<N, ValueType>>::operator=;
  };  // end of fsarray

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief specialisation of IsAssignableTo for `fsarray`'s
  template <unsigned short N, typename ValueType, typename ValueType2>
  struct IsAssignableTo<tfel::math::fsarray<N, ValueType>,
                        tfel::math::fsarray<N, ValueType2>> {
    //! \brief result
    static constexpr auto cond = isAssignableTo<ValueType, ValueType2>();
  };

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_FSARRAY_HXX */
