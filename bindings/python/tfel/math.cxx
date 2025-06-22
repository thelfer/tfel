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
#ifdef TFEL_NUMPY_SUPPORT
#include "TFEL/Numpy/InitNumpy.hxx"
#endif /* TFEL_NUMPY_SUPPORT */

void declareTFELMathVector();
void declaretvector();
void declarestensor();
void declarest2tost2();
void declareEvaluator();

#ifdef TFEL_NUMPY_SUPPORT
void declareAccelerationAlgorithms();
#endif /* TFEL_NUMPY_SUPPORT */

BOOST_PYTHON_MODULE(math) {
#ifdef TFEL_NUMPY_SUPPORT
  tfel::numpy::initializeNumPy();
#endif /* TFEL_NUMPY_SUPPORT */
  declaretvector();
  declarestensor();
  declarest2tost2();
  declareTFELMathVector();
  declareEvaluator();
#ifdef TFEL_NUMPY_SUPPORT
  declareAccelerationAlgorithms();
#endif /* TFEL_NUMPY_SUPPORT */
}
