/*!
 * \file   bindings/python/mfront/LibraryDescription.cxx
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
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "TFEL/Python/VectorConverter.hxx"
#include "MFront/LibraryDescription.hxx"

static std::vector<std::string> LibraryDescription_get_epts(
    const mfront::LibraryDescription& t) {
  return t.epts;
}

static void LibraryDescription_set_epts(mfront::LibraryDescription& t,
                                        const std::vector<std::string>& h) {
  t.epts = h;
}

void declareLibraryDescription() {
  using namespace boost::python;
  using namespace mfront;

  enum_<LibraryDescription::LibraryType>("LibraryType")
      .value("SHARED_LIBRARY", LibraryDescription::SHARED_LIBRARY)
      .value("MODULE", LibraryDescription::MODULE);

  class_<LibraryDescription, bases<CompiledTargetDescriptionBase>> w(
      "LibraryDescription", no_init);
  w.def(init<std::string, std::string, std::string,
             LibraryDescription::LibraryType>())
      .def_readonly("type", &LibraryDescription::type)
      .def(self_ns::str(self_ns::self));

  w.add_property("epts", LibraryDescription_get_epts,
                 LibraryDescription_set_epts);

  def("convert", convert);
  def("mergeLibraryDescription", mergeLibraryDescription);

  class_<std::vector<LibraryDescription>>("LibraryDescriptionVector")
      .def("__iter__", iterator<std::vector<LibraryDescription>>())
      .def("__len__", &std::vector<LibraryDescription>::size);
}
