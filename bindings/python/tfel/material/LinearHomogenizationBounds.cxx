/*!
 * \file   LinearHomogenizationBounds.cxx
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
#include "TFEL/Material/LinearHomogenizationBounds.hxx"

template <typename real, typename StressType>
  static tfel::math::st2tost2<3, StressType>
  computeVoigtStiffness(
      const std::array<real, 2>& f_i,
      const std::array<tfel::math::st2tost2<3, StressType>, 2>& C_i){
   return tfel::material::homogenization::elasticity::computeVoigtStiffness<3,2,real,StressType>(f_i,C_i);
 }

template <typename real,typename StressType>
  static tfel::math::st2tost2<3, StressType>
  computeReussStiffness(
      const std::array<real, 2>& f_i,
      const std::array<tfel::math::st2tost2<3, StressType>, 2>& C_i){
  return tfel::material::homogenization::elasticity::computeReussStiffness<3,2,real,StressType>(f_i,C_i);
  }

template <typename real,typename StressType>
  static std::pair<std::pair<StressType, StressType>,std::pair<StressType, StressType>>
  computeIsotropicHashinShtrikmanBounds(const std::array<real, 2>& f_i,
                                        const std::array<StressType, 2>& K_i,
                                        const std::array<StressType, 2>& G_i){
  return tfel::material::homogenization::elasticity::computeIsotropicHashinShtrikmanBounds<3,2,real,StressType>(f_i,K_i,G_i);
  }

void declareLinearHomogenizationBounds(pybind11::module_&);

void declareLinearHomogenizationBounds(pybind11::module_& m) {
  m.def("computeVoigtStiffness",
        &computeVoigtStiffness<double,double>);
  m.def("computeReussStiffness",
        &computeReussStiffness<double,double>);
  m.def("computeIsotropicHashinShtrikmanBounds",
        &computeIsotropicHashinShtrikmanBounds<double,double>);
}
