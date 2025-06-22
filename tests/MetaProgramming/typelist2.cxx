/*!
 * \file   tests/MetaProgramming/typelist2.cxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "TFEL/Metaprogramming/TypeList.hxx"
#include "TFEL/Math/General/LogicalOperations.hxx"

template <typename T, template <class> class U>
class generic_container_;

template <template <class> class U>
class generic_container_<tfel::meta::TLE, U> {};

template <typename T, template <class> class U>
class generic_container_ : public U<typename T::Current>,
                           public generic_container_<typename T::Next, U> {};

template <typename T>
struct holder {
  T value;

  void set(const T& src) { value = src; }
};

template <typename T>
class generic_container : public generic_container_<T, holder> {
 private:
  static_assert(tfel::meta::TLElementsAreUnique<T>::cond);

 public:
  template <unsigned int N>
  typename tfel::meta::TLFindNthElt<T, N>::type get() const
      requires(N < tfel::meta::TLSize<T>::value) {
    return static_cast<
               const holder<typename tfel::meta::TLFindNthElt<T, N>::type>*>(
               this)
        ->value;
  }

  template <unsigned int N>
  void set(const typename tfel::meta::TLFindNthElt<T, N>::type& src) requires(
      N < tfel::meta::TLSize<T>::value) {
    static_cast<holder<typename tfel::meta::TLFindNthElt<T, N>::type>*>(this)
        ->set(src);
  }
};

template <class T>
struct Is_same {
  template <class T2>
  struct test {
    static constexpr bool cond = std::is_same<T, T2>::value;
  };
};

template <typename T, class Test>
class Mytest {
  using my_test_class = typename Test::template test<T>;

 public:
  static constexpr bool cond = my_test_class::cond;
};

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::meta;

  typedef GenerateTypeList<std::string, double, int>::type my_list;
  assert(TLElementsAreUnique<my_list>::cond);

  generic_container<TLNode<double, TLNode<std::string, TLE>>> test;

  test.set<0>(13.254);
  test.set<1>("Coucou");
  assert(std::abs(test.get<0>() - 13.254) < 1.e-14);
  assert(test.get<1>() == "Coucou");

  assert(Is_same<double>::test<double>::cond);
  assert(!Is_same<double>::test<int>::cond);
  assert((Mytest<double, Is_same<double>>::cond));

  return EXIT_SUCCESS;
}
