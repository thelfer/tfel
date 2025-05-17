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
  static tfel::math::st2tost2<3u, real>
  computeSphereLocalisationTensor(
    const StressType& young,
    const real& nu,
    const StressType& young_i,
    const real& nu_i) {
    return tfel::material::homogenization::elasticity::computeSphereLocalisationTensor<real,StressType>(young, nu,young_i,nu_i);
  }
  
  template <typename real, typename StressType>
  static tfel::math::st2tost2<3u, real>
  computeAxisymmetricalEllipsoidLocalisationTensor(
    const StressType& young,
    const real& nu,
    const StressType& young_i,
    const real& nu_i,
    const tfel::math::tvector<3u, real>& n_a,
    const real& e) {
    return tfel::material::homogenization::elasticity::computeAxisymmetricalEllipsoidLocalisationTensor<real,StressType>(young, nu,young_i,nu_i,n_a,e);
  }
  
  template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, real>
  computeEllipsoidLocalisationTensor(
      const StressType& young,
      const real& nu,
      const StressType& young_i,
      const real& nu_i,
      const tfel::math::tvector<3u, real>& n_a,
      const LengthType& a,
      const tfel::math::tvector<3u, real>& n_b,
      const LengthType& b,
      const LengthType& c) {
    return tfel::material::homogenization::elasticity::computeEllipsoidLocalisationTensor<real,StressType,LengthType>(young, nu,young_i,nu_i,n_a,a,n_b,b,c);
  }
  
  template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, real>
  computeAnisotropicLocalisationTensor(
      const tfel::math::st2tost2<3u, StressType>& C0_glob,
      const tfel::math::st2tost2<3u, StressType>& C0_loc,
      const tfel::math::tvector<3u, real>& n_a,
      const LengthType& a,
      const tfel::math::tvector<3u, real>& n_b,
      const LengthType& b,
      const LengthType& c,
      const std::size_t max_it = 12) {
    return tfel::material::homogenization::elasticity::computeAnisotropicLocalisationTensor<real,StressType,LengthType>(C0_glob, C0_loc,n_a,a,n_b,b,c,max_it);
  }


void declareLocalisationTensors(pybind11::module_&);

void declareLocalisationTensors(pybind11::module_& m) {
  m.def("computeSphereLocalisationTensor",
        &computeSphereLocalisationTensor<double,double>);
  m.def("computeAxisymmetricalEllipsoidLocalisationTensor",
        &computeAxisymmetricalEllipsoidLocalisationTensor<double,double>);
  m.def("computeEllipsoidLocalisationTensor",
        &computeEllipsoidLocalisationTensor<double,double,double>);
  m.def("computeAnisotropicLocalisationTensor",
        &computeAnisotropicLocalisationTensor<double,double,double>);
}
