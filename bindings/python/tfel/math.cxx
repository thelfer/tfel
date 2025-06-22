/*!
 * \file  bindings/python/tfel/math.cxx
 * \brief
 * \author Thomas Helfer
 * \date 30/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

void declareTFELMathVector();
void declaretvector();
void declarestensor();
void declarest2tost2();
void declareEvaluator();

BOOST_PYTHON_MODULE(math) {
  declaretvector();
  declarestensor();
  declarest2tost2();
  declareTFELMathVector();
  declareEvaluator();
}
