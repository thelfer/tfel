/*!
 * \file   tests/Math/levenberg-marquardt4.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   19 nov 2008
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

#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

#include <cstdlib>
#include <cmath>

#include <utility>
#include <vector>

#include <memory>
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Math/LevenbergMarquardt.hxx"
#include "TFEL/Math/LevenbergMarquardt/LevenbergMarquardtEvaluatorWrapper.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
#if __GNUC__ != 3
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  const double x_data[] = {0.840188, 0.783099, 0.911647, 0.335223, 0.277775,
                           0.477397, 0.364784, 0.952230, 0.635712, 0.141603};

  const double y_data[] = {2.12989, 2.29532, 1.93195, 2.72801, 2.71560,
                           2.66178, 2.69570, 1.89371, 2.52963, 2.72843};
  std::vector<std::string> v;
  v.push_back("x");
  v.push_back("p0");
  v.push_back("p1");
  auto test = std::make_shared<Evaluator>(v, "p1*exp(p0*cos(x*x))");
  LevenbergMarquardt<LevenbergMarquardtEvaluatorWrapper> levmar(
      LevenbergMarquardtEvaluatorWrapper(test, 1u, 2u));
  vector<double> x(1u);
  vector<double> p(2u);
  unsigned short i;
  for (i = 0; i != 10; ++i) {
    x(0) = x_data[i];
    levmar.addData(x, y_data[i]);
  }
  // initial guess
  p(0) = 1.2;
  p(1) = -0.2;
  levmar.setInitialGuess(p);
  // execute
  p = levmar.execute();
  cout << "res : ";
  copy(p.begin(), p.end(), ostream_iterator<double>(cout, " "));
  cout << endl;
#endif
  return EXIT_SUCCESS;
}  // end of main
