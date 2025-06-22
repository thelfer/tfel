/*!
 * \file   CMakeGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11 juil. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MFront/TargetsDescription.hxx"
#include "MFront/GeneratorOptions.hxx"
#include "MFront/CMakeGenerator.hxx"

/*!
 * \brief a wrapper structure created for the python bindings.
 */
struct CMakeGenerator {
  /*!
   * \param[in] t: target description
   */
  void exe0(const mfront::TargetsDescription&);
  /*!
   * \param[in] t: target description
   * \param[in] o: generator options
   */
  void exe1(const mfront::TargetsDescription&, const mfront::GeneratorOptions&);
  /*!
   * \param[in] t: target description
   * \param[in] tn: target to be build
   */
  void exe2(const mfront::TargetsDescription&, const std::string&);
  /*!
   * \param[in] t: target description
   * \param[in] o: generator options
   * \param[in] tn: target to be build
   */
  void exe3(const mfront::TargetsDescription&,
            const mfront::GeneratorOptions&,
            const std::string&);
  /*!
   * \param[in] t: target description
   */
  void generate0(const mfront::TargetsDescription&);
  /*!
   * \param[in] t: target description
   * \param[in] o: generator options
   */
  void generate1(const mfront::TargetsDescription&,
                 const mfront::GeneratorOptions&);
  //!
  void callCMake0();
  /*!
   * \param[in] tn: target to be build
   */
  void callCMake1(const std::string&);
};  // end of CMakeGenerator

void CMakeGenerator::exe0(const mfront::TargetsDescription& t) {
  this->exe3(t, mfront::GeneratorOptions(), "all");
}

void CMakeGenerator::exe1(const mfront::TargetsDescription& t,
                          const mfront::GeneratorOptions& o) {
  this->exe3(t, o, "all");
}

void CMakeGenerator::exe2(const mfront::TargetsDescription& t,
                          const std::string& tn) {
  this->exe3(t, mfront::GeneratorOptions(), tn);
}

void CMakeGenerator::exe3(const mfront::TargetsDescription& t,
                          const mfront::GeneratorOptions& o,
                          const std::string& tn) {
  mfront::generateCMakeListsFile(t, o);
  mfront::callCMake(tn);
}

void CMakeGenerator::generate0(const mfront::TargetsDescription& t) {
  this->generate1(t, mfront::GeneratorOptions());
}

void CMakeGenerator::generate1(const mfront::TargetsDescription& t,
                               const mfront::GeneratorOptions& o) {
  mfront::generateCMakeListsFile(t, o);
}

void CMakeGenerator::callCMake0() { this->callCMake1("all"); }

void CMakeGenerator::callCMake1(const std::string& tn) {
  mfront::callCMake(tn);
}

void declareCMakeGenerator();

void declareCMakeGenerator() {
  using namespace boost::python;
  using namespace mfront;

  class_<CMakeGenerator>("CMakeGenerator")
      .def("generate", &CMakeGenerator::generate0,
           "generate the `CMake.mfront` file")
      .def("generate", &CMakeGenerator::generate1,
           "generate the `CMake.mfront` file "
           "using the provided options")
      .def("callCMake", &CMakeGenerator::callCMake0, "call the `make` utility")
      .def("callCMake", &CMakeGenerator::callCMake1,
           "call the `make` utility for the specified target")
      .def("exe", &CMakeGenerator::exe0,
           "This methods generates the `CMake.mfront` "
           "file in the `src` subdirectory and "
           "calls `make` to generate the target "
           "named `all`.")
      .def("exe", &CMakeGenerator::exe1,
           "This methods generates the `CMake.mfront` "
           "file in the `src` subdirectory "
           "using the provided options and "
           "calls `make` to generate the target "
           "named `all`.")
      .def("exe", &CMakeGenerator::exe2,
           "This methods generates the `CMake.mfront` "
           "file in the `src` subdirectory and "
           "calls `make` to generate the given target.")
      .def("exe", &CMakeGenerator::exe3,
           "This methods generates the `CMake.mfront` "
           "file in the `src` subdirectory "
           "using the provided options and "
           "calls `make` to generate the given target.");

}  // end of declareCMakeGenerator
