/*!
 * \file   include/TFEL/Utilities/GenTypeBase.hxx
 * \brief  This file implements the GenTypeBase class and some
 * internal classes used by the GenType class.
 * \see    TL
 * \author Thomas Helfer
 * \date   19 Apr. 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_GENTYPEBASE_HXX
#define LIB_TFEL_UTILITIES_GENTYPEBASE_HXX

#include <cmath>
#include <limits>
#include <utility>
#include <variant>
#include <type_traits>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/TypeList.hxx"
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeCastError.hxx"

namespace tfel::utilities::internals {

  /*! \brief an helper class to a add methods to a GenType for a specific type.
   *
   * This class is based on the curiously recurring template
   * pattern for achieving this.
   * \param typename Child, the Child of this class.
   * \param typename T, the specific type.
   * \see GenTypeSpecialisation.ixx for examples.
   * \see http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern
   * \author Thomas Helfer.
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
   * \author Thomas Helfer.
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

  /*!
   * \brief a partial specialisation to end the recursion.
   * \author Thomas Helfer.
   * \date   20 Apr. 2007.
   */
  template <typename Child>
  class GenTypeSpecializedMethods<Child, tfel::meta::TLE> {};

  template <typename List, typename... Types>
  struct StdVariantFromTypeList {
    //! \brief a simple alias
    using type = typename StdVariantFromTypeList<typename List::Next,
                                                 Types...,
                                                 typename List::Current>::type;
  };

  template <typename... Types>
  struct StdVariantFromTypeList<tfel::meta::TLE, Types...> {
    //! \brief a simple alias
    using type = std::variant<std::monostate, Types...>;
  };

}  // end of namespace tfel::utilities::internals

namespace tfel::utilities {

  /*!
   * \brief the base class of GenType.
   * \param  typename List, the list of types contained in the GenType.
   * \author Thomas Helfer.
   * \date   20 Apr. 2007.
   */
  template <typename List>
  struct GenTypeBase
      : public tfel::utilities::internals::
            GenTypeSpecializedMethods<GenTypeBase<List>, List>,
        public tfel::utilities::internals::StdVariantFromTypeList<List>::type {
    // a simple check
    static_assert(tfel::meta::TLElementsAreUnique<List>::cond);

    /*!
     * \brief a simple wrapper around std::enable_if
     * \tparam T: tested type which must belong to List for the requirement
     * to hold true \tparam R: result
     */
    template <typename T, typename R = T>
    using count =
        typename tfel::meta::TLCountNbrOfT<typename std::decay<T>::type, List>;
    /*!
     * \brief a simple wrapper around std::enable_if
     * \tparam T: tested type which must belong to List for the requirement to
     * hold true \tparam R: result
     */
    template <typename T, typename R = T>
    using type_check = typename std::enable_if<count<T>::value == 1, R>::type;
    //! \brief a simple alias
    using variant =
        typename tfel::utilities::internals::StdVariantFromTypeList<List>::type;
    //! \brief default constructor
    GenTypeBase() = default;
    //! \brief move constructor
    GenTypeBase(GenTypeBase &&) = default;
    //! \brief copy constructor
    GenTypeBase(const GenTypeBase &) = default;
    //! \brief constructor from a value
    template <typename T1, typename = type_check<T1, void>>
    GenTypeBase(T1 &&value) : variant(std::forward<T1>(value)) {}
    // \brief assignement operator
    GenTypeBase &operator=(GenTypeBase &&) = default;
    // \brief assignement operator
    GenTypeBase &operator=(const GenTypeBase &) = default;
    // \brief assignement operator from a value
    template <typename T1, typename = type_check<T1, void>>
    GenTypeBase &operator=(T1 &&value) {
      variant::operator=(std::forward<T1>(value));
      return *this;
    }
    //! \return true if empty
    bool empty() const { return std::holds_alternative<std::monostate>(*this); }
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
    TFEL_INLINE type_check<T1, void> set(T1 &&src) {
      this->operator=(std::forward<T1>(src));
    }
    //
    template <typename T1>
    TFEL_INLINE type_check<T1, bool> is() const {
      return std::holds_alternative<T1>(*this);
    }
    //! \return the value hold by the `GenTypeBase`.
    template <typename T1>
    TFEL_INLINE type_check<T1, const T1 &> get() const {
      tfel::raise_if<GenTypeCastError>(!this->template is<T1>());
      return std::get<T1>(*this);
    }
    //! \return the value hold by the `GenTypeBase`.
    template <typename T1>
    TFEL_INLINE type_check<T1, T1 &> get() {
      tfel::raise_if<GenTypeCastError>(!this->template is<T1>());
      return std::get<T1>(*this);
    }
    //! \return the type index of the object
    TFEL_INLINE unsigned short getTypeIndex() const { return this->index(); }

   protected:
    //! \brief clear the GenType
    TFEL_INLINE void clear() { variant::operator=(std::monostate()); }
    //! \brief set the value of the GenType.
    template <typename T1>
    TFEL_INLINE type_check<T1, void> set_uninitialised() {
      this->operator=(T1());
    }
  };

}  // end of namespace tfel::utilities

namespace tfel::utilities {

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
  typename T::return_type apply(T &, GenTypeBase<List> &, GenTypeBase<List> &);

  template <typename... Types>
  using GenType =
      GenTypeBase<typename tfel::meta::GenerateTypeList<Types...>::type>;

}  // end of namespace tfel::utilities

#include "TFEL/Utilities/GenTypeBase.ixx"
#include "TFEL/Utilities/GenTypeSpecialisation.ixx"

#endif /* LIB_TFEL_UTILITIES_GENTYPEBASE_HXX */
