/*!
 * \file   tests/Math/promote.cxx
 * \brief
 * \author Helfer Thomas
 * \date   03 May 2006
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

#include "promote-header.hxx"

#define ASSERT(X)                                 \
  if (!(X)) {                                     \
    std::cerr << "(" << #X << ")"                 \
              << " is not satified" << std::endl; \
    std::exit(EXIT_FAILURE);                      \
  }                                               \
  static_cast<void>(0)

std::string Test(const float&) { return "float"; }

std::string Test(const long double&) { return "long double"; }

std::string Test(const double&) { return "double"; }

std::string Test(const int&) { return "int"; }

std::string Test(const long int&) { return "long int"; }

std::string Test(const long unsigned int&) { return "long unsigned int"; }

std::string Test(const unsigned int&) { return "unsigned int"; }

std::string Test(const short&) { return "short"; }

std::string Test(const unsigned short&) { return "unsigned short"; }

std::string Test(const tfel::math::Complex<unsigned short>&) {
  return "complex unsigned short";
}

std::string Test(const tfel::math::Complex<short>&) { return "complex short"; }

std::string Test(const tfel::math::Complex<unsigned int>&) {
  return "complex unsigned int";
}

std::string Test(const tfel::math::Complex<int>&) { return "complex int"; }

std::string Test(const tfel::math::Complex<float>&) { return "complex float"; }

std::string Test(const tfel::math::Complex<double>&) {
  return "complex double";
}

std::string Test(const tfel::math::Complex<long double>&) {
  return "complex long double";
}

struct A {};

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::typetraits;
  using namespace tfel::typetraits::internals;
  Promote<int, double>::type a;
  Promote<int, unsigned int>::type b;
  Promote<int, float>::type c;
  Promote<unsigned int, float>::type d;
  Promote<double, float>::type e;
  Promote<short, short>::type f;
  Promote<short, unsigned short>::type g;
  Promote<double, unsigned short>::type h;
  Promote<double, double>::type i;
  Promote<unsigned short, unsigned int>::type j;
  Promote<tfel::math::Complex<double>, unsigned int>::type k;
  Promote<unsigned int, tfel::math::Complex<unsigned short>>::type l;
  Promote<double, tfel::math::Complex<double>>::type m;
  Promote<unsigned int, long unsigned int>::type n;
  Promote<int, long int>::type o;
  Promote<unsigned short, unsigned short>::type p;
  const unsigned short q = 2u;
  const unsigned short r = 3u;
  /*  Beginning of the tests */
  ASSERT(Test(a) == "double");
  ASSERT((Test(b) == "float") || (Test(b) == "long int"));
  ASSERT(Test(c) == "float");
  ASSERT(Test(d) == "float");
  ASSERT(Test(e) == "double");
  ASSERT(Test(f) == "short");
  ASSERT(Test(g) == "int");
  ASSERT(Test(h) == "double");
  ASSERT(Test(i) == "double");
  ASSERT(Test(j) == "unsigned int");
  ASSERT(Test(k) == "complex double");
  ASSERT(Test(l) == "complex unsigned int");
  ASSERT(Test(m) == "complex double");
  ASSERT(Test(n) == "long unsigned int");
  ASSERT(Test(o) == "long int");
  ASSERT(Test(p) == "unsigned short");
  ASSERT(Test(p) == "unsigned short");
  ASSERT(Test(q * r) == "int");
  return EXIT_SUCCESS;
}
