/*!
 *\file   qtSpecific.hxx
 *\brief  This file pecialises some class traits for quantities.
 *\author Thomas Helfer
 *\date   06 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 *linking exception or the CECILL-A licence. A copy of thoses licences are
 *delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTSPECIFIC_HXX
#define LIB_TFEL_MATH_QTSPECIFIC_HXX

#include "TFEL/TypeTraits/AbsType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/qt.hxx"

namespace tfel::typetraits {

  /*!
   * \brief Partial specialisation for qt
   * \see   IsScalar
   */
  template <tfel::math::QuantityConcept QuantityType>
  struct IsScalar<QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsScalar
   */
  template <tfel::math::QuantityConcept QuantityType>
  struct IsScalar<const QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsReal
   */
  template <tfel::math::QuantityConcept QuantityType>
  struct IsReal<QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsReal<base_type<QuantityType>>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsReal
   */
  template <tfel::math::QuantityConcept QuantityType>
  struct IsReal<const QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsReal<base_type<QuantityType>>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsComplex
   */
  template <tfel::math::QuantityConcept QuantityType>
  struct IsComplex<QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsComplex<base_type<QuantityType>>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsComplex
   */
  template <tfel::math::QuantityConcept QuantityType>
  struct IsComplex<const QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsComplex<base_type<QuantityType>>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   RealPartType
   */
  template <tfel::math::NoUnitQuantityConcept QuantityType>
  struct RealPartType<QuantityType> {
    //! \brief result of the metafunction
    using type =
        tfel::math::qt<tfel::math::quantity_unit<QuantityType>,
                       typename RealPartType<base_type<QuantityType>>::type>;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   RealPartType
   */
  template <tfel::math::NoUnitQuantityConcept QuantityType>
  struct RealPartType<const QuantityType> {
    //! \brief result of the metafunction
    using type =
        tfel::math::qt<tfel::math::quantity_unit<QuantityType>,
                       typename RealPartType<base_type<QuantityType>>::type>;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsAssignableTo
   */
  template <tfel::math::QuantityConcept QuantityType1,
            tfel::math::QuantityConcept QuantityType2>
  struct IsAssignableTo<QuantityType1, QuantityType2> {
    //! \brief result of the metafunction
    static constexpr bool cond =
        (std::same_as<
            typename Promote<tfel::math::base_type<QuantityType1>,
                             tfel::math::base_type<QuantityType2>>::type,
            tfel::math::base_type<QuantityType2>>)&&  //
        (std::same_as<tfel::math::quantity_unit<QuantityType1>,
                      tfel::math::quantity_unit<QuantityType2>>);
  };

  /*!
   * \brief Partial specialisation for quantity
   * \see   IsAssignableTo
   */
  template <tfel::math::NoUnitQuantityConcept QuantityType,
            tfel::math::StandardArithmeticTypeConcept ScalarType>
  struct IsAssignableTo<QuantityType, ScalarType> {
    //! \brief result of the metafunction
    static constexpr bool cond =
        IsAssignableTo<base_type<QuantityType>, ScalarType>::cond;
  };

  /*!
   * \brief Partial specialisation for quantity
   * \see   IsAssignableTo
   */
  template <StandardArithmeticTypeConcept ScalarType,
            tfel::math::NoUnitQuantityConcept QuantityType>
  struct IsAssignableTo<ScalarType, QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond =
        IsAssignableTo<ScalarType, base_type<QuantityType>>::cond;
  };

  template <tfel::math::QuantityConcept QuantityType>
  struct AbsType<QuantityType> {
    using type = tfel::math::qt<
        tfel::math::quantity_unit<QuantityType>,
        typename AbsType<tfel::math::base_type<QuantityType>>::type>;
  };  // end of struct AbsType

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_QTSPECIFIC_HXX */
