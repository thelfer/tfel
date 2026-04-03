/*!
 * \file   include/TFEL/Metaprogramming/TypeList.ixx
 *
 * \brief
 * \author Thomas Helfer
 * \date   07 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_META_TYPELIST_IXX
#define LIB_TFEL_META_TYPELIST_IXX

#include <type_traits>
#include "TFEL/Metaprogramming/Forward/TypeList.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"

namespace tfel::meta {

  /*!
   * Node of TypeList
   * \param T : current type
   * \param U : next type
   */
  template <typename T, typename U>
  struct TFEL_VISIBILITY_LOCAL TLNode : public TL {
    static_assert(std::is_base_of_v<TL, U>);
    //! \brief current element
    using Current = T;
    //! \brief next node
    using Next = U;
  };

  /*!
   * A special type to represent the end of a typelist
   * \param T : current type
   * \param U : next type
   */
  struct TFEL_VISIBILITY_LOCAL TLE : public TL {};

  template <typename TList, template <typename> class Transform>
  struct TFEL_VISIBILITY_LOCAL TLTransform {
    static_assert(std::is_base_of_v<TL, TList>);
    //! \brief result of the metafunction
    using type =
        TLNode<typename Transform<typename TList::Current>::type,
               typename TLTransform<typename TList::Next, Transform>::type>;
  };

  /*!
   * Partial Specialisation to end the recursion.
   * \author Thomas Helfer
   * \date   30 August 2006
   */
  template <template <typename> class Transform>
  struct TFEL_VISIBILITY_LOCAL TLTransform<TLE, Transform> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  template <typename T>
  struct TFEL_VISIBILITY_LOCAL TLSize {
    static constexpr unsigned int value = 1 + TLSize<typename T::Next>::value;
  };

  /*!
   * Partial Specialisation to end the recursion.
   * \author Thomas Helfer
   * \date   30 August 2006
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL TLSize<TLE> {
    static constexpr unsigned int value = 0;
  };

  /*!
   * Partial Specialisation to end the recursion.
   * \author Thomas Helfer
   * \date   30 August 2006
   */
  template <typename T>
  struct TFEL_VISIBILITY_LOCAL TLCountNbrOfT<T, TLE> {
    static constexpr unsigned int value = 0;
  };

  template <typename T, typename U>
  struct TFEL_VISIBILITY_LOCAL TLCountNbrOfT {
    static constexpr unsigned int value =
        (std::is_same_v<T, typename U::Current> ? 1 : 0) +
        TLCountNbrOfT<T, typename U::Next>::value;
  };

  namespace internals {

    /*!
     * \brief  An helper class for the TLFindEltPos metafunction.
     * \author Thomas Helfer
     * \date   30 August 2006
     */
    template <typename T, typename List>
    struct TFEL_VISIBILITY_LOCAL TLFindEltPos_ {
      static constexpr unsigned int value =
          (TLCountNbrOfT<T, typename List::Next>::value == 1)
              ? 1 + TLFindEltPos_<T, typename List::Next>::value
              : 0u;
    };

    /*!
     * Partial Specialisation to end the recursion.
     * \author Thomas Helfer
     * \date   30 August 2006
     */
    template <typename T>
    struct TFEL_VISIBILITY_LOCAL TLFindEltPos_<T, TLE> {
      static constexpr unsigned int value = 0u;
    };

  }  // end of namespace internals

  template <typename T, typename TList>
  struct TFEL_VISIBILITY_LOCAL TLFindEltPos {
    static_assert(std::is_base_of_v<TL, TList>);
    static_assert(TLCountNbrOfT<T, TList>::value == 1);
    //! \brief result of the metafunction
    static constexpr unsigned int value =
        tfel::meta::internals::TLFindEltPos_<T, TList>::value;
  };

  template <unsigned int N>
  struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt<TLE, N> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  template <typename TList>
  struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt<TList, 0u> {
    static_assert(std::is_base_of_v<TL, TList>);
    //! \brief result of the metafunction
    typedef TList type;
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt<TLE, 0u> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  template <typename TList, unsigned int N>
  struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt {
    static_assert(std::is_base_of_v<TL, TList>);
    //! \brief result of the metafunction
    typedef
        typename TLRemoveNthFirstElt<typename TList::Next, N - 1>::type type;
  };

  /*!
   * Partial specialisation of TLFindNthElt to end the recursion
   */
  template <unsigned int N>
  struct TFEL_VISIBILITY_LOCAL TLFindNthElt<TLE, N> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  /*!
   * Partial specialisation of TLFindNthElt to end the recursion
   */
  template <typename T>
  struct TFEL_VISIBILITY_LOCAL TLFindNthElt<T, 0u> {
    static_assert(std::is_base_of_v<TL, T>);
    //! \brief result of the metafunction
    typedef typename T::Current type;
  };

  /*!
   * Partial specialisation of TLFindNthElt to end the recursion
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL TLFindNthElt<TLE, 0u> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  template <typename T, unsigned int N>
  struct TFEL_VISIBILITY_LOCAL TLFindNthElt {
    static_assert(std::is_base_of_v<TL, T>);
    /*!
     * The result of the metafunction. Return the Nth element of the T or TLE if
     * T contains less than N elements.
     */
    typedef typename TLFindNthElt<typename T::Next, N - 1>::type type;
  };

  template <typename T, typename U>
  struct TFEL_VISIBILITY_LOCAL TLElementIsUnique {
    /*!
     * The result of the metafunction. True if T is unique in U.
     */
    static constexpr bool cond = TLCountNbrOfT<T, U>::value == 1;
  };

  namespace internals {

    /*!
     * \brief  An helper class for the TLElementsAreUnique metafunction.
     * \author Thomas Helfer
     * \date   30 August 2006
     */
    template <typename T, typename U>
    struct TFEL_VISIBILITY_LOCAL TLElementsAreUniqueImpl {
      static constexpr bool c1 =
          tfel::meta::TLElementIsUnique<typename T::Current, U>::cond;
      static constexpr bool c2 =
          TLElementsAreUniqueImpl<typename T::Next, U>::cond;
      static constexpr bool cond = c1 && c2;
    };

    /*!
     * \brief Partial specialisation of TLElementsAreUniqueImpl
     */
    template <typename U>
    struct TFEL_VISIBILITY_LOCAL TLElementsAreUniqueImpl<TLE, U> {
      static constexpr bool cond = true;
    };

  }  // end of namespace internals

  template <typename T>
  struct TFEL_VISIBILITY_LOCAL TLElementsAreUnique {
    /*!
     * The result of the metafunction. True if all elements in T are unique.
     */
    static constexpr bool cond =
        tfel::meta::internals::TLElementsAreUniqueImpl<T, T>::cond;
  };

  template <typename TList, typename T>
  struct TFEL_VISIBILITY_LOCAL TLPrepend {
    static_assert(std::is_base_of_v<TL, TList>);
    //! \brief result of the metafunction
    using type = TLNode<T, TList>;
  };

  template <typename T>
  struct TFEL_VISIBILITY_LOCAL TLAppend<TLE, T> {
    //! \brief result of the metafunction
    using type = TLNode<T, TLE>;
  };

  template <typename TList, typename T>
  struct TFEL_VISIBILITY_LOCAL TLAppend {
    static_assert(std::is_base_of_v<TL, TList>);
    //! \brief result of the metafunction
    using type = TLNode<typename TList::Current,
                        typename TLAppend<typename TList::Next, T>::type>;
  };

  template <typename Second>
  struct TFEL_VISIBILITY_LOCAL TLConcatenate<TLE, Second> {
    static_assert(std::is_base_of_v<TL, Second>);
    //! \brief result of the metafunction
    using type = Second;
  };

  template <typename First, typename Second>
  struct TLConcatenate {
    static_assert(std::is_base_of_v<TL, First>);
    static_assert(std::is_base_of_v<TL, Second>);
    //! \brief result of the metafunction
    using type =
        TLNode<typename First::Current,
               typename TLConcatenate<typename First::Next, Second>::type>;
  };

  template <typename BaseType>
  struct TFEL_VISIBILITY_LOCAL TLExtractSubClassesOf<TLE, BaseType> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  template <typename TList, typename BaseType>
  struct TFEL_VISIBILITY_LOCAL TLExtractSubClassesOf {
    static_assert(std::is_base_of_v<TL, TList>);
    //! \brief the result of the metafunction.
    using type = std::conditional_t<
        std::is_base_of_v<BaseType, typename TList::Current>,
        TLNode<typename TList::Current,
               typename TLExtractSubClassesOf<typename TList::Next,
                                              BaseType>::type>,
        typename TLExtractSubClassesOf<typename TList::Next, BaseType>::type>;
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL TLUnique<TLE> {
    //! \brief the result of the metafunction.
    using type = TLE;
  };

  template <typename TList>
  struct TLUnique {
   private:
    typedef typename TList::Next Next;
    typedef typename TList::Current Current;
    typedef typename TLUnique<Next>::type UniqueNext;
    typedef typename TLPrepend<UniqueNext, Current>::type Choice;
    static constexpr bool unique = TLElementIsUnique<Current, TList>::cond;

   public:
    //! \brief the result of the metafunction.
    using type = std::conditional_t<unique, Choice, UniqueNext>;
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL TLContainsInvalidType<TLE> {
    //! \brief the result of the metafunction.
    static constexpr bool cond = false;
  };

  template <typename TList>
  struct TLContainsInvalidType {
   private:
    //! \brief a simple alias
    typedef typename TList::Current Current;
    //! \brief a simple alias
    typedef typename TList::Next Next;
    //! check if the current type is invalid.
    static constexpr bool cond1 = tfel::typetraits::IsInvalid<Current>::cond;
    //! check if all the next types in the TypeLis are invalid.
    static constexpr bool cond2 = TLContainsInvalidType<Next>::cond;

   public:
    //! the result.
    static constexpr bool cond = cond1 || cond2;
  };

  template <typename TList>
  class TLMaxSize {
    //! a simple alias
    typedef typename TList::Current Current;
    //! a simple alias
    typedef typename TList::Next Next;
    //! the result of the algorithm for the next types of the TL.
    static constexpr size_t nextValue = TLMaxSize<Next>::value;

   public:
    //! the result of the algorithm.
    static constexpr size_t value = sizeof(Current) > nextValue
                                        ? sizeof(Current)
                                        : nextValue;
  };  // end of struct TLMaxSize

  //! Partial specialisation to end the recursion.
  /*
   * \return cons size_t value, the result.
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL TLMaxSize<TLE> {
    //! the result
    static constexpr size_t value = 1;
  };  // end of struct TLMaxSize

  template <typename TList>
  class TLMaxAlign {
    //! a simple alias
    typedef typename TList::Current Current;
    //! a simple alias
    typedef typename TList::Next Next;
    //! the result of the algorithm for the next types of the TL.
    static constexpr size_t nextValue = TLMaxAlign<Next>::value;

   public:
    //! the result of the algorithm.
    static constexpr size_t value = alignof(Current) > nextValue
                                        ? alignof(Current)
                                        : nextValue;
  };  // end of struct TLMaxAlign

  //! Partial specialisation to end the recursion.
  /*
   * \return cons size_t value, the result.
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL TLMaxAlign<TLE> {
    //! the result
    static constexpr size_t value = alignof(char);
  };  // end of struct TLMaxAlign

  template <typename TList, size_t size>
  class TLComputeAlignBound {
    //! a simple alias
    typedef typename TList::Current Current;
    //! a simple alias
    typedef typename TList::Next Next;
    //! the result of the algorithm for next types of the TL.
    typedef typename TLComputeAlignBound<Next, size>::type TailResult;

   public:
    //! the result.
    using type = std::conditional_t<(sizeof(Current) <= size),
                                    TLNode<Current, TailResult>,
                                    TailResult>;
  };

  //! \brief artial specialisation to end the recursion.
  template <size_t size>
  struct TFEL_VISIBILITY_LOCAL TLComputeAlignBound<TLE, size> {
    //! \brief result of the metafunction
    using type = TLE;
  };

  namespace internals {

    template <unsigned short N>
    struct TFEL_VISIBILITY_LOCAL IntegerValue {
      static constexpr unsigned short value = N;
    };  // end of struct IntegerValue

    template <unsigned short N, typename T, typename TList>
    struct TFEL_VISIBILITY_LOCAL TLPosition {
      static_assert(std::is_base_of_v<TL, TList>);
      typedef typename std::conditional<
          std::is_same_v<T, typename TList::Current>,
          IntegerValue<N>,
          TLPosition<N + 1, T, typename TList::Next>>::type Select;
      static constexpr unsigned short value = Select::value;
    };  // end of TLPosition

    template <unsigned short N, typename T>
    struct TLPosition<N, T, TLE> {
      static constexpr unsigned short value = N;
    };  // end of TLPosition

  }  // end of namespace internals

  template <typename T, typename TList>
  struct TFEL_VISIBILITY_LOCAL TLPosition {
    static_assert(std::is_base_of_v<TL, TList>);
    static constexpr unsigned short value =
        tfel::meta::internals::TLPosition<0u, T, TList>::value;
  };

}  // namespace tfel::meta

#endif /* LIB_TFEL_META_TYPELIST_IXX */
