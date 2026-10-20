/*!
 * \file   HillTensors.cxx
 * \brief
 * \author Antoine MARTIN
 * \date 14/05/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
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

void declareHillTensors(pybind11::module_&);

using namespace tfel::material;
void declareHillTensors(pybind11::module_& m) {
  m.def("computeSphereHillTensor",
        [](const double young, const double nu) {
          return homogenization::elasticity::computeSphereHillPolarisationTensor<
              double>(young, nu);
        },
        pybind11::arg("young"), pybind11::arg("nu"));
  m.def("computeSphereHillTensor",
        [](const IsotropicModuli<double>& IM) {
          return homogenization::elasticity::computeSphereHillPolarisationTensor<
              double>(IM);
        },
        pybind11::arg("IM"));
  m.def("computeAxisymmetricalHillTensor",
        [](const double young, const double nu,
           const tfel::math::tvector<3u, double>& n_a, const double e) {
          return homogenization::elasticity::
              computeAxisymmetricalHillPolarisationTensor<double>(young, nu,
                                                                  n_a, e);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("n_a"),
        pybind11::arg("e"));
  m.def("computeAxisymmetricalHillTensor",
        [](const IsotropicModuli<double>& IM,
           const tfel::math::tvector<3u, double>& n_a, const double e) {
          return homogenization::elasticity::
              computeAxisymmetricalHillPolarisationTensor<double>(IM, n_a, e);
        },
        pybind11::arg("IM"), pybind11::arg("n_a"), pybind11::arg("e"));
  m.def("computeHillTensor",
        [](const double young, const double nu,
           const tfel::math::tvector<3u, double>& n_a,
           const double a,
           const tfel::math::tvector<3u, double>& n_b,
           const double b, const double c) {
          return homogenization::elasticity::computeHillPolarisationTensor<double>(
              young, nu, n_a, a, n_b, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("n_b"), pybind11::arg("b"),
        pybind11::arg("c"));
  m.def("computeHillTensor",
        [](const IsotropicModuli<double>& IM,
           const tfel::math::tvector<3u, double>& n_a,
           const double a,
           const tfel::math::tvector<3u, double>& n_b,
           const double b, const double c) {
          return homogenization::elasticity::computeHillPolarisationTensor<double>(
              IM, n_a, a, n_b, b, c);
        },
        pybind11::arg("IM"), pybind11::arg("n_a"), pybind11::arg("a"),
        pybind11::arg("n_b"), pybind11::arg("b"), pybind11::arg("c"));
  m.def(
      "computeAnisotropicHillTensor",
      [](const tfel::math::st2tost2<3u, double>& C0,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const tfel::math::tvector<3u, double>& n_b, const double b,
         const double c, const std::size_t& max_it) {
        return homogenization::elasticity::computeAnisotropicHillTensor<double>(
            C0, n_a, a, n_b, b, c, max_it);
      },
      pybind11::arg("C0"), pybind11::arg("n_a"), pybind11::arg("a"),
      pybind11::arg("n_b"), pybind11::arg("b"), pybind11::arg("c"),
      pybind11::arg("max_it"));
}
