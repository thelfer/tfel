/*!
 * \file   LinearHomogenizationSchemes.cxx
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
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"
#include "TFEL/Material/HomogenizationSecondMoments.hxx"

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareDistribution(pybind11::module_& m,
                                                        const char* const n) {
  using D =
      tfel::material::homogenization::elasticity::Distribution<StressType>;

  pybind11::class_<D>(
      m, n, pybind11::buffer_protocol(),
      "Here is a structure that defines a distribution of inclusions for "
      "Ponte-Castaneda & Willis scheme")
      .def(pybind11::init<const D&>())
      .def(pybind11::init<>(
          [](const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_a,
             const tfel::types::length<StressType>& a,
             const tfel::math::tvector<3u, tfel::types::real<StressType>>& n_b,
             const tfel::types::length<StressType>& b,
             const tfel::types::length<StressType>& c) {
            D D_ = {.n_a = n_a, .a = a, .n_b = n_b, .b = b, .c = c};
            return D_;
          }),
           pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("n_b"),
           pybind11::arg("b"), pybind11::arg("c"))
      .def_readwrite("n_a", &D::n_a)
      .def_readwrite("a", &D::a)
      .def_readwrite("n_b", &D::n_b)
      .def_readwrite("b", &D::b)
      .def_readwrite("c", &D::c);
}

void declareLinearHomogenizationSchemes(pybind11::module_&);

static const char compute_sphere_dilute_scheme_doc[] =
    "This function gives the homogenized moduli for a dilute scheme, for\n"
    "spheres.\n"
    "Return an object of type YoungNuModuli<StressType> containing\n"
    "Young modulus and Poisson ratio\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of spheres\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions";

static const char compute_sphere_dilute_scheme_IM_doc[] =
    "This function is an overload of computeSphereDiluteScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type KGModuli<StressType> containing\n"
    "bulk modulus and shear modulus\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of spheres\n"
    "IM_i: Isotropic moduli of the inclusions";

static const char compute_sphere_mori_tanaka_scheme_doc[] =
    "This function gives the homogenized moduli for a Mori-Tanaka scheme,\n"
    "for spheres.\n"
    "Return an object of type YoungNuModuli<StressType> containing\n"
    "Young modulus and Poisson ratio\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of spheres\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions";

static const char compute_sphere_mori_tanaka_scheme_IM_doc[] =
    "This function is an overload of computeSphereMoriTanakaScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type KGModuli<StressType> containing\n"
    "bulk modulus and shear modulus\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of spheres\n"
    "IM_i: Isotropic moduli of the inclusions";

static const char compute_isotropic_dilute_scheme_doc[] =
    "This function gives the homogenized moduli for a dilute scheme, for an\n"
    "isotropic distribution of ellipsoids\n"
    "Return an object of type YoungNuModuli<StressType> containing\n"
    "Young modulus and Poisson ratio\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "a: length of the first semi-axis\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_isotropic_dilute_scheme_IM_doc[] =
    "This function is an overload of computeIsotropicDiluteScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type KGModuli<StressType> containing\n"
    "bulk modulus and shear modulus\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "a: length of the first semi-axis\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_isotropic_mori_tanaka_scheme_doc[] =
    "This function gives the homogenized moduli for a Mori-Tanaka scheme,\n"
    "for an isotropic distribution of ellipsoids\n"
    "Return an object of type YoungNuModuli<StressType> containing\n"
    "Young modulus and Poisson ratio\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "a: length of the first semi-axis\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_isotropic_mori_tanaka_scheme_IM_doc[] =
    "This function is an overload of computeIsotropicMoriTanakaScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type KGModuli<StressType> containing\n"
    "bulk modulus and shear modulus\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "a: length of the first semi-axis\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_transverse_isotropic_dilute_scheme_doc[] =
    "This function gives the homogenized stiffness for a dilute scheme, for\n"
    "a transverse isotropic distribution of ellipsoids. One principal axis\n"
    "(the same principal axis for all ellipsoids) is oriented in a fixed\n"
    "direction n. The distribution of the other axes are in the plane normal\n"
    "to n, and the distribution of these axes inside this plane is\n"
    "isotropic.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis which has a fixed orientation and\n"
    "a semi-length $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_transverse_isotropic_dilute_scheme_IM_doc[] =
    "This function is an overload of computeTransverseIsotropicDiluteScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis which has a fixed orientation and\n"
    "a semi-length $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_transverse_isotropic_mori_tanaka_scheme_doc[] =
    "This function gives the homogenized stiffness for a Mori-Tanaka scheme,\n"
    "for a transverse isotropic distribution of ellipsoids. One principal\n"
    "axis (the same principal axis for all ellipsoids) is oriented in a\n"
    "fixed direction n. The distribution of the other axes are in the plane\n"
    "normal to n, and the distribution of these axes inside this plane is\n"
    "isotropic.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis which has a fixed orientation and\n"
    "a semi-length $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_transverse_isotropic_mori_tanaka_scheme_IM_doc[] =
    "This function is an overload of\n"
    "computeTransverseIsotropicMoriTanakaScheme with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis which has a fixed orientation and\n"
    "a semi-length $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis";

static const char compute_oriented_dilute_scheme_doc[] =
    "This function gives the homogenized stiffness for a dilute scheme, for\n"
    "a distribution of oriented ellipsoids : all principal axes have the\n"
    "same fixed orientation.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose semi-length is $b$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis";

static const char compute_oriented_dilute_scheme_IM_doc[] =
    "This function is an overload of computeOrientedDiluteScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose semi-length is $b$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis";

static const char compute_oriented_mori_tanaka_scheme_doc[] =
    "This function gives the homogenized stiffness for a Mori-Tanaka scheme,\n"
    "for a distribution of oriented ellipsoids : all principal axes have the\n"
    "same fixed orientation.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose semi-length is $b$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis";

static const char compute_oriented_mori_tanaka_scheme_IM_doc[] =
    "This function is an overload of computeOrientedMoriTanakaScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose semi-length is $b$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis";

static const char compute_isotropic_pcw_scheme_doc[] =
    "This function gives the homogenized stiffness for a Ponte-Castaneda and\n"
    "Willis scheme, for a distribution of ellipsoids with the same shape and\n"
    "a uniform isotropic distribution of their orientations\n"
    "(formula 3.27 of Ponte Castaneda 1995).\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "a: length of the first semi-axis (of the ellipsoids)\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis\n"
    "D: Distribution that characterizes the distribution";

static const char compute_isotropic_pcw_scheme_IM_doc[] =
    "This function is an overload of computeIsotropicPCWScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "a: length of the first semi-axis (of the ellipsoids)\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis\n"
    "D: Distribution that characterizes the distribution";

static const char compute_transverse_isotropic_pcw_scheme_doc[] =
    "This function gives the homogenized stiffness for a Ponte-Castaneda and\n"
    "Willis scheme, for a distribution of ellipsoids with the same shape and\n"
    "a transverse isotropic distribution of their orientations\n"
    "(formula 3.27 of Ponte Castaneda 1995).\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$ (related\n"
    "to the shape of the inclusions)\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "b: length of the second semi-axis\n"
    "c: length of the third semi-axis\n"
    "D: Distribution that characterizes the distribution";

static const char compute_transverse_isotropic_pcw_scheme_IM_doc[] =
    "This function is an overload of computeTransverseIsotropicPCWScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$ (related\n"
    "to the shape of the inclusions)\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis\n"
    "D: Distribution that characterizes the distribution";

static const char compute_oriented_pcw_scheme_doc[] =
    "This function gives the homogenized stiffness for a Ponte-Castaneda and\n"
    "Willis scheme, for a distribution of ellipsoids with the same shape and\n"
    "a fixed orientation (formula 3.25 of Ponte Castaneda 1995).\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "young,nu: Young modulus and Poisson's ratio of the matrix\n"
    "f: volumic fraction of ellipsoids\n"
    "young_i,nu_i: Young modulus and Poisson's ratio of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$ (related\n"
    "to the shape of the inclusions)\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose semi-length is $b$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis\n"
    "D: Distribution that characterizes the distribution";

static const char compute_oriented_pcw_scheme_IM_doc[] =
    "This function is an overload of computeTransverseIsotropicPCWScheme\n"
    "with IsotropicModuli.\n"
    "Return an object of type st2tost2<3u,StressType>\n"
    "IM0: Isotropic moduli of the matrix\n"
    "f: volumic fraction of the inclusions\n"
    "IM_i: Isotropic moduli of the inclusions\n"
    "n_a: direction of the principal axis whose semi-length is $a$ (related\n"
    "to the shape of the inclusions)\n"
    "a: length of semi-axis relative to the direction $n_a$\n"
    "n_b: direction of the principal axis whose semi-length is $b$\n"
    "b: length of the semi-axis relative to the direction $n_b$\n"
    "c: length of the third semi-axis\n"
    "D: Distribution that characterizes the distribution";

static const char compute_mean_squared_equivalent_strain_doc[] =
    "This function computes the averages on each phase of the square of the\n"
    "equivalent strain assuming a Hashin-Shtrikman homogenization scheme,\n"
    "and assuming two phases: spherical isotropic inclusions embedded in an\n"
    "isotropic matrix (note that the inclusion can be softer or stiffer than\n"
    "the matrix)\n"
    "Return an object of type std::pair<real<StressType>,real<StressType>>\n"
    "IM0: IsotropicModuli of the matrix\n"
    "f: volumic fraction of inclusions\n"
    "IMi: IsotropicModuli of the spherical inclusions\n"
    "em2: Square of the hydrostatic part of the macroscopic strain (third of\n"
    "the trace of the strain)\n"
    "eeq2: Square of the equivalent macroscopic strain";

static const char compute_mean_squared_hydrostatic_strain_doc[] =
    "This function computes the averages on each phase of the square of the\n"
    "third of the trace of the strain assuming a Hashin-Shtrikman\n"
    "homogenization scheme, and assuming two phases: spherical isotropic\n"
    "inclusions embedded in an isotropic matrix (note that the inclusion can\n"
    "be softer or stiffer than the matrix)\n"
    "Return an object of type std::pair<real<StressType>,real<StressType>>\n"
    "IM0: IsotropicModuli of the matrix\n"
    "f: volumic fraction of inclusions\n"
    "IMi: IsotropicModuli of the spherical inclusions\n"
    "em2: Square of the hydrostatic part of the macroscopic strain (third of\n"
    "the trace of the strain)\n"
    "eeq2: Square of the equivalent macroscopic strain";

void declareLinearHomogenizationSchemes(pybind11::module_& m) {
  using namespace tfel::material;
  m.def("computeSphereDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i) {
          return homogenization::elasticity::computeSphereDiluteScheme(
              young, nu, f, young_i, nu_i);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"),
        compute_sphere_dilute_scheme_doc);
  m.def("computeSphereDiluteScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i) {
          return homogenization::elasticity::computeSphereDiluteScheme(
              IM0, f, IM_i);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        compute_sphere_dilute_scheme_IM_doc);
  m.def("computeSphereMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i) {
          return homogenization::elasticity::computeSphereMoriTanakaScheme(
              young, nu, f, young_i, nu_i);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"),
        compute_sphere_mori_tanaka_scheme_doc);
  m.def("computeSphereMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i) {
          return homogenization::elasticity::computeSphereMoriTanakaScheme(
              IM0, f, IM_i);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        compute_sphere_mori_tanaka_scheme_IM_doc);
  m.def("computeIsotropicDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i, const double a,
           const double b, const double c) {
          return homogenization::elasticity::computeIsotropicDiluteScheme(
              young, nu, f, young_i, nu_i, a, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("a"),
        pybind11::arg("b"), pybind11::arg("c"),
        compute_isotropic_dilute_scheme_doc);
  m.def("computeIsotropicDiluteScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i, const double a, const double b,
           const double c) {
          return homogenization::elasticity::computeIsotropicDiluteScheme(
              IM0, f, IM_i, a, b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
        compute_isotropic_dilute_scheme_IM_doc);
  m.def("computeIsotropicMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i, const double a,
           const double b, const double c) {
          return homogenization::elasticity::computeIsotropicMoriTanakaScheme(
              young, nu, f, young_i, nu_i, a, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("a"),
        pybind11::arg("b"), pybind11::arg("c"),
        compute_isotropic_mori_tanaka_scheme_doc);
  m.def("computeIsotropicMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i, const double a, const double b,
           const double c) {
          return homogenization::elasticity::computeIsotropicMoriTanakaScheme(
              IM0, f, IM_i, a, b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
        compute_isotropic_mori_tanaka_scheme_IM_doc);

  declareDistribution<double>(m, "Distribution");
  m.def("computeIsotropicPCWScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i, const double a,
           const double b, const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeIsotropicPCWScheme(
              young, nu, f, young_i, nu_i, a, b, c, D);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("a"),
        pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("D"),
        compute_isotropic_pcw_scheme_doc);
  m.def("computeIsotropicPCWScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i, const double a, const double b,
           const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeIsotropicPCWScheme(
              IM0, f, IM_i, a, b, c, D);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
        pybind11::arg("D"), compute_isotropic_pcw_scheme_IM_doc);

  // TransverseIsotropic
  m.def("computeTransverseIsotropicDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicDiluteScheme(young, nu, f, young_i,
                                                     nu_i, n_a, a, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
        compute_transverse_isotropic_dilute_scheme_doc);
  m.def("computeTransverseIsotropicDiluteScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicDiluteScheme(IM0, f, IM_i, n_a, a, b,
                                                     c);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("b"),
        pybind11::arg("c"), compute_transverse_isotropic_dilute_scheme_IM_doc);

  m.def("computeTransverseIsotropicMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicMoriTanakaScheme(young, nu, f, young_i,
                                                         nu_i, n_a, a, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
        compute_transverse_isotropic_mori_tanaka_scheme_doc);
  m.def("computeTransverseIsotropicMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const double b, const double c) {
          return homogenization::elasticity::
              computeTransverseIsotropicMoriTanakaScheme(IM0, f, IM_i, n_a, a,
                                                         b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("b"),
        pybind11::arg("c"),
        compute_transverse_isotropic_mori_tanaka_scheme_IM_doc);

  m.def(
      "computeTransverseIsotropicPCWScheme",
      [](const double young, const double nu, const double f,
         const double young_i, const double nu_i,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const double b, const double c,
         const homogenization::elasticity::Distribution<double>& D) {
        return homogenization::elasticity::computeTransverseIsotropicPCWScheme(
            young, nu, f, young_i, nu_i, n_a, a, b, c, D);
      },
      pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
      pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("n_a"),
      pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
      pybind11::arg("D"), compute_transverse_isotropic_pcw_scheme_doc);
  m.def(
      "computeTransverseIsotropicPCWScheme",
      [](const IsotropicModuli<double>& IM0, const double f,
         const IsotropicModuli<double>& IM_i,
         const tfel::math::tvector<3u, double>& n_a, const double a,
         const double b, const double c,
         const homogenization::elasticity::Distribution<double>& D) {
        return homogenization::elasticity::computeTransverseIsotropicPCWScheme(
            IM0, f, IM_i, n_a, a, b, c, D);
      },
      pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
      pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("b"),
      pybind11::arg("c"), pybind11::arg("D"),
      compute_transverse_isotropic_pcw_scheme_IM_doc);

  // Oriented
  m.def("computeOrientedDiluteScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedDiluteScheme(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("n_b"), pybind11::arg("b"),
        pybind11::arg("c"), compute_oriented_dilute_scheme_doc);
  m.def("computeOrientedDiluteScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedDiluteScheme(
              IM0, f, IM_i, n_a, a, n_b, b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("n_b"),
        pybind11::arg("b"), pybind11::arg("c"),
        compute_oriented_dilute_scheme_IM_doc);

  m.def("computeOrientedMoriTanakaScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedMoriTanakaScheme(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("n_b"), pybind11::arg("b"),
        pybind11::arg("c"), compute_oriented_mori_tanaka_scheme_doc);
  m.def("computeOrientedMoriTanakaScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c) {
          return homogenization::elasticity::computeOrientedMoriTanakaScheme(
              IM0, f, IM_i, n_a, a, n_b, b, c);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("n_b"),
        pybind11::arg("b"), pybind11::arg("c"),
        compute_oriented_mori_tanaka_scheme_IM_doc);

  m.def("computeOrientedPCWScheme",
        [](const double young, const double nu, const double f,
           const double young_i, const double nu_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeOrientedPCWScheme(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c, D);
        },
        pybind11::arg("young"), pybind11::arg("nu"), pybind11::arg("f"),
        pybind11::arg("young_i"), pybind11::arg("nu_i"), pybind11::arg("n_a"),
        pybind11::arg("a"), pybind11::arg("n_b"), pybind11::arg("b"),
        pybind11::arg("c"), pybind11::arg("D"), compute_oriented_pcw_scheme_doc);
  m.def("computeOrientedPCWScheme",
        [](const IsotropicModuli<double>& IM0, const double f,
           const IsotropicModuli<double>& IM_i,
           const tfel::math::tvector<3u, double>& n_a, const double a,
           const tfel::math::tvector<3u, double>& n_b, const double b,
           const double c,
           const homogenization::elasticity::Distribution<double>& D) {
          return homogenization::elasticity::computeOrientedPCWScheme(
              IM0, f, IM_i, n_a, a, n_b, b, c, D);
        },
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IM_i"),
        pybind11::arg("n_a"), pybind11::arg("a"), pybind11::arg("n_b"),
        pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("D"),
        compute_oriented_pcw_scheme_IM_doc);
  m.def("computeMeanSquaredEquivalentStrain",
        &homogenization::elasticity::computeMeanSquaredEquivalentStrain<double>,
        pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IMi"),
        pybind11::arg("em2"), pybind11::arg("eeq2"),
        compute_mean_squared_equivalent_strain_doc);
  m.def(
      "computeMeanSquaredHydrostaticStrain",
      &homogenization::elasticity::computeMeanSquaredHydrostaticStrain<double>,
      pybind11::arg("IM0"), pybind11::arg("f"), pybind11::arg("IMi"),
      pybind11::arg("em2"), pybind11::arg("eeq2"),
      compute_mean_squared_hydrostatic_strain_doc);
}
