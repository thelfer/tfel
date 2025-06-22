/*!
 * \file   bindings/python/mfront/DSLFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MFront/DSLFactory.hxx"

void declareDSLFactory() {
  using namespace boost::python;
  using namespace mfront;
  class_<DSLFactory, boost::noncopyable>("DSLFactory", no_init)
      .def("getDSLFactory", DSLFactory::getDSLFactory,
           return_value_policy<reference_existing_object>())
      .staticmethod("getDSLFactory")
      .def("createNewParser", &DSLFactory::createNewDSL)
      .def("createNewDSL", &DSLFactory::createNewDSL);
}
