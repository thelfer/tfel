/*!
 * \file   bindings/python/mfront/CompiledTargetDescriptionBase.cxx
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
#include "MFront/CompiledTargetDescriptionBase.hxx"

#define COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(X)     \
  static std::vector<std::string> CompiledTargetDescriptionBase_get##X(  \
      const mfront::CompiledTargetDescriptionBase& t) {                  \
    return t.X;                                                          \
  }                                                                      \
  static void CompiledTargetDescriptionBase_set##X(                      \
      mfront::CompiledTargetDescriptionBase& t,                          \
      const std::vector<std::string>& h) {                               \
    t.X = h;                                                             \
  }                                                                      \
  static void CompiledTargetDescriptionBase_addProperty##X(              \
      boost::python::class_<mfront::CompiledTargetDescriptionBase>& w) { \
    w.add_property(#X, CompiledTargetDescriptionBase_get##X,             \
                   CompiledTargetDescriptionBase_set##X);                \
  }

#define COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(X, Y) \
  CompiledTargetDescriptionBase_addProperty##Y(X)

COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(sources)
COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(cppflags)
COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(include_directories)
COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(link_directories)
COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(link_libraries)
COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(deps)
COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY_HELPERFUNCTIONS(ldflags)

void declareCompiledTargetDescriptionBase() {
  using namespace boost::python;
  using namespace mfront;

  class_<CompiledTargetDescriptionBase> w("CompiledTargetDescriptionBase",
                                          no_init);
  w.def(init<std::string, std::string, std::string>())
      .def_readonly("name", &CompiledTargetDescriptionBase::name)
      .def_readonly("prefix", &CompiledTargetDescriptionBase::prefix)
      .def_readonly("suffix", &CompiledTargetDescriptionBase::suffix);

  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, sources);
  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, cppflags);
  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, include_directories);
  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, link_directories);
  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, link_libraries);
  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, deps);
  COMPILEDTARGETDESCRIPTIONBASE_ADDPROPERTY(w, ldflags);
}
