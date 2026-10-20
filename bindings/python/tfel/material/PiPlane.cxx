
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

void declarePiPlane(pybind11::module_& m) {
  std::tuple<double, double> (*p1)(const double, const double, const double) =
      tfel::material::projectOnPiPlane;
  m.def("buildFromPiPlane", &tfel::material::buildFromPiPlane<double>,
        pybind11::arg("s0"), pybind11::arg("s1"));
  m.def("projectOnPiPlane", p1,
        pybind11::arg("s11"), pybind11::arg("s22"), pybind11::arg("s33"));
  m.def("projectOnPiPlane", projectOnPiPlane<1u>, pybind11::arg("s"));
  m.def("projectOnPiPlane", projectOnPiPlane<2u>, pybind11::arg("s"));
  m.def("projectOnPiPlane", projectOnPiPlane<3u>, pybind11::arg("s"));
}
