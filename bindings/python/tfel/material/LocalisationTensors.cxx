/*!
 * \file   LocalisationTensors.cxx
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
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"

template <typename real, typename StressType>
  static tfel::math::st2tost2<3u, double>
  computeSphereLocalisationTensor(const StressType& young, const real& nu) {
    return tfel::material::homogenization::elasticity::computeSphereHillPolarisationTensor(young, nu);
  }



void declareLocalisationTensors(pybind11::module_&);

void declareLocalisationTensors(pybind11::module_& m) {
  m.def("computeSphereLocalisationTensor",
        &computeSphereLocalisationTensor<double,double>);
}
