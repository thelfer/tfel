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
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Metaprogramming/IsSubClassOf.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"

namespace tfel {

  namespace meta {

    /*!
     * Node of TypeList
     * \param T : current type
     * \param U : next type
     */
    template <typename T, typename U>
    struct TFEL_VISIBILITY_LOCAL TLNode : public TL {
      TFEL_STATIC_ASSERT((IsSubClassOf<U, TL>::cond));
      /*!
       * Current element.
       */
      typedef T Current;
      /*!
       * Next Node.
       */
      typedef U Next;
    };

    /*!
     * A special type to represent the end of a typelist
     * \param T : current type
     * \param U : next type
     */
    struct TFEL_VISIBILITY_LOCAL TLE : public TL {};

    template <typename Tlist, template <typename> class Transform>
    struct TFEL_VISIBILITY_LOCAL TLTransform {
      TFEL_STATIC_ASSERT((IsSubClassOf<Tlist, TL>::cond));
      typedef TLNode<
          typename Transform<typename Tlist::Current>::type,
          typename TLTransform<typename Tlist::Next, Transform>::type>
          type;
    };

    /*!
     * Partial Specialisation to end the recursion.
     * \author Thomas Helfer
     * \date   30 August 2006
     */
    template <template <typename> class Transform>
    struct TFEL_VISIBILITY_LOCAL TLTransform<TLE, Transform> {
      typedef TLE type;
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
          (std::is_same<T, typename U::Current>::value ? 1 : 0) +
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

    template <typename T, typename List>
    struct TFEL_VISIBILITY_LOCAL TLFindEltPos {
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      TFEL_STATIC_ASSERT((TLCountNbrOfT<T, List>::value == 1));
      /*!
       * The result of the metafunction.
       */
      static constexpr unsigned int value =
          tfel::meta::internals::TLFindEltPos_<T, List>::value;
    };

    template <unsigned int N>
    struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt<TLE, N> {
      typedef TLE type;
    };

    template <typename List>
    struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt<List, 0u> {
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      typedef List type;
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt<TLE, 0u> {
      typedef TLE type;
    };

    template <typename List, unsigned int N>
    struct TFEL_VISIBILITY_LOCAL TLRemoveNthFirstElt {
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      typedef
          typename TLRemoveNthFirstElt<typename List::Next, N - 1>::type type;
    };

    /*!
     * Partial specialisation of TLFindNthElt to end the recursion
     */
    template <unsigned int N>
    struct TFEL_VISIBILITY_LOCAL TLFindNthElt<TLE, N> {
      /*!
       * The result of the metafunction.
       */
      typedef TLE type;
    };

    /*!
     * Partial specialisation of TLFindNthElt to end the recursion
     */
    template <typename T>
    struct TFEL_VISIBILITY_LOCAL TLFindNthElt<T, 0u> {
      TFEL_STATIC_ASSERT((IsSubClassOf<T, TL>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef typename T::Current type;
    };

    /*!
     * Partial specialisation of TLFindNthElt to end the recursion
     */
    template <>
    struct TFEL_VISIBILITY_LOCAL TLFindNthElt<TLE, 0u> {
      /*!
       * The result of the metafunction.
       */
      typedef TLE type;
    };

    template <typename T, unsigned int N>
    struct TFEL_VISIBILITY_LOCAL TLFindNthElt {
      TFEL_STATIC_ASSERT((IsSubClassOf<T, TL>::cond));
      /*!
       * The result of the metafunction. Return the Nth element of the T or TLE
       * if T contains less than N elements.
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

    template <typename List, typename T>
    struct TFEL_VISIBILITY_LOCAL TLPrepend {
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef TLNode<T, List> type;
    };

    template <typename T>
    struct TFEL_VISIBILITY_LOCAL TLAppend<TLE, T> {
      /*!
       * The result of the metafunction.
       */
      typedef TLNode<T, TLE> type;
    };

    template <typename List, typename T>
    struct TFEL_VISIBILITY_LOCAL TLAppend {
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef TLNode<typename List::Current,
                     typename TLAppend<typename List::Next, T>::type>
          type;
    };

    template <typename Second>
    struct TFEL_VISIBILITY_LOCAL TLConcatenate<TLE, Second> {
      TFEL_STATIC_ASSERT((IsSubClassOf<Second, TL>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef Second type;
    };

    template <typename First, typename Second>
    struct TLConcatenate {
     private:
      typedef typename TLConcatenate<typename First::Next, Second>::type N_;

     public:
      TFEL_STATIC_ASSERT((IsSubClassOf<First, TL>::cond));
      TFEL_STATIC_ASSERT((IsSubClassOf<Second, TL>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef TLNode<typename First::Current, N_> type;
    };

    template <typename BaseType>
    struct TFEL_VISIBILITY_LOCAL TLExtractSubClassesOf<TLE, BaseType> {
      /*!
       * The result of the metafunction.
       */
      typedef TLE type;
    };

    template <typename List, typename BaseType>
    struct TFEL_VISIBILITY_LOCAL TLExtractSubClassesOf {
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef typename std::conditional<
          IsSubClassOf<typename List::Current, BaseType>::cond,
          TLNode<typename List::Current,
                 typename TLExtractSubClassesOf<typename List::Next,
                                                BaseType>::type>,
          typename TLExtractSubClassesOf<typename List::Next,
                                         BaseType>::type>::type type;
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL TLUnique<TLE> {
      /*!
       * The result of the metafunction.
       */
      typedef TLE type;
    };

    template <typename List>
    struct TLUnique {
     private:
      typedef typename List::Next Next;
      typedef typename List::Current Current;
      typedef typename TLUnique<Next>::type UniqueNext;
      typedef typename TLPrepend<UniqueNext, Current>::type Choice;
      static constexpr bool unique = TLElementIsUnique<Current, List>::cond;

     public:
      /*!
       * The result of the metafunction.
       */
      typedef typename std::conditional<unique, Choice, UniqueNext>::type type;
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL TLContainsInvalidType<TLE> {
      /*!
       * The result of the metafunction.
       */
      static constexpr bool cond = false;
    };

    template <typename TList>
    struct TLContainsInvalidType {
     private:
      //! a simple alias.
      typedef typename TList::Current Current;
      //! a simple alias.
      typedef typename TList::Next Next;
      //! check if the current type is invalid.
      static constexpr bool cond1 = tfel::typetraits::IsInvalid<Current>::cond;
      //! check if all the next types in the TypeLis are invalid.
      static constexpr bool cond2 = TLContainsInvalidType<Next>::cond;

     public:
      //! the result.
      static constexpr bool cond = cond1 || cond2;
    };

    template <typename List>
    class TLMaxSize {
      //! a simple alias
      typedef typename List::Current Current;
      //! a simple alias
      typedef typename List::Next Next;
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

    template <typename List>
    class TLMaxAlign {
      //! a simple alias
      typedef typename List::Current Current;
      //! a simple alias
      typedef typename List::Next Next;
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

    template <typename List, size_t size>
    class TLComputeAlignBound {
      //! a simple alias
      typedef typename List::Current Current;
      //! a simple alias
      typedef typename List::Next Next;
      //! the result of the algorithm for next types of the TL.
      typedef typename TLComputeAlignBound<Next, size>::type TailResult;

     public:
      //! the result.
      typedef typename std::conditional<(sizeof(Current) <= size),
                                        TLNode<Current, TailResult>,
                                        TailResult>::type type;
    };

    //! Partial specialisation to end the recursion.
    /*
     * \return type, the result.
     */
    template <size_t size>
    struct TFEL_VISIBILITY_LOCAL TLComputeAlignBound<TLE, size> {
      //! the result.
      typedef TLE type;
    };

    namespace internals {

      template <unsigned short N>
      struct TFEL_VISIBILITY_LOCAL IntegerValue {
        static constexpr unsigned short value = N;
      };  // end of struct IntegerValue

      template <unsigned short N, typename T, typename List>
      struct TFEL_VISIBILITY_LOCAL TLPosition {
        typedef typename std::conditional<
            std::is_same<T, typename List::Current>::value,
            IntegerValue<N>,
            TLPosition<N + 1, T, typename List::Next>>::type Select;
        static constexpr unsigned short value = Select::value;

       private:
        TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
      };  // end of TLPosition

      template <unsigned short N, typename T>
      struct TLPosition<N, T, TLE> {
        static constexpr unsigned short value = N;
      };  // end of TLPosition

    }  // end of namespace internals

    template <typename T, typename List>
    struct TFEL_VISIBILITY_LOCAL TLPosition {
      static constexpr unsigned short value =
          tfel::meta::internals::TLPosition<0u, T, List>::value;

     private:
      TFEL_STATIC_ASSERT((IsSubClassOf<List, TL>::cond));
    };

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_META_TYPELIST_IXX */
