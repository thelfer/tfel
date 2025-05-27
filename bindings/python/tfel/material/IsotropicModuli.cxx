/*!
 * \file   IsotropicModuli.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   17/05/2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/IsotropicModuli.hxx"

  
  
  template <typename StressType>
  static tfel::math::st2tost2<3u, double>
  computeHillPolarisationTensor(
      const StressType& young,
      const real& nu,
      const tfel::math::tvector<3u, real>& n_a,
      const LengthType& a,
      const tfel::math::tvector<3u, real>& n_b,
      const LengthType& b,
      const LengthType& c) {
    return tfel::material::homogenization::elasticity::computeHillPolarisationTensor<real,StressType,LengthType>(young, nu,n_a,a,n_b,b,c);
  }  


void declareHillTensors(pybind11::module_&);

void declareHillTensors(pybind11::module_& m) {
  m.def("computeSphereHillPolarisationTensor",
        &computeSphereHillPolarisationTensor<double,double>);
}
