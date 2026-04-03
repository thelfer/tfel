/*!
 * \file  include/TFEL/Math/runtime_array.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 août 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_RUNTIME_ARRAY_HXX
#define LIB_TFEL_MATH_RUNTIME_ARRAY_HXX

#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/GenericRuntimeArray.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"
#include "TFEL/Math/Array/ArrayConcept.hxx"
#include "TFEL/Math/Forward/runtime_array.hxx"

namespace tfel::math {

  template <typename ValueType>
  struct runtime_array
      : GenericRuntimeArray<runtime_array<ValueType>,
                            RuntimeVectorArrayPolicy<ValueType>>,
        ArrayConceptBase<runtime_array<ValueType>> {
    //! \brief a simple alias
    using GenericRuntimeArrayBase =
        GenericRuntimeArray<runtime_array<ValueType>,
                            RuntimeVectorArrayPolicy<ValueType>>;
    //! \brief default constructor
    runtime_array() = default;
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
    // \brief destructor
    ~runtime_array() noexcept = default;
  };  // end of runtime_array

  /*!
   * \tparam T: numeric type used by the array
   */
  template <typename T>
  struct UnaryResultType<runtime_array<T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    runtime_array<ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for runtime_array and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename T, typename Scal>
  class ComputeBinaryOperationResult<ArrayTag,
                                     ScalarTag,
                                     runtime_array<T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    runtime_array<ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for runtime_array and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename T, typename Scal>
  class ComputeBinaryOperationResult<ArrayTag,
                                     ScalarTag,
                                     runtime_array<T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    runtime_array<ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for runtime_array and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     ArrayTag,
                                     Scal,
                                     runtime_array<T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    runtime_array<ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for runtime_array
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<runtime_array<T>, runtime_array<T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    runtime_array<ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for runtime_array
   * \see   ResultType
   */
  template <typename T, typename T2>
  class ResultType<runtime_array<T>, runtime_array<T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    runtime_array<ResultNumericType>>;
  };

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief partial specialisation for the assignement of runtime arrays
   */
  template <typename ValueType, typename ValueType2>
  struct IsAssignableTo<tfel::math::runtime_array<ValueType>,
                        tfel::math::runtime_array<ValueType2>> {
    //! \brief result
    static constexpr auto cond = isAssignableTo<ValueType, ValueType2>();
  };

  /*!
   * \brief partial specialisation allowing arrays of views to be assigned to a
   * runtime array
   */
  template <typename MappedType,
            typename MemoryIndexingPolicyType,
            typename ViewIndexingPolicyType,
            typename ValueType>
  struct IsAssignableTo<tfel::math::ViewsArray<MappedType,
                                               MemoryIndexingPolicyType,
                                               ViewIndexingPolicyType>,
                        tfel::math::runtime_array<ValueType>> {
    //! \brief a simple alias
    using RuntimeArrayIndexingPolicy =
        typename tfel::math::runtime_array<ValueType>::indexing_policy;
    //! \brief a simple alias
    using View =
        typename tfel::math::ViewsArray<MappedType,
                                        MemoryIndexingPolicyType,
                                        ViewIndexingPolicyType>::view_type;

    //! \brief result
    static constexpr auto cond =
        ((tfel::math::checkIndexingPoliciesCompatiblity<
             RuntimeArrayIndexingPolicy,
             MemoryIndexingPolicyType>()) &&
         (isAssignableTo<View, ValueType>()));
  };

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_RUNTIME_ARRAY_HXX */
