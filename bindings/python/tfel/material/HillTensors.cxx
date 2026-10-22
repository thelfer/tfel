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

static const char compute_sphere_hill_tensor_doc[] =
    "This function builds the Hill tensor of a sphere embedded in an\n"
    "isotropic matrix.\n"
    "Return an object of type st2tost2<3u,compliance<StressType>>\n"
    "young: Young modulus of the matrix\n"
    "nu: Poisson's ratio of the matrix";

static const char compute_sphere_hill_tensor_IM_doc[] =
    "This function is an overload of computeSphereHillPolarisationTensor\n"
    "with elastic moduli of IsotropicModuli<StressType> type\n"
    "Return an object of type st2tost2<3u,compliance<StressType>>\n"
    "IM0: isotropic moduli of the matrix";

static const char compute_axisymmetrical_hill_tensor_doc[] =
    "This function builds the Hill tensor of an axisymmetrical ellipsoid\n"
    "embedded in an isotropic matrix. The function takes into account the\n"
    "orientation of the ellipsoid.\n"
    "Return an object of type st2tost2<3u,compliance<StressType>>\n"
    "young: Young modulus of the matrix\n"
    "nu: Poisson's ratio of the matrix\n"
    "n_a: direction of the principal axis\n"
    "e: aspect ratio of the ellipsoid (e>1 : prolate, e<1 : oblate)";

static const char compute_axisymmetrical_hill_tensor_IM_doc[] =
    "This function is an overload of\n"
    "computeAxisymmetricalHillPolarisationTensor with elastic moduli of\n"
    "IsotropicModuli<StressType> type\n"
    "Return an object of type st2tost2<3u,compliance<StressType>>\n"
    "IM0: isotropic moduli of the matrix";

static const char compute_hill_tensor_doc[] =
    "This function builds the Hill tensor of a general ellipsoid embedded\n"
    "in an isotropic matrix. The function takes into account the orientation\n"
    "of the ellipsoid.\n"
    "Return an object of type st2tost2<3u,compliance<StressType>>\n"
    "young: Young modulus of the matrix\n"
    "nu: Poisson's ratio of the matrix\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis";

static const char compute_hill_tensor_IM_doc[] =
    "This function is an overload of computeHillPolarisationTensor\n"
    "with elastic moduli of IsotropicModuli<StressType> type\n"
    "Return an object of type st2tost2<3u,compliance<StressType>>\n"
    "IM0: isotropic moduli of the matrix\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis";

static const char compute_anisotropic_hill_tensor_doc[] =
    "This function builds the Hill tensor of a general ellipsoid embedded in\n"
    "an ANISOTROPIC matrix. The function returns the Hill tensor in the\n"
    "global basis\n"
    "Return an object of type st2tost2<3u,types::compliance<StressType>>\n"
    "C: stiffness tensor of the matrix in the global basis\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis\n"
    "max_it: maximal number of iterations for integration";

void declareHillTensors(pybind11::module_& m) {
  m.def("computeSphereHillTensor",
        [](const double young, const double nu) {
          return homogenization::elasticity::computeSphereHillPolarisationTensor<
              double>(young, nu);
        },
        pybind11::arg("young"), pybind11::arg("nu"),
        compute_sphere_hill_tensor_doc);
  m.def("computeSphereHillTensor",
        [](const IsotropicModuli<double>& IM0) {
          return homogenization::elasticity::computeSphereHillPolarisationTensor<
              double>(IM0);
        },
        pybind11::arg("IM0"), compute_sphere_hill_tensor_IM_doc);
  m.def("computeAxisymmetricalHillTensor",
        [](const double young, const double nu,
           const tfel::math::tvector<3u, double>& n_a, const double e) {
          return homogenization::elasticity::
              computeAxisymmetricalHillPolarisationTensor<double>(young, nu,
                                                                  n_a, e);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("n_a"),
        pybind11::arg("e"), compute_axisymmetrical_hill_tensor_doc);
  m.def("computeAxisymmetricalHillTensor",
        [](const IsotropicModuli<double>& IM0,
           const tfel::math::tvector<3u, double>& n_a, const double e) {
          return homogenization::elasticity::
              computeAxisymmetricalHillPolarisationTensor<double>(IM0, n_a, e);
        },
        pybind11::arg("IM0"), pybind11::arg("n_a"), pybind11::arg("e"),
        compute_axisymmetrical_hill_tensor_IM_doc);
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
        pybind11::arg("c"), compute_hill_tensor_doc);
  m.def("computeHillTensor",
        [](const IsotropicModuli<double>& IM0,
           const tfel::math::tvector<3u, double>& n_a,
           const double a,
           const tfel::math::tvector<3u, double>& n_b,
           const double b, const double c) {
          return homogenization::elasticity::computeHillPolarisationTensor<double>(
              IM0, n_a, a, n_b, b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("n_a"), pybind11::arg("a"),
        pybind11::arg("n_b"), pybind11::arg("b"), pybind11::arg("c"),
        compute_hill_tensor_IM_doc);
  m.def(
      "computeAnisotropicHillTensor",
      [](const tfel::math::st2tost2<3u, double>& C,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const tfel::math::tvector<3u, double>& n_b, const double b,
         const double c, const std::size_t& max_it) {
        return homogenization::elasticity::computeAnisotropicHillTensor<double>(
            C, n_a, a, n_b, b, c, max_it);
      },
      pybind11::arg("C"), pybind11::arg("n_a"), pybind11::arg("a"),
      pybind11::arg("n_b"), pybind11::arg("b"), pybind11::arg("c"),
      pybind11::arg("max_it"), compute_anisotropic_hill_tensor_doc);
}
