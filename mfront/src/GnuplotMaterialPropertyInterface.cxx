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

#include "TFEL/Raise.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
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
    tfel::raise_if(++current == endTokens,
                   msg +
                       ": unexpected end of file.\n"
                       "Error at line : " +
                       std::to_string((--current)->line));
    return current;
  }

  std::string GnuplotMaterialPropertyInterface::getName() { return "gnuplot"; }

  GnuplotMaterialPropertyInterface::GnuplotMaterialPropertyInterface() =
      default;

  GnuplotMaterialPropertyInterface::~GnuplotMaterialPropertyInterface() =
      default;

  std::string GnuplotMaterialPropertyInterface::eraseQuote(
      const std::string& in) {
    auto res = in;
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
      tfel::raise_if((key != "@TestBounds") && (key != "@Graph"),
                     "GnuplotMaterialPropertyInterface::treatKeyword: "
                     "unsupported key '" +
                         key + "'");
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
    throw_if(!this->registredTestBounds.insert(b).second,
             "test bounds for variable '" + b.first + "' already registred");
    return {true, ++p};
  }  // end of registerTestBounds

  std::pair<bool, GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::registerGraph(tokens_iterator current,
                                                  const tokens_iterator pe) {
    const std::string method("GnuplotMaterialPropertyInterface::registerGraph");
    auto throw_if = [&method](const bool b, const std::string& m) {
      tfel::raise_if(b, method + ": " + m);
    };
    current = nextToken(--current, pe, method);
    throw_if(current->value != "{",
             "expected '{'.\n"
             "Error at line '" +
                 std::to_string(current->line) + "'");
    current = nextToken(current, pe, method);
    while ((current->value != "}") && (current != pe)) {
      throw_if(current->value == "{",
               "'{' is not allowed in graph.\n"
               "Error at line '" +
                   std::to_string(current->line) + "'");
      // on lit les mots clefs
      if (current->value == "Label") {
        // current=readUntilEndOfInstruction(yLabel,++current,pe);
        // suppress " at begin and end of string if any
        current = nextToken(current, pe, method);
        yLabel = eraseQuote(current->value);
        current = nextToken(current, pe, method);
        throw_if(current->value != ";",
                 "Label : you must give only a string.\n"
                 "Error at line '" +
                     std::to_string(current->line) + "'");
        current = nextToken(current, pe, method);
      } else if (current->value == "ExperimentalData") {
        current = nextToken(current, pe, method);
        const auto input = eraseQuote(current->value);
        current = nextToken(current, pe, method);
        dataFile mydataFile;
        mydataFile.name = eraseQuote(current->value);
        current = nextToken(current, pe, method);
        mydataFile.legend = eraseQuote(current->value);
        experimentalData.insert({input, mydataFile});
        current = nextToken(current, pe, method);
        throw_if(current->value != ";",
                 "ExperimentalData : you provide more than three arguments.\n"
                 "Error at line '" +
                     std::to_string(current->line) + "'");
        current = nextToken(current, pe, method);
      } else {
        throw_if(true, "Keyword " + current->value +
                           "' is not allowed in graph.\n"
                           "Error at line '" +
                           std::to_string(current->line) + "'");
      }
    }
    throw_if(current == pe,
             "File ended before the end of graph.\n"
             "Error at line '" +
                 std::to_string(current->line) + "'");
    return make_pair(true, ++current);
  }  // end of registerGraph

  void GnuplotMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    using std::string;
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto target = name + "GnuplotGraph";
    insert_if(d.specific_targets[target].deps, name + "CppTest");
    insert_if(d.specific_targets[target].sources, "./" + name + "CppTest");
    insert_if(d.specific_targets[target].sources,
              string(GNUPLOT_PATH) + " " + name + ".gp");
    insert_if(d.specific_targets["graph"].deps, target);
    insert_if(d.specific_targets["check"].deps, target);
  }

  void GnuplotMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto& file = fd.fileName;
    const auto& className = mpd.className;
    const auto& material = mpd.material;
    const auto& inputs = mpd.inputs;
    const auto name =
        (material.empty()) ? className : material + "_" + className;
    std::ofstream out("src/" + name + ".gp");
    tfel::raise_if(!out,
                   "GnuplotMaterialPropertyInterface::writeOutputFiles: "
                   "unable to open 'src/" +
                       name +
                       ".gp' "
                       "for writing output file.");
    out.exceptions(std::ios::badbit | std::ios::failbit);
    // writing src file
    std::string base;
    if (file.find('/') != std::string::npos) {
      base = file.substr(0, file.rfind('/')) + '/';
    }
    // en attendant qu'on puisse passer testbounds
    auto testBounds(this->registredTestBounds);
    for (const auto& i : mpd.inputs) {
      if (testBounds.count(i.name) == 0) {
        if (hasBounds(i)) {
          testBounds.insert({i.name, i.getBounds()});
        }
      }
    }
    out << "# generated by mfront from " << file << "\n\n";
    out << "set term postscript eps color enhanced\n"
        << "set encoding iso_8859_1\n"
        << "set grid\n\n"
        << "set key under\n\n";
    // On doit demander a l'utilisateur ce que l'on veut pour le placement de la
    // legende !!!!
    if (!yLabel.empty()) out << "set ylabel '" << yLabel << "'\n";

    // on boucle sur toutes les variables pour generer les plot
    const auto csvFileName = name + "_test.csv";
    if (!inputs.empty()) {
      if (inputs.size() == 1) {
        out << "set xlabel '" << inputs.begin()->name << "'\n";
        out << "set output '" << name << "_" << inputs.begin()->name
            << ".eps'\n";
        out << "plot '" << csvFileName << "' u 1:2 notitle";
        // adding experimental data if any
        for (auto it = experimentalData.begin(); it != experimentalData.end();
             ++it) {
          if (inputs[0].name == (*it).first) {
            out << ",\\\n'../" << base + it->second.name << "' u 1:2 t '"
                << it->second.legend << "'";
          }
        }
        out << '\n';
      } else {
        int column = 0;
        for (auto p3 = inputs.begin(); p3 != inputs.end(); p3++) {
          out << "set xlabel '" << p3->name << "'\n";
          out << "set output '" << name << "_" << p3->name << ".eps'\n";
          int columnIni = ++column;
          out << "plot '" << csvFileName << "' u " << columnIni << ":"
              << ++column << " t '";
          for (auto p4 = inputs.begin(); p4 != inputs.end(); ++p4) {
            if (p4 != p3) {
              for (auto p6 = testBounds.begin(); p6 != testBounds.end(); ++p6) {
                if (p6->first == p4->name) {
                  out << p4->name << "=" << p6->second.lowerBound;
                  if (p3 + 1 == inputs.end()) {
                    if (p4 + 2 != inputs.end()) out << ",";
                  } else {
                    if (p4 + 1 != inputs.end()) out << ",";
                  }
                }
              }
            }
          }
          out << "'";
          out << " w l lw 2 ";
          out << ", '" << csvFileName << "' u " << columnIni << ":" << ++column
              << " t '";
          for (auto p4 = inputs.begin(); p4 != inputs.end(); ++p4) {
            if (p4 != p3) {
              // pb si vide !
              for (auto p6 = testBounds.begin(); p6 != testBounds.end(); ++p6) {
                if (p6->first == p4->name) {
                  out << p4->name << "="
                      << (p6->second.upperBound - p6->second.lowerBound) / 2.;
                  if (p3 + 1 == inputs.end()) {
                    if (p4 + 2 != inputs.end()) out << ",";
                  } else {
                    if (p4 + 1 != inputs.end()) out << ",";
                  }
                }
              }
            }
          }
          out << "'";
          out << " w l lw 2 ";
          out << ", '" << csvFileName << "' u " << columnIni << ":" << ++column
              << " t '";
          for (auto p4 = inputs.begin(); p4 != inputs.end(); ++p4) {
            if (p4 != p3) {
              for (auto p6 = testBounds.begin(); p6 != testBounds.end(); ++p6) {
                if (p6->first == p4->name) {
                  out << p4->name << "=" << p6->second.upperBound;
                  if (p3 + 1 == inputs.end()) {
                    if (p4 + 2 != inputs.end()) out << ",";
                  } else {
                    if (p4 + 1 != inputs.end()) out << ",";
                  }
                }
              }
            }
          }
          out << "' w l lw 2 ";
          // adding experimental data if any
          for (auto it = experimentalData.begin(); it != experimentalData.end();
               ++it) {
            if (p3->name == (*it).first) {
              out << "\\\n"
                  << ",'../" << base + (*it).second.name << "' u 1:2 t '"
                  << (it->second).legend << "' w p lw 2";
            }
          }
          out << '\n';
        }
      }
      out.close();
    }

  }  // end of GnuplotMaterialPropertyInterface::writeSrcFile()

}  // end of namespace mfront
