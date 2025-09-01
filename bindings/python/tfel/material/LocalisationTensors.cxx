/*!
 * \file   LocalisationTensors.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   14/05/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
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

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, tfel::types::real<StressType>> computeSphereLocalisationTensor(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i) {
  return tfel::material::homogenization::elasticity::
      computeSphereLocalisationTensor<StressType>(young, nu, young_i, nu_i);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, tfel::types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::real<StressType>& e) {
  return tfel::material::homogenization::elasticity::
      computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
          young, nu, young_i, nu_i, n_a, e);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, tfel::types::real<StressType>> computeEllipsoidLocalisationTensor(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeEllipsoidLocalisationTensor<StressType>(young, nu, young_i, nu_i,
                                                     n_a, a, n_b, b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, tfel::types::real<StressType>> computeAnisotropicLocalisationTensor(
        const tfel::math::st2tost2<3u, StressType>& C0_glob,
        const tfel::math::st2tost2<3u, StressType>& Ci_loc,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c,
        const std::size_t max_it = 12) {
  return tfel::material::homogenization::elasticity::
      computeAnisotropicLocalisationTensor<StressType>(C0_glob, Ci_loc, n_a, a,
                                                       n_b, b, c, max_it);
}

void declareLocalisationTensors(pybind11::module_&);

void declareLocalisationTensors(pybind11::module_& m) {
  m.def("computeSphereLocalisationTensor",
        &computeSphereLocalisationTensor<double>);
  m.def("computeAxisymmetricalEllipsoidLocalisationTensor",
        &computeAxisymmetricalEllipsoidLocalisationTensor<double>);
  m.def("computeEllipsoidLocalisationTensor",
        &computeEllipsoidLocalisationTensor<double>);
  m.def("computeAnisotropicLocalisationTensor",
        &computeAnisotropicLocalisationTensor<double>);
}
