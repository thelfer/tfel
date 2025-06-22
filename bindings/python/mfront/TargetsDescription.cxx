/*!
 * \file   bindings/python/mfront/TargetsDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"

static std::vector<std::string> TargetsDescription_getHeaders(
    const mfront::TargetsDescription& t) {
  return t.headers;
}

static void TargetsDescription_setHeaders(mfront::TargetsDescription& t,
                                          const std::vector<std::string>& h) {
  t.headers = h;
}

static const std::vector<mfront::LibraryDescription>&
TargetsDescription_getLibraries(mfront::TargetsDescription& t) {
  return t.libraries;
}  // end of TargetsDescription_getLibraries

void declareTargetsDescription() {
  using namespace boost::python;
  using namespace mfront;
  using LibraryAccess =
      LibraryDescription& (TargetsDescription::*)(const std::string&);
  class_<mfront::TargetsDescription>("TargetsDescription")
      .def("getLibrary",
           static_cast<LibraryAccess>(&TargetsDescription::getLibrary),
           arg("library"), return_internal_reference<>())
      .add_property("headers", TargetsDescription_getHeaders,
                    TargetsDescription_setHeaders)
      .def("getLibraries", TargetsDescription_getLibraries,
           return_internal_reference<>())
      .add_property("libraries", make_function(&TargetsDescription_getLibraries,
                                               return_internal_reference<>()))
      //
      .def_readwrite("specific_targets", &TargetsDescription::specific_targets)
      .def(self_ns::str(self_ns::self));
  def("describes", mfront::describes);
  def("mergeTargetsDescription", mfront::describes);
}
