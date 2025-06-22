/*!
 * \file   bindings/python/mfront/LibraryDescription.cxx
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

#define LIBRARYDESCRIPTION_ADDPROPERTY_HELPERFUNCTIONS(X)                     \
  static std::vector<std::string> LibraryDescription_get##X(                  \
      const mfront::LibraryDescription& t) {                                  \
    return t.X;                                                               \
  }                                                                           \
  static void LibraryDescription_set##X(mfront::LibraryDescription& t,        \
                                        const std::vector<std::string>& h) {  \
    t.X = h;                                                                  \
  }                                                                           \
  static void LibraryDescription_addProperty##X(                              \
      boost::python::class_<mfront::LibraryDescription>& w) {                 \
    w.add_property(#X, LibraryDescription_get##X, LibraryDescription_set##X); \
  }

#define LIBRARYDESCRIPTION_ADDPROPERTY(X, Y) \
  LibraryDescription_addProperty##Y(X)

LIBRARYDESCRIPTION_ADDPROPERTY_HELPERFUNCTIONS(sources)
LIBRARYDESCRIPTION_ADDPROPERTY_HELPERFUNCTIONS(cppflags)
LIBRARYDESCRIPTION_ADDPROPERTY_HELPERFUNCTIONS(ldflags)
LIBRARYDESCRIPTION_ADDPROPERTY_HELPERFUNCTIONS(epts)
LIBRARYDESCRIPTION_ADDPROPERTY_HELPERFUNCTIONS(deps)

void declareLibraryDescription() {
  using namespace boost::python;
  using namespace mfront;

  enum_<LibraryDescription::LibraryType>("LibraryType")
      .value("SHARED_LIBRARY", LibraryDescription::SHARED_LIBRARY)
      .value("MODULE", LibraryDescription::MODULE);

  class_<LibraryDescription> w("LibraryDescription", no_init);
  w.def(init<std::string, std::string, std::string,
             LibraryDescription::LibraryType>())
      .def_readonly("name", &LibraryDescription::name)
      .def_readonly("prefix", &LibraryDescription::prefix)
      .def_readonly("suffix", &LibraryDescription::suffix)
      .def_readonly("type", &LibraryDescription::type)
      .def(self_ns::str(self_ns::self));

  LIBRARYDESCRIPTION_ADDPROPERTY(w, sources);
  LIBRARYDESCRIPTION_ADDPROPERTY(w, cppflags);
  LIBRARYDESCRIPTION_ADDPROPERTY(w, ldflags);
  LIBRARYDESCRIPTION_ADDPROPERTY(w, epts);
  LIBRARYDESCRIPTION_ADDPROPERTY(w, deps);

  def("convert", convert);
  def("mergeLibraryDescription", mergeLibraryDescription);
}
