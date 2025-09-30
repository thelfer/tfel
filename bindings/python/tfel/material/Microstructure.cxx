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
static void declareEllipse(pybind11::module_& m, const char* const n) {
  using Elli = tfel::material::homogenization::elasticity::Ellipse<LengthType>;
  using real = tfel::types::real<LengthType>;
  using StressType = tfel::types::stress<LengthType>;

  pybind11::class_<Elli>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const Elli&>())
      .def(pybind11::init<const LengthType&,const LengthType&>());
  }
  
template <tfel::math::ScalarConcept LengthType>
    requires(
        tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                           LengthType>())
static void declareDisk(pybind11::module_& m, const char* const n) {
  using Di = tfel::material::homogenization::elasticity::Disk<LengthType>;
  using real = tfel::types::real<LengthType>;
  using StressType = tfel::types::stress<LengthType>;

  pybind11::class_<Di>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const Di&>())
      .def(pybind11::init<>());
  }
  
  
  
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
  
  
template <tfel::math::ScalarConcept LengthType>
    requires(
        tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                           LengthType>())
static void declareEllipsoid(pybind11::module_& m, const char* const n) {
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using real = tfel::types::real<LengthType>;
  using StressType = tfel::types::stress<LengthType>;

  pybind11::class_<Ell>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const Ell&>())
      .def(pybind11::init<const LengthType&,const LengthType&,const LengthType&>());
  }

template <tfel::math::ScalarConcept LengthType>
    requires(
        tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                           LengthType>())
static void declareSpheroid(pybind11::module_& m, const char* const n) {
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using real = tfel::types::real<LengthType>;
  using StressType = tfel::types::stress<LengthType>;

  pybind11::class_<Sphe>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const Sphe&>())
      .def(pybind11::init<const LengthType&,const LengthType&>());
  }
  
  
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
      .def(pybind11::init<const Sph&,real,const tfel::math::st2tost2<3, StressType> &>())
      .def("computeMeanLocalisator",[](SD &sd,const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12){
          return sd.computeMeanLocalisator(C0,isotropic_matrix,verbose,max_iter_anisotropic_integration);
      });
  }
  
  
template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareIsotropicDistribution(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using IsoD = tfel::material::homogenization::elasticity::IsotropicDistribution<StressType>;

  pybind11::class_<IsoD>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const IsoD&>())
      .def(pybind11::init<const Ell&,real,const tfel::math::st2tost2<3, StressType> &>())
      .def(pybind11::init<const Sphe&,real,const tfel::math::st2tost2<3, StressType> &>())
      .def("computeMeanLocalisator",[](IsoD &isod, const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12){
          return isod.computeMeanLocalisator(C0,isotropic_matrix,verbose,max_iter_anisotropic_integration);
      });
  }
  
template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareTransverseIsotropicDistribution(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using TID = tfel::material::homogenization::elasticity::TransverseIsotropicDistribution<StressType>;

  pybind11::class_<TID>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const TID&>())
      .def(pybind11::init<const Ell&,real,const tfel::math::st2tost2<3, StressType> &, const tfel::math::tvector<3, real> &, unsigned short int &  >())
      .def(pybind11::init<const Sphe&,real,const tfel::math::st2tost2<3, StressType> &, const tfel::math::tvector<3, real> &, unsigned short int & >())
      .def("computeMeanLocalisator",[](TID &tid, const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12){
          return tid.computeMeanLocalisator(C0,isotropic_matrix,verbose,max_iter_anisotropic_integration);
      });
  }
  
template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
static void declareOrientedDistribution(pybind11::module_& m, const char* const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using OD = tfel::material::homogenization::elasticity::OrientedDistribution<StressType>;

  pybind11::class_<OD>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const OD&>())
      .def(pybind11::init<const Ell&,real,const tfel::math::st2tost2<3, StressType> &, const tfel::math::tvector<3, real> &, const tfel::math::tvector<3, real> &  >())
      .def(pybind11::init<const Sphe&,real,const tfel::math::st2tost2<3, StressType> &, const tfel::math::tvector<3, real> &, const tfel::math::tvector<3, real> & >())
      .def("computeMeanLocalisator",[](OD &od, const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12){
          return od.computeMeanLocalisator(C0,isotropic_matrix,verbose,max_iter_anisotropic_integration);
      });
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
      })
      .def("removeInclusionPhase",[](PM &pm, unsigned int i){
          return pm.removeInclusionPhase(i);
      })
      .def("get_number_of_phases",[](PM &pm){
          return pm.get_number_of_phases();
      })
      .def("get_matrix_fraction",[](PM &pm){
          return pm.get_matrix_fraction();
      })
      .def("get_matrix_elasticity",[](PM &pm){
          return pm.get_matrix_elasticity();
      })
      .def("get_inclusionPhase",[](PM &pm, unsigned int i){
          return pm.get_inclusionPhase(i);
      });
  }
  

void declareMicrostructure(pybind11::module_&);

void declareMicrostructure(pybind11::module_& m) {
  declareEllipse<double>(m, "Ellipse");
  declareDisk<double>(m, "Disk");
  declareSphere<double>(m, "Sphere");
  declareEllipsoid<double>(m, "Ellipsoid");
  declareSpheroid<double>(m, "Spheroid");
  declareSphereDistribution<double>(m, "SphereDistribution");
  declareIsotropicDistribution<double>(m, "IsotropicDistribution");
  declareTransverseIsotropicDistribution<double>(m, "TransverseIsotropicDistribution");
  declareOrientedDistribution<double>(m, "OrientedDistribution");
  declareParticulateMicrostructure<3,double>(m, "ParticulateMicrostructure3D");
 
  
}
