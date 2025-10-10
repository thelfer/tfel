/*!
 * \file  bindings/python/tfel/st2tost2.cxx
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
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

template <unsigned short N>
static void declarest2tost2(pybind11::module_& m, const char* const n) {
  using st2tost2 = tfel::math::st2tost2<N, double>;
  pybind11::class_<st2tost2>(m, n, pybind11::buffer_protocol())
      .def_static("zero", &st2tost2::zero)
      .def_buffer([](st2tost2& s) -> pybind11::buffer_info {
        return pybind11::buffer_info(
            s.data(),       /* Pointer to buffer */
            sizeof(double), /* Size of one scalar */
            pybind11::format_descriptor<double>::format(), /* Python
                                                       struct-style format
                                                       descriptor */
            2,                      /* Number of dimensions */
            {s.size(0), s.size(1)}, /* Buffer dimensions */
            {s.size(1) * sizeof(double), sizeof(double)});
      })
      .def(pybind11::init<>())
      .def(pybind11::init<double>())
      .def("__repr__",
           [](const st2tost2& s) {
             std::ostringstream os;
             os << s;
             return os.str();
           })
      .def("__getitem__",
           [](const st2tost2& s, const unsigned short r,
              const unsigned short c) {
             if ((r >= s.size(0)) && (c >= s.size(1))) {
               tfel::raise<std::range_error>(
                   "invalid index '(" + std::to_string(static_cast<int>(r)) +
                   ", " + std::to_string(static_cast<int>(c)) + "'");
             }
             return s(r, c);
           })
      .def("__setitem__",
           [](st2tost2& s, const unsigned short r, const unsigned short c,
              const double v) {
             if ((r >= s.size(0)) && (c >= s.size(1))) {
               tfel::raise<std::range_error>(
                   "invalid index '(" + std::to_string(static_cast<int>(r)) +
                   ", " + std::to_string(static_cast<int>(c)) + "'");
             }
             s(r, c) = v;
           })
      .def(
          "__iter__",
          [](const st2tost2& s) {
            return pybind11::make_iterator(s.begin(), s.end());
          },
          pybind11::keep_alive<0,
                               1>())  // keep object alive while iterator exists
      .def("__add__",
           [](const st2tost2& a, const st2tost2& b) -> st2tost2 {
             return a + b;
           })
      .def("__sub__",
           [](const st2tost2& a, const st2tost2& b) -> st2tost2 {
             return a - b;
           })
      .def("__mul__",
           [](const st2tost2& a, const double b) -> st2tost2 { return a * b; })
      .def("__mul__",
           [](const double a, const st2tost2& b) -> st2tost2 { return a * b; })
      .def("__div__",
           [](const st2tost2& a, const double b) -> st2tost2 { return a / b; })
      .def(pybind11::self += pybind11::self)
      .def(pybind11::self -= pybind11::self)
      .def(pybind11::self *= double())
      .def(pybind11::self /= double())
      .def("__neg__", [](const st2tost2& s) -> st2tost2 { return -s; });
}

void declarest2tost2(pybind11::module_&);
void declarest2tost2(pybind11::module_& m) {
  using namespace tfel::math;
  declarest2tost2<1u>(m, "ST2toST21D");
  declarest2tost2<2u>(m, "ST2toST22D");
  declarest2tost2<3u>(m, "ST2toST23D");
}  // end of declarest2tost2
