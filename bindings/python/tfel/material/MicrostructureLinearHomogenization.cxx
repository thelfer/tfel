/*!
 * \file   MicrostructureHomogenization.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   23/06/2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/MicrostructureDescription.hxx"
#include "TFEL/Material/MicrostructureLinearHomogenization.hxx"
  
template <unsigned short int N,tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareHomogenizationScheme(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using HS = tfel::material::homogenization::elasticity::HomogenizationScheme<N,StressType>;

  pybind11::class_<HS>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const HS&>())
      .def_readonly("homogenized_stiffness",&HS::homogenized_stiffness)
      .def_readonly("effective_polarisation",&HS::effective_polarisation)
      .def_readonly("mean_strain_localisation_tensors",&HS::mean_strain_localisation_tensors);
  }

template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      tfel::material::homogenization::elasticity::HomogenizationScheme<N,StressType> computeDiluteScheme(
          tfel::material::homogenization::elasticity::ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          int max_iter_anisotropic_integration = 12
          ){
          return tfel::material::homogenization::elasticity::computeDilute<N,StressType>(micro,polarisations,max_iter_anisotropic_integration);}
          
  
template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      tfel::material::homogenization::elasticity::HomogenizationScheme<N,StressType> computeMoriTanakaScheme(
          tfel::material::homogenization::elasticity::ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          int max_iter_anisotropic_integration = 12
          ){
          return tfel::material::homogenization::elasticity::computeMoriTanaka<N,StressType>(micro,polarisations,max_iter_anisotropic_integration);}
          
template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      tfel::material::homogenization::elasticity::HomogenizationScheme<N,StressType> computeSelfConsistentScheme(
          tfel::material::homogenization::elasticity::ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          int max_iter,
          bool isotropic,
          int max_iter_anisotropic_integration = 12
          ){
          return tfel::material::homogenization::elasticity::computeSelfConsistent<N,StressType>(micro,polarisations,max_iter,isotropic,max_iter_anisotropic_integration);}


  
void declareMicrostructureLinearHomogenization(pybind11::module_&);

void declareMicrostructureLinearHomogenization(pybind11::module_& m) {
  declareHomogenizationScheme<3u,double>(m, "HomogenizationScheme");
  m.def("computeMoriTanakaScheme",
        &computeMoriTanakaScheme<3,double>);
  m.def("computeDiluteScheme",
        &computeDiluteScheme<3,double>);
  m.def("computeSelfConsistentScheme",
        &computeSelfConsistentScheme<3,double>);
  
}
