/*!
 * \file   mfront/src/GnuplotMaterialPropertyInterface.cxx
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
#include <algorithm>
#include <stdexcept>

#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/GnuplotMaterialPropertyInterface.hxx"

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

  GnuplotMaterialPropertyInterface::dataFile::dataFile() = default;
  GnuplotMaterialPropertyInterface::dataFile::dataFile(
      const GnuplotMaterialPropertyInterface::dataFile&) = default;
  GnuplotMaterialPropertyInterface::dataFile::dataFile(
      GnuplotMaterialPropertyInterface::dataFile&&) = default;
  GnuplotMaterialPropertyInterface::dataFile&
  GnuplotMaterialPropertyInterface::dataFile::operator=(
      const GnuplotMaterialPropertyInterface::dataFile&) = default;
  GnuplotMaterialPropertyInterface::dataFile&
  GnuplotMaterialPropertyInterface::dataFile::operator=(
      GnuplotMaterialPropertyInterface::dataFile&&) = default;
  GnuplotMaterialPropertyInterface::dataFile::~dataFile() = default;

  GnuplotMaterialPropertyInterface::tokens_iterator
  GnuplotMaterialPropertyInterface::nextToken(tokens_iterator current,
                                              const tokens_iterator endTokens,
                                              const std::string& msg) {
    using namespace std;
    string what(msg);
    if (++current == endTokens) {
      what += "unexpected end of file.\n";
      what += "Error at line : ";
      what += to_string((--current)->line);
      throw(std::runtime_error(what));
    }
    return current;
  }

  std::string GnuplotMaterialPropertyInterface::getName() { return "gnuplot"; }

  GnuplotMaterialPropertyInterface::GnuplotMaterialPropertyInterface() =
      default;

  GnuplotMaterialPropertyInterface::~GnuplotMaterialPropertyInterface() =
      default;

  std::string GnuplotMaterialPropertyInterface::eraseQuote(
      const std::string& in) {
    using namespace std;
    string res(in);
    if ((res[0] == '"') && (res[res.size() - 1] == '"')) {
      res.erase(res.begin());
      res.erase(res.end() - 1);
    }
    return res;
  }

  std::pair<bool, GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::treatKeyword(
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator endTokens) {
    if (std::find(i.begin(), i.end(), "gnuplot") != i.end()) {
      if ((key != "@TestBounds") && (key != "@Graph")) {
        throw(std::runtime_error(
            "GnuplotMaterialPropertyInterface::treatKeyword: "
            "unsupported key '" +
            key + "'"));
      }
    }
    if (key == "@TestBounds") {
      return registerTestBounds(current, endTokens);
    } else if (key == "@Graph") {
      return registerGraph(current, endTokens);
    }
    return {false, current};
  }  // end of treatKeyword

  std::pair<bool, GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::registerTestBounds(
      tokens_iterator current, const tokens_iterator endTokens) {
    using namespace std;
    string msg("GnuplotMaterialPropertyInterface::registerTestBounds : ");
    VariableBoundsDescription boundsDescription;

    current = nextToken(--current, endTokens, msg);
    if (current->value == "{") {
      msg += "'{' is not allowed in graph bounds.\n";
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

  std::pair<bool, GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::registerGraph(
      tokens_iterator current, const tokens_iterator endTokens) {
    using namespace std;
    string msg("GnuplotMaterialPropertyInterface::registerGraph : ");
    current = nextToken(--current, endTokens, msg);
    if (current->value != "{") {
      msg += "expected '{'.\n";
      msg += "Error at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    current = nextToken(current, endTokens, msg);
    while ((current->value != "}") && (current != endTokens)) {
      if (current->value == "{") {
        msg += "'{' is not allowed in graph.\n";
        msg += "Error at line : ";
        msg += to_string(current->line);
        throw(runtime_error(msg));
      }
      // on lit les mots clefs
      if (current->value == "Label") {
        //	current=readUntilEndOfInstruction(yLabel,++current,endTokens);
        // suppress " at begin and end of string if any

        current = nextToken(current, endTokens, msg);
        yLabel = eraseQuote(current->value);
        current = nextToken(current, endTokens, msg);
        if (current->value != ";") {
          msg += "Label : you must give only a string.\n";
          msg += "Error at line : ";
          msg += to_string((--current)->line);
          throw(runtime_error(msg));
        }
        current = nextToken(current, endTokens, msg);
      } else if (current->value == "ExperimentalData") {
        current = nextToken(current, endTokens, msg);
        string input = eraseQuote(current->value);
        current = nextToken(current, endTokens, msg);
        dataFile mydataFile;
        mydataFile.name = eraseQuote(current->value);
        current = nextToken(current, endTokens, msg);
        mydataFile.legend = eraseQuote(current->value);
        experimentalData.insert({input, mydataFile});
        current = nextToken(current, endTokens, msg);
        if (current->value != ";") {
          msg += "ExperimentalData : you provide more than three arguments.\n";
          msg += "Error at line : ";
          msg += to_string((--current)->line);
          throw(runtime_error(msg));
        }
        current = nextToken(current, endTokens, msg);
        //      } else if (current->value == "xxx") {
      } else {
        msg += "Keyword ";
        msg += current->value;
        msg += " is not allowed in graph.\n";
        msg += "Error at line : ";
        msg += to_string(current->line);
        throw(runtime_error(msg));
      }
      // current=nextToken(current,endTokens,msg);
    }
    if (current == endTokens) {
      msg += "File ended before the end of graph.\n";
      msg += "Error at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    return make_pair(true, ++current);
  }  // end of registerGraph

  void GnuplotMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) {
    using std::string;
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto target = name + "GnuplotGraph";
    d.specific_targets[target].first.push_back(name + "CppTest");
    d.specific_targets[target].second.push_back("@./" + name + "CppTest");
    d.specific_targets[target].second.push_back(
        string("@") + string(GNUPLOT_PATH) + " " + name + ".gp");
    d.specific_targets["graph"].first.push_back(target);
    d.specific_targets["check"].first.push_back(target);
  }

  void GnuplotMaterialPropertyInterface::writeOutputFiles(
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
    this->srcFileName += ".gp";
    this->srcFile.open(this->srcFileName);
    if (!this->srcFile) {
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit | ios::failbit);
    // writing src file
    string base;
    if (file.find('/') != string::npos) {
      base = file.substr(0, file.rfind('/')) + '/';
    }
    // en attendant qu'on puisse passer testbounds
    if (testBounds.empty()) testBounds = bounds;
    this->srcFile << "# generated by mfront from " << file << endl << endl;
    this->srcFile << "set term postscript eps color enhanced" << endl
                  << "set encoding iso_8859_1" << endl
                  << "set grid" << endl
                  << endl
                  << "set key under" << endl
                  << endl;
    // On doit demander a l'utilisateur ce que l'on veut pour le placement de la
    // legende !!!!
    if (!yLabel.empty())
      this->srcFile << "set ylabel '" << yLabel << "'" << endl;

    // on boucle sur toutes les variables pour generer les plot
    const auto csvFileName = name + "_test.csv";
    if (!inputs.empty()) {
      if (inputs.size() == 1) {
        this->srcFile << "set xlabel '" << inputs.begin()->name << "'" << endl;
        this->srcFile << "set output '" << name << "_" << inputs.begin()->name
                      << ".eps'" << endl;
        this->srcFile << "plot '" << csvFileName << "' u 1:2 notitle";
        // adding experimental data if any
        for (multimap<string, dataFile>::const_iterator it =
                 experimentalData.begin();
             it != experimentalData.end(); ++it) {
          if (inputs[0].name == (*it).first) {
            this->srcFile << ",\\\n'../" << base + it->second.name
                          << "' u 1:2 t '" << it->second.legend << "'";
          }
        }
        this->srcFile << endl;
      } else {
        int column = 0;
        for (auto p3 = inputs.begin(); p3 != inputs.end(); p3++) {
          this->srcFile << "set xlabel '" << p3->name << "'" << endl;
          this->srcFile << "set output '" << name << "_" << p3->name << ".eps'"
                        << endl;
          int columnIni = ++column;
          this->srcFile << "plot '" << csvFileName << "' u " << columnIni << ":"
                        << ++column << " t '";
          for (auto p4 = inputs.begin(); p4 != inputs.end(); ++p4) {
            if (p4 != p3) {
              for (auto p6 = testBounds.begin(); p6 != testBounds.end(); ++p6) {
                if (p6->varName == p4->name) {
                  this->srcFile << p4->name << "=" << p6->lowerBound;
                  if (p3 + 1 == inputs.end()) {
                    if (p4 + 2 != inputs.end()) this->srcFile << ",";
                  } else {
                    if (p4 + 1 != inputs.end()) this->srcFile << ",";
                  }
                }
              }
            }
          }
          this->srcFile << "'";
          this->srcFile << " w l lw 2 ";
          this->srcFile << ", '" << csvFileName << "' u " << columnIni << ":"
                        << ++column << " t '";
          for (auto p4 = inputs.begin(); p4 != inputs.end(); ++p4) {
            if (p4 != p3) {
              // pb si vide !
              for (auto p6 = testBounds.begin(); p6 != testBounds.end(); ++p6) {
                if (p6->varName == p4->name) {
                  this->srcFile << p4->name << "="
                                << (p6->upperBound - p6->lowerBound) / 2.;
                  if (p3 + 1 == inputs.end()) {
                    if (p4 + 2 != inputs.end()) this->srcFile << ",";
                  } else {
                    if (p4 + 1 != inputs.end()) this->srcFile << ",";
                  }
                }
              }
            }
          }
          this->srcFile << "'";
          this->srcFile << " w l lw 2 ";
          this->srcFile << ", '" << csvFileName << "' u " << columnIni << ":"
                        << ++column << " t '";
          for (auto p4 = inputs.begin(); p4 != inputs.end(); ++p4) {
            if (p4 != p3) {
              for (auto p6 = testBounds.begin(); p6 != testBounds.end(); ++p6) {
                if (p6->varName == p4->name) {
                  this->srcFile << p4->name << "=" << p6->upperBound;
                  if (p3 + 1 == inputs.end()) {
                    if (p4 + 2 != inputs.end()) this->srcFile << ",";
                  } else {
                    if (p4 + 1 != inputs.end()) this->srcFile << ",";
                  }
                }
              }
            }
          }
          this->srcFile << "'";
          this->srcFile << " w l lw 2 ";
          // adding experimental data if any
          for (auto it = experimentalData.begin(); it != experimentalData.end();
               ++it) {
            if (p3->name == (*it).first) {
              this->srcFile << "\\" << endl
                            << ",'../" << base + (*it).second.name
                            << "' u 1:2 t '" << (it->second).legend
                            << "' w p lw 2";
            }
          }
          this->srcFile << endl;
        }
      }
      this->srcFile.close();
    }

  }  // end of GnuplotMaterialPropertyInterface::writeSrcFile(void)

}  // end of namespace mfront
