/*!
 * \file  bindings/python/mfront/MTestParser.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "MTest/MTest.hxx"
#include "MTest/MTestParser.hxx"

void declareMTestParser();

static void execute(mtest::MTestParser& p,
                    mtest::MTest& m,
                    const std::string& f) {
  p.execute(m, f, {}, {});
}

static void execute2(mtest::MTestParser& p,
                     mtest::MTest& m,
                     const std::string& f,
                     const std::vector<std::string>& e) {
  p.execute(m, f, e, {});
}

static void execute3(mtest::MTestParser& p,
                     mtest::MTest& m,
                     const std::string& f,
                     const std::vector<std::string>& e,
                     const std::map<std::string, std::string>& s) {
  p.execute(m, f, e, s);
}

void declareMTestParser() {
  using namespace boost;
  using namespace boost::python;
  using namespace mtest;
  class_<MTestParser, noncopyable>("MTestParser")
      .def("execute", execute)
      .def("execute", execute2)
      .def("execute", execute3)
      .def("parseString", &MTestParser::parseString);

}  // end of declareExternalLibraryManager
