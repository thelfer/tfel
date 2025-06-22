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

#define TFEL_MATH_RUNTIME_ARRAY_DEFAULT_METHODS(X, Y)                     \
  /*! \brief default constructor */                                       \
  X() = default;                                                          \
  /*! \brief move constructor */                                          \
  X(X&&) = default;                                                       \
  /*! \brief copy constructor */                                          \
  X(const X&) = default;                                                  \
  /*! \brief move assignement */                                          \
  X& operator=(X&&) = default;                                            \
  /*! \brief standard assignement */                                      \
  X& operator=(const X&) = default;                                       \
  /*!                                                                     \
   * \brief constructor from a value                                      \
   * \param[in] value: value used to initialize the array                 \
   */                                                                     \
  template <typename ValueType2,                                          \
            typename std::enable_if<                                      \
                isAssignableTo<ValueType2, typename X::value_type>(),     \
                bool>::type = true>                                       \
  explicit X(const ValueType2& value) : Y(value) {}                       \
  /*!                                                                     \
   * \brief constructor from an initializer list                          \
   * \param[in] values: values                                            \
   */                                                                     \
  template <typename ValueType2,                                          \
            typename std::enable_if<                                      \
                isAssignableTo<ValueType2, typename X::value_type>(),     \
                bool>::type = true>                                       \
  X(const std::initializer_list<ValueType2>& values) : Y(values) {}       \
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
  X(const OtherArray& src) : Y(src) {}                                    \
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
  explicit X(const InputIterator p) : Y(p) {}                             \
  /* inheriting GenericFixedSizeArray' assignement operators */           \
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
    template <typename ValueType,
              typename std::enable_if<
                  isAssignableTo<ValueType,
                                 typename GenericRuntimeArray::value_type>(),
                  bool>::type = true>
    explicit GenericRuntimeArray(const typename ArrayPolicy::IndexingPolicy&,
                                 const ValueType&);
    /*!
     * \brief constructor from an initializer list
     * \param[in] values: values
     */
    template <
        typename ValueType,
        typename std::enable_if<
            ((isAssignableTo<ValueType,
                             typename GenericRuntimeArray::value_type>()) &&
             (ArrayPolicy::IndexingPolicy::arity == 1) &&
             (ArrayPolicy::IndexingPolicy::areDataContiguous)),
            bool>::type = true>
    GenericRuntimeArray(const typename GenericRuntimeArray::indexing_policy,
                        const std::initializer_list<ValueType>&);
    /*!
     * \brief constructor from an initializer list
     * \param[in] values: values
     */
    template <
        typename ValueType,
        typename std::enable_if<
            ((isAssignableTo<ValueType,
                             typename GenericRuntimeArray::value_type>()) &&
             (ArrayPolicy::IndexingPolicy::arity == 1) &&
             (ArrayPolicy::IndexingPolicy::areDataContiguous)),
            bool>::type = true>
    GenericRuntimeArray(const std::initializer_list<ValueType>&);
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
    explicit GenericRuntimeArray(const OtherArray&);
    //! \return a pointer to the underlying array serving as element storage.
    typename GenericRuntimeArray::pointer data() noexcept;
    //! \return a pointer to the underlying array serving as element storage.
    typename GenericRuntimeArray::const_pointer data() const noexcept;
    //! \brief resize the array
    void resize(const typename ArrayPolicy::IndexingPolicy&);
    /*!
     * \return the physical size used by the underlying array. This size must
     * be greater than than the logical number of elements contained in the
     * array which is returned by `IndexingPolicy::size`.
     */
    typename GenericRuntimeArray::size_type getContainerSize() const noexcept;
    // inheriting MutableRuntimeArrayBase' assignement operator
    using MutableRuntimeArrayBase<GenericRuntimeArray<Child, ArrayPolicy>,
                                  ArrayPolicy>::operator=;

    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&> operator=(
        const OtherArray&);
    //
    template <typename OtherArray>
    std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&> operator+=(
        const OtherArray&);
    //
    template <typename OtherArray>
    std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&> operator-=(
        const OtherArray&);
    //
    template <typename ValueType2>
    std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<ValueType2,
                                  typename GenericRuntimeArray::value_type,
                                  OpMult>,
            typename GenericRuntimeArray::value_type>(),
        Child&>
    operator*=(const ValueType2&) noexcept;
    //
    template <typename ValueType2>
    std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<typename GenericRuntimeArray::value_type,
                                  ValueType2,
                                  OpDiv>,
            typename GenericRuntimeArray::value_type>(),
        Child&>
    operator/=(const ValueType2&) noexcept;
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
