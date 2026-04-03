
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

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/LibraryDescription.hxx"

static std::vector<std::string> LibraryDescription_get_epts(
    const mfront::LibraryDescription& t) {
  return t.epts;
}

static void LibraryDescription_set_epts(mfront::LibraryDescription& t,
                                        const std::vector<std::string>& h) {
  t.epts = h;
}

void declareLibraryDescription(pybind11::module_&);

void declareLibraryDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::enum_<LibraryDescription::LibraryType>(m, "LibraryType")
      .value("SHARED_LIBRARY", LibraryDescription::SHARED_LIBRARY)
      .value("MODULE", LibraryDescription::MODULE);

  pybind11::class_<LibraryDescription, CompiledTargetDescriptionBase> w(
      m, "LibraryDescription");
  w.def(pybind11::init<std::string, std::string, std::string,
                       LibraryDescription::LibraryType>())
      .def_readonly("type", &LibraryDescription::type)
      .def("__repr__", [](const LibraryDescription& l) {
        auto os = std::ostringstream{};
        os << l;
        return os.str();
      });
  w.def_property("epts", LibraryDescription_get_epts,
                 LibraryDescription_set_epts);

  m.def("convert", convert);
  m.def("mergeLibraryDescription", mergeLibraryDescription);

  //   class_<std::vector<LibraryDescription>>("LibraryDescriptionVector")
  //       .def("__iter__", iterator<std::vector<LibraryDescription>>())
  //       .def("__len__", &std::vector<LibraryDescription>::size);
}
