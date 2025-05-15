/*!
 * \file   LinearHomogenizationSchemes.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   14/05/2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"
#include "TFEL/Material/LinearHomogenizationBounds.hxx"

template <typename real, typename StressType>
  static std::pair<double, double>
  computeSphereDiluteScheme(const StressType& young, const real& nu, const real& f, const StressType& young_i, const StressType& nu_i) {
    return tfel::material::homogenization::elasticity::computeSphereDiluteScheme(young, nu,f,young_i,nu_i);
  }


void declareLinearHomogenizationSchemes(pybind11::module_&);

void declareLinearHomogenizationSchemes(pybind11::module_& m) {
  m.def("computeSphereDiluteScheme",
        &computeSphereDiluteScheme<double,double>);
}
