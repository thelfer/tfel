/*!
 * \file   include/TFEL/Math/General/DerivativeType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DERIVATIVETYPE_HXX
#define LIB_TFEL_MATH_DERIVATIVETYPE_HXX

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"

namespace tfel::math {

  /*!
   * \brief an helper metafunction used to determine the type representing
   * the derivative of an object of type `FunctionType` with respect to an
   * object of type `VariableType`.
   *
   * \tparam FunctionTag: tag associated with the function type
   * \tparam VariableTag: tag associated with the variable type
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   *
   * This helper metafunction must be specialised by every code.
   */
  template <typename FunctionTag,
            typename VariableTag,
            typename FunctionType,
            typename VariableType>
  struct DerivativeTypeDispatcher {
    //! \brief a simple alias
    using FunctionArrayPolicy = typename FunctionType::array_policy;
    //! \brief a simple alias
    using VariableArrayPolicy = typename VariableType::array_policy;
    //! \brief a boolean stating if both objects have fixed sizes
    static constexpr auto haveFixedSizes =
        (hasArrayPolicyFixedSizes<FunctionArrayPolicy>() &&
         hasArrayPolicyFixedSizes<VariableArrayPolicy>());
    //! \brief result
    using type =
        std::conditional_t<haveFixedSizes,
                           FixedSizeArrayDerivative<FunctionType, VariableType>,
                           tfel::meta::InvalidType>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation if the first object does not have a tag.
   * \tparam VariableTag: tag associated with the variable type
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename VariableTag, typename FunctionType, typename VariableType>
  struct DerivativeTypeDispatcher<tfel::meta::InvalidType,
                                  VariableTag,
                                  FunctionType,
                                  VariableType> {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation if the second object does not have a tag.
   * \tparam FunctionTag: tag associated with the function type
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionTag, typename FunctionType, typename VariableType>
  struct DerivativeTypeDispatcher<FunctionTag,
                                  tfel::meta::InvalidType,
                                  FunctionType,
                                  VariableType> {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation if none o the two objects have a tag.
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeTypeDispatcher<tfel::meta::InvalidType,
                                  tfel::meta::InvalidType,
                                  FunctionType,
                                  VariableType> {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief a meta function returning the type of the derivative of
   * a variable of type `FunctionType` with respect to a variable of type
   * `VariableType`.
   * \tparam is_function_type_scalar: boolean stating if `FunctionType` is a
   * scalar
   * \tparam is_variable_type_scalar: boolean stating if `VariableType` is a
   * scalar
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <bool is_function_type_scalar,
            bool is_variable_type_scalar,
            typename FunctionType,
            typename VariableType>
  struct DerivativeTypeImplementation {
    //! \brief in none is scalar, use the derivative type dispatcher
    using type = typename tfel::math::DerivativeTypeDispatcher<
        typename ComputeObjectTag<FunctionType>::type,
        typename ComputeObjectTag<VariableType>::type,
        FunctionType,
        VariableType>::type;
  };  // end of DerivativeTypeImplementation

  /*!
   * \brief partial specialisation of both types are scalar
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeTypeImplementation<true, true, FunctionType, VariableType> {
    //! \brief the result
    using type = result_type<FunctionType, VariableType, OpDiv>;
  };

  /*!
   * \brief partial specialisation of both types are scalar
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeTypeImplementation<false, true, FunctionType, VariableType> {
    //! \brief the result
    using type = result_type<FunctionType, VariableType, OpDiv>;
  };

  /*!
   * \brief a meta function returning the type of the derivative of
   * a variable of type `FunctionType` with respect to variables of type
   * `VariablesType`.
   * \tparam FunctionType: function type
   * \tparam VariablesTypes: variables types
   */
  template <typename FunctionType, typename... VariablesTypes>
  struct DerivativeType;

  /*!
   * \partial specialisation for more than two variables.
   * \tparam FunctionType: function type
   * \tparam VariableType: first variable type
   * \tparam OtherVariablesTypes: other variables' types
   */
  template <typename FunctionType,
            typename VariableType,
            typename... OtherVariablesTypes>
  struct DerivativeType<FunctionType, VariableType, OtherVariablesTypes...> {
    //! \brief the result
    using type = typename DerivativeType<
        typename DerivativeType<FunctionType, VariableType>::type,
        OtherVariablesTypes...>::type;
  };  // end of DerivativeType
  /*!
   * \partial specialisation for one variable.
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeType<FunctionType, VariableType> {
    //! \brief the result
    using type = typename DerivativeTypeImplementation<isScalar<FunctionType>(),
                                                       isScalar<VariableType>(),
                                                       FunctionType,
                                                       VariableType>::type;
  };  // end of DerivativeType

  /*!
   * \brief an helper metafunction to define the derivative of a function with
   * respect to several variables.
   * \tparam FunctionType: type of the function
   * \tparam VariableType: type of the first variable
   * \tparam OtherVariableTypes: type of the other variables
   */
  template <typename FunctionType,
            typename VariableType,
            typename... OtherVariableTypes>
  struct DerivativeTypeMetaFunction {
    //! \brief result of the metafunction
    using type = typename DerivativeTypeMetaFunction<
        typename DerivativeTypeMetaFunction<FunctionType, VariableType>::type,
        OtherVariableTypes...>::type;
  };
  /*!
   * \brief partial specialisation of the helper metafunction
   * to end the recursion.
   * \tparam FunctionType: type of the function
   * \tparam VariableType: type of the variable
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeTypeMetaFunction<FunctionType, VariableType> {
    //! \brief result of the metafunction
    using type =
        typename tfel::math::DerivativeType<FunctionType, VariableType>::type;
  };
  /*!
   * \brief a simple alias giving the type representing the derivative of an
   * object of type `FunctionType` with respect to objects of the type
   * given by the template argument pack `VariableTypes`.
   * \param[in] FunctionType: function type
   * \param[in] VariableTypes: variable types
   */
  template <typename FunctionType, typename... VariableTypes>
  using derivative_type =
      typename DerivativeTypeMetaFunction<FunctionType, VariableTypes...>::type;

  /*!
   * \brief compute a numerical approximation of the derivative  a a function
   * with respect to a variable with a centered finite difference scheme.
   * \tparam FunctionType: type of the function to be differentiated.
   * \tparam VariableType: type of the variable.
   * \tparam ScalarType: type of the perturbation value.
   * \param[in] f: function.
   * \param[in] v: variable.
   * \param[in] e: perturbation value.
   */
  template <typename FunctionType, typename VariableType, typename ScalarType>
  derivative_type<std::invoke_result_t<FunctionType, VariableType>,
                  VariableType>
  computeNumericalDerivative(const FunctionType&,
                             const VariableType&,
                             const ScalarType&);

}  // end of namespace tfel::math

#include "TFEL/Math/General/DerivativeType.ixx"

#endif /* LIB_TFEL_MATH_DERIVATIVETYPE_HXX */
