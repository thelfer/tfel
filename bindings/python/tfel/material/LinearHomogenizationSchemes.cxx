/*!
 * \file   LinearHomogenizationSchemes.cxx
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
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"

template <typename real, typename StressType>
  static std::pair<StressType, real>
  computeSphereDiluteScheme(const StressType& young, const real& nu, const real& f, const StressType& young_i, const real& nu_i) {
    return tfel::material::homogenization::elasticity::computeSphereDiluteScheme<real,StressType>(young, nu,f,young_i,nu_i);
  }
  
template <typename real, typename StressType>
  static std::pair<StressType, real>
  computeSphereMoriTanakaScheme(const StressType& young, const real& nu, const real& f, const StressType& young_i, const real& nu_i) {
    return tfel::material::homogenization::elasticity::computeSphereMoriTanakaScheme<real,StressType>(young, nu,f,young_i,nu_i);
  }
  
template <typename real, typename StressType, typename LengthType>
  static std::pair<StressType, real>
  computeIsotropicDiluteScheme(const StressType& young,
                               const real& nu,
                               const real& f,
                               const StressType& young_i,
                               const real& nu_i,
                               const LengthType& a,
                               const LengthType& b,
                               const LengthType& c){
    return tfel::material::homogenization::elasticity::computeIsotropicDiluteScheme<real,StressType,LengthType>(young, nu,f,young_i,nu_i,a,b,c);                           
  }
  
 template <typename real, typename StressType, typename LengthType>
  static std::pair<StressType, real>
  computeIsotropicMoriTanakaScheme(const StressType& young,
                               const real& nu,
                               const real& f,
                               const StressType& young_i,
                               const real& nu_i,
                               const LengthType& a,
                               const LengthType& b,
                               const LengthType& c){
    return tfel::material::homogenization::elasticity::computeIsotropicMoriTanakaScheme<real,StressType,LengthType>(young, nu,f,young_i,nu_i,a,b,c);                           
  }
  
 template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, StressType>
  computeTransverseIsotropicDiluteScheme(const StressType& young,
                                         const real& nu,
                                         const real& f,
                                         const StressType& young_i,
                                         const real& nu_i,
                                         const tfel::math::tvector<3u, real>& n_a,
                                         const LengthType& a,
                                         const LengthType& b,
                                         const LengthType& c){
    return tfel::material::homogenization::elasticity::computeTransverseIsotropicDiluteScheme<real,StressType,LengthType>(young, nu,f,young_i,nu_i,n_a,a,b,c);                           
  }
  
 template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, StressType>
  computeTransverseIsotropicMoriTanakaScheme(const StressType& young,
                                         const real& nu,
                                         const real& f,
                                         const StressType& young_i,
                                         const real& nu_i,
                                         const tfel::math::tvector<3u, real>& n_a,
                                         const LengthType& a,
                                         const LengthType& b,
                                         const LengthType& c){
    return tfel::material::homogenization::elasticity::computeTransverseIsotropicMoriTanakaScheme<real,StressType,LengthType>(young, nu,f,young_i,nu_i,n_a,a,b,c);                           
  }
  
 template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, StressType>
  computeOrientedDiluteScheme(const StressType& young,
                              const real& nu,
                              const real& f,
                              const StressType& young_i,
                              const real& nu_i,
                              const tfel::math::tvector<3u, real>& n_a,
                              const LengthType& a,
                              const tfel::math::tvector<3u, real>& n_b,
                              const LengthType& b,
                              const LengthType& c){
    return tfel::material::homogenization::elasticity::computeOrientedDiluteScheme<real,StressType,LengthType>(young, nu,f,young_i,nu_i,n_a,a,n_b,b,c);                          
  }
 
 template <typename real, typename StressType, typename LengthType>
  static tfel::math::st2tost2<3u, StressType>
  computeOrientedMoriTanakaScheme(const StressType& young,
                              const real& nu,
                              const real& f,
                              const StressType& young_i,
                              const real& nu_i,
                              const tfel::math::tvector<3u, real>& n_a,
                              const LengthType& a,
                              const tfel::math::tvector<3u, real>& n_b,
                              const LengthType& b,
                              const LengthType& c){
    return tfel::material::homogenization::elasticity::computeOrientedMoriTanakaScheme<real,StressType,LengthType>(young, nu,f,young_i,nu_i,n_a,a,n_b,b,c);                          
  }
                              
                              
void declareLinearHomogenizationSchemes(pybind11::module_&);

void declareLinearHomogenizationSchemes(pybind11::module_& m) {
  m.def("computeSphereDiluteScheme",
        &computeSphereDiluteScheme<double,double>);
  m.def("computeSphereMoriTanakaScheme",
        &computeSphereMoriTanakaScheme<double,double>);
  m.def("computeIsotropicDiluteScheme",
        &computeIsotropicDiluteScheme<double,double,double>);
  m.def("computeIsotropicMoriTanakaScheme",
        &computeIsotropicMoriTanakaScheme<double,double,double>);
  m.def("computeTransverseIsotropicDiluteScheme",
        &computeTransverseIsotropicDiluteScheme<double,double,double>);
  m.def("computeTransverseIsotropicMoriTanakaScheme",
        &computeTransverseIsotropicMoriTanakaScheme<double,double,double>);
  m.def("computeOrientedDiluteScheme",
        &computeOrientedDiluteScheme<double,double,double>);
  m.def("computeOrientedMoriTanakaScheme",
        &computeOrientedMoriTanakaScheme<double,double,double>);
}
