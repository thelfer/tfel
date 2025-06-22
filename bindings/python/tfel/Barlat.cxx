/*!
 * \file   Barlat.cxx
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
#include "TFEL/Material/Barlat2004YieldCriterion.hxx"

template <unsigned short N>
static tfel::math::st2tost2<N, double> makeBarlatLinearTransformation(
    const double c12,
    const double c21,
    const double c13,
    const double c31,
    const double c23,
    const double c32,
    const double c44,
    const double c55,
    const double c66) {
  return tfel::material::makeBarlatLinearTransformation<N>(
      c12, c21, c13, c31, c23, c32, c44, c55, c66);
}

template <unsigned short N>
static double computeBarlatStress(const tfel::math::stensor<N, double>& s,
                                  const tfel::math::st2tost2<N, double>& l1,
                                  const tfel::math::st2tost2<N, double>& l2,
                                  const double a,
                                  const double e) {
  return tfel::material::computeBarlatStress(s, l1, l2, a, e);
}  // end of computeBarlatStress

void declareBarlat();
void declareBarlat() {
  using namespace boost::python;
  def("makeBarlatLinearTransformation1D", &makeBarlatLinearTransformation<1u>);
  def("makeBarlatLinearTransformation2D", &makeBarlatLinearTransformation<2u>);
  def("makeBarlatLinearTransformation3D", &makeBarlatLinearTransformation<3u>);
  def("computeBarlatStress", &computeBarlatStress<1u>);
  def("computeBarlatStress", &computeBarlatStress<2u>);
  def("computeBarlatStress", &computeBarlatStress<3u>);
}
