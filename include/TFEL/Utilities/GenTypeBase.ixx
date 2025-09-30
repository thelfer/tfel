/*!
 * \file   include/TFEL/Utilities/GenTypeBase.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   05/02/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_GENTYPEBASEIXX
#define LIB_TFEL_GENTYPEBASEIXX

namespace tfel::utilities::internals {

  template <typename T, typename List, unsigned short N = 0>
  struct GenTypeBaseApply {
    typedef typename T::return_type return_type;

   private:
    struct EndRecursion {
      [[noreturn]] static return_type apply(const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static return_type apply(T&, const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static return_type apply(GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static return_type apply(T&, GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
    };  // end of struct EndRecursion

    struct EndRecursionII {
      [[noreturn]] static void apply(const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static void apply(T&, const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static void apply(GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static void apply(T&, GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
    };  // end of struct EndRecursionII

   public:
    typedef typename std::conditional<
        N + 1 == tfel::meta::TLSize<List>::value,
        typename std::conditional<std::is_same<return_type, void>::value,
                                  EndRecursionII,
                                  EndRecursion>::type,
        GenTypeBaseApply<T, List, N + 1>>::type Next;

    static return_type apply(const GenTypeBase<List>& v) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value;
      if (v.template is<current_value>()) {
        return T::apply(v.template get<current_value>());
      } else {
        return Next::apply(v);
      }
    }

    static return_type apply(T& f, const GenTypeBase<List>& v) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value;
      if (v.template is<current_value>()) {
        return f(v.template get<current_value>());
      } else {
        return Next::apply(f, v);
      }
    }

    static return_type apply(GenTypeBase<List>& v) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value;
      if (v.template is<current_value>()) {
        return T::apply(v.template get<current_value>());
      } else {
        return Next::apply(v);
      }
    }

    static return_type apply(T& f, GenTypeBase<List>& v) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value;
      if (v.template is<current_value>()) {
        return f(v.template get<current_value>());
      } else {
        return Next::apply(f, v);
      }
    }

  };  // end of struct GenTypeBaseApply

  template <typename T,
            typename List,
            unsigned short N = 0,
            unsigned short M = 0>
  struct GenTypeBaseApplyII {
   public:
    typedef typename T::return_type return_type;

   private:
    struct EndRecursion {
      [[noreturn]] static return_type apply(const GenTypeBase<List>&,
					    const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static return_type apply(T&,
					    const GenTypeBase<List>&,
					    const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static return_type apply(GenTypeBase<List>&, GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static return_type apply(T&, GenTypeBase<List>&, GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
    };  // end of struct EndRecursion

    struct EndRecursionII {
      [[noreturn]] static void apply(const GenTypeBase<List>&, const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static void apply(T&,
				      const GenTypeBase<List>&,
				      const GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static void apply(GenTypeBase<List>&, GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
      [[noreturn]] static void apply(T&, GenTypeBase<List>&, GenTypeBase<List>&) {
        throw(GenTypeCastError());
      }
    };  // end of struct EndRecursionII

   public:
    typedef typename std::conditional<
        ((N + 1 == tfel::meta::TLSize<List>::value) ||
         (M + 1 == tfel::meta::TLSize<List>::value)),
        typename std::conditional<std::is_same<return_type, void>::value,
                                  EndRecursionII,
                                  EndRecursion>::type,
        GenTypeBaseApplyII<T, List, N + 1, M + 1>>::type Next;
    typedef typename std::conditional<
        ((N + 1 == tfel::meta::TLSize<List>::value) ||
         (M + 1 == tfel::meta::TLSize<List>::value)),
        typename std::conditional<std::is_same<return_type, void>::value,
                                  EndRecursionII,
                                  EndRecursion>::type,
        GenTypeBaseApplyII<T, List, N + 1, M>>::type NextI;
    typedef typename std::conditional<
        ((N + 1 == tfel::meta::TLSize<List>::value) ||
         (M + 1 == tfel::meta::TLSize<List>::value)),
        typename std::conditional<std::is_same<return_type, void>::value,
                                  EndRecursionII,
                                  EndRecursion>::type,
        GenTypeBaseApplyII<T, List, N, M + 1>>::type NextII;

    static return_type apply(const GenTypeBase<List>& v1,
                             const GenTypeBase<List>& v2) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value1;
      typedef typename tfel::meta::TLFindNthElt<List, M>::type current_value2;
      const auto b1 = v1.template is<current_value1>();
      const auto b2 = v2.template is<current_value2>();
      if (b1 && b2) {
        return T::apply(v1.template get<current_value1>(),
                        v2.template get<current_value2>());
      } else if ((b1) && (!b2)) {
        return NextII::apply(v1, v2);
      } else if ((b2) && (!b1)) {
        return NextI::apply(v1, v2);
      }
      return Next::apply(v1, v2);
    }

    static return_type apply(T& f,
                             const GenTypeBase<List>& v1,
                             const GenTypeBase<List>& v2) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value1;
      typedef typename tfel::meta::TLFindNthElt<List, M>::type current_value2;
      const auto b1 = v1.template is<current_value1>();
      const auto b2 = v2.template is<current_value2>();
      if (b1 && b2) {
        return f(v1.template get<current_value1>(),
                 v2.template get<current_value2>());
      } else if ((b1) && (!b2)) {
        return NextII::apply(f, v1, v2);
      } else if ((b2) && (!b1)) {
        return NextI::apply(f, v1, v2);
      }
      return Next::apply(f, v1, v2);
    }

    static return_type apply(GenTypeBase<List>& v1, GenTypeBase<List>& v2) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value1;
      typedef typename tfel::meta::TLFindNthElt<List, M>::type current_value2;
      const auto b1 = v1.template is<current_value1>();
      const auto b2 = v2.template is<current_value2>();
      if (b1 && b2) {
        return T::apply(v1.template get<current_value1>(),
                        v2.template get<current_value2>());
      } else if ((b1) && (!b2)) {
        return NextII::apply(v1, v2);
      } else if ((b2) && (!b1)) {
        return NextI::apply(v1, v2);
      }
      return Next::apply(v1, v2);
    }

    static return_type apply(T& f,
                             GenTypeBase<List>& v1,
                             GenTypeBase<List>& v2) {
      typedef typename tfel::meta::TLFindNthElt<List, N>::type current_value1;
      typedef typename tfel::meta::TLFindNthElt<List, M>::type current_value2;
      const auto b1 = v1.template is<current_value1>();
      const auto b2 = v2.template is<current_value2>();
      if (b1 && b2) {
        return f(v1.template get<current_value1>(),
                 v2.template get<current_value2>());
      } else if ((b1) && (!b2)) {
        return NextII::apply(f, v1, v2);
      } else if ((b2) && (!b1)) {
        return NextI::apply(f, v1, v2);
      }
      return Next::apply(f, v1, v2);
    }

  };  // end of struct GenTypeBaseApply

}  // end of namespace tfel::utilities::internals

namespace tfel::utilities {

  template <typename T, typename List>
  typename T::return_type apply(const GenTypeBase<List>& v) {
    return internals::GenTypeBaseApply<T, List>::apply(v);
  }

  template <typename T, typename List>
  typename T::return_type apply(T& f, const GenTypeBase<List>& v) {
    return internals::GenTypeBaseApply<T, List>::apply(f, v);
  }

  template <typename T, typename List>
  typename T::return_type apply(const GenTypeBase<List>& v1,
                                const GenTypeBase<List>& v2) {
    return internals::GenTypeBaseApplyII<T, List>::apply(v1, v2);
  }

  template <typename T, typename List>
  typename T::return_type apply(T& f,
                                const GenTypeBase<List>& v1,
                                const GenTypeBase<List>& v2) {
    return internals::GenTypeBaseApplyII<T, List>::apply(f, v1, v2);
  }

  template <typename T, typename List>
  typename T::return_type apply(GenTypeBase<List>& v) {
    return internals::GenTypeBaseApply<T, List>::apply(v);
  }

  template <typename T, typename List>
  typename T::return_type apply(T& f, GenTypeBase<List>& v) {
    return internals::GenTypeBaseApply<T, List>::apply(f, v);
  }

  template <typename T, typename List>
  typename T::return_type apply(GenTypeBase<List>& v1, GenTypeBase<List>& v2) {
    return internals::GenTypeBaseApplyII<T, List>::apply(v1, v2);
  }

  template <typename T, typename List>
  typename T::return_type apply(T& f,
                                GenTypeBase<List>& v1,
                                GenTypeBase<List>& v2) {
    return internals::GenTypeBaseApplyII<T, List>::apply(f, v1, v2);
  }

  template <typename List>
  std::ostream& operator<<(std::ostream& os, const GenTypeBase<List>& g) {
    g.print(os);
    return os;
  }

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_GENTYPEBASEIXX */
