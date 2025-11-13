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

template <unsigned short int N, tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareInclusion(pybind11::module_ &m,
                                                     const char *const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<N, LengthType>;
  pybind11::class_<I, std::shared_ptr<I>>(m, n, pybind11::buffer_protocol());
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareDisk(pybind11::module_ &m,
                                                const char *const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<2u, LengthType>;
  using Di = tfel::material::homogenization::elasticity::Disk<LengthType>;
  pybind11::class_<Di, I, std::shared_ptr<Di>>(m, n,
                                               pybind11::buffer_protocol())
      .def(pybind11::init<const Di &>())
      .def(pybind11::init<>());
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareEllipse(pybind11::module_ &m,
                                                   const char *const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<2u, LengthType>;
  using Elli = tfel::material::homogenization::elasticity::Ellipse<LengthType>;
  pybind11::class_<Elli, I, std::shared_ptr<Elli>>(m, n,
                                                   pybind11::buffer_protocol())
      .def(pybind11::init<const Elli &>())
      .def(pybind11::init<const LengthType &, const LengthType &>())
      .def("a",
           [](Elli &elli) {
             const auto arr = elli.semiLengths;
             return arr[0];
           })
      .def("b", [](Elli &elli) {
        const auto arr = elli.semiLengths;
        return arr[1];
      });
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareSphere(pybind11::module_ &m,
                                                  const char *const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<3u, LengthType>;
  using Sph = tfel::material::homogenization::elasticity::Sphere<LengthType>;
  pybind11::class_<Sph, I, std::shared_ptr<Sph>>(m, n,
                                                 pybind11::buffer_protocol())
      .def(pybind11::init<const Sph &>())
      .def(pybind11::init<>());
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareSpheroid(pybind11::module_ &m,
                                                    const char *const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<3u, LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  pybind11::class_<Sphe, I, std::shared_ptr<Sphe>>(m, n,
                                                   pybind11::buffer_protocol())
      .def(pybind11::init<const Sphe &>())
      .def(pybind11::init<const LengthType &, const LengthType &>())
      .def("axis_length",
           [](Sphe &sphe) {
             const auto arr = sphe.semiLengths;
             return arr[0];
           })
      .def("transverse_length", [](Sphe &sphe) {
        const auto arr = sphe.semiLengths;
        return arr[1];
      });
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareEllipsoid(pybind11::module_ &m,
                                                     const char *const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<3u, LengthType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  pybind11::class_<Ell, I, std::shared_ptr<Ell>>(m, n,
                                                 pybind11::buffer_protocol())
      .def(pybind11::init<const Ell &>())
      .def(pybind11::init<const LengthType &, const LengthType &,
                          const LengthType &>())
      .def_readwrite("semi_lengths", &Ell::semiLengths);
}

template <unsigned short int N, tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareInclusionDistribution(pybind11::
                                                                     module_ &m,
                                                                 const char
                                                                     *const n) {
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      N, StressType>;
  pybind11::class_<ID, std::shared_ptr<ID>>(m, n, pybind11::buffer_protocol());
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareSphereDistribution(pybind11::module_
                                                                  &m,
                                                              const char
                                                                  *const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Sph = tfel::material::homogenization::elasticity::Sphere<LengthType>;
  using SD = tfel::material::homogenization::elasticity::SphereDistribution<
      StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<SD, ID, std::shared_ptr<SD>>(m, n,
                                                pybind11::buffer_protocol())
      .def(pybind11::init<const SD &>())
      .def(pybind11::init<const Sph &, real,
                          const tfel::math::st2tost2<3u, StressType> &>())
      .def(
          pybind11::init<const Sph &, real,
                         const tfel::material::IsotropicModuli<StressType> &>())
      .def_readwrite("inclusion", &SD::inclusion)
      .def_readwrite("fraction", &SD::fraction)
      .def_readwrite("stiffness", &SD::stiffness)
      .def("is_isotropic", &SD::is_isotropic)
      .def("computeMeanLocalisator",
           [](SD &sd, const tfel::material::IsotropicModuli<StressType> &IM0) {
             return sd.computeMeanLocalisator(IM0);
           })
      .def(
          "computeMeanLocalisator",
          [](SD &sd, const tfel::math::st2tost2<3u, StressType> &C0,
             int max_iter_anisotropic_integration) {
            return sd.computeMeanLocalisator(C0,
                                             max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareIsotropicDistribution(pybind11::
                                                                     module_ &m,
                                                                 const char
                                                                     *const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using IsoD =
      tfel::material::homogenization::elasticity::IsotropicDistribution<
          StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<IsoD, ID, std::shared_ptr<IsoD>>(m, n,
                                                    pybind11::buffer_protocol())
      .def(pybind11::init<const IsoD &>())
      .def(
          pybind11::init<const Ell &, real,
                         const tfel::material::IsotropicModuli<StressType> &>())
      .def(
          pybind11::init<const Sphe &, real,
                         const tfel::material::IsotropicModuli<StressType> &>())
      .def_readwrite("inclusion", &IsoD::inclusion)
      .def_readwrite("fraction", &IsoD::fraction)
      .def_readwrite("stiffness", &IsoD::stiffness)
      .def("is_isotropic", &IsoD::is_isotropic)
      .def("computeMeanLocalisator",
           [](IsoD &isod,
              const tfel::material::IsotropicModuli<StressType> &IM0) {
             return isod.computeMeanLocalisator(IM0);
           })
      .def(
          "computeMeanLocalisator",
          [](IsoD &isod, const tfel::math::st2tost2<3u, StressType> &C0,
             int max_iter_anisotropic_integration) {
            return isod.computeMeanLocalisator(
                C0, max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareTransverseIsotropicDistribution(pybind11::module_
                                                                              &m,
                                                                          const char
                                                                              *const
                                                                                  n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using TID = tfel::material::homogenization::elasticity::
      TransverseIsotropicDistribution<StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<TID, ID, std::shared_ptr<TID>>(m, n,
                                                  pybind11::buffer_protocol())
      .def(pybind11::init<const TID &>())
      .def(pybind11::init<const Ell &, real,
                          const tfel::material::IsotropicModuli<StressType> &,
                          const tfel::math::tvector<3, real> &,
                          unsigned short int &>())
      .def(pybind11::init<const Sphe &, real,
                          const tfel::material::IsotropicModuli<StressType> &,
                          const tfel::math::tvector<3, real> &,
                          unsigned short int &>())
      .def_readwrite("inclusion", &TID::inclusion)
      .def_readwrite("fraction", &TID::fraction)
      .def_readwrite("stiffness", &TID::stiffness)
      .def("is_isotropic", &TID::is_isotropic)
      .def(
          "computeMeanLocalisator",
          [](TID &tid, const tfel::material::IsotropicModuli<StressType> &IM0) {
            return tid.computeMeanLocalisator(IM0);
          })
      .def(
          "computeMeanLocalisator",
          [](TID &tid, const tfel::math::st2tost2<3u, StressType> &C0,
             int max_iter_anisotropic_integration) {
            return tid.computeMeanLocalisator(C0,
                                              max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareOrientedDistribution(pybind11::module_
                                                                   &m,
                                                               const char
                                                                   *const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using OD = tfel::material::homogenization::elasticity::OrientedDistribution<
      StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<OD, ID, std::shared_ptr<OD>>(m, n,
                                                pybind11::buffer_protocol())
      .def(pybind11::init<const OD &>())
      .def(pybind11::init<const Ell &, real,
                          const tfel::math::st2tost2<3, StressType> &,
                          const tfel::math::tvector<3, real> &,
                          const tfel::math::tvector<3, real> &>())
      .def(pybind11::init<const Sphe &, real,
                          const tfel::math::st2tost2<3, StressType> &,
                          const tfel::math::tvector<3, real> &,
                          const tfel::math::tvector<3, real> &>())
      .def(pybind11::init<const Ell &, real,
                          const tfel::material::IsotropicModuli<StressType> &,
                          const tfel::math::tvector<3, real> &,
                          const tfel::math::tvector<3, real> &>())
      .def(pybind11::init<const Sphe &, real,
                          const tfel::material::IsotropicModuli<StressType> &,
                          const tfel::math::tvector<3, real> &,
                          const tfel::math::tvector<3, real> &>())
      .def_readwrite("inclusion", &OD::inclusion)
      .def_readwrite("fraction", &OD::fraction)
      .def_readwrite("stiffness", &OD::stiffness)
      .def("is_isotropic", &OD::is_isotropic)
      .def("computeMeanLocalisator",
           [](OD &od, const tfel::material::IsotropicModuli<StressType> &IM0) {
             return od.computeMeanLocalisator(IM0);
           })
      .def(
          "computeMeanLocalisator",
          [](OD &od, const tfel::math::st2tost2<3u, StressType> &C0,
             int max_iter_anisotropic_integration) {
            return od.computeMeanLocalisator(C0,
                                             max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <unsigned short int N, tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareParticulateMicrostructure(pybind11::
                                                                        module_
                                                                            &m,
                                                                    const char
                                                                        *const
                                                                            n) {
  using PM =
      tfel::material::homogenization::elasticity::ParticulateMicrostructure<
          N, StressType>;

  pybind11::class_<PM>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const PM &>())
      .def(pybind11::init<const tfel::math::st2tost2<N, StressType> &>())
      .def(
          pybind11::init<const tfel::material::IsotropicModuli<StressType> &>())
      .def("addInclusionPhase", &PM::addInclusionPhase)
      .def("removeInclusionPhase", &PM::removeInclusionPhase)
      .def("replaceMatrixPhase",
           [](PM &pm, const tfel::math::st2tost2<N, StressType> &C0) {
             return pm.replaceMatrixPhase(C0);
           })
      .def("replaceMatrixPhase",
           [](PM &pm, const tfel::material::IsotropicModuli<StressType> &IM0) {
             return pm.replaceMatrixPhase(IM0);
           })
      .def("get_number_of_phases", &PM::get_number_of_phases)
      .def("get_matrix_fraction", &PM::get_matrix_fraction)
      .def("get_matrix_elasticity", &PM::get_matrix_elasticity)
      .def("is_isotropic_matrix", &PM::is_isotropic_matrix)
      .def("get_inclusionPhase", &PM::get_inclusionPhase);
}

void declareMicrostructure(pybind11::module_ &);

void declareMicrostructure(pybind11::module_ &m) {
  declareInclusion<3u, double>(m, "Inclusion3D");
  declareInclusion<2u, double>(m, "Inclusion2D");
  declareEllipse<double>(m, "Ellipse");
  declareDisk<double>(m, "Disk");
  declareSphere<double>(m, "Sphere");
  declareSpheroid<double>(m, "Spheroid");
  declareEllipsoid<double>(m, "Ellipsoid");
  declareInclusionDistribution<3u, double>(m, "InclusionDistribution");
  declareSphereDistribution<double>(m, "SphereDistribution");
  declareIsotropicDistribution<double>(m, "IsotropicDistribution");
  declareTransverseIsotropicDistribution<double>(
      m, "TransverseIsotropicDistribution");
  declareOrientedDistribution<double>(m, "OrientedDistribution");
  declareParticulateMicrostructure<3, double>(m, "ParticulateMicrostructure");
}
