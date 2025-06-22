/*!
 * \file   tests/Math/ExpressionTemplates/test_expr7.cxx
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

#include <cassert>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/ScalarObjectRandomAccessConstIterator.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  vector<double> v(4);
  vector<double>::size_type i;

  v[0] = 12.12;
  v[1] = 12.234;
  v[2] = 22.4322;
  v[3] = 2.2112;

  ScalarObjectRandomAccessConstIterator<int, vector<double>, OpMult> begin(
      2, v.begin());
  ScalarObjectRandomAccessConstIterator<int, vector<double>, OpMult> end(
      2, v.end());
  ScalarObjectRandomAccessConstIterator<int, vector<double>, OpMult> p;

  ObjectScalarRandomAccessConstIterator<vector<double>, int, OpMult> begin2(
      v.begin(), 3);
  ObjectScalarRandomAccessConstIterator<vector<double>, int, OpMult> end2(
      v.end(), 3);
  ObjectScalarRandomAccessConstIterator<vector<double>, int, OpMult> p2;

  for (i = 0, p = begin; p != end; ++p, ++i) {
    assert(abs(*p - 2 * v[i]) < 1.e-14);
  }

  for (i = 0, p2 = begin2; p2 != end2; ++p2, ++i) {
    assert(abs(*p2 - 3 * v[i]) < 1.e-14);
  }

  return EXIT_SUCCESS;
}
