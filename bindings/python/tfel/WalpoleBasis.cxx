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

void declareWalpoleBasis(pybind11::module_& m) {
  using namespace tfel::math;
  using TIBasis = TransverseIsotropicWalpoleBasis<double>;
  pybind11::class_<TIBasis>(
      m, "WalpoleBasis",
      "This file enables some operations in different Walpole bases relative\n"
      "to different cases of symmetry for st2tost2")
      .def(pybind11::init<>())
      .def("p",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::p(n);
           },
           pybind11::arg("n"))
      .def("q",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::q(n);
           },
           pybind11::arg("n"))
      .def("E1",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::E1(n);
           },
           pybind11::arg("n"))
      .def("E2",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::E2(n);
           },
           pybind11::arg("n"))
      .def("E3",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::E3(n);
           },
           pybind11::arg("n"))
      .def("E4",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::E4(n);
           },
           pybind11::arg("n"))
      .def("F",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::F(n);
           },
           pybind11::arg("n"))
      .def("G",
           +[](const TIBasis&, const tvector<3u, double>& n) {
             return TIBasis::G(n);
           },
           pybind11::arg("n"))
      .def("components",
           +[](const TIBasis&,
               const tvector<3u, double>& n, const st2tost2<3u, double>& P) {
             return TIBasis::components(n, P);
           },
           pybind11::arg("n"), pybind11::arg("P"));
}
