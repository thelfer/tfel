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
#include "TFEL/Material/MicrostructureLinearHomogenization.ixx"
  
template <unsigned short int N,tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareHomogenizationScheme(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using HS = tfel::material::homogenization::elasticity::HomogenizationScheme<N,StressType>;

  pybind11::class_<HS>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const HS&>())
      .def("homogenized_stiffness",[](HS &hs){
          return hs.homogenized_stiffness;});
  }
  
  
template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      tfel::material::homogenization::elasticity::HomogenizationScheme<N,StressType> computeMoriTanakaScheme(
          tfel::material::homogenization::elasticity::ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12
          ){
          return tfel::material::homogenization::elasticity::computeMoriTanaka<N,StressType>(micro,polarisations,isotropic_matrix,verbose,max_iter_anisotropic_integration);}


  
void declareMicrostructureLinearHomogenization(pybind11::module_&);

void declareMicrostructureLinearHomogenization(pybind11::module_& m) {
  declareHomogenizationScheme<3,double>(m, "HomogenizationScheme3D");
  m.def("computeMoriTanakaScheme3D",
        &computeMoriTanakaScheme<3,double>);
  m.def("computeMoriTanakaScheme2D",
        &computeMoriTanakaScheme<2,double>);
  
}
