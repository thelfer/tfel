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
#include "TFEL/Material/LocalisationTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"


void declareLocalisationTensors(pybind11::module_&);
using namespace tfel::material;
void declareLocalisationTensors(pybind11::module_& m) {

  m.def("computeSphereLocalisationTensor", [](const double young, const double nu, const double young_i, const double nu_i){return homogenization::elasticity::computeSphereLocalisationTensor(young,nu,young_i,nu_i);});
  m.def("computeSphereLocalisationTensor", [](const IsotropicModuli<double> &IM, const IsotropicModuli<double> &IMi){return homogenization::elasticity::computeSphereLocalisationTensor(IM,IMi);});
  m.def("computeAxisymmetricalLocalisationTensor", [](const double young, const double nu, const double young_i, const double nu_i, const tfel::math::tvector<3u,double>& n_a, const double e){return homogenization::elasticity::computeAxisymmetricalEllipsoidLocalisationTensor(young,nu,young_i,nu_i,n_a,e);});
  m.def("computeAxisymmetricalLocalisationTensor",[](const IsotropicModuli<double> &IM, const IsotropicModuli<double> &IMi, const tfel::math::tvector<3u,double>& n_a, const double e){return homogenization::elasticity::computeAxisymmetricalEllipsoidLocalisationTensor(IM,IMi,n_a,e);});
  m.def("computeLocalisationTensor", [](const double young, const double nu, const double young_i, const double nu_i, const tfel::math::tvector<3u,double>& n_a, const double a, const tfel::math::tvector<3u,double>& n_b, const double b, const double c){return homogenization::elasticity::computeEllipsoidLocalisationTensor(young,nu,young_i,nu_i,n_a,a,n_b,b,c);});
  m.def("computeLocalisationTensor",[](const IsotropicModuli<double> &IM, const IsotropicModuli<double> &IMi, const tfel::math::tvector<3u,double>& n_a, const double a, const tfel::math::tvector<3u,double>& n_b, const double b, const double c){return homogenization::elasticity::computeEllipsoidLocalisationTensor(IM,IMi,n_a,a,n_b,b,c);});
  m.def("computeLocalisationTensor",[](const IsotropicModuli<double> &IM, const tfel::math::st2tost2<3u,double> &Ci_loc, const tfel::math::tvector<3u,double>& n_a, const double a, const tfel::math::tvector<3u,double>& n_b, const double b, const double c){return homogenization::elasticity::computeEllipsoidLocalisationTensor(IM,Ci_loc,n_a,a,n_b,b,c);});
  m.def("computeAnisotropicLocalisationTensor",[](const tfel::math::st2tost2<3u,double> &C0, const tfel::math::st2tost2<3u,double> &Ci_loc, const tfel::math::tvector<3u,double>& n_a, const double a, const tfel::math::tvector<3u,double>& n_b, const double b, const double c, const std::size_t max_it){return homogenization::elasticity::computeAnisotropicLocalisationTensor(C0,Ci_loc,n_a,a,n_b,b,c,max_it);});

}
