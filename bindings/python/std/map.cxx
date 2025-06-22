/*!
 * \file   bindings/python/std/map.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  30 avr 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <cmath>
#include <string>
#include <boost/python.hpp>
#include "TFEL/Python/MapConverter.hxx"

BOOST_PYTHON_MODULE(_stl_map) {
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::python;
  using boost::shared_ptr;
  // class_<map<string,double>,shared_ptr<map<string,double> >
  // >("StringDoubleMap")
  //   .def(map_indexing_suite<map<string,double> >());

  // class_<map<string,string>,shared_ptr<map<string,string> >
  // >("StringStringMap")
  //   .def(map_indexing_suite<map<string,string> >());

  initializeMapConverter<double, double>();
  initializeMapConverter<string, double>();
  initializeMapConverter<string, string>();
}
