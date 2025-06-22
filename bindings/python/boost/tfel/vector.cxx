/*!
 * \file   bindings/python/tfel/vector.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 avr 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Python/VectorConverter.hxx"

void declareTFELMathVector();

void declareTFELMathVector() {
  using namespace tfel::python;
  using tfel::math::vector;
  initializeVectorConverter<vector<double>>();
}
