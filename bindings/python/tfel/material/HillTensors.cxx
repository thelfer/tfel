/*!
 * \file   HillTensors.cxx
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
    st2tost2<3u, tfel::types::compliance<StressType>> computeSphereHillTensor(
        const StressType& young, const tfel::types::real<StressType>& nu) {
  return tfel::material::homogenization::elasticity::
      computeSphereHillPolarisationTensor<StressType>(young, nu);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<
        3u,
        tfel::types::compliance<
            StressType>> computeAxisymmetricalHillTensor(const StressType&
                                                             young,
                                                         const tfel::types::
                                                             real<StressType>&
                                                                 nu,
                                                         const tfel::math::tvector<
                                                             3u,
                                                             tfel::types::real<
                                                                 StressType>>&
                                                             n_a,
                                                         const tfel::types::
                                                             real<StressType>&
                                                                 e) {
  return tfel::material::homogenization::elasticity::
      computeAxisymmetricalHillPolarisationTensor<StressType>(young, nu, n_a,
                                                              e);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, tfel::types::compliance<StressType>> computeHillTensor(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeHillPolarisationTensor<StressType>(young, nu, n_a, a, n_b, b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<
        3u,
        tfel::types::compliance<
            StressType>> computeAnisotropicHillTensor(const tfel::math::
                                                          st2tost2<3u,
                                                                   StressType>&
                                                              C0,
                                                      const tfel::math::tvector<
                                                          3u,
                                                          tfel::types::real<
                                                              StressType>>& n_a,
                                                      const tfel::types::length<
                                                          StressType>& a,
                                                      const tfel::math::tvector<
                                                          3u,
                                                          tfel::types::real<
                                                              StressType>>& n_b,
                                                      const tfel::types::length<
                                                          StressType>& b,
                                                      const tfel::types::length<
                                                          StressType>& c,
                                                      const std::size_t max_it =
                                                          12) {
  return tfel::material::homogenization::elasticity::
      computeAnisotropicHillTensor<StressType>(C0, n_a, a, n_b, b, c, max_it);
}

void declareHillTensors(pybind11::module_&);

void declareHillTensors(pybind11::module_& m) {
  m.def("computeSphereHillTensor", &computeSphereHillTensor<double>);
  m.def("computeAxisymmetricalHillTensor",
        &computeAxisymmetricalHillTensor<double>);
  m.def("computeHillTensor", &computeHillTensor<double>);
  m.def("computeAnisotropicHillTensor", &computeAnisotropicHillTensor<double>);
}
