/*!
 * \file   mfront/src/CppTestMaterialPropertyInterface.cxx
 * \brief
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontWarningMode.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CppMaterialPropertyInterface.hxx"
#include "MFront/CppTestMaterialPropertyInterface.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  std::string CppTestMaterialPropertyInterface::getName() { return "cpptest"; }

  CppTestMaterialPropertyInterface::CppTestMaterialPropertyInterface() =
      default;

  CppTestMaterialPropertyInterface::~CppTestMaterialPropertyInterface() =
      default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppTestMaterialPropertyInterface::treatKeyword(
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator p,
      const tokens_iterator pe) {
    if (std::find(i.begin(), i.end(), "cpptest") != i.end()) {
      tfel::raise_if(key != "@TestBounds",
                     "CppTestMaterialPropertyInterface::treatKeyword: "
                     "unsupported key '" +
                         key + "'");
    }
    if (key == "@TestBounds") {
      if (i.empty()) {
        reportWarning("keyword '" + key +
                      "' is used without being restricted to the " +
                      this->getName() +
                      " interface, which could be a portability "
                      "issue. Please add [" +
                      this->getName() + "] after the keyword (i.e. replace '" +
                      key + "' by '" + key + "[" + this->getName() + "]')");
      }
      return registerTestBounds(p, pe);
    }
    return {false, p};
  }  // end of treatKeyword

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppTestMaterialPropertyInterface::registerTestBounds(
      tokens_iterator p, const tokens_iterator pe) {
    using tfel::utilities::CxxTokenizer;
    const std::string m =
        "GnuplotMaterialPropertyInterface::registerTestBounds";
    auto throw_if = [&m](const bool b, const std::string& msg) {
      tfel::raise_if(b, m + ": " + msg);
    };
    const auto b = mfront::readVariableBounds(p, pe);
    CxxTokenizer::readSpecifiedToken(m, ";", p, pe);
    throw_if(b.second.boundsType != VariableBoundsDescription::LOWERANDUPPER,
             "invalid bounds type");
    throw_if(!testBounds.insert(b).second,
             "test bounds for variable '" + b.first + "' already registred");
    return {true, ++p};
  }  // end of registerTestBounds

  void CppTestMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = "Cpp" + getMaterialLawLibraryNameBase(mpd);
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto target = name + "CppTest";
    insert_if(d.specific_targets[target].deps, lib);
    insert_if(d.specific_targets[target].sources, name + "-CppTest.o");
    insert_if(d.specific_targets[target].cmds,
              "$(CXX) $(LDFLAGS) -L. -l" + lib + " $^ -o $@");
    insert_if(d.specific_targets["check"].deps, target);
  }  // end of CppMaterialPropertyInterface::getSpecificTargets

  void CppTestMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto& file = fd.fileName;
    const auto& className = mpd.className;
    const auto& material = mpd.material;
    const auto& inputs = mpd.inputs;
    const auto name =
        (material.empty()) ? className : material + "_" + className;
    const auto srcFileName = "src/" + name + "-CppTest.cxx";
    std::ofstream srcFile(srcFileName);
    tfel::raise_if(!srcFile,
                   "MaterialPropertyDSL::writeOutputFiles: "
                   "unable to open '" +
                       srcFileName +
                       "' "
                       "for writing output file.");
    srcFile.exceptions(std::ios::badbit | std::ios::failbit);
    // on initialise avec bounds
    // nombre de point de calcul
    int numberOfLines = 100;
    srcFile << "// generated by mfront from " << file << "\n\n"
            << "#include <vector>\n"
            << "#include <iostream>\n"
            << "#include <sstream>\n"
            << "#include <fstream>\n\n";
    srcFile << "#include\"" << name << "-cxx.hxx\"\n\n";
    srcFile
        << "int\n main(const int argc, const char * const * const argv)\n{\n";
    srcFile << "using namespace std;\n";
    srcFile << "using namespace mfront;\n";
    srcFile << "  " << name << " my" << name << " ;\n";
    srcFile << "  int nb=" << numberOfLines << " ;\n\n";
    // definition of the tests
    auto tests = this->testBounds;
    for (const auto& i : mpd.inputs) {
      if ((i.hasBounds()) && (tests.count(i.name) == 0)) {
        tests.insert({i.name, i.getBounds()});
      }
    }
    tfel::raise_if(tests.empty(),
                   "MaterialPropertyDSL::writeTestFile:\n"
                   "No bounds defined to draw graph !");
    for (const auto& t : tests) {
      const auto& n = t.first;
      const auto& b = t.second;
      tfel::raise_if(b.boundsType == VariableBoundsDescription::LOWER,
                     "MaterialPropertyDSL::writeTestFile:\n"
                     "No upper bound defined to draw graph !");
      tfel::raise_if(b.boundsType == VariableBoundsDescription::UPPER,
                     "MaterialPropertyDSL::writeTestFile:\n"
                     "No lower bound defined to draw graph !");
      srcFile << "  const double " << n << "_min = " << b.lowerBound << " ;\n"
              << "  const double " << n << "_max = " << b.upperBound << " ;\n"
              << "  const double " << n << "_moy = (" << n << "_max - " << n
              << "_min)/2. ;\n";
    }
    // fichier csv
    auto csvFileName = name;
    csvFileName += "_test";
    csvFileName += ".csv";
    srcFile << "  ofstream file(\"" << csvFileName << "\");\n";
    srcFile << "  if (! file.is_open()) {\n"
            << "    cerr << \"could not open file " << csvFileName << "\\n\";\n"
            << "    return 1;\n"
            << "  }\n";
    srcFile << "  file.precision(14);\n";
    // on boucle sur toutes les variables pour generer le fichier csv
    if (!inputs.empty()) {
      for (auto p3 = inputs.begin(); p3 != inputs.end(); ++p3) {
        srcFile << "  double " << p3->name << "_interval = (" << p3->name
                << "_max - " << p3->name << "_min)/nb;\n";
      }
      srcFile << "  for(int i=0; i<nb; i++) {\n";
      for (auto p3 = inputs.begin(); p3 != inputs.end(); ++p3) {
        srcFile << "    file<<" << p3->name << "_min+i*" << p3->name
                << "_interval<<\" \"\n";
        // all min
        srcFile << "        <<my" << name << ".defaultCompute(";
        for (auto p4 = inputs.begin(); p4 != inputs.end();) {
          if (p3 != p4) srcFile << p4->name << "_min";
          if (p3 == p4)
            srcFile << p3->name << "_min+i*" << p3->name << "_interval";
          if ((++p4) != inputs.end()) srcFile << ",";
        }
        srcFile << ") << \" \"\n";
        // all moy
        srcFile << "        <<my" << name << ".defaultCompute(";
        for (auto p4 = inputs.begin(); p4 != inputs.end();) {
          if (p3 != p4)
            srcFile << p4->name << "_moy";
          else
            srcFile << p3->name << "_min+i*" << p3->name << "_interval";
          if ((++p4) != inputs.end()) srcFile << ",";
        }
        srcFile << ") << \" \"\n";
        // all max
        srcFile << "        <<my" << name << ".defaultCompute(";
        for (auto p4 = inputs.begin(); p4 != inputs.end();) {
          if (p3 != p4)
            srcFile << p4->name << "_max";
          else
            srcFile << p3->name << "_min+i*" << p3->name << "_interval";
          if ((++p4) != inputs.end()) srcFile << ",";
        }
        srcFile << ") << \" \";\n";
      }
      srcFile << "    file << endl ;\n";
      srcFile << "  }\n";
    }

    srcFile << "  return 0;\n"
            << "}\n";
    srcFile.close();
  }  // end of CppTestMaterialPropertyInterface::writeSrcFile

}  // end of namespace mfront
