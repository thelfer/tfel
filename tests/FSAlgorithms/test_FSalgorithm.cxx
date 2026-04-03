/*!
 * \file   tests/FSAlgorithms/test_FSalgorithm.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   30 Jun 2006
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

#include <iostream>
#include <iterator>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

static double add(double a, double b) { return a + b; }

static double mult(double a, double b) { return a * b; }

static double times2(double a) { return 2 * a; }

static double null_mult(double, double) { return 0.; }

struct abs_compare {
  template <typename T>
  bool operator()(const T& a, const T& b) {
    return std::abs(a) > std::abs(b);
  }
};

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::fsalgo;

  double tab1[] = {5., -16., 9.};
  double tab2[3];
  double tab3[3];
  unsigned int tab4[] = {15, 1, 32109};
  unsigned int tab5[3];
  unsigned int i;

  assert(std::abs(*min_element<3>::exe(tab1) + 16.) < 1.e-14);
  assert(std::abs(*max_element<3>::exe(tab1) - 9.) < 1.e-14);
  assert(std::abs(*max_element<3>::exe(tab1, abs_compare()) + 16.) < 1.e-14);

  copy<3>::exe(tab1, tab2);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab1[i] - tab2[i]) < 1.e-14);
  }
  fill<3>::exe(tab1, 3.);
  fill<3>::exe(tab2, 6.);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab1[i] - 3.) < 1.e-14);
    assert(std::abs(tab2[i] - 6.) < 1.e-14);
  }

  transform<3>::exe(tab1, tab2, tab3, mult);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab3[i] - 18.) < 1.e-14);
  }

  fill<3>::exe(tab1, 3.);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab1[i] - 3.) < 1.e-14);
  }

  transform<3>::exe(tab1, tab2, times2);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab2[i] - 6.) < 1.e-14);
  }

  fill<3>::exe(tab1, 6.);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab1[i] - 6.) < 1.e-14);
  }

  copy<3>::exe(tab4, tab5);
  for (i = 0; i != 3; ++i) {
    assert(tab4[i] == tab5[i]);
    assert(equal<3>::exe(tab4, tab5));
  }

  swap_ranges<3>::exe(tab1, tab3);
  for (i = 0; i != 3; ++i) {
    assert(std::abs(tab3[i] - 6.) < 1.e-14);
    assert(std::abs(tab1[i] - 18.) < 1.e-14);
  }

  tab1[0] = 3.;
  tab1[1] = 12.1;
  tab1[2] = 2.786;

  assert(std::abs(accumulate<3>::exe(tab1, 1.) - 18.886) < 1.e-14);

  tab1[0] = 3.5;
  tab1[1] = 8.;
  tab1[2] = 3.14159;
  assert(std::abs(accumulate<3>::exe(tab1, 2., mult) - 175.929) < 1.e-3);

  tab1[0] = 2.;
  tab1[1] = 3.;
  tab1[2] = 1.5;
  tab2[0] = 2.2;
  tab2[1] = 3.6;
  tab2[2] = 2.;

  assert(std::abs(inner_product<3>::exe(tab1, tab2, -16.) - 2.2) < 1.e-3);
  assert(std::abs(inner_product<3>::exe(tab1, tab2, -16., add, mult) - 2.2) <
         1.e-3);
  assert(std::abs(inner_product<3>::exe(tab1, tab2, -16., add, null_mult) +
                  16) < 1.e-3);
  assert(std::abs(inner_product<3>::exe(tab1, tab2, -16., add, add) + 1.7) <
         1.e-3);

  return EXIT_SUCCESS;
}
