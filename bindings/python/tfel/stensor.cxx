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

#include <boost/python.hpp>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"

template <unsigned short N>
static double& stensor_getitem(tfel::math::stensor<N, double>& v,
                               const unsigned short i) {
  return v[i];
}

template <unsigned short N>
static void stensor_setitem(tfel::math::stensor<N, double>& v,
                            const unsigned short i,
                            const double vi) {
  v[i] = vi;
}

template <unsigned short N>
static std::string stensor_str(const tfel::math::stensor<N, double>& v) {
  std::ostringstream os;
  os << v;
  return os.str();
}

template <unsigned short N, typename scalar>
static tfel::math::stensor<N, double> stensor_idiv(
    tfel::math::stensor<N, double>& v, const scalar s) {
  v /= s;
  return v;
}

template <unsigned short N>
static tfel::math::stensor<N, double> stensor_add(
    const tfel::math::stensor<N, double>& v1,
    const tfel::math::stensor<N, double>& v2) {
  return v1 + v2;
}

template <unsigned short N>
static tfel::math::stensor<N, double> stensor_sub(
    const tfel::math::stensor<N, double>& v1,
    const tfel::math::stensor<N, double>& v2) {
  return v1 - v2;
}

template <unsigned short N, typename scalar>
static tfel::math::stensor<N, double> stensor_mul(
    const tfel::math::stensor<N, double>& v1, const scalar s) {
  return s * v1;
}

template <unsigned short N>
static void declarestensor(const char* const n) {
  using namespace boost;
  using namespace boost::python;
  using boost::python::iterator;
  using stensor = tfel::math::stensor<N, double>;

  class_<stensor>(n, init<>())
      .def(init<double>())
      .def("__getitem__", &stensor_getitem<N>,
           return_value_policy<copy_non_const_reference>())
      .def("__setitem__", &stensor_setitem<N>, with_custodian_and_ward<1, 2>())
      .def("__iter__", iterator<stensor>())
      .def("__str__", stensor_str<N>)
      .def("__add__", stensor_add<N>)
      .def("__sub__", stensor_sub<N>)
      .def("__mul__", stensor_mul<N, double>)
      .def("__mul__", stensor_mul<N, int>)
      .def(self += self)
      .def(self -= self)
      .def(self *= double())
#if PY_MAJOR_VERSION >= 3
      .def("__itruediv__", stensor_idiv<N, double>)
      .def("__itruediv__", stensor_idiv<N, int>)
#else  /* PY_MAJOR_VERSION >= 3 */
      .def(self /= double())
      .def(self /= int())
#endif /* PY_MAJOR_VERSION >= 3 */
      ;
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

void declarestensor();

void declarestensor() {
  using namespace boost::python;
  using namespace tfel::math;
  double (*tresca1D)(const stensor<1u, double>&) = stensor_tresca<1u>;
  double (*tresca2D)(const stensor<2u, double>&) = stensor_tresca<2u>;
  double (*tresca3D)(const stensor<3u, double>&) = stensor_tresca<3u>;
  double (*sigmaeq1D)(const stensor<1u, double>&) = sigmaeq;
  double (*sigmaeq2D)(const stensor<2u, double>&) = sigmaeq;
  double (*sigmaeq3D)(const stensor<3u, double>&) = sigmaeq;
  def("tresca", tresca1D);
  def("tresca", tresca2D);
  def("tresca", tresca3D);
  def("sigmaeq", sigmaeq1D);
  def("sigmaeq", sigmaeq2D);
  def("sigmaeq", sigmaeq3D);
  def("makeStensor1D", makeStensor1D);
  def("makeStensor2D", makeStensor2D);
  def("makeStensor3D", makeStensor3D);
  declarestensor<1u>("Stensor1D");
  declarestensor<2u>("Stensor2D");
  declarestensor<3u>("Stensor3D");
}  // end of declarestensor
