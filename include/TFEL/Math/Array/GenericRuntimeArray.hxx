/*!
 * \file  include/TFEL/Math/Array/GenericRuntimeArray.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_HXX
#define LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_HXX

#include <vector>
#include <type_traits>
#include <initializer_list>
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/MutableRuntimeArrayBase.hxx"

#define TFEL_MATH_RUNTIME_ARRAY_DEFAULT_METHODS(X, Y)                          \
  /*! \brief default constructor */                                            \
  X() = default;                                                               \
  /*! \brief move constructor */                                               \
  X(X&&) = default;                                                            \
  /*! \brief copy constructor */                                               \
  X(const X&) = default;                                                       \
  /*! \brief move assignement */                                               \
  X& operator=(X&&) = default;                                                 \
  /*! \brief standard assignement */                                           \
  X& operator=(const X&) = default;                                            \
  /*!                                                                          \
   * \brief constructor from a value                                           \
   * \param[in] value: value used to initialize the array                      \
   */                                                                          \
  template <typename ValueType2>                                               \
  explicit X(const ValueType2& value) requires(                                \
      isAssignableTo<ValueType2, typename X::value_type>())                    \
      : Y(value) {}                                                            \
  /*!                                                                          \
   * \brief constructor from an initializer list                               \
   * \param[in] values: values                                                 \
   */                                                                          \
  X(const std::initializer_list<typename X::value_type>& values)               \
      : Y(values) {}                                                           \
  /*!                                                                          \
   * \brief constructor from an initializer list                               \
   * \param[in] values: values                                                 \
   */                                                                          \
  template <typename ValueType2>                                               \
  X(const std::initializer_list<ValueType2>& values)                           \
  requires(isAssignableTo<ValueType2, typename X::value_type>())               \
      : Y(values) {}                                                           \
  /*!                                                                          \
   * \brief copy constructor from an object assignable the X class.            \
   * \param[in] src: source                                                    \
   *                                                                           \
   * This is mostly used by expression objects and views.                      \
   */                                                                          \
  template <typename OtherArray>                                               \
  X(const OtherArray& src)                                                     \
  requires((isAssignableTo<OtherArray, X>()) &&                                \
           (!std::is_same_v<OtherArray, X>))                                   \
      : Y(src) {}                                                              \
  /*!                                                                          \
   * \brief Default Constructor.                                               \
   * \param const base_type<T>*                                                \
   * const, pointer to a tabular used to initialise the components             \
   * of the stensor. This tabular is left unchanged.                           \
   */                                                                          \
  template <typename InputIterator>                                            \
  explicit X(const InputIterator p) requires(                                  \
      std::is_same_v<typename std::iterator_traits<InputIterator>::value_type, \
                     base_type<typename X::value_type>>)                       \
      : Y(p) {}                                                                \
  /* inheriting GenericFixedSizeArray' assignement operators */                \
  using Y::operator=

namespace tfel::math {

  /*!
   * \brief a fixed size array class with storage on the stack.
   * \tparam N: container size
   * \tparam ArrayPolicy: array policy
   */
  template <typename Child, typename ArrayPolicy>
  struct GenericRuntimeArray
      : MutableRuntimeArrayBase<GenericRuntimeArray<Child, ArrayPolicy>,
                                ArrayPolicy> {
    //! \brief a simple alias
    using Container = std::vector<typename ArrayPolicy::value_type>;
    //! \brief a simple alias
    using value_type = typename MutableRuntimeArrayBase<
        GenericRuntimeArray<Child, ArrayPolicy>,
        ArrayPolicy>::value_type;
    //! \brief default constructor
    GenericRuntimeArray() = default;
    //! \brief default constructor
    GenericRuntimeArray(const typename ArrayPolicy::IndexingPolicy&);
    //! \brief copy constructor
    GenericRuntimeArray(const GenericRuntimeArray&) = default;
    //! \brief move constructor
    GenericRuntimeArray(GenericRuntimeArray&&) = default;
    // exposing assignement operators from MutableRuntimeArrayBase
    /*!
     * \brief constructor from a value
     * \param[in] value: value used to initialize the array
     */
    template <typename ValueType>
    explicit GenericRuntimeArray(
        const typename ArrayPolicy::IndexingPolicy&,
        const ValueType&) requires(isAssignableTo<ValueType,
                                                  value_type>());
    /*!
     * \brief constructor from an initializer list
     * \param[in] values: values
     */
    template <typename ValueType>
    GenericRuntimeArray(
        const typename GenericRuntimeArray<Child, ArrayPolicy>::indexing_policy,
        const std::initializer_list<
            ValueType>&) requires((isAssignableTo<ValueType,
                                                  value_type>()) &&
                                  (ArrayPolicy::IndexingPolicy::arity == 1) &&
                                  (ArrayPolicy::IndexingPolicy::
                                       areDataContiguous));
    /*!
     * \brief constructor from an initializer list
     * \param[in] values: values
     */
    template <typename ValueType>
    GenericRuntimeArray(const std::initializer_list<ValueType>&) requires(
        (isAssignableTo<
            ValueType, value_type>()) &&
        (ArrayPolicy::IndexingPolicy::arity == 1) &&
        (ArrayPolicy::IndexingPolicy::areDataContiguous));
    /*!
     * \brief copy constructor from an object assignable to the `Child` class.
     * \param[in] src: source
     *
     * This is mostly used by expression objects and views.
     */
    template <typename OtherArray>
    explicit GenericRuntimeArray(const OtherArray&) requires(
        (isAssignableTo<OtherArray, Child>()) &&
        (!std::is_same_v<OtherArray, Child>));
    //! \return a pointer to the underlying array serving as element storage.
    typename GenericRuntimeArray<Child, ArrayPolicy>::pointer data() noexcept;
    //! \return a pointer to the underlying array serving as element storage.
    typename GenericRuntimeArray<Child, ArrayPolicy>::const_pointer data()
        const noexcept;
    //! \brief resize the array
    void resize(const typename ArrayPolicy::IndexingPolicy&);
    /*!
     * \return the physical size used by the underlying array. This size must
     * be greater than than the logical number of elements contained in the
     * array which is returned by `IndexingPolicy::size`.
     */
    typename GenericRuntimeArray<Child, ArrayPolicy>::size_type
    getContainerSize() const noexcept;
    // inheriting MutableRuntimeArrayBase' assignement operator
    using MutableRuntimeArrayBase<GenericRuntimeArray<Child, ArrayPolicy>,
                                  ArrayPolicy>::operator=;

    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    Child& operator=(const OtherArray&) requires(
        isAssignableTo<OtherArray, Child>());
    //
    template <typename OtherArray>
    Child& operator+=(const OtherArray&) requires(
        isAssignableTo<OtherArray, Child>());
    //
    template <typename OtherArray>
    Child& operator-=(const OtherArray&) requires(
        isAssignableTo<OtherArray, Child>());
    //
    template <typename ValueType2>
    Child& operator*=(const ValueType2&) noexcept requires(
        isAssignableTo<
            BinaryOperationResult<
                ValueType2, value_type, OpMult>,
            value_type>());
    //
    template <typename ValueType2>
    Child& operator/=(const ValueType2&) noexcept requires(
        isAssignableTo<
            BinaryOperationResult<
                value_type,
                ValueType2,
                OpDiv>,
            value_type>());
    //
    bool empty() const;
    //
    void clear();
    //
    void shrink_to_fit();

   protected:
    //! \brief assignement operator
    GenericRuntimeArray& operator=(const GenericRuntimeArray&);
    //! \brief move assigment
    GenericRuntimeArray& operator=(GenericRuntimeArray&&);

   protected:
    //! \brief values holded
    Container data_values;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/Array/GenericRuntimeArray.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_HXX */
