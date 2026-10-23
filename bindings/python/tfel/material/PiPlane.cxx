
/*!
 * \file   bindings/python/tfel/PiPlane.cxx
 * \brief
 * \author Thomas Helfer
 * \date 18/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/PiPlane.hxx"

template <unsigned short N>
static std::tuple<double, double> projectOnPiPlane(
    const tfel::math::stensor<N, double>& s) {
  return tfel::material::projectOnPiPlane(s);
}  // end of projectOnPiPlane

void declarePiPlane(pybind11::module_&);

static const char project_on_pi_plane_scalar_doc[] =
    "This function project a stress state defined by the the principal\n"
    "stress axes $\\left(S_{0},S_{1},S_{2}\\right)$ on the $\\pi$-plane,\n"
    "which is defined by: $S_{0}+S_{1}+S_{2}=0$\n"
    "A basis of the $\\pi$-plane is defined by the vectors $n_{0}$ and\n"
    "$n_{1}$\n"
    "s0: first eigen value\n"
    "s1: second eigen value\n"
    "s2: third eigen value\n"
    "Return the projection of the stresses on the $\\pi$-plane";

static const char project_on_pi_plane_stensor_doc[] =
    "This function project a stress state defined by the the principal\n"
    "stress axes $\\left(S_{0},S_{1},S_{2}\\right)$ on the $\\pi$-plane,\n"
    "which is defined by: $S_{0}+S_{1}+S_{2}=0$\n"
    "A basis of the $\\pi$-plane is defined by the vectors $n_{0}$ and\n"
    "$n_{1}$\n"
    "s: stress tensor\n"
    "Return the projection of the stresses on the $\\pi$-plane";

static const char build_from_pi_plane_doc[] =
    "This function returns the stress state\n"
    "$s_{0}\\,\\vec{n}_{0}+s_{0}\\,\\vec{n}_{1}$, defined by three\n"
    "eigenvalues, where $\\vec{n}_{0}$ and $\\vec{n}_{1}$ is the basis of\n"
    "the $\\pi$-plane\n"
    "s0: first position in the $\\pi$-plane.\n"
    "s1: second position in the $\\pi$-plane.\n"
    "Return a stress state corresponding to the point in the\n"
    "$\\pi$-plane.";

void declarePiPlane(pybind11::module_& m) {
  std::tuple<double, double> (*p1)(const double, const double, const double) =
      tfel::material::projectOnPiPlane;
  m.def("buildFromPiPlane", &tfel::material::buildFromPiPlane<double>,
        pybind11::arg("s0"), pybind11::arg("s1"), build_from_pi_plane_doc);
  m.def("projectOnPiPlane", p1,
        pybind11::arg("s0"), pybind11::arg("s1"), pybind11::arg("s2"),
        project_on_pi_plane_scalar_doc);
  m.def("projectOnPiPlane", projectOnPiPlane<1u>, pybind11::arg("s"),
        project_on_pi_plane_stensor_doc);
  m.def("projectOnPiPlane", projectOnPiPlane<2u>, pybind11::arg("s"),
        project_on_pi_plane_stensor_doc);
  m.def("projectOnPiPlane", projectOnPiPlane<3u>, pybind11::arg("s"),
        project_on_pi_plane_stensor_doc);
}
