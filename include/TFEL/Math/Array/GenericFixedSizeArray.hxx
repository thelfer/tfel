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

#define TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(X, Y)                  \
  /*! \brief default constructor */                                       \
  constexpr X() noexcept = default;                                       \
  /*! \brief move constructor */                                          \
  constexpr X(X&&) noexcept = default;                                    \
  /*! \brief copy constructor */                                          \
  constexpr X(const X&) noexcept = default;                               \
  /*! \brief move assignement */                                          \
  constexpr X& operator=(X&&) noexcept = default;                         \
  /*! \brief standard assignement */                                      \
  constexpr X& operator=(const X&) noexcept = default;                    \
  /*!                                                                     \
   * \brief constructor from a value                                      \
   * \param[in] value: value used to initialize the array                 \
   */                                                                     \
  template <typename ValueType2,                                          \
            typename std::enable_if<                                      \
                isAssignableTo<ValueType2, typename X::value_type>(),     \
                bool>::type = true>                                       \
  constexpr explicit X(const ValueType2& value) noexcept : Y(value) {}    \
  /*!                                                                     \
   * \brief constructor from an initializer list                          \
   * \param[in] values: values                                            \
   */                                                                     \
  template <typename ValueType2,                                          \
            typename std::enable_if<                                      \
                isAssignableTo<ValueType2, typename X::value_type>(),     \
                bool>::type = true>                                       \
  constexpr X(const std::initializer_list<ValueType2>& values) noexcept   \
      : Y(values) {}                                                      \
  /*!                                                                     \
   * \brief copy constructor from an object assignable the X class.       \
   * \param[in] src: source                                               \
   *                                                                      \
   * This is mostly used by expression objects and views.                 \
   */                                                                     \
  template <typename OtherArray,                                          \
            typename std::enable_if<((isAssignableTo<OtherArray, X>()) && \
                                     (!std::is_same_v<OtherArray, X>)),   \
                                    bool>::type = true>                   \
  constexpr X(const OtherArray& src) noexcept : Y(src) {}                 \
  /*!                                                                     \
   * \brief Default Constructor.                                          \
   * \param const base_type<T>*                                           \
   * const, pointer to a tabular used to initialise the components        \
   * of the stensor. This tabular is left unchanged.                      \
   */                                                                     \
  template <                                                              \
      typename InputIterator,                                             \
      std::enable_if_t<std::is_same_v<typename std::iterator_traits<      \
                                          InputIterator>::value_type,     \
                                      base_type<typename X::value_type>>, \
                       bool> = true>                                      \
  constexpr explicit X(const InputIterator p) : Y(p) {}                   \
  /* inheriting GenericFixedSizeArray' assignement operators */           \
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
    //! \brief default constructor
    constexpr GenericFixedSizeArray() noexcept;
    //! \brief copy constructor
    constexpr GenericFixedSizeArray(const GenericFixedSizeArray&) noexcept;
    //! \brief move constructor
    constexpr GenericFixedSizeArray(GenericFixedSizeArray&&) noexcept;
    // exposing assignement operators from MutableFixedSizeArrayBase
    /*!
     * \brief constructor from a value
     * \param[in] value: value used to initialize the array
     */
    template <typename ValueType,
              typename std::enable_if<
                  isAssignableTo<ValueType,
                                 typename GenericFixedSizeArray::value_type>(),
                  bool>::type = true>
    constexpr explicit GenericFixedSizeArray(const ValueType&) noexcept;
    /*!
     * \brief constructor from an initializer list
     * \param[in] values: values
     */
    template <typename ValueType2,
              typename std::enable_if<
                  isAssignableTo<ValueType2,
                                 typename GenericFixedSizeArray::value_type>(),
                  bool>::type = true>
    constexpr GenericFixedSizeArray(
        const std::initializer_list<ValueType2>& values) noexcept;
    /*!
     * \brief Default Constructor.
     * \param const base_type<T>*
     * const, pointer to a tabular used to initialise the components
     * of the stensor. This tabular is left unchanged.
     */
    template <typename InputIterator,
              std::enable_if_t<
                  std::is_same_v<
                      typename std::iterator_traits<InputIterator>::value_type,
                      base_type<typename GenericFixedSizeArray::value_type>>,
                  bool> = true>
    constexpr explicit GenericFixedSizeArray(const InputIterator);
    /*!
     * \brief copy constructor from an object assignable to the `Child` class.
     * \param[in] src: source
     *
     * This is mostly used by expression objects and views.
     */
    template <typename OtherArray,
              typename std::enable_if<((isAssignableTo<OtherArray, Child>()) &&
                                       (!std::is_same_v<OtherArray, Child>)),
                                      bool>::type = true>
    constexpr GenericFixedSizeArray(const OtherArray&) noexcept;
    /*!
     * \return the physical size used by the underlying array. This size must
     * be greater than than the logical number of elements contained in the
     * array which is returned by `IndexingPolicy::size`.
     */
    constexpr typename GenericFixedSizeArray::size_type getContainerSize()
        const noexcept;
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
    constexpr std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&>
    operator=(const OtherArray&) noexcept;
    //
    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&>
    operator+=(const OtherArray&) noexcept;
    //
    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&>
    operator-=(const OtherArray&) noexcept;
    //
    template <typename ValueType2>
    constexpr std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<ValueType2,
                                  typename GenericFixedSizeArray::value_type,
                                  OpMult>,
            typename GenericFixedSizeArray::value_type>(),
        Child&>
    operator*=(const ValueType2&) noexcept;
    //
    template <typename ValueType2>
    constexpr std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<typename GenericFixedSizeArray::value_type,
                                  ValueType2,
                                  OpDiv>,
            typename GenericFixedSizeArray::value_type>(),
        Child&>
    operator/=(const ValueType2&) noexcept;
    //! \return a pointer to the underlying array serving as element storage.
    constexpr typename GenericFixedSizeArray::pointer data() noexcept;
    //! \return a pointer to the underlying array serving as element storage.
    constexpr typename GenericFixedSizeArray::const_pointer data()
        const noexcept;

   protected:
    //! \brief assignement operator
    constexpr GenericFixedSizeArray& operator=(
        const GenericFixedSizeArray&) noexcept;
    //! \brief move assigment
    constexpr GenericFixedSizeArray& operator=(
        GenericFixedSizeArray&&) noexcept;
    //! \brief values holded
    typename ArrayPolicy::value_type v[N];
  };

}  // end of namespace tfel::math

#include "TFEL/Math/Array/GenericFixedSizeArray.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_HXX */
