/*!
 * \file  bindings/python/tfel/st2tost2.cxx
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

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

template <unsigned short N>
static double& st2tost2_getitem(tfel::math::st2tost2<N, double>& v,
                                const unsigned short i,
                                const unsigned short j) {
  return v(i, j);
}

template <unsigned short N>
static void st2tost2_setitem(tfel::math::st2tost2<N, double>& v,
                             const unsigned short i,
                             const unsigned short j,
                             const double vi) {
  v(i, j) = vi;
}

template <unsigned short N>
static std::string st2tost2_str(const tfel::math::st2tost2<N, double>& v) {
  std::ostringstream os;
  os << v;
  return os.str();
}

template <unsigned short N>
static void declarest2tost2(const char* const n) {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::math;
  using boost::python::iterator;
  class_<st2tost2<N, double>>(n, init<>())
      .def(init<double>())
      .def("__getitem__", &st2tost2_getitem<N>,
           return_value_policy<copy_non_const_reference>())
      .def("__setitem__", &st2tost2_setitem<N>, with_custodian_and_ward<1, 2>())
      .def("__iter__", iterator<st2tost2<N, double>>())
      .def("__str__", st2tost2_str<N>);
}  // end of declarest2tost2

void declarest2tost2();
void declarest2tost2() {
  using namespace boost::python;
  using namespace tfel::math;
  declarest2tost2<1u>("ST2toST21D");
  declarest2tost2<2u>("ST2toST22D");
  declarest2tost2<3u>("ST2toST23D");
}  // end of declarest2tost2
