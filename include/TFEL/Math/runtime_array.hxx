/*!
 * \file  include/TFEL/Math/runtime_array.hxx
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

#ifndef LIB_TFEL_MATH_RUNTIME_ARRAY_HXX
#define LIB_TFEL_MATH_RUNTIME_ARRAY_HXX

#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/GenericRuntimeArray.hxx"

namespace tfel::math {

  template <typename ValueType = double>
  struct runtime_array
      : GenericRuntimeArray<runtime_array<ValueType>,
                            RuntimeVectorArrayPolicy<ValueType>> {
    //! \brief a simple alias
    using GenericRuntimeArrayBase =
        GenericRuntimeArray<runtime_array<ValueType>,
                            RuntimeVectorArrayPolicy<ValueType>>;
    //! \brief default constructor
    runtime_array() = delete;
    //! \brief move constructor
    runtime_array(runtime_array&&) = default;
    //! \brief copy constructor
    runtime_array(const runtime_array&) = default;
    //! \brief move assignement
    runtime_array& operator=(runtime_array&&) = default;
    //! \brief standard assignement
    runtime_array& operator=(const runtime_array&) = default;
    // inheriting GenericRuntimeArray' constructors
    using GenericRuntimeArray<
        runtime_array<ValueType>,
        RuntimeVectorArrayPolicy<ValueType>>::GenericRuntimeArray;
    // inheriting GenericRuntimeArray' assignement operators
    using GenericRuntimeArray<runtime_array<ValueType>,
                              RuntimeVectorArrayPolicy<ValueType>>::operator=;
  };  // end of runtime_array

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief specialisation
   */
  template <typename ValueType, typename ValueType2>
  struct IsAssignableTo<tfel::math::runtime_array<ValueType>,
                        tfel::math::runtime_array<ValueType2>> {
    //! \brief result
    static constexpr auto cond = isAssignableTo<ValueType, ValueType2>();
  };

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_RUNTIME_ARRAY_HXX */
