/*!
 * \file  bindings/python/tfel/WalpoleBasis.cxx
 * \brief
 * \author Antoine Martin
 * \date 25/05/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Math/ST2toST2/WalpoleBasis.hxx"

void declareWalpoleBasis(pybind11::module_ &m) {
  using namespace tfel::math;
  using TIBasis = TransverseIsotropicWalpoleBasis<double>;
  pybind11::class_<TIBasis>(m, "WalpoleBasis")
      .def(pybind11::init<>())
      .def("p", [](const tvector<3u, double> &n) { return TIBasis::p(n); })
      .def("q", [](const tvector<3u, double> &n) { return TIBasis::q(n); })
      .def("E1", [](const tvector<3u, double> &n) { return TIBasis::E1(n); })
      .def("E2", [](const tvector<3u, double> &n) { return TIBasis::E2(n); })
      .def("E3", [](const tvector<3u, double> &n) { return TIBasis::E3(n); })
      .def("E4", [](const tvector<3u, double> &n) { return TIBasis::E4(n); })
      .def("F", [](const tvector<3u, double> &n) { return TIBasis::F(n); })
      .def("G", [](const tvector<3u, double> &n) { return TIBasis::G(n); })
      .def("components",
           [](const tvector<3u, double> &n, const st2tost2<3u, double> &A) {
             return TIBasis::components(n, A);
           });
}
