/*!
 * \file   Microstructure.cxx
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
  
template <tfel::math::ScalarConcept LengthType>
    requires(
        tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                           LengthType>())
static void declareSphere(pybind11::module_& m, const char* const n) {
  using Sph = tfel::material::homogenization::elasticity::Sphere<LengthType>;
  using real = tfel::types::real<LengthType>;
  using StressType = tfel::types::stress<LengthType>;

  pybind11::class_<Sph>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const Sph&>())
      .def(pybind11::init<>());
  }
  
//template <unsigned short int N,tfel::math::ScalarConcept StressType>
//requires(tfel::math::checkUnitCompatibility<
//         tfel::math::unit::Stress, StressType>())
//static void declarePhase(pybind11::module_& m, const char* const n) {
//  using Ph = tfel::material::homogenization::elasticity::Phase<N,StressType>;
//  using real = tfel::types::real<StressType>;
//  using LengthType = tfel::types::length<StressType>;
//
//  pybind11::class_<Ph>(m, n, pybind11::buffer_protocol())
//      .def(pybind11::init<const Ph&>())
//      .def(pybind11::init<const real , const tfel::math::st2tost2<N, StressType> &>());
//  }
  
  
template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareSphereDistribution(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Sph = tfel::material::homogenization::elasticity::Sphere<LengthType>;
  using SD = tfel::material::homogenization::elasticity::SphereDistribution<StressType>;

  pybind11::class_<SD>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const SD&>())
      .def(pybind11::init<const Sph&,real,const tfel::math::st2tost2<3, StressType> &>());
  }
  
  
template <unsigned short int N,tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareParticulateMicrostructure(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using ID = tfel::material::homogenization::elasticity::SphereDistribution<StressType>;
  using PM = tfel::material::homogenization::elasticity::ParticulateMicrostructure<N,StressType>;
  
  pybind11::class_<PM>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const PM&>())
      .def(pybind11::init<const tfel::math::st2tost2<N, StressType> &>())
      .def("addInclusionPhase",[](PM &pm, ID &inclusionPhase){
          return pm.addInclusionPhase(inclusionPhase);
      });
  }
  

void declareMicrostructure(pybind11::module_&);

void declareMicrostructure(pybind11::module_& m) {
  declareSphere<double>(m, "Sphere");
  //declarePhase<3,double>(m, "Phase3D");
  //declarePhase<2,double>(m, "Phase2D");
  declareSphereDistribution<double>(m, "SphereDistribution");
  declareParticulateMicrostructure<3,double>(m, "ParticulateMicrostructure3D");
  //declareParticulateMicrostructure<2,double>(m, "ParticulateMicrostructure2D");
  
}
