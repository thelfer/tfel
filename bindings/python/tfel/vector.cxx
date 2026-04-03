/*!
 * \file   bindings/python/tfel/vector.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 avr 2010
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
#include "TFEL/Math/vector.hxx"

void declareTFELMathVector(pybind11::module_&);

void declareTFELMathVector(pybind11::module_& m) {
  using vector = tfel::math::vector<double>;
  pybind11::class_<vector>(m, "Vector", pybind11::buffer_protocol())
      .def_buffer([](vector& v) -> pybind11::buffer_info {
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
      .def("__len__", [](const vector& v) { return v.size(); })
      .def("__repr__",
           [](const vector& v) {
             std::ostringstream os;
             os << "[";
             for (auto p = v.begin(); p != v.end(); ++p) {
               if (p != v.begin()) {
                 os << ", ";
               }
               os << *p;
             }
             os << "]";
             return os.str();
           })
      .def("__getitem__",
           [](const vector& v, const unsigned short idx) {
             if (idx >= v.size()) {
               tfel::raise<std::range_error>(
                   "invalid index '" + std::to_string(static_cast<int>(idx)) +
                   "'");
             }
             return v[idx];
           })
      .def("__setitem__",
           [](vector& v, const unsigned short idx, const double value) {
             if (idx >= v.size()) {
               tfel::raise<std::range_error>(
                   "invalid index '" + std::to_string(static_cast<int>(idx)) +
                   "'");
             }
             v[idx] = value;
           })
      .def(
          "__iter__",
          [](const vector& v) {
            return pybind11::make_iterator(v.begin(), v.end());
          },
          pybind11::keep_alive<0,
                               1>())  // keep object alive while iterator exists
      .def("__add__",
           [](const vector& a, const vector& b) -> vector {
             if (a.size() != b.size()) {
               tfel::raise<std::runtime_error>(
                   "unmatched vector sizes ('" +
                   std::to_string(static_cast<int>(a.size())) + "', vs " +
                   std::to_string(static_cast<int>(b.size())) + "')");
             }
             auto r = vector(a.size());
             r = a + b;
             return r;
           })
      .def("__sub__",
           [](const vector& a, const vector& b) -> vector {
             if (a.size() != b.size()) {
               tfel::raise<std::runtime_error>(
                   "unmatched vector sizes ('" +
                   std::to_string(static_cast<int>(a.size())) + "', vs " +
                   std::to_string(static_cast<int>(b.size())) + "')");
             }
             auto r = vector(a.size());
             r = a - b;
             return r;
           })
      .def("__mul__",
           [](const vector& a, const double b) {
             auto r = vector(a.size());
             r = a * b;
             return r;
           })
      .def("__mul__",
           [](const double a, const vector& b) -> vector {
             auto r = vector(b.size());
             r = a * b;
             return r;
           })
      .def("__div__",
           [](const vector& a, const double b) -> vector {
             auto r = vector(a.size());
             r = a / b;
             return r;
           })
      .def("__iadd__",
           [](vector& a, const vector& b) {
             if (a.size() != b.size()) {
               tfel::raise<std::runtime_error>(
                   "unmatched vector sizes ('" +
                   std::to_string(static_cast<int>(a.size())) + "', vs " +
                   std::to_string(static_cast<int>(b.size())) + "')");
             }
             a += b;
           })
      .def("__isub__",
           [](vector& a, const vector& b) {
             if (a.size() != b.size()) {
               tfel::raise<std::runtime_error>(
                   "unmatched vector sizes ('" +
                   std::to_string(static_cast<int>(a.size())) + "', vs " +
                   std::to_string(static_cast<int>(b.size())) + "')");
             }
             a -= b;
           })
      .def(pybind11::self *= double())
      .def(pybind11::self /= double())
      .def("__neg__", [](const vector& v) -> vector {
        auto r = vector(v.size());
        r = -v;
        return r;
      });
}
