/*!
 * \file   LinearHomogenizationSchemes.cxx
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
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareDistribution(pybind11::module_& m,
                                                        const char* const n) {
  using D =
      tfel::material::homogenization::elasticity::Distribution<StressType>;

  pybind11::class_<D>(m, n, pybind11::buffer_protocol())
      .def(pybind11::init<const D&>())
      .def(pybind11::init<>(
          [](const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
             const tfel::types::length<StressType>& a,
             const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
             const tfel::types::length<StressType>& b,
             const tfel::types::length<StressType>& c) {
            D D_ = {.n_a = n_a, .a = a, .n_b = n_b, .b = b, .c = c};
            return D_;
          }))
      .def_readwrite("n_a", &D::n_a)
      .def_readwrite("a", &D::a)
      .def_readwrite("n_b", &D::n_b)
      .def_readwrite("b", &D::b)
      .def_readwrite("c", &D::c);
}

void declareLinearHomogenizationSchemes(pybind11::module_&);

void declareLinearHomogenizationSchemes(pybind11::module_& m) {
  using namespace tfel::material;
  m.def("computeSphereDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i) {
          return homogenization::elasticity::computeSphereDiluteScheme(
              young, nu, f, young_i, nu_i);
        });
  m.def("computeSphereDiluteScheme", [](const IsotropicModuli<double>& IM,
                                        const double f,
                                        const IsotropicModuli<double>& IMi) {
    return homogenization::elasticity::computeSphereDiluteScheme(IM, f, IMi);
  });
  m.def("computeSphereMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i) {
          return homogenization::elasticity::computeSphereMoriTanakaScheme(
              young, nu, f, young_i, nu_i);
        });
  m.def("computeSphereMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi) {
          return homogenization::elasticity::computeSphereMoriTanakaScheme(
              IM, f, IMi);
        });
  m.def("computeIsotropicDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i, const double a,
           const double b, const double c) {
          return homogenization::elasticity::computeIsotropicDiluteScheme(
              young, nu, f, young_i, nu_i, a, b, c);
        });
  m.def("computeIsotropicDiluteScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi, const double a, const double b,
           const double c) {
          return homogenization::elasticity::computeIsotropicDiluteScheme(
              IM, f, IMi, a, b, c);
        });
  m.def("computeIsotropicMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i, const double a,
           const double b, const double c) {
          return homogenization::elasticity::computeIsotropicMoriTanakaScheme(
              young, nu, f, young_i, nu_i, a, b, c);
        });
  m.def("computeIsotropicMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi, const double a, const double b,
           const double c) {
          return homogenization::elasticity::computeIsotropicMoriTanakaScheme(
              IM, f, IMi, a, b, c);
        });

  declareDistribution<double>(m, "Distribution");
  m.def("computeIsotropicPCWScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i, const double a,
           const double b, const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeIsotropicPCWScheme(
              young, nu, f, young_i, nu_i, a, b, c, D);
        });
  m.def("computeIsotropicPCWScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi, const double a, const double b,
           const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeIsotropicPCWScheme(
              IM, f, IMi, a, b, c, D);
        });

  // TransverseIsotropic
  m.def("computeTransverseIsotropicDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicDiluteScheme(young, nu, f, young_i,
                                                     nu_i, n_a, a, b, c);
        });
  m.def("computeTransverseIsotropicDiluteScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicDiluteScheme(IM, f, IMi, n_a, a, b, c);
        });

  m.def("computeTransverseIsotropicMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicMoriTanakaScheme(young, nu, f, young_i,
                                                         nu_i, n_a, a, b, c);
        });
  m.def("computeTransverseIsotropicMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicMoriTanakaScheme(IM, f, IMi, n_a, a, b,
                                                         c);
        });

  m.def(
      "computeTransverseIsotropicPCWScheme",
      [](const double young, const double nu, const double f,
         const double young_i, const double nu_i,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const double b, const double c,
         const homogenization::elasticity::Distribution<double>& D) {
        return homogenization::elasticity::computeTransverseIsotropicPCWScheme(
            young, nu, f, young_i, nu_i, n_a, a, b, c, D);
      });
  m.def(
      "computeTransverseIsotropicPCWScheme",
      [](const IsotropicModuli<double>& IM, const double f,
         const IsotropicModuli<double>& IMi,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const double b, const double c,
         const homogenization::elasticity::Distribution<double>& D) {
        return homogenization::elasticity::computeTransverseIsotropicPCWScheme(
            IM, f, IMi, n_a, a, b, c, D);
      });

  // Oriented
  m.def("computeOrientedDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedDiluteScheme(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c);
        });
  m.def("computeOrientedDiluteScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedDiluteScheme(
              IM, f, IMi, n_a, a, n_b, b, c);
        });

  m.def("computeOrientedMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedMoriTanakaScheme(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c);
        });
  m.def("computeOrientedMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedMoriTanakaScheme(
              IM, f, IMi, n_a, a, n_b, b, c);
        });

  m.def("computeOrientedPCWScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeOrientedPCWScheme(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c, D);
        });
  m.def("computeOrientedPCWScheme",
        [](const IsotropicModuli<double>& IM, const double f,
           const IsotropicModuli<double>& IMi,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeOrientedPCWScheme(
              IM, f, IMi, n_a, a, n_b, b, c, D);
        });
}
