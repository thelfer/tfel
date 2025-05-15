/*!
 * \file   HillTensors.cxx
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
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"

template <typename real, typename StressType>
  static tfel::math::st2tost2<3u, double>
  computeSphereHillPolarisationTensor(const StressType& young, const real& nu) {
    return tfel::material::homogenization::elasticity::computeSphereHillPolarisationTensor(young, nu);
  }

template <typename real, typename StressType>
  static tfel::math::st2tost2<3u, double>
  computeAxisymmetricalHillPolarisationTensor(
      const StressType& young,
      const real& nu,
      const tfel::math::tvector<3u, real>& n_a,
      const real& e) {
    return tfel::material::homogenization::elasticity::computeAxisymmetricalHillPolarisationTensor(young, nu,n_a,e);
  }  

void declareIsotropicHillTensors(pybind11::module_&);

void declareIsotropicHillTensors(pybind11::module_& m) {
  m.def("computeSphereHillPolarisationTensor",
        &computeSphereHillPolarisationTensor<double,double>);
  m.def("computeAxisymmetricalHillPolarisationTensor",
        &computeAxisymmetricalHillPolarisationTensor<double,double>);
}
