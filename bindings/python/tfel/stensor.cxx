/*!
 * \file  bindings/python/tfel/stensor.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 août 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include <TFEL/Math/stensor.hxx>
#include <TFEL/Math/Stensor/StensorConceptIO.hxx>

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
  using namespace std;
  ostringstream os;
  os << v;
  return os.str();
}

template <unsigned short N>
static void declarestensor(const char* const n) {
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::math;
  using boost::python::iterator;

  class_<stensor<N, double>>(n, init<>())
      .def(init<double>())
      .def("__getitem__", &stensor_getitem<N>,
           return_value_policy<copy_non_const_reference>())
      .def("__setitem__", &stensor_setitem<N>, with_custodian_and_ward<1, 2>())
      .def("__iter__", iterator<stensor<N, double>>())
      .def("__str__", stensor_str<N>);
}

template <unsigned short N>
static double stensor_tresca(const tfel::math::stensor<N, double>& s) {
  using namespace tfel::math;
  return tresca(s);
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
  declarestensor<1u>("Stensor1D");
  declarestensor<2u>("Stensor2D");
  declarestensor<3u>("Stensor3D");
}  // end of declarestensor
