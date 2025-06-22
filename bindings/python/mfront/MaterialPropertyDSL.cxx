/*!
 * \file   bindings/python/mfront/MaterialPropertyDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/08/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <memory>
#include "TFEL/Python/SharedPtr.hxx"
#include <boost/python.hpp>
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyDSL.hxx"

void declareMaterialPropertyDSL();
void declareMaterialPropertyDSL() {
  using namespace boost::python;
  using namespace mfront;
  class_<MaterialPropertyDSL, std::shared_ptr<MaterialPropertyDSL>,
         bases<AbstractDSL>, boost::noncopyable>("MaterialPropertyDSL", no_init)
      .def("getMaterialPropertyDescription",
           &MaterialPropertyDSL::getMaterialPropertyDescription,
           return_internal_reference<>());
}
