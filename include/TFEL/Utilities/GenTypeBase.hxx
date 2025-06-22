/*!
 * \file   include/TFEL/Utilities/GenTypeBase.hxx
 * \brief  This file implements the GenTypeBase class and some
 * internal classes used by the GenType class.
 * \see    TL
 * \author Helfer Thomas
 * \date   19 Apr. 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_GENTYPEBASE_H
#define LIB_TFEL_UTILITIES_GENTYPEBASE_H

#include <cmath>
#include <limits>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Metaprogramming/TypeList.hxx"
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeCastError.hxx"

namespace tfel {

  namespace utilities {

    namespace internals {

      //! a helper function for destroying a plain old data type.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeTrivialDestroy {
        //! the helper function.
        /*
         * \param void *const, a pointer (not used).
         */
        TFEL_INLINE static void exe(void *const) {}
      };

      //! an helper function to call the destructor of T.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeGenericDestroy {
        //! the helper function.
        /*
         * \param void *const, a pointer to a T object.
         */
        TFEL_INLINE static void exe(void *const p) {
          static_cast<void>(p);
          static_cast<T *>(p)->~T();
        }
      };

      //! an helper function to call the assignement operator of T.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeAssign {
        //! the helper function.
        /*
         * \param void *const, a pointer to a T-object.
         * \param const void *const, a pointer to a T-object.
         * \return const bool, the result of the comparison.
         */
        static void exe(void *const p, const void *const p2) {
          T &tmp = *(static_cast<T *>(p));
          const T &tmp2 = *(static_cast<const T *>(p2));
          tmp = tmp2;
        }
      };

      //! an helper function to call the copy constructor of T.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeCopy {
        //! the helper function.
        /*
         * \param void *const, a pointer to a T-object.
         * \param const void *const, a pointer to a T-object.
         */
        static void exe(void *const p, const void *const p2) {
          const T &tmp2 = *(static_cast<const T *>(p2));
          // placement new
          new (p) T(tmp2);
        }
      };

      //! an helper class to a add methods to a GenType for a specific type.
      /*
       * This class is based on the curiously recurring template
       * pattern for achieving this.
       * \param typename Child, the Child of this class.
       * \param typename T, the specific type.
       * \see GenTypeSpecialisation.ixx for examples.
       * \see http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename Child, typename T>
      struct GenTypeSpecializedAccessor {};

      //! an helper class to a add methods to a GenType.
      /*
       * This class is based on the curiously recurring template
       * pattern for achieving this.
       * \param typename Child, the Child of this class.
       * \param typename List, the List of types contained in the GenType.
       * \see GenTypeSpecialisation.ixx for examples.
       * \see http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename Child, typename List>
      class GenTypeSpecializedMethods
#ifndef DOXYGEN_SHOULD_SKIP_THIS
          : public GenTypeSpecializedMethods<Child, typename List::Next>,
            public GenTypeSpecializedAccessor<Child, typename List::Current>
#else  /* DOXYGEN_SHOULD_SKIP_THIS */
          public GenTypeSpecializedAccessor<Child, typename List::Current>
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
      {
        //! a simple alias.
        typedef typename List::Current Current;

       public:
        //! cast operator.
        TFEL_INLINE operator Current &() {
          return static_cast<Child *>(this)->template get<Current>();
        }
        //! cast operator (const version).
        TFEL_INLINE operator const Current &() const {
          return static_cast<const Child *>(this)->template get<Current>();
        }
      };

      //! a partial specialisation to end the recursion.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename Child>
      class GenTypeSpecializedMethods<Child, tfel::meta::TLE> {};

    }  // end of namespace internals

  }  // end of namespace utilities

}  // end of namespace tfel

#ifdef _MSC_VER
#include "TFEL/Utilities/GenTypeBase_MSC.hxx"
#else

namespace tfel {

  namespace utilities {

    namespace internals {

      //! An helper class to fill the runtime methods of a GenType.
      /*
       * \param unsigned short N, the current index.
       * \param typename List, a list of types.
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename List, typename... Types>
      struct TFEL_VISIBILITY_LOCAL GenTypeRunTimeMethods
#ifndef DOXYGEN_SHOULD_SKIP_THIS
          : public GenTypeRunTimeMethods<typename List::Next,
                                         Types...,
                                         typename List::Current>
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
      {
      };

      //! A partial specialisation to end the recursion.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename... Types>
      struct TFEL_VISIBILITY_LOCAL
          GenTypeRunTimeMethods<tfel::meta::TLE, Types...> {
        //! choose the function used to destroy the N-1 type of the typelist.
        template <typename T>
        using GenTypeDestroy = typename std::conditional<
            std::is_trivial<T>::value,
            GenTypeTrivialDestroy<T>,
            GenTypeGenericDestroy<T>>::type;  //! size of the typelise
        static constexpr const auto N = sizeof...(Types);
        //! a simple alias.
        using DestructorPtr = void (*)(void *const);
        //! a simple alias.
        using AssignOperatorPtr = void (*)(void *const, const void *const);
        //! a simple alias.
        using CopyConstructorPtr = void (*)(void *const, const void *const);
        /*!
         * \param[in] i: index
         * \return a pointer to a specific destructor
         */
        DestructorPtr get_destructor(const unsigned short i) {
          constexpr const DestructorPtr m[N] = {&GenTypeDestroy<Types>::exe...};
          return (i >= N) ? nullptr : m[i];
        }
        /*!
         * \param[in] i: index
         * \return a pointer to a copy constructor
         */
        CopyConstructorPtr get_copy_constructor(const unsigned short i) {
          constexpr const CopyConstructorPtr m[N] = {
              &GenTypeCopy<Types>::exe...};
          return (i >= N) ? nullptr : m[i];
        }
        /*!
         * \param[in] i: index
         * \return a pointer to an assignement operator
         */
        AssignOperatorPtr get_assignement_operator(const unsigned short i) {
          constexpr const AssignOperatorPtr m[N] = {
              &GenTypeAssign<Types>::exe...};
          return (i >= N) ? nullptr : m[i];
        }
      };

    }  // end of namespace internals

    //! The base class of GenType.
    /*
     * \param  typename List, the list of types contained in the GenType.
     * \author Helfer Thomas.
     * \date   20 Apr. 2007.
     */
    template <typename List>
    struct GenTypeBase
        : public tfel::utilities::internals::
              GenTypeSpecializedMethods<GenTypeBase<List>, List>,
          public tfel::utilities::internals::GenTypeRunTimeMethods<List> {
      /*!
       * \brief a simple wrapper around std::enable_if
       * \tparam T: tested type which must belong to List for the requirement to
       * hold true \tparam R: result
       */
      template <typename T, typename R = T>
      using requires = typename std::
          enable_if<tfel::meta::TLCountNbrOfT<T, List>::value == 1, R>::type;
      //! number of object that the GenType can hold
      static constexpr unsigned short ListSize =
          tfel::meta::TLSize<List>::value;
      //! Default constructor.
      TFEL_INLINE GenTypeBase() = default;
      /*!
       * intialize by value
       * \param[in] v: value
       */
      template <typename T1, requires<T1, bool> = true>
      TFEL_INLINE GenTypeBase(const T1 &v) {
        this->template set<T1>(v);
      }
      /*!
       * \brief copy constructor
       * \param src: the source
       */
      TFEL_INLINE GenTypeBase(const GenTypeBase &src) : index(src.index) {
        if (!this->empty()) {
          // create a new object by copy
          auto *tmp = reinterpret_cast<void *>(&buffer);
          auto *tmp2 = reinterpret_cast<const void *>(&(src.buffer));
          (*(this->get_copy_constructor(this->index)))(tmp, tmp2);
        }
      }
      /*!
       * \brief assignement operator
       * \param src: the right-hand side
       */
      TFEL_INLINE GenTypeBase &operator=(const GenTypeBase &src) {
        // check for self-assignement
        if (this != &src) {
          if (this->index == src.index) {
            if (src.index != ListSize) {
              // the two GenTypes hold the same objects
              // we then use the assignement operator.
              auto *tmp = reinterpret_cast<void *>(&buffer);
              auto *tmp2 = reinterpret_cast<const void *>(&(src.buffer));
              (*(this->get_assignement_operator(this->index)))(tmp, tmp2);
            } else {
              // src is not initialized,
              // we then destroy holded object
              if (!this->empty()) {
                auto *tmp = reinterpret_cast<void *>(&buffer);
                (*(this->get_destructor(this->index)))(tmp);
              }
              this->index = ListSize;
            }
          } else {
            // the two GenType have two differents objects.
            // we first destroy the previous object and create a new
            // one by copy.
            if (!this->empty()) {
              auto *tmp = reinterpret_cast<void *>(&buffer);
              (*(this->get_destructor(this->index)))(tmp);
            }
            this->index = src.index;
            if (!this->empty()) {
              auto *tmp = reinterpret_cast<void *>(&buffer);
              auto *tmp2 = reinterpret_cast<const void *>(&(src.buffer));
              (*(this->get_copy_constructor(this->index)))(tmp, tmp2);
            }
          }
        }
        return *this;
      }
      //! \return true if empty
      bool empty(void) const { return this->index == ListSize; }
      /*!
       * \brief copy a GenType (calls the assignement operator).
       * \param src: object to be copied
       */
      bool copy(const GenTypeBase &src) {
        this->operator=(src);
        return true;
      }
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE requires<T1, void> set(const T1 &src) {
        if (this->index == tfel::meta::TLFindEltPos<T1, List>::value) {
          // a silly trick to avoir a gcc warning
          union {
            storage_t *c;
            T1 *ptr;
          } ptr;
          // The GenType already holds an object of type T1
          ptr.c = &(this->buffer);
          *(ptr.ptr) = src;
        } else {
          // We create a new object of type T1 by calling the copy constructor
          this->template set_uninitialised<T1>();
          void *p = reinterpret_cast<void *>(&(this->buffer));
          // the magic of placement new...
          new (p) T1(src);
        }
      }
      //! assignement operator.
      /*
       * \param  const T1&, the value affected to the GenType.
       * \return GenTypeBase&, a reference to this.
       * \pre    T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE requires<T1, GenTypeBase &> operator=(const T1 &src) {
        this->template set<T1>(src);
        return *this;
      }
      template <typename T1>
      TFEL_INLINE requires<T1, bool> is() const {
        return this->index == tfel::meta::TLPosition<T1, List>::value;
      }  // end of Value::is
      //! get the value contained in the GenType (const version).
      /*
       * \return const T1&, the value affected to the GenType.
       * \pre    T1 must be a type that the GenType can hold.
       * \throw  GenTypeCastError, if the type contained in the GenType does
       * not match.
       */
      template <typename T1>
      TFEL_INLINE requires<T1, const T1 &> get(void) const {
        // a silly trick to avoir a gcc warning
        union {
          const storage_t *c;
          const T1 *ptr;
        } ptr;
        if (!this->template is<T1>()) {
          throw(GenTypeCastError());
        }
        ptr.c = &(this->buffer);
        return *(ptr.ptr);
      }
      //! get the value contained in the GenType.
      /*
       * \return T1&, the value affected to the GenType.
       * \pre    T1 must be a type that the GenType can hold.
       * \throw  GenTypeCastError, if the type contained in the GenType does not
       * match.
       */
      template <typename T1>
      TFEL_INLINE requires<T1, T1 &> get(void) {
        // a silly trick to avoir a gcc warning
        union {
          storage_t *c;
          T1 *ptr;
        } ptr;
        if (!this->template is<T1>()) {
          throw(GenTypeCastError());
        }
        ptr.c = &(this->buffer);
        return *(ptr.ptr);
      }
      /*!
       * \return the type index of the object
       */
      TFEL_INLINE unsigned short getTypeIndex(void) const {
        return this->index;
      }
      //! destructor
      TFEL_INLINE ~GenTypeBase() { this->clear(); }

     protected:
      //! clear the GenType
      TFEL_INLINE void clear() {
        if (!this->empty()) {
          // destroy the current object
          auto *tmp = reinterpret_cast<void *>(&buffer);
          (*(this->get_destructor(this->index)))(tmp);
        }
        this->index = ListSize;
      }
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE requires<T1, void> set_uninitialised(void) {
        this->clear();
        this->index = tfel::meta::TLFindEltPos<T1, List>::value;
      }
      //! container type
      typedef typename std::aligned_storage<
          tfel::meta::TLMaxSize<List>::value,
          tfel::meta::TLMaxAlign<List>::value>::type storage_t;
      //! the buffer where objects are hold.
      storage_t buffer;
      //! index to the current type hold by the GenType.
      unsigned short index = ListSize;

     private:
      //! a simple assertion
      TFEL_STATIC_ASSERT((tfel::meta::TLElementsAreUnique<List>::cond));
    };

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* _MSC_VER */

namespace tfel {

  namespace utilities {

    /*!
     * Apply function T::apply to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template <typename T, typename List>
    typename T::return_type apply(const GenTypeBase<List> &);

    /*!
     * Apply functor T to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template <typename T, typename List>
    typename T::return_type apply(T &, const GenTypeBase<List> &);

    /*!
     * Apply function T::apply to a GenTypeBase for the types holded
     * by the two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each pair of types in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if one of the GenTypeBase does not
     * hold any object.
     */
    template <typename T, typename List>
    typename T::return_type apply(const GenTypeBase<List> &,
                                  const GenTypeBase<List> &);

    /*!
     * Apply functor T to a GenTypeBase for the types holded by the
     * two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each pair of types
     * in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template <typename T, typename List>
    typename T::return_type apply(T &,
                                  const GenTypeBase<List> &,
                                  const GenTypeBase<List> &);

    /*!
     * Apply function T::apply to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template <typename T, typename List>
    typename T::return_type apply(GenTypeBase<List> &);

    /*!
     * Apply functor T to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template <typename T, typename List>
    typename T::return_type apply(T &, GenTypeBase<List> &);

    /*!
     * Apply function T::apply to a GenTypeBase for the types holded
     * by the two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each pair of types in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if one of the GenTypeBase does not
     * hold any object.
     */
    template <typename T, typename List>
    typename T::return_type apply(GenTypeBase<List> &, GenTypeBase<List> &);

    /*!
     * Apply functor T to a GenTypeBase for the types holded by the
     * two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each pair of types
     * in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template <typename T, typename List>
    typename T::return_type apply(T &,
                                  GenTypeBase<List> &,
                                  GenTypeBase<List> &);

    template <typename... Types>
    using GenType =
        GenTypeBase<typename tfel::meta::GenerateTypeList<Types...>::type>;

  }  // end of namespace utilities

}  // end of namespace tfel

#include "TFEL/Utilities/GenTypeBase.ixx"
#include "TFEL/Utilities/GenTypeSpecialisation.ixx"

#endif /* LIB_TFEL_UTILITIES_GENTYPEBASE_H */
