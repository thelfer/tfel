/*!
 * \file   LinearHomogenizationSchemes.cxx
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
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static std::
    pair<StressType, tfel::types::real<StressType>> computeSphereDiluteScheme(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const tfel::types::real<StressType>& f,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i) {
  return tfel::material::homogenization::elasticity::computeSphereDiluteScheme<
      StressType>(young, nu, f, young_i, nu_i);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static std::
    pair<StressType,
         tfel::types::real<
             StressType>> computeSphereMoriTanakaScheme(const StressType& young,
                                                        const tfel::types::real<
                                                            StressType>& nu,
                                                        const tfel::types::real<
                                                            StressType>& f,
                                                        const StressType&
                                                            young_i,
                                                        const tfel::types::real<
                                                            StressType>& nu_i) {
  return tfel::material::homogenization::elasticity::
      computeSphereMoriTanakaScheme<StressType>(young, nu, f, young_i, nu_i);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static std::
    pair<
        StressType,
        tfel::types::real<
            StressType>> computeIsotropicDiluteScheme(const StressType& young,
                                                      const tfel::types::real<
                                                          StressType>& nu,
                                                      const tfel::types::real<
                                                          StressType>& f,
                                                      const StressType& young_i,
                                                      const tfel::types::real<
                                                          StressType>& nu_i,
                                                      const tfel::types::length<
                                                          StressType>& a,
                                                      const tfel::types::length<
                                                          StressType>& b,
                                                      const tfel::types::length<
                                                          StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeIsotropicDiluteScheme<StressType>(young, nu, f, young_i, nu_i, a,
                                               b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static std::
    pair<StressType,
         tfel::types::real<
             StressType>> computeIsotropicMoriTanakaScheme(const StressType&
                                                               young,
                                                           const tfel::types::
                                                               real<StressType>&
                                                                   nu,
                                                           const tfel::types::
                                                               real<StressType>&
                                                                   f,
                                                           const StressType&
                                                               young_i,
                                                           const tfel::types::
                                                               real<StressType>&
                                                                   nu_i,
                                                           const tfel::types::
                                                               length<
                                                                   StressType>&
                                                                   a,
                                                           const tfel::types::
                                                               length<
                                                                   StressType>&
                                                                   b,
                                                           const tfel::types::
                                                               length<
                                                                   StressType>&
                                                                   c) {
  return tfel::material::homogenization::elasticity::
      computeIsotropicMoriTanakaScheme<StressType>(young, nu, f, young_i, nu_i,
                                                   a, b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, StressType> computeTransverseIsotropicDiluteScheme(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const tfel::types::real<StressType>& f,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeTransverseIsotropicDiluteScheme<StressType>(young, nu, f, young_i,
                                                         nu_i, n_a, a, b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, StressType> computeTransverseIsotropicMoriTanakaScheme(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const tfel::types::real<StressType>& f,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeTransverseIsotropicMoriTanakaScheme<StressType>(
          young, nu, f, young_i, nu_i, n_a, a, b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, StressType> computeOrientedDiluteScheme(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const tfel::types::real<StressType>& f,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeOrientedDiluteScheme<StressType>(young, nu, f, young_i, nu_i, n_a,
                                              a, n_b, b, c);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<3u, StressType> computeOrientedMoriTanakaScheme(
        const StressType& young,
        const tfel::types::real<StressType>& nu,
        const tfel::types::real<StressType>& f,
        const StressType& young_i,
        const tfel::types::real<StressType>& nu_i,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
        const tfel::types::length<StressType>& a,
        const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
        const tfel::types::length<StressType>& b,
        const tfel::types::length<StressType>& c) {
  return tfel::material::homogenization::elasticity::
      computeOrientedMoriTanakaScheme<StressType>(young, nu, f, young_i, nu_i,
                                                  n_a, a, n_b, b, c);
}

void declareLinearHomogenizationSchemes(pybind11::module_&);

void declareLinearHomogenizationSchemes(pybind11::module_& m) {
  m.def("computeSphereDiluteScheme", &computeSphereDiluteScheme<double>);
  m.def("computeSphereMoriTanakaScheme",
        &computeSphereMoriTanakaScheme<double>);
  m.def("computeIsotropicDiluteScheme", &computeIsotropicDiluteScheme<double>);
  m.def("computeIsotropicMoriTanakaScheme",
        &computeIsotropicMoriTanakaScheme<double>);
  m.def("computeTransverseIsotropicDiluteScheme",
        &computeTransverseIsotropicDiluteScheme<double>);
  m.def("computeTransverseIsotropicMoriTanakaScheme",
        &computeTransverseIsotropicMoriTanakaScheme<double>);
  m.def("computeOrientedDiluteScheme", &computeOrientedDiluteScheme<double>);
  m.def("computeOrientedMoriTanakaScheme",
        &computeOrientedMoriTanakaScheme<double>);
}
