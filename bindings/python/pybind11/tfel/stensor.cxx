/*!
 * \file  bindings/python/tfel/stensor.cxx
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
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"

template <unsigned short N>
static void declarestensor(pybind11::module_& m, const char* const n) {
  using stensor = tfel::math::stensor<N, double>;
  pybind11::class_<stensor>(m, n, pybind11::buffer_protocol())
      .def_buffer([](stensor& s) -> pybind11::buffer_info {
        return pybind11::buffer_info(
            s.data(),       /* Pointer to buffer */
            sizeof(double), /* Size of one scalar */
            pybind11::format_descriptor<double>::format(), /* Python
                                                       struct-style format
                                                       descriptor */
            1,          /* Number of dimensions */
            {s.size()}, /* Buffer dimensions */
            {sizeof(double)});
      })
      .def(pybind11::init<>())
      .def(pybind11::init<double>())
      .def("__len__", [](const stensor& s) { return s.size(); })
      .def("__repr__",
           [](const stensor& s) {
             std::ostringstream os;
             os << s;
             return os.str();
           })
      .def("__getitem__",
           [](const stensor& s, const unsigned short idx) {
             if (idx >= s.size()) {
               tfel::raise<std::range_error>(
                   "invalid index '" + std::to_string(static_cast<int>(idx)) +
                   "'");
             }
             return s[idx];
           })
      .def("__setitem__",
           [](stensor& s, const unsigned short idx, const double v) {
             if (idx >= s.size()) {
               tfel::raise<std::range_error>(
                   "invalid index '" + std::to_string(static_cast<int>(idx)) +
                   "'");
             }
             s[idx] = v;
           })
      .def(
          "__iter__",
          [](const stensor& s) {
            return pybind11::make_iterator(s.begin(), s.end());
          },
          pybind11::keep_alive<0,
                               1>())  // keep object alive while iterator exists
      .def("__add__",
           [](const stensor& a, const stensor& b) -> stensor { return a + b; })
      .def("__sub__",
           [](const stensor& a, const stensor& b) -> stensor { return a - b; })
      .def("__mul__",
           [](const stensor& a, const double b) -> stensor { return a * b; })
      .def("__mul__",
           [](const double a, const stensor& b) -> stensor { return a * b; })
      .def("__div__",
           [](const stensor& a, const double b) -> stensor { return a / b; })
      .def(pybind11::self += pybind11::self)
      .def(pybind11::self -= pybind11::self)
      .def(pybind11::self *= double())
      .def(pybind11::self /= double())
      .def("__neg__", [](const stensor& s) -> stensor { return -s; });
}

template <unsigned short N>
static double stensor_tresca(const tfel::math::stensor<N, double>& s) {
  return tfel::math::tresca(s);
}

static tfel::math::stensor<1u, double> makeStensor1D(
    const std::tuple<double, double, double>& s) {
  return {std::get<0>(s), std::get<1>(s), std::get<2>(s)};
}

static tfel::math::stensor<2u, double> makeStensor2D(
    const std::tuple<double, double, double>& s) {
  return {std::get<0>(s), std::get<1>(s), std::get<2>(s), 0.};
}

static tfel::math::stensor<3u, double> makeStensor3D(
    const std::tuple<double, double, double>& s) {
  return {std::get<0>(s), std::get<1>(s), std::get<2>(s), 0., 0., 0.};
}

void declarestensor(pybind11::module_& m);

void declarestensor(pybind11::module_& m) {
  using namespace tfel::math;
  double (*tresca1D)(const stensor<1u, double>&) = stensor_tresca<1u>;
  double (*tresca2D)(const stensor<2u, double>&) = stensor_tresca<2u>;
  double (*tresca3D)(const stensor<3u, double>&) = stensor_tresca<3u>;
  double (*sigmaeq1D)(const stensor<1u, double>&) = sigmaeq;
  double (*sigmaeq2D)(const stensor<2u, double>&) = sigmaeq;
  double (*sigmaeq3D)(const stensor<3u, double>&) = sigmaeq;
  m.def("tresca", tresca1D);
  m.def("tresca", tresca2D);
  m.def("tresca", tresca3D);
  m.def("sigmaeq", sigmaeq1D);
  m.def("sigmaeq", sigmaeq2D);
  m.def("sigmaeq", sigmaeq3D);
  m.def("makeStensor1D", makeStensor1D);
  m.def("makeStensor2D", makeStensor2D);
  m.def("makeStensor3D", makeStensor3D);
  declarestensor<1u>(m, "Stensor1D");
  declarestensor<2u>(m, "Stensor2D");
  declarestensor<3u>(m, "Stensor3D");
}  // end of declarestensor
