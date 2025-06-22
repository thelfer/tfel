/*!
 * \file   Hosford.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/Material/Hosford1972YieldCriterion.hxx"

template <unsigned short N>
static double computeHosfordStress(const tfel::math::stensor<N, double>& s,
                                   const double a,
                                   const double e) {
  return tfel::material::computeHosfordStress(s, a, e);
}  // end of computeHosfordStress

void declareHosford();
void declareHosford() {
  using namespace boost::python;
  def("computeHosfordStress", &computeHosfordStress<1u>);
  def("computeHosfordStress", &computeHosfordStress<2u>);
  def("computeHosfordStress", &computeHosfordStress<3u>);
}
