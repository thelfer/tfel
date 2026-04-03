/*!
 * \file   include/TFEL/Math/General/ResultType.hxx
 * \brief  This file declares the ResultType class.
 * \author Thomas Helfer
 * \date   24 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_RESULT_TYPE_HXX

#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel::math {

  // forward declaration
  struct OpDiv;

  /*
   * \class ComputeBinaryOperationResult
   * \brief A helper class for ResultType.
   * This default version returns InvalidType both for
   * Result and Handle.
   * \param typename TagA, tag of the type of the first
   * argument of the operation.
   * \param typename TagB, tag of the type of second
   * argument of the operation.
   * \param typename A, type of the first argument of the
   * operation.
   * \param typename B, type of the second argument of the
   * operation.
   * \param typename Op, operation.
   * \return type, type of the result of the operation.
   * \see ResultType.
   * \see ComputeObjectTag.
   */
  template <typename TagA, typename TagB, typename A, typename B, typename Op>
  struct ComputeBinaryOperationResult {
    //! \brief result of the binary operation.
    using type = tfel::meta::InvalidType;
  };  // end of ComputeBinaryOperationResult

  /*
   * \class ResultType
   * This metafunction gives the result of a binary operation.
   * This class is meant to be specialised on a per objet basis.
   * (in comparison, ComputeBinaryResult is meant to be specialised
   * on a per concept basis)
   * The helper class ComputeBinaryOperationResult is only convenient to declare
   * scalar to objet operations.
   * \param typename A, type of the first argument of the
   * operation.
   * \param typename B, type of the second argument of the
   * operation.
   * \param typename Op, operation.
   * \return type, type of the result.
   */
  template <typename A, typename B, typename Op>
  struct ResultType {
    //! a simple alias
    using TypeA = std::decay_t<A>;
    //! a simple alias
    using TypeB = std::decay_t<B>;
    //! Tag of the object A
    using TagA = typename ComputeObjectTag<TypeA>::type;
    //! Tag of the object B
    using TagB = typename ComputeObjectTag<TypeB>::type;
    //! \brief result
    using type =
        typename ComputeBinaryOperationResult<TagA, TagB, TypeA, TypeB, Op>::
            type;
  };

  //! \brief a simple alias
  template <typename A, typename B, typename Op>
  using result_type = typename ResultType<A, B, Op>::type;

  /*!
   *
   */

  /*!
   * \brief metafunction returning the inverse of a type.
   * \tparam isScalar: boolean stating of the given type is scalar.
   * \tparam MathObjectType: type
   */
  template <bool isScalar, typename MathObjectType>
  struct InverseTypeImplementation;

  /*!
   * \brief partial specialisation of the `InverseTypeImplementation` for
   * scalars.
   * \tparam ScalarTypes: type whose inverse is requested
   */
  template <typename ScalarType>
  struct InverseTypeImplementation<true, ScalarType> {
    //! \brief result
    using type =
        result_type<tfel::typetraits::base_type<ScalarType>, ScalarType, OpDiv>;
  };

  /*!
   * \brief an help structure to compute the inverse of a MathObject. This
   * structure is meant to be specialized when meaningful.
   * \tparam MathObjectTag: tag associated with the math object
   * \tparam MathObjectType: type of the considered math object
   */
  template <typename MathObjectTag, typename MathObjectType>
  struct InverseTypeDispatcher {
    //! \brief default result
    using type = tfel::meta::InvalidType;
  };

  /*!
   * \brief partial specialisation of the `InverseTypeImplementation` for
   * non scalar types.
   * \tparam MathObjectType: type whose inverse is requested
   */
  template <typename MathObjectType>
  struct InverseTypeImplementation<false, MathObjectType> {
    //! \brief result
    using type = typename InverseTypeDispatcher<
        typename ComputeObjectTag<MathObjectType>::type,
        MathObjectType>::type;
  };  // end of struct InverseTypeImplementation

  /*!
   * \brief  a simple alias to compute the type of the inverse of a type
   */
  template <typename MathObjectType>
  using invert_type = typename InverseTypeImplementation<
      tfel::typetraits::isScalar<MathObjectType>(),
      MathObjectType>::type;

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_RESULT_TYPE_HXX */
