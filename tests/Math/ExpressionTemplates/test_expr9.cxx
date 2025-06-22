/*!
 * \file   tests/Math/ExpressionTemplates/test_expr9.cxx
 * \brief
 * \author Helfer Thomas
 * \date   17 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iterator>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  vector<double> v1(4);
  vector<double> v2(4);
  vector<double>::size_type i;
  v1(0) = 12.12;
  v1(1) = 12.234;
  v1(2) = 22.4322;
  v1(3) = 2.2112;
  v2(0) = 2.;
  v2(1) = 4.;
  v2(2) = 5.;
  v2(3) = 6.;
  ObjectObjectRandomAccessConstIterator<vector<double>, vector<double>, OpMinus>
      begin(v1.begin(), v2.begin());
  ObjectObjectRandomAccessConstIterator<vector<double>, vector<double>, OpMinus>
      end(v1.end(), v2.end());
  ObjectObjectRandomAccessConstIterator<vector<double>, vector<double>, OpMinus>
      p;
  for (i = 0, p = begin; p != end; ++p, ++i) {
    assert(abs(*p - (v1(i) - v2(i))) < 1.e-14);
  }
  return EXIT_SUCCESS;
}
