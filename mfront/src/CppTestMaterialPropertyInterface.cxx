/*!
 * \file   mfront/src/CppTestMaterialPropertyInterface.cxx
 * \brief
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>

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

  CppTestMaterialPropertyInterface::tokens_iterator
  CppTestMaterialPropertyInterface::nextToken(tokens_iterator current,
                                              const tokens_iterator endTokens,
                                              const std::string& msg) {
    using namespace std;
    string what(msg);
    if (++current == endTokens) {
      what += "unexpected end of file.\n";
      what += "Error at line : ";
      what += to_string((--current)->line);
      throw(runtime_error(what));
    }
    return current;
  }

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
      tokens_iterator current,
      const tokens_iterator endTokens) {
    if (std::find(i.begin(), i.end(), "cpptest") != i.end()) {
      if (key != "@TestBounds") {
        throw(std::runtime_error(
            "CppTestMaterialPropertyInterface::treatKeyword: "
            "unsupported key '" +
            key + "'"));
      }
    }
    if (key == "@TestBounds") {
      return registerTestBounds(current, endTokens);
    }
    return {false, current};
  }  // end of treatKeyword

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppTestMaterialPropertyInterface::registerTestBounds(
      tokens_iterator current, const tokens_iterator endTokens) {
    using namespace std;
    string msg("CppTestMaterialPropertyInterface::registerTestBounds : ");
    VariableBoundsDescription boundsDescription;
    current = nextToken(--current, endTokens, msg);
    if (current->value == "{") {
      msg += "'{' is not allowed in test bounds.\n";
      msg += "Error at line : ";
      msg += to_string(current->line);
    }
    // howto test if current->value is a word ????
    boundsDescription.lineNumber = current->line;
    boundsDescription.varName = current->value;
    // on ne peut pas tester si la variable existe bien en input :-(
    // peut-on avoir ici autre chose que 1 ?
    boundsDescription.varNbr = 1;
    current = nextToken(current, endTokens, msg);
    if (current->value != "in") {
      msg += "expected 'in' (read ";
      msg += current->value;
      msg += ")\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    current = nextToken(current, endTokens, msg);
    if (current->value != "[") {
      msg += "expected '[' (read ";
      msg += current->value;
      msg += ")\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    current = nextToken(current, endTokens, msg);
    istringstream converterL(current->value);
    converterL >> boundsDescription.lowerBound;
    boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
    if (!converterL && (!converterL.eof())) {
      msg += "could not read lower bound value\nRead : ";
      msg += current->value;
      msg += "\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    current = nextToken(current, endTokens, msg);
    if (current->value != ":") {
      msg += "expected ':' (read ";
      msg += current->value;
      msg += ")\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    current = nextToken(current, endTokens, msg);
    istringstream converterU(current->value);
    converterU >> boundsDescription.upperBound;
    if (!converterU && (!converterU.eof())) {
      msg += "could not read upper bound value\nRead : ";
      msg += current->value;
      msg += "\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    if (boundsDescription.boundsType ==
        VariableBoundsDescription::LowerAndUpper) {
      if (boundsDescription.lowerBound > boundsDescription.upperBound) {
        msg +=
            "lower bound value is greater than upper bound value for variable ";
        msg += boundsDescription.varName;
        msg += "\nError at line : ";
        msg += to_string(current->line);
        throw(runtime_error(msg));
      }
    }
    current = nextToken(current, endTokens, msg);
    if (current->value != "]") {
      msg += "expected ']' (read ";
      msg += current->value;
      msg += ")\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    current = nextToken(current, endTokens, msg);
    if (current->value != ";") {
      msg += "expected ';' (read ";
      msg += current->value;
      msg += ")\nError at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    testBounds.push_back(boundsDescription);
    return make_pair(true, ++current);
  }  // end of registerTestBounds

  void CppTestMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) {
    const auto lib =
        "Cpp" + getMaterialLawLibraryNameBase(mpd.library, mpd.material);
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto target = name + "CppTest";
#if defined _WIN32 || defined _WIN64
    d.specific_targets[target].first.push_back(lib + ".dll");
#else
    d.specific_targets[target].first.push_back(lib + ".so");
#endif
    d.specific_targets[target].first.push_back(name + "-CppTest.o");
    d.specific_targets[target].second.push_back("@$(CXX) $(LDFLAGS) -L. -l" +
                                                lib + " $^ -o $@");
    d.specific_targets["check"].first.push_back(target);
  }  // end of CppMaterialPropertyInterface::getSpecificTargets

  void CppTestMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) {
    using namespace std;
    const auto& file = fd.fileName;
    const auto& className = mpd.className;
    const auto& material = mpd.material;
    const auto& inputs = mpd.inputs;
    const auto& bounds = mpd.bounds;
    const auto name =
        (material.empty()) ? className : material + "_" + className;
    this->srcFileName = "src/" + name;
    this->srcFileName += "-CppTest.cxx";
    this->srcFile.open(this->srcFileName);
    if (!this->srcFile) {
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit | ios::failbit);
    // on initialise avec bounds
    std::vector<VariableBoundsDescription> tests(bounds);
    // nombre de point de calcul
    int numberOfLines = 100;
    this->srcFile << "// generated by mfront from " << file << endl << endl;
    this->srcFile << "#include <vector>\n"
                  << "#include <iostream>\n"
                  << "#include <sstream>\n"
                  << "#include <fstream>\n"
                  << endl;
    this->srcFile << "#include\"" << name << "-cxx.hxx\"\n\n";
    this->srcFile
        << "int\n main(const int argc, const char * const * const argv)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace mfront;\n";
    this->srcFile << "  " << name << " my" << name << " ;" << endl;
    this->srcFile << "  int nb=" << numberOfLines << " ;" << endl << endl;
    // on boucle sur toutes les variables pour recuperer les bornes
    if (testBounds.empty()) {
      // we use bounds
      if (tests.empty()) {
        throw(
            runtime_error("MaterialPropertyDSL::writeTestFile:\n"
                          "No bounds defined to draw graph !"));
      }
    } else {
      tests = testBounds;
    }

    for (const auto& t : tests) {
      if (t.boundsType == VariableBoundsDescription::Lower) {
        throw runtime_error(
            "MaterialPropertyDSL::writeTestFile:\nNo upper bound defined to "
            "draw graph !");
      }
      if (t.boundsType == VariableBoundsDescription::Upper) {
        throw runtime_error(
            "MaterialPropertyDSL::writeTestFile:\nNo lower bound defined to "
            "draw graph !");
      }
      this->srcFile << "  const double " << t.varName
                    << "_min = " << t.lowerBound << " ;" << endl;
      this->srcFile << "  const double " << t.varName
                    << "_max = " << t.upperBound << " ;" << endl;
      this->srcFile << "  const double " << t.varName << "_moy = (" << t.varName
                    << "_max - " << t.varName << "_min)/2. ;" << endl;
    }
    // fichier csv
    string csvFileName = name;
    csvFileName += "_test";
    csvFileName += ".csv";
    this->srcFile << "  ofstream file(\"" << csvFileName << "\");" << endl;
    this->srcFile << "  if (! file.is_open()) {" << endl
                  << "    cerr << \"Could not open file " << csvFileName
                  << "\" << endl;" << endl
                  << "    return 1;" << endl
                  << "  }" << endl;
    this->srcFile << "  file.precision(14);" << endl;
    // on boucle sur toutes les variables pour generer le fichier csv
    if (!inputs.empty()) {
      for (auto p3 = inputs.begin(); p3 != inputs.end(); ++p3) {
        this->srcFile << "  double " << p3->name << "_interval = (" << p3->name
                      << "_max - " << p3->name << "_min)/nb;" << endl;
      }
      this->srcFile << "  for(int i=0; i<nb; i++) {" << endl;
      for (auto p3 = inputs.begin(); p3 != inputs.end(); ++p3) {
        this->srcFile << "    file<<" << p3->name << "_min+i*" << p3->name
                      << "_interval<<\" \"" << endl;
        // all min
        this->srcFile << "        <<my" << name << ".defaultCompute(";
        for (auto p4 = inputs.begin(); p4 != inputs.end();) {
          if (p3 != p4) this->srcFile << p4->name << "_min";
          if (p3 == p4)
            this->srcFile << p3->name << "_min+i*" << p3->name << "_interval";
          if ((++p4) != inputs.end()) this->srcFile << ",";
        }
        this->srcFile << ") << \" \"" << endl;
        // all moy
        this->srcFile << "        <<my" << name << ".defaultCompute(";
        for (auto p4 = inputs.begin(); p4 != inputs.end();) {
          if (p3 != p4)
            this->srcFile << p4->name << "_moy";
          else
            this->srcFile << p3->name << "_min+i*" << p3->name << "_interval";
          if ((++p4) != inputs.end()) this->srcFile << ",";
        }
        this->srcFile << ") << \" \"" << endl;
        // all max
        this->srcFile << "        <<my" << name << ".defaultCompute(";
        for (auto p4 = inputs.begin(); p4 != inputs.end();) {
          if (p3 != p4)
            this->srcFile << p4->name << "_max";
          else
            this->srcFile << p3->name << "_min+i*" << p3->name << "_interval";
          if ((++p4) != inputs.end()) this->srcFile << ",";
        }
        this->srcFile << ") << \" \";" << endl;
      }
      this->srcFile << "    file << endl ;" << endl;
      this->srcFile << "  }" << endl;
    }

    this->srcFile << "  return 0;" << endl << "}" << endl;
    this->srcFile.close();
  }  // end of CppTestMaterialPropertyInterface::writeSrcFile

}  // end of namespace mfront
