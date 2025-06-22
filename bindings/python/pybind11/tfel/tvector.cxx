/*!
 * \file  bindings/python/tfel/tvector.cxx
 * \brief
 * \author Thomas Helfer
 * \date 30/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"

template <unsigned short N>
static void declaretvector(pybind11::module_& m, const char* const n) {
  using tvector = tfel::math::tvector<N, double>;
  pybind11::class_<tvector>(m, n, pybind11::buffer_protocol())
      .def_buffer([](tvector& v) -> pybind11::buffer_info {
        return pybind11::buffer_info(
            v.data(),       /* Pointer to buffer */
            sizeof(double), /* Size of one scalar */
            pybind11::format_descriptor<double>::format(), /* Python
                                                       struct-style format
                                                       descriptor */
            1,          /* Number of dimensions */
            {v.size()}, /* Buffer dimensions */
            {sizeof(double)});
      })
      .def(pybind11::init<>())
      .def(pybind11::init<double>())
      .def("__len__", [](const tvector& v) { return v.size(); })
      .def("__repr__",
           [](const tvector& v) {
             std::ostringstream os;
             os << v;
             return os.str();
           })
      .def("__getitem__",
           [](const tvector& v, const unsigned short idx) {
             if (idx >= v.size()) {
               tfel::raise<std::range_error>(
                   "invalid index '" + std::to_string(static_cast<int>(idx)) +
                   "'");
             }
             return v[idx];
           })
      .def("__setitem__",
           [](tvector& v, const unsigned short idx, const double value) {
             if (idx >= v.size()) {
               tfel::raise<std::range_error>(
                   "invalid index '" + std::to_string(static_cast<int>(idx)) +
                   "'");
             }
             v[idx] = value;
           })
      .def(
          "__iter__",
          [](const tvector& v) {
            return pybind11::make_iterator(v.begin(), v.end());
          },
          pybind11::keep_alive<0,
                               1>())  // keep object alive while iterator exists
      .def("__add__",
           [](const tvector& a, const tvector& b) -> tvector { return a + b; })
      .def("__sub__",
           [](const tvector& a, const tvector& b) -> tvector { return a - b; })
      .def("__mul__",
           [](const tvector& a, const double b) -> tvector { return a * b; })
      .def("__mul__",
           [](const double a, const tvector& b) -> tvector { return a * b; })
      .def("__div__",
           [](const tvector& a, const double b) -> tvector { return a / b; })
      .def(pybind11::self += pybind11::self)
      .def(pybind11::self -= pybind11::self)
      .def(pybind11::self *= double())
      .def(pybind11::self /= double())
      .def("__neg__", [](const tvector& v) -> tvector { return -v; });
}

void declaretvector(pybind11::module_& m);

void declaretvector(pybind11::module_& m) {
  declaretvector<1u>(m, "TVector1D");
  declaretvector<2u>(m, "TVector2D");
  declaretvector<3u>(m, "TVector3D");
}  // end of declaretvector
