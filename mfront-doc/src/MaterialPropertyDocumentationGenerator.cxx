/*!
 * \file  MaterialPropertyDocumentationGenerator.cxx
 * \brief
 * \author Maxence Wangermez
 * \date   16 avril 2024
 * \copyright Copyright (C) 2006-2024 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <fstream>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/DocumentationGeneratorBase.hxx"
#include "MFront/MaterialPropertyDocumentationGenerator.hxx"

namespace mfront {

  // map::at() is missing in ekopath standard library
  template <typename Map>
  static typename Map::mapped_type map_at(const Map& m,
                                          const typename Map::key_type& k) {
    const auto p = m.find(k);
    tfel::raise_if(p == m.end(), "map_at : unknown key '" + k + "'");
    return p->second;
  }

  static std::string getCodeBlocksDocumentation(
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool standalone) {
    const auto* const basic_title_level = standalone ? "" : "#";
    std::ostringstream out;
    const auto name = mfront::getMaterialLawLibraryNameBase(mpd);
    const auto& c = mpd.f;
    if (!fd.description.empty()) {
      out << basic_title_level << "## " << name << " description\n"  //
          << fd.description << "\n\n";
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      out << basic_title_level << "## " << name << " listing\n"
          << '\n'
          << "~~~~~~~ {.cpp}\n"
          << c.body << '\n'
          << "~~~~~~~ \n";
    }
    return out.str();
  }

  static void writeStandardLatexMacros(std::ostream& os) {
    os << "\\newcommand{\\tensor}[1]{\\underline{#1}}\n"
       << "\\newcommand{\\tensorq}[1]{\\underline{\\mathbf{#1}}}\n"
       << "\\newcommand{\\ust}[1]{\\underset{\\tilde{}}{\\mathbf{#1}}}\n"
       << "\\newcommand{\\transpose}[1]{#1^{\\mathop{T}}}\n"
       << "\\newcommand{\\tsigma}{\\underline{\\sigma}}\n"
       << "\\newcommand{\\sigmaeq}{\\sigma_{\\mathrm{eq}}}\n"
       << "\\newcommand{\\epsilonth}{\\epsilon^{\\mathrm{th}}}\n"
       << "\\newcommand{\\tepsilonto}{\\underline{\\epsilon}^{\\mathrm{to}}}\n"
       << "\\newcommand{\\tepsilonel}{\\underline{\\epsilon}^{\\mathrm{el}}}\n"
       << "\\newcommand{\\tepsilonth}{\\underline{\\epsilon}^{\\mathrm{th}}}\n"
       << "\\newcommand{\\tepsilonvis}{\\underline{\\epsilon}^{\\mathrm{vis}}}"
          "\n"
       << "\\newcommand{\\tdepsilonvis}{\\underline{\\dot{\\epsilon}}^{"
          "\\mathrm{vis}}}\n"
       << "\\newcommand{\\tepsilonp}{\\underline{\\epsilon}^{\\mathrm{p}}}\n"
       << "\\newcommand{\\tdepsilonp}{\\underline{\\dot{\\epsilon}}^{\\mathrm{"
          "p}}}\n"
       << "\\newcommand{\\trace}[1]{\\mathrm{tr}\\paren{#1}}\n"
       << "\\newcommand{\\Frac}[2]{{\\displaystyle \\frac{\\displaystyle "
          "#1}{\\displaystyle #2}}}\n"
       << "\\newcommand{\\deriv}[2]{{\\displaystyle \\frac{\\displaystyle "
          "\\partial #1}{\\displaystyle \\partial #2}}}\n"
       << "\\newcommand{\\dtot}{\\mathrm{d}}\n"
       << "\\newcommand{\\paren}[1]{\\left(#1\\right)}\n"
       << "\\newcommand{\\bts}[1]{\\left.#1\\right|_{t}}\n"
       << "\\newcommand{\\mts}[1]{\\left.#1\\right|_{t+\\theta\\,\\Delta\\,t}}"
          "\n"
       << "\\newcommand{\\ets}[1]{\\left.#1\\right|_{t+\\Delta\\,t}}\n\n";
  }  // end of writeStandardLatexMacros

  static void getData(std::vector<DocumentationGeneratorBase::Data>& data,
                      const VariableDescription& v) {
    auto ndata = DocumentationGeneratorBase::Data{};
    ndata.name = v.name;
    ndata.arraySize = v.arraySize;
    ndata.type = v.type;
    data.push_back(ndata);
  }  // end of getData

  static std::vector<DocumentationGeneratorBase::Data> getData(
      const VariableDescription& vd) {
    using namespace tfel::material;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    auto data = std::vector<DocumentationGeneratorBase::Data>{};
    getData(data, vd);
    // description deserves a specific treatment
    return data;
  }

  static std::vector<DocumentationGeneratorBase::Data> getData(
      const VariableDescriptionContainer& vdc) {
    using namespace tfel::material;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    auto data = std::vector<DocumentationGeneratorBase::Data>{};
    for (const auto& vd : vdc) {
      getData(data, vd);
    }
    // description deserves a specific treatment
    for (auto& d : data) {
      if (glossary.contains(d.externalName)) {
        const auto& e = glossary.getGlossaryEntry(d.externalName);
        std::ostringstream os;
        os << e.getShortDescription();
        const auto& cd = e.getDescription();
        for (const auto& cd_pcd : cd) {
          if (!cd_pcd.empty()) {
            os << cd_pcd << '\n';
          }
        }
        d.description += os.str();
      }
      auto ddc = d.descriptions.size();
      if (ddc == 1u) {
        auto pd = d.descriptions.begin();
        if (!pd->second.empty()) {
          if (!d.description.empty()) {
            d.description += "\n";
          }
          d.description += pd->second;
        }
      } else if (ddc != 0u) {
        // Two cases: all descriptions are the same
        bool b = true;
        auto pd = d.descriptions.begin();
        auto pd2 = pd;
        advance(pd2, 1u);
        for (; (pd2 != d.descriptions.end()) && (b); ++pd2) {
          b = pd->second == pd2->second;
        }
        if (b) {
          if (!d.description.empty()) {
            d.description += "\n";
          }
          d.description += pd->second;
        } else {
          for (pd2 = pd; (pd2 != d.descriptions.end()) && (b); ++pd2) {
            if (!pd2->second.empty()) {
              if (!d.description.empty()) {
                d.description += "\n";
              }
              if (pd2->first == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
                d.description += "Default Hypothesis : " + pd2->second;
              } else {
                d.description += ModellingHypothesis::toString(pd2->first) +
                                 " : " + pd2->second;
              }
            }
          }
        }
      }
    }
    return data;
  }

  static void printData(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const std::string& title,
      const std::vector<DocumentationGeneratorBase::Data>& data,
      const bool standalone,
      const std::string& language = "english") {
    using namespace tfel::material;
    if (data.empty()) {
      return;
    }
    const auto* const basic_title_level = standalone ? "" : "#";
    auto translations =
        std::map<std::string, std::map<std::string, std::string>>{};
    auto& en = translations["english"];
    auto& fr = translations["french"];
    en["variable name"] = "variable name";
    fr["variable name"] = "nom";
    en["variable type"] = "variable type";
    fr["variable type"] = "type";
    en["array size"] = "array size";
    fr["array size"] = "taille";
    en["defined for"] = "defined for";
    fr["defined for"] = "définie pour";
    en["description"] = "description";
    fr["description"] = "description";
    en["default value"] = "default value";
    fr["default value"] = "valeur par défaut";
    en["default value for"] = "default value for";
    fr["default value for"] =
        "valeur par défaut pour l'hypothèse de modélsation ";
    const auto& l = translations[language];
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "printData : begin\n";
    }
    // const auto& dh = mpd.getDistinctModellingHypotheses();
    auto cbnames = std::set<std::string>{};
    // for (const auto& h : dh) {
    //   const auto& d = mpd.getBehaviourData(h);
    //   const auto& cn = d.getCodeBlockNames();
    //   cbnames.insert(cn.begin(), cn.end());
    // }
    cbnames.insert(mfront::getMaterialLawLibraryNameBase(mpd));
    os << basic_title_level  //
       << "##  " << title << "\n\n";
    for (const auto& d : data) {
      os << "* " << d.externalName << ":\n";
      if (d.externalName != d.name) {
        os << "\t+ " << map_at(l, "variable name") << ": " << d.name << '\n';
      }
      os << "\t+ " << map_at(l, "variable type") << ": " << d.type << '\n';
      if (d.arraySize != 1u) {
        os << "\t+ " << map_at(l, "array size") << ": " << d.arraySize << '\n';
      }
      // if (d.hypotheses.size() != dh.size()) {
      //   os << "\t+ " << map_at(l, "defined for") << " ";
      //   for (auto pvh = d.hypotheses.begin(); pvh != d.hypotheses.end();) {
      //     os << ModellingHypothesis::toString(*pvh);
      //     if (++pvh != d.hypotheses.end()) {
      //       os << ", ";
      //     }
      //   }
      //   os << '\n';
      // }
      if (!d.description.empty()) {
        os << "\t+ " << map_at(l, "description") << ": " << d.description
           << '\n';
      }
      // for (const auto& h : d.hypotheses) {
      //   if (mpd.isParameterName(h, d.name)) {
      //     if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      //       os << "\t+ " << map_at(l, "default value") << ": ";
      //     } else {
      //       os << "\t+ "
      //          << map_at(l, "default value for") + " " +
      //                 ModellingHypothesis::toString(h) + ": ";
      //     }
      //     if (d.type == "int") {
      //       os << mpd.getIntegerParameterDefaultValue(h, d.name);
      //     } else if (d.type == "ushort") {
      //       os << mpd.getUnsignedShortParameterDefaultValue(h, d.name);
      //     } else {
      //       const auto& p =
      //           mpd.getBehaviourData(h).getParameters().getVariable(d.name);
      //       if (p.arraySize == 1u) {
      //         os << mpd.getFloattingPointParameterDefaultValue(h, d.name);
      //       } else {
      //         for (unsigned short i = 0; i != p.arraySize;) {
      //           os << mpd.getFloattingPointParameterDefaultValue(h, d.name,
      //           i); if (++i != p.arraySize) {
      //             os << " ";
      //           }
      //         }
      //       }
      //     }
      //     os << '\n';
      //   }
      // }
      // codes blocks referring to the current variable
      auto vcb =
          std::map<std::string, std::vector<ModellingHypothesis::Hypothesis>>{};
      auto dvcb =
          std::map<std::string, std::vector<ModellingHypothesis::Hypothesis>>{};
      // for (const auto& cbname : cbnames) {
      //   // for (const auto& h : dh) {
      //   //   const auto& bd = mpd.getBehaviourData(h);
      //   //   const bool b = (bd.isIntegrationVariableName(d.name) ||
      //   //                   bd.isExternalStateVariableName(d.name));
      //   //   if (bd.hasCode(cbname)) {
      //   //     const auto& cb = bd.getCodeBlock(cbname);
      //   //     if (cb.members.find(d.name) != cb.members.end()) {
      //   //       vcb[cbname].push_back(h);
      //   //     }
      //   //     if (b) {
      //   //       if (cb.members.find("d" + d.name) != cb.members.end()) {
      //   //         dvcb[cbname].push_back(h);
      //   //       }
      //   //     }
      //   //   }
      //   // }
      // }
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        if (!vcb.empty()) {
          os << "\t+ used in ";
          for (auto pc = vcb.begin(); pc != vcb.end();) {
            os << pc->first;
            // if (pc->second.size() != dh.size()) {
            //   os << " (";
            //   for (auto pvh = pc->second.begin(); pvh != pc->second.end();) {
            //     os << ModellingHypothesis::toString(*pvh);
            //     if (++pvh != pc->second.end()) {
            //       os << ", ";
            //     }
            //   }
            //   os << ")";
            // }
            if (++pc != vcb.end()) {
              os << ", ";
            }
          }
          os << '\n';
        }
        if (!dvcb.empty()) {
          os << "\t+ increment (or rate) used in ";
          for (auto pc = dvcb.begin(); pc != dvcb.end();) {
            os << pc->first;
            // if (pc->second.size() != dh.size()) {
            //   os << " (";
            //   for (auto pvh = pc->second.begin(); pvh != pc->second.end();) {
            //     os << ModellingHypothesis::toString(*pvh);
            //     if (++pvh != pc->second.end()) {
            //       os << ", ";
            //     }
            //   }
            //   os << ")";
            // }
            if (++pc != dvcb.end()) {
              os << ", ";
            }
          }
          os << '\n';
        }
      }
    }
    // bounds
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "printData : end\n";
    }
  }

  MaterialPropertyDocumentationGenerator::
      MaterialPropertyDocumentationGenerator(
          const int argc,
          const char* const* const argv,
          std::shared_ptr<MaterialPropertyDSL> d,
          const std::string& f)

      : DocumentationGeneratorBase(argc, argv, f), dsl(d) {
    // registring interfaces
    if (!this->interfaces.empty()) {
      dsl->setInterfaces(this->interfaces);
    }
  }  // end of
     // MaterialPropertyDocumentationGenerator::MaterialPropertyDocumentationGenerator

  void MaterialPropertyDocumentationGenerator::exe() const {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "Treating file '" << this->file << "'\n";
    }
    // analysing the file
    this->dsl->analyseFile(this->file, this->ecmds, this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& mpd = this->dsl->getMaterialPropertyDescription();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "MaterialPropertyDocumentationGenerator::exe : begin\n";
    }
    std::ofstream output_file;
    auto& out = [&output_file, &mpd, this]() -> std::ostream& {
      if (this->std_output) {
        return std::cout;
      }
      const auto name = mfront::getMaterialLawLibraryNameBase(mpd);
      output_file.open(name + ".md");
      output_file.exceptions(std::ios::badbit | std::ios::failbit);
      tfel::raise_if(!output_file,
                     "MaterialPropertyDocumentationGenerator::exe: "
                     "could not open file 'src/" +
                         name + ".txt'");
      return output_file;
    }();
    writeStandardLatexMacros(out);
    if (this->otype == FULL) {
      this->writeFullOutput(out, mpd, fd);
    } else if (this->otype == WEB) {
      this->writeWebOutput(out, mpd, fd);
    } else {
      tfel::raise(
          "MaterialPropertyDocumentationGenerator::exe: "
          "unsupported output type");
    }
    if (!this->std_output) {
      output_file.close();
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "MaterialPropertyDocumentationGenerator::exe : end\n";
    }
  }  // end of MaterialPropertyDocumentationGenerator::exe

  void MaterialPropertyDocumentationGenerator::writeWebOutput(
      std::ostream& out,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    using namespace tfel::utilities;
    out << "# " << mfront::getMaterialLawLibraryNameBase(mpd) << mpd.className
        << " material property description\n\n"
        << "* file   : " << fd.fileName << '\n'
        << "* author : ";
    if (!fd.authorName.empty()) {
      out << fd.authorName << '\n';
    } else {
      out << "(unspecified)\n";
    }
    out << "* date   : ";
    if (!fd.date.empty()) {
      out << fd.date;
    } else {
      out << "(unspecified)\n";
    }
    // if (mpd.hasAttribute(MaterialPropertyDescription::algorithm)) {
    //   out << "* algorithm: "
    //       << mpd.getAttribute<std::string>(
    //              MaterialPropertyDescription::algorithm)
    //       << '\n';
    // }
    out << "\n\n";
    if (!fd.description.empty()) {
      const auto d = tokenize(fd.description, '\n');
      for (const auto& l : d) {
        if ((l.size() >= 2) && ((l)[0] == '*') && ((l)[1] == ' ')) {
          out << l.substr(2) << '\n';
        } else {
          out << l << '\n';
        }
      }
    } else {
      out << "No description specified\n";
    }
    //
    std::ifstream f(this->file);
    tfel::raise_if(!f,
                   "MaterialPropertyDocumentationGenerator::writeWebOutput: "
                   "can't open file '" +
                       this->file + "'");
    out << '\n'
        << "## Source code\n"
        << '\n'
        << "~~~~{#" << mpd.className << " .cpp .numberLines}\n"
        << f.rdbuf() << '\n'
        << "~~~~\n"
        << '\n';
  }  // end of MaterialPropertyDocumentationGenerator::writeWebOutput

  void MaterialPropertyDocumentationGenerator::writeFullOutput(
      std::ostream& out,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    using namespace tfel::utilities;
    using namespace tfel::material;
    const auto* const basic_title_level = this->standalone ? "" : "#";
    if (this->standalone) {
      out << "---\n"
          << "title: Description of material property "
          << mfront::getMaterialLawLibraryNameBase(mpd) << mpd.className
          << '\n';
      if (!fd.authorName.empty()) {
        out << "author: " << fd.authorName << '\n';
      }
      if (!fd.date.empty()) {
        out << "date: " << fd.date << '\n';
      }
      out << "lang: en-EN\n"
          << "link-citations: true\n"
          << "colorlinks: true\n"
          << "figPrefixTemplate: $$i$$\n"
          << "tblPrefixTemplate: $$i$$\n"
          << "secPrefixTemplate: $$i$$\n"
          << "eqnPrefixTemplate: ($$i$$)\n"
          << "---";
    } else {
      out << "# " << mfront::getMaterialLawLibraryNameBase(mpd) << '_'
          << mpd.className << " material property description\n\n"
          << "* file: " << fd.fileName << '\n'
          << "* author: ";
      if (!fd.authorName.empty()) {
        out << fd.authorName << '\n';
      } else {
        out << "(unspecified)\n";
      }
      out << "* date: ";
      if (!fd.date.empty()) {
        out << fd.date;
      } else {
        out << "(unspecified)";
      }
    }
    out << "\n\n";
    if (!fd.description.empty()) {
      const auto d = tokenize(fd.description, '\n');
      for (const auto& l : d) {
        if ((l.size() >= 2) && ((l)[0] == '*') && ((l)[1] == ' ')) {
          out << l.substr(2) << '\n';
        } else {
          out << l << '\n';
        }
      }
    } else {
      out << "No description specified";
    }
    out << '\n'  //
        << basic_title_level << "# Variables\n\n";
    // printData(out, mpd, "Material properties",
    //           getData(mpd,
    //           &MaterialPropertyDescription::getMaterialProperties),
    //           this->standalone);
    out << '\n';
    printData(out, mpd, "Output", getData(mpd.output), this->standalone);
    // out << '\n';
    // printData(out, mpd, "Constante",
    //           getData(mpd, &MaterialPropertyDescription::staticVars),
    //           this->standalone);
    out << '\n';
    if (mpd.hasParameters()) {
      printData(out, mpd, "Parameters", getData(mpd.parameters),
                this->standalone);
    }
    out << '\n';
    printData(out, mpd, "Inputs", getData(mpd.inputs), this->standalone);
    out << '\n';
    const auto code = getCodeBlocksDocumentation(mpd, fd, this->standalone);
    if (!code.empty() != 0) {
      out << basic_title_level  //
          << "# Code documentation\n\n"
          << code << '\n';
    }
  }  // end of MaterialPropertyDocumentationGenerator::writeFullOutput

  MaterialPropertyDocumentationGenerator::
      ~MaterialPropertyDocumentationGenerator() = default;

}  // end of namespace mfront
