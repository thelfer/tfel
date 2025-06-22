/*!
 * \file   bindings/python/mfront/TargetsDescription.cxx
 * \brief
 * \author THOMAS HELFER
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

static auto TargetsDescription_size(const mfront::TargetsDescription& t)
    -> decltype(t.end() - t.begin()) {
  return t.end() - t.begin();
}

static std::vector<std::string> TargetsDescription_getHeaders(
    const mfront::TargetsDescription& t) {
  return t.headers;
}

static void TargetsDescription_setHeaders(mfront::TargetsDescription& t,
                                          const std::vector<std::string>& h) {
  t.headers = h;
}

void declareTargetsDescription() {
  using namespace boost::python;
  using namespace mfront;
  using Op1 = LibraryDescription& (
      TargetsDescription::*)(const std::string&, const std::string&,
                             const std::string&,
                             const LibraryDescription::LibraryType);
  using Op2 = LibraryDescription& (TargetsDescription::*)(const std::string&,
                                                          const std::string&,
                                                          const std::string&);
  using Op3 = LibraryDescription& (TargetsDescription::*)(const std::string&,
                                                          const std::string&);
  using Access =
      LibraryDescription& (TargetsDescription::*)(const std::string&);
  class_<mfront::TargetsDescription>("TargetsDescription")
      .def("__iter__", iterator<mfront::TargetsDescription>())
      .def("__len__", TargetsDescription_size)
      .def("__getitem__", static_cast<Access>(&TargetsDescription::operator[]),
           arg("library"), return_internal_reference<>())
      .def("__setitem__", static_cast<Access>(&TargetsDescription::operator[]),
           arg("library"), return_internal_reference<>())
      .def("__call__", static_cast<Op1>(&TargetsDescription::operator()),
           return_internal_reference<>())
      .def("__call__", static_cast<Op2>(&TargetsDescription::operator()),
           return_internal_reference<>())
      .def("__call__", static_cast<Op3>(&TargetsDescription::operator()),
           return_internal_reference<>())
      .add_property("headers", TargetsDescription_getHeaders,
                    TargetsDescription_setHeaders)
      .def_readwrite("specific_targets", &TargetsDescription::specific_targets)
      .def(self_ns::str(self_ns::self));
  def("describes", mfront::describes);
  def("mergeTargetsDescription", mfront::describes);
}
