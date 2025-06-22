/*!
 * \file  include/TFEL/Math/Array/ArrayCommonMethods.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_HXX
#define LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_HXX

#include <type_traits>
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"
#include "TFEL/Math/Array/MultipleIndicesFunctors.hxx"

namespace tfel::math {

  /*!
   * \brief a structure gathering access methods for const arrays.
   * \tparam Child: child class
   * \tparam ArrayPolicy: array policy
   */
  template <typename Child, typename ArrayPolicyType>
  struct ConstArrayCommonMethods {
    //! \brief exposing the array policy
    using array_policy = ArrayPolicyType;
    //! \brief exposing the indexing policy
    using indexing_policy = typename ArrayPolicyType::IndexingPolicy;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given index
     * \param[in] i: requested index
     */
    constexpr typename ArrayPolicyType::const_reference operator[](
        const typename ArrayPolicyType::IndexingPolicy::size_type)
        const noexcept;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given indices
     * \param[in] i: requested indices
     */
    constexpr typename ArrayPolicyType::const_reference operator[](
        const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                         ArrayPolicyType::IndexingPolicy::arity>&)
        const noexcept;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given indices
     * \param[in] i: requested indices
     */
    template <typename... Indices>
    constexpr typename ArrayPolicyType::const_reference operator()(
        const Indices...) const noexcept;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given indices
     * \param[in] i: requested indices
     */
    constexpr typename ArrayPolicyType::const_reference operator()(
        const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                         ArrayPolicyType::IndexingPolicy::arity>&)
        const noexcept;

  };  // end of ConstArrayCommonMethods

  /*!
   * \brief a structure gathering access methods and helper function for arrays.
   * \tparam Child: child class
   * \tparam ArrayPolicyType: array policy
   */
  template <typename Child, typename ArrayPolicyType>
  struct MutableArrayCommonMethods
      : ConstArrayCommonMethods<Child, ArrayPolicyType> {
    // exposing const access methods
    using ConstArrayCommonMethods<Child, ArrayPolicyType>::operator[];
    using ConstArrayCommonMethods<Child, ArrayPolicyType>::operator();
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given index
     * \param[in] i: requested index
     */
    constexpr typename ArrayPolicyType::reference operator[](
        const typename ArrayPolicyType::IndexingPolicy::size_type i) noexcept;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given indices
     * \param[in] i: requested indices
     */
    constexpr typename ArrayPolicyType::reference operator[](
        const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                         ArrayPolicyType::IndexingPolicy::arity>&) noexcept;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given indices
     * \param[in] i: requested indices
     */
    template <typename... Indices>
    constexpr typename ArrayPolicyType::reference operator()(
        const Indices... i) noexcept;
    /*!
     * \brief access operator
     * \return a reference to the data associated with the given indices
     * \param[in] i: requested indices
     */
    constexpr typename ArrayPolicyType::reference operator()(
        const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                         ArrayPolicyType::IndexingPolicy::arity>&) noexcept;
    /*!
     * \brief assign values values from an initializer list
     * \param[in] values: values to be assigned
     */
    template <typename ValueType>
    constexpr std::enable_if_t<
        isAssignableTo<ValueType, typename ArrayPolicyType::value_type>(),
        Child&>
    operator=(const std::initializer_list<ValueType>&) noexcept;
    /*!
     * \brief import array values from a sequence
     * \param[in] p: random access iterator to the first element of the
     * sequence
     * \param[in] pe: random access iterator after the last element
     * of the sequence
     */
    template <typename ImportIndexingPolicy, typename InputIterator>
    constexpr void import(const ImportIndexingPolicy&,
                          const InputIterator,
                          const InputIterator);
    /*!
     * \brief fill the array with the given value
     * \param[in] v: value
     */
    template <typename ValueType2>
    constexpr std::enable_if_t<
        isAssignableTo<ValueType2, typename ArrayPolicyType::value_type>(),
        void>
    fill(const ValueType2&);

   protected:
    /*!
     * \tparam OtherArray: type of the given array
     * \brief assign the values of the given array to the values of this array.
     * \note this shall not be used directly, because no runtime checks are
     * performed.
     */
    template <typename OtherArray>
    constexpr void assign(const OtherArray&) noexcept;
    /*!
     * \tparam OtherArray: type of the given array
     * \brief add the values of the given array to the values of this array.
     * \note this shall not be used directly, because no runtime checks are
     * performed.
     */
    template <typename OtherArray>
    constexpr void addAndAssign(const OtherArray&) noexcept;
    /*!
     * \tparam OtherArray: type of the given array
     * \brief substract the values of the given array to the values of this
     * array.
     * \note this shall not be used directly, because no runtime checks
     * are performed.
     */
    template <typename OtherArray>
    constexpr void substractAndAssign(const OtherArray&) noexcept;
    /*!
     * \brief multiplication by a scalar
     * \param[in] s: scalar value
     */
    template <typename ValueType2>
    constexpr std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<ValueType2,
                                  typename ArrayPolicyType::value_type,
                                  OpMult>,
            typename ArrayPolicyType::value_type>(),
        void>
    multiplyByScalar(const ValueType2&);
  };

}  // end of namespace tfel::math

#include "TFEL/Math/Array/ArrayCommonMethods.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS */
