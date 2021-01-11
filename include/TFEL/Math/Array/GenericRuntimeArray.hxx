/*!
 * \file  include/TFEL/Math/Array/GenericRuntimeArray.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_HXX
#define LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_HXX

#include <vector>
#include <type_traits>
#include <initializer_list>
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/MutableRuntimeArrayBase.hxx"

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
    //     template <typename OtherArray,
    //               typename std::enable_if<((isAssignableTo<OtherArray,
    //               Child>()) &&
    //                                        (!std::is_same_v<OtherArray,
    //                                        Child>)),
    //                                       bool>::type = true>
    //      explicit GenericRuntimeArray(const OtherArray&);
    //! \return a pointer to the underlying array serving as element storage.
    typename GenericRuntimeArray::pointer data() noexcept;
    //! \return a pointer to the underlying array serving as element storage.
    typename GenericRuntimeArray::const_pointer data() const noexcept;
    /*!
     * \return the physical size used by the underlying array. This size must
     * be greater than than the logical number of elements contained in the
     * array which is returned by `IndexingPolicy::size`.
     */
    typename GenericRuntimeArray::size_type getContainer() const noexcept;
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

   protected:
    //! \brief assignement operator
    GenericRuntimeArray& operator=(const GenericRuntimeArray&);
    //! \brief move assigment
    GenericRuntimeArray& operator=(GenericRuntimeArray&&);

   private:
    //! \brief values holded
    std::vector<typename ArrayPolicy::value_type> data_values;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/Array/GenericRuntimeArray.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_HXX */
