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
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"

  template <typename real, typename StressType>
  static tfel::math::st2tost2<3u, double>
  computeSphereHillTensor(const StressType& young, const real& nu) {
    return tfel::material::homogenization::elasticity::computeSphereHillPolarisationTensor<real,StressType>(young, nu);
  }

  template <typename real, typename StressType>
  static tfel::math::st2tost2<3u, double>
  computeAxisymmetricalHillTensor(
      const StressType& young,
      const real& nu,
      const tfel::math::tvector<3u, real>& n_a,
      const real& e) {
    return tfel::material::homogenization::elasticity::computeAxisymmetricalHillPolarisationTensor<real,StressType>(young, nu,n_a,e);
  }
  
  template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, double>
  computeHillTensor(
      const StressType& young,
      const real& nu,
      const tfel::math::tvector<3u, real>& n_a,
      const LengthType& a,
      const tfel::math::tvector<3u, real>& n_b,
      const LengthType& b,
      const LengthType& c) {
    return tfel::material::homogenization::elasticity::computeHillPolarisationTensor<real,StressType,LengthType>(young, nu,n_a,a,n_b,b,c);
  }  
  
  template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, double>
  computeAnisotropicHillTensor(
      const tfel::math::st2tost2<3u,StressType>& C0,const tfel::math::tvector<3u, real>& n_a,
      const LengthType& a,
      const tfel::math::tvector<3u, real>& n_b,
      const LengthType& b,
      const LengthType& c,
      const std::size_t max_it = 12) {
    return tfel::material::homogenization::elasticity::computeAnisotropicHillTensor<real,StressType,LengthType>(C0,n_a,a,n_b,b,c,max_it);
  }

void declareHillTensors(pybind11::module_&);

void declareHillTensors(pybind11::module_& m) {
  m.def("computeSphereHillTensor",
        &computeSphereHillTensor<double,double>);
  m.def("computeAxisymmetricalHillTensor",
        &computeAxisymmetricalHillTensor<double,double>);
  m.def("computeHillTensor",
        &computeHillTensor<double,double,double>);
  m.def("computeAnisotropicHillTensor",
        &computeAnisotropicHillTensor<double,double,double>);
}
