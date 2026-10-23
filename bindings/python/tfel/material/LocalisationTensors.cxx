/*!
 * \file   LocalisationTensors.cxx
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
#include "TFEL/Material/LocalisationTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"

void declareLocalisationTensors(pybind11::module_&);
using namespace tfel::material;

static const char compute_sphere_localisation_tensor_doc[] =
    "This function builds the strain localisation tensor of a sphere\n"
    "with a general elasticity, embedded in an isotropic matrix.\n"
    "Return an object of type st2tost2\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions";

static const char compute_sphere_localisation_tensor_IM_doc[] =
    "This function is an overload of computeSphereLocalisationTensor\n"
    "with elastic moduli of IsotropicModuli<StressType> type\n"
    "Return an object of type st2tost2\n"
    "IM0: isotropic moduli of the matrix\n"
    "IM_i: isotropic moduli of the inclusions";

static const char compute_axisymmetrical_localisation_tensor_doc[] =
    "This function builds the strain localisation tensor of an\n"
    "axisymmetrical ellipsoid with a general elasticity, embedded in an\n"
    "isotropic matrix. The ellipsoid also has a specific orientation\n"
    "given by the vector $n_a$, axis of the ellipsoid, whose semi-length\n"
    "is $a$.\n"
    "Return an object of type st2tost2\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the axis of the ellipsoid (whose semi-length is\n"
    "$a$)\n"
    "a: length of semi-axis relative to the direction $n_a$";

static const char compute_axisymmetrical_localisation_tensor_IM_doc[] =
    "This function is an overload of\n"
    "computeAxisymmetricalEllipsoidLocalisationTensor with elastic moduli\n"
    "of IsotropicModuli<StressType> type\n"
    "Return an object of type st2tost2\n"
    "IM0: isotropic moduli of the matrix\n"
    "IM_i: isotropic moduli of the inclusions\n"
    "n_a: direction of the axis of the ellipsoid (whose semi-length is\n"
    "$a$)\n"
    "a: length of semi-axis relative to the direction $n_a$";

static const char compute_localisation_tensor_doc[] =
    "This function builds the strain localisation tensor of a general\n"
    "ellipsoid with a general elasticity, embedded in an isotropic\n"
    "matrix. The localisation tensor $A$ is defined as follows :\n"
    "$\\epsilon = A:E_0$ where $E_0$ is the uniform strain tensor imposed\n"
    "at infinity, and $\\epsilon$ is the strain tensor solution of Eshelby\n"
    "problem for the ellipsoid. The ellipsoid also has a specific\n"
    "orientation given by the vectors $n_a$, $n_b$, which provides a\n"
    "direct local basis $(n_a,n_b,n_c)$. The elasticity $C_i$ is\n"
    "specified in this local basis.\n"
    "Return an object of type st2tost2, which is the fourth-order\n"
    "localisation tensor $A$ (in the global basis).\n"
    "IM0: isotropic moduli of the matrix\n"
    "C_i: stiffness of the inclusions\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis";

static const char compute_localisation_tensor_young_nu_doc[] =
    "This function is an overload of computeEllipsoidLocalisationTensor\n"
    "with elastic moduli in the form (young,nu)\n"
    "Return an object of type st2tost2, which is the fourth-order\n"
    "localisation tensor $A$\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis";

static const char compute_localisation_tensor_IM_doc[] =
    "This function is another overload of\n"
    "computeEllipsoidLocalisationTensor with elastic moduli of\n"
    "IsotropicModuli<StressType> type\n"
    "Return an object of type st2tost2, which is the fourth-order\n"
    "localisation tensor $A$\n"
    "IM0: isotropic moduli of the matrix\n"
    "IM_i: isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis";

static const char compute_anisotropic_localisation_tensor_doc[] =
    "This function builds the strain localisation tensor of a general\n"
    "ellipsoid embedded in an ANISOTROPIC matrix. The localisation tensor\n"
    "$A$ is defined as follows : $\\epsilon = A:E_0$ where $E_0$ is the\n"
    "uniform strain tensor imposed at infinity, and $\\epsilon$ is the\n"
    "strain tensor solution of Eshelby problem for the ellipsoid. The\n"
    "ellipsoid also has a specific orientation given by the vectors\n"
    "$n_a$, $n_b$. A LOCAL basis can then be defined as n_a, n_b, n_c\n"
    "where n_c=cross_product(n_a,n_b)\n"
    "Return an object of type st2tost2<3u,real>, which is the\n"
    "fourth-order localisation tensor $A$ in the GLOBAL BASIS\n"
    "C_0_glob: stiffness tensor of the matrix in the GLOBAL basis\n"
    "C_i_loc: stiffness tensor of the inclusion in the LOCAL basis, which\n"
    "is the basis (n_a,n_b,n_c)\n"
    "n_a: direction of the principal axis whose length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose length is $b$\n"
    "b: length of semi-axis relative to the direction $n_b$\n"
    "c: length of the remaining semi-axis\n"
    "max_it: maximal number of iterations for integration";

void declareLocalisationTensors(pybind11::module_& m) {
  m.def("computeSphereLocalisationTensor",
        [](const double young, const double nu, const double young_i,
           const double nu_i) {
          return homogenization::elasticity::computeSphereLocalisationTensor(
              young, nu, young_i, nu_i);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("young_i"),
        pybind11::arg("nu_i"), compute_sphere_localisation_tensor_doc);
  m.def("computeSphereLocalisationTensor",
        [](const IsotropicModuli<double>& IM0,
           const IsotropicModuli<double>& IM_i) {
          return homogenization::elasticity::computeSphereLocalisationTensor(
              IM0, IM_i);
        },
        pybind11::arg("IM0"), pybind11::arg("IM_i"),
        compute_sphere_localisation_tensor_IM_doc);
  m.def("computeAxisymmetricalLocalisationTensor",
        [](const double young, const double nu, const double young_i,
           const double nu_i, const tfel::math::tvector<3u, double>& n_a,
           const double e) {
          return homogenization::elasticity::
              computeAxisymmetricalEllipsoidLocalisationTensor(
                  young, nu, young_i, nu_i, n_a, e);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("young_i"),
        pybind11::arg("nu_i"), pybind11::arg("n_a"), pybind11::arg("e"),
        compute_axisymmetrical_localisation_tensor_doc);
  m.def(
      "computeAxisymmetricalLocalisationTensor",
      [](const IsotropicModuli<double>& IM0,
         const IsotropicModuli<double>& IM_i,
         const tfel::math::tvector<3u, double>& n_a, const double e) {
        return homogenization::elasticity::
            computeAxisymmetricalEllipsoidLocalisationTensor(IM0, IM_i, n_a, e);
      },
      pybind11::arg("IM0"), pybind11::arg("IM_i"), pybind11::arg("n_a"),
      pybind11::arg("e"), compute_axisymmetrical_localisation_tensor_IM_doc);
  m.def("computeLocalisationTensor",
        [](const double young, const double nu, const double young_i,
           const double nu_i, const tfel::math::tvector<3u, double>& n_a,
           const double a, const tfel::math::tvector<3u, double>& n_b,
           const double b, const double c) {
          return homogenization::elasticity::computeEllipsoidLocalisationTensor(
              young, nu, young_i, nu_i, n_a, a, n_b, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("young_i"),
        pybind11::arg("nu_i"), pybind11::arg("n_a"), pybind11::arg("a"),
        pybind11::arg("n_b"), pybind11::arg("b"), pybind11::arg("c"),
        compute_localisation_tensor_young_nu_doc);
  m.def(
      "computeLocalisationTensor",
      [](const IsotropicModuli<double>& IM0,
         const IsotropicModuli<double>& IM_i,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const tfel::math::tvector<3u, double>& n_b, const double b,
         const double c) {
        return homogenization::elasticity::computeEllipsoidLocalisationTensor(
            IM0, IM_i, n_a, a, n_b, b, c);
      },
      pybind11::arg("IM0"), pybind11::arg("IM_i"), pybind11::arg("n_a"),
      pybind11::arg("a"), pybind11::arg("n_b"), pybind11::arg("b"),
      pybind11::arg("c"), compute_localisation_tensor_IM_doc);
  m.def("computeLocalisationTensor",
        [](const IsotropicModuli<double>& IM0,
           const tfel::math::st2tost2<3u, double>& C_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeEllipsoidLocalisationTensor(
              IM0, C_i, n_a, a, n_b, b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("C_i"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("n_b"), pybind11::arg("b"),
        pybind11::arg("c"), compute_localisation_tensor_doc);
  m.def(
      "computeAnisotropicLocalisationTensor",
      [](const tfel::math::st2tost2<3u, double>& C_0_glob,
         const tfel::math::st2tost2<3u, double>& C_i_loc,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const tfel::math::tvector<3u, double>& n_b, const double b,
         const double c, const std::size_t max_it) {
        return homogenization::elasticity::computeAnisotropicLocalisationTensor(
            C_0_glob, C_i_loc, n_a, a, n_b, b, c, max_it);
      },
      pybind11::arg("C_0_glob"), pybind11::arg("C_i_loc"),
      pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("n_b"),
      pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("max_it"),
      compute_anisotropic_localisation_tensor_doc);
}
