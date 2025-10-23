/*!
 * \file   LinearHomogenizationBounds.cxx
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
#include "TFEL/Material/LinearHomogenizationBounds.hxx"

template <tfel::math::ScalarConcept StressType,std::size_t N>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<N, StressType> computeVoigtStiffness(
        const std::vector<tfel::types::real<StressType>>& f_i,
        const std::vector<tfel::math::st2tost2<N, StressType>>& C_i) {
  return tfel::material::homogenization::elasticity::computeVoigtStiffness<
      N, StressType>(f_i, C_i);
}


template <tfel::math::ScalarConcept StressType,std::size_t N>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static tfel::math::
    st2tost2<N, StressType> computeReussStiffness(
        const std::vector<tfel::types::real<StressType>>& f_i,
        const std::vector<tfel::math::st2tost2<N, StressType>>& C_i) {
  return tfel::material::homogenization::elasticity::computeReussStiffness<
      N, StressType>(f_i, C_i);
}


template <tfel::math::ScalarConcept StressType,std::size_t N>
requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                            StressType>()) static std::
    pair<
        std::
    pair<StressType,StressType>,
        std::
    pair<StressType,StressType>> computeIsotropicHashinShtrikmanBounds(const std::
                                                                   vector<
                                                                       tfel::types::
                                                                           real<
                                                                               StressType>>& f_i,
                                                               const std::vector<
                                                                   StressType>& K_i,
                                                               const std::vector<
                                                                   StressType>& G_i) {
  return tfel::material::homogenization::elasticity::
      computeIsotropicHashinShtrikmanBounds<N, StressType>(f_i, K_i, G_i);
}


void declareLinearHomogenizationBounds(pybind11::module_&);

void declareLinearHomogenizationBounds(pybind11::module_& m) {
  m.def("computeVoigtStiffness3D", &computeVoigtStiffness<double,3>);
  m.def("computeReussStiffness3D", &computeReussStiffness<double,3>);
  m.def("computeIsotropicHashinShtrikmanBounds3D",
        &computeIsotropicHashinShtrikmanBounds<double,3>);
  m.def("computeVoigtStiffness2D", &computeVoigtStiffness<double,2>);
  m.def("computeReussStiffness2D", &computeReussStiffness<double,2>);
  m.def("computeIsotropicHashinShtrikmanBounds2D",
        &computeIsotropicHashinShtrikmanBounds<double,2>);
}
