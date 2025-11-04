/*!
 * \file  include/TFEL/Math/Array/GenericFixedSizeArray.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_HXX
#define LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_HXX

#include <iterator>
#include <type_traits>
#include <initializer_list>
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/MutableFixedSizeArrayBase.hxx"

#define TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(X, Y)                      \
  /*! \brief default constructor */                                           \
  TFEL_HOST_DEVICE constexpr X() noexcept = default;                          \
  /*! \brief move constructor */                                              \
  TFEL_HOST_DEVICE constexpr X(X&&) noexcept = default;                       \
  /*! \brief copy constructor */                                              \
  TFEL_HOST_DEVICE constexpr X(const X&) noexcept = default;                  \
  /*! \brief move assignement */                                              \
  TFEL_HOST_DEVICE constexpr X& operator=(X&&) noexcept = default;            \
  /*! \brief standard assignement */                                          \
  TFEL_HOST_DEVICE constexpr X& operator=(const X&) noexcept = default;       \
  /*!                                                                         \
   * \brief constructor from a value                                          \
   * \param[in] value: value used to initialize the array                     \
   */                                                                         \
  template <typename ValueType2>                                              \
  TFEL_HOST_DEVICE constexpr explicit X(                                      \
      const ValueType2&                                                       \
          value) noexcept requires(isAssignableTo<ValueType2,                 \
                                                  typename X::value_type>())  \
      : Y(value) {}                                                           \
  /*!                                                                         \
   * \brief constructor from an initializer list                              \
   * \param[in] values: values                                                \
   */                                                                         \
  TFEL_HOST_DEVICE constexpr X(                                               \
      const std::initializer_list<typename X::value_type>& values) noexcept   \
      : Y(values) {}                                                          \
  /*!                                                                         \
   * \brief constructor from an initializer list                              \
   * \param[in] values: values                                                \
   */                                                                         \
  template <typename ValueType2>                                              \
  TFEL_HOST_DEVICE constexpr X(                                               \
      const std::initializer_list<ValueType2>&                                \
          values) noexcept requires(isAssignableTo<ValueType2,                \
                                                   typename X::value_type>()) \
      : Y(values) {}                                                          \
  /*!                                                                         \
   * \brief copy constructor from an object assignable the X class.           \
   * \param[in] src: source                                                   \
   *                                                                          \
   * This is mostly used by expression objects and views.                     \
   */                                                                         \
  template <typename OtherArray>                                              \
  TFEL_HOST_DEVICE constexpr X(const OtherArray& src) noexcept requires(      \
      (isAssignableTo<OtherArray, X>()) && (!std::is_same_v<OtherArray, X>))  \
      : Y(src) {}                                                             \
  /*!                                                                         \
   * \brief Default Constructor.                                              \
   * \param const base_type<T>*                                               \
   * const, pointer to a tabular used to initialise the components            \
   * of the stensor. This tabular is left unchanged.                          \
   */                                                                         \
  template <typename InputIterator>                                           \
  TFEL_HOST_DEVICE constexpr explicit X(                                      \
      const InputIterator                                                     \
          p) noexcept requires(std::                                          \
                                   is_same_v<                                 \
                                       typename std::iterator_traits<         \
                                           InputIterator>::value_type,        \
                                       base_type<typename X::value_type>>)    \
      : Y(p) {}                                                               \
  /* inheriting GenericFixedSizeArray' assignement operators */               \
  using Y::operator=

namespace tfel::math {

  /*!
   * \brief a fixed size array class with storage on the stack.
   * \tparam N: container size
   * \tparam ArrayPolicy: array policy
   */
  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N =
                getArrayPolicySize<ArrayPolicy>()>
  struct GenericFixedSizeArray
      : MutableFixedSizeArrayBase<GenericFixedSizeArray<Child, ArrayPolicy, N>,
                                  ArrayPolicy> {
    static_assert(hasArrayPolicyFixedSizes<ArrayPolicy>(),
                  "array policy must have fixed sizes");
    static_assert(getArrayPolicySize<ArrayPolicy>() != 0,
                  "invalid array policy size");
    static_assert(N != 0, "invalid container size");
    static_assert(N >= getArrayPolicyMinimalDataSize<ArrayPolicy>(),
                  "specified container size is lower than "
                  "the minimal container size");
    //
    using value_type = typename MutableFixedSizeArrayBase<
        GenericFixedSizeArray<Child, ArrayPolicy, N>,
        ArrayPolicy>::value_type;
    //! \brief default constructor
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray() noexcept;
    //! \brief copy constructor
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray(
        const GenericFixedSizeArray&) noexcept;
    //! \brief move constructor
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray(
        GenericFixedSizeArray&&) noexcept;
    // exposing assignement operators from MutableFixedSizeArrayBase
    /*!
     * \brief constructor from a value
     * \param[in] value: value used to initialize the array
     */
    template <typename ValueType>
    TFEL_HOST_DEVICE constexpr explicit GenericFixedSizeArray(
        const ValueType&) noexcept
        requires(isAssignableTo<ValueType, value_type>());
    /*!
     * \brief constructor from an initializer list
     * \param[in] values: values
     */
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray(
        const std::initializer_list<ValueType2>& values) noexcept  //
        requires(isAssignableTo<ValueType2, value_type>());
    /*!
     * \brief default donstructor.
     * \param p pointer to an array used to initialise the components
     * of the stensor. This array is left unchanged.
     */
    template <typename InputIterator>
    TFEL_HOST_DEVICE constexpr explicit GenericFixedSizeArray(
        const InputIterator) noexcept  //
        requires(std::is_same_v<
                 typename std::iterator_traits<InputIterator>::value_type,
                 base_type<value_type>>);
    /*!
     * \brief copy constructor from an object assignable to the `Child` class.
     * \param[in] src: source
     *
     * This is mostly used by expression objects and views.
     */
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray(
        const OtherArray&) noexcept  //
        requires((isAssignableTo<OtherArray, Child>()) &&
                 (!std::is_same_v<OtherArray, Child>));
    /*!
     * \return the physical size used by the underlying array. This size must
     * be greater than than the logical number of elements contained in the
     * array which is returned by `IndexingPolicy::size`.
     */
    TFEL_HOST_DEVICE constexpr
        typename GenericFixedSizeArray<Child, ArrayPolicy, N>::size_type
        getContainerSize() const noexcept;
    // inheriting MutableFixedSizeArrayBase' assignement operator
    using MutableFixedSizeArrayBase<
        GenericFixedSizeArray<Child, ArrayPolicy, N>,
        ArrayPolicy>::operator=;
    //
    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr Child& operator=(const OtherArray&) noexcept
        requires(isAssignableTo<OtherArray, Child>());
    //
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr Child& operator+=(const OtherArray&) noexcept
        requires(isAssignableTo<OtherArray, Child>());
    //
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr Child& operator-=(const OtherArray&) noexcept
        requires(isAssignableTo<OtherArray, Child>());
    //
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr Child& operator*=(const ValueType2&) noexcept  //
        requires(isAssignableTo<
                 BinaryOperationResult<ValueType2, value_type, OpMult>,
                 value_type>());
    //
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr Child& operator/=(const ValueType2&) noexcept  //
        requires(
            isAssignableTo<BinaryOperationResult<value_type, ValueType2, OpDiv>,
                           value_type>());
    //! \return a pointer to the underlying array serving as element storage.
    TFEL_HOST_DEVICE constexpr
        typename GenericFixedSizeArray<Child, ArrayPolicy, N>::pointer
        data() noexcept;
    //! \return a pointer to the underlying array serving as element storage.
    TFEL_HOST_DEVICE constexpr
        typename GenericFixedSizeArray<Child, ArrayPolicy, N>::const_pointer
        data() const noexcept;

   protected:
    //! \brief assignement operator
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray& operator=(
        const GenericFixedSizeArray&) noexcept;
    //! \brief move assigment
    TFEL_HOST_DEVICE constexpr GenericFixedSizeArray& operator=(
        GenericFixedSizeArray&&) noexcept;
    //! \brief values holded
    typename ArrayPolicy::value_type v[N];
  };

}  // end of namespace tfel::math

#include "TFEL/Math/Array/GenericFixedSizeArray.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_HXX */
