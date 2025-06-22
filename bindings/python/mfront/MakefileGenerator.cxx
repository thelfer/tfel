/*!
 * \file   MakefileGenerator.cxx
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
#include "MFront/MakefileGenerator.hxx"

/*!
 * \brief a wrapper structure created for the python bindings.
 */
struct MakefileGenerator {
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
  void callMake0();
  /*!
   * \param[in] tn: target to be build
   */
  void callMake1(const std::string&);
};  // end of MakefileGenerator

void MakefileGenerator::exe0(const mfront::TargetsDescription& t) {
  this->exe3(t, mfront::GeneratorOptions(), "all");
}

void MakefileGenerator::exe1(const mfront::TargetsDescription& t,
                             const mfront::GeneratorOptions& o) {
  this->exe3(t, o, "all");
}

void MakefileGenerator::exe2(const mfront::TargetsDescription& t,
                             const std::string& tn) {
  this->exe3(t, mfront::GeneratorOptions(), tn);
}

void MakefileGenerator::exe3(const mfront::TargetsDescription& t,
                             const mfront::GeneratorOptions& o,
                             const std::string& tn) {
  mfront::generateMakeFile(t, o);
  mfront::callMake(tn);
}

void MakefileGenerator::generate0(const mfront::TargetsDescription& t) {
  this->generate1(t, mfront::GeneratorOptions());
}

void MakefileGenerator::generate1(const mfront::TargetsDescription& t,
                                  const mfront::GeneratorOptions& o) {
  mfront::generateMakeFile(t, o);
}

void MakefileGenerator::callMake0() { this->callMake1("all"); }

void MakefileGenerator::callMake1(const std::string& tn) {
  mfront::callMake(tn);
}

void declareMakefileGenerator();

void declareMakefileGenerator() {
  using namespace boost::python;
  using namespace mfront;

  class_<MakefileGenerator>("MakefileGenerator")
      .def("generate", &MakefileGenerator::generate0,
           "generate the `Makefile.mfront` file")
      .def("generate", &MakefileGenerator::generate1,
           "generate the `Makefile.mfront` file "
           "using the provided options")
      .def("callMake", &MakefileGenerator::callMake0, "call the `make` utility")
      .def("callMake", &MakefileGenerator::callMake1,
           "call the `make` utility for the specified target")
      .def("exe", &MakefileGenerator::exe0,
           "This methods generates the `Makefile.mfront` "
           "file in the `src` subdirectory and "
           "calls `make` to generate the target "
           "named `all`.")
      .def("exe", &MakefileGenerator::exe1,
           "This methods generates the `Makefile.mfront` "
           "file in the `src` subdirectory "
           "using the provided options and "
           "calls `make` to generate the target "
           "named `all`.")
      .def("exe", &MakefileGenerator::exe2,
           "This methods generates the `Makefile.mfront` "
           "file in the `src` subdirectory and "
           "calls `make` to generate the given target.")
      .def("exe", &MakefileGenerator::exe3,
           "This methods generates the `Makefile.mfront` "
           "file in the `src` subdirectory "
           "using the provided options and "
           "calls `make` to generate the given target.");

}  // end of declareMakefileGenerator
