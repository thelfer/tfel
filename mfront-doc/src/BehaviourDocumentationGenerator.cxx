/*!
 * \file  BehaviourDocumentationGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   01 avril 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/DocumentationGeneratorBase.hxx"
#include "MFront/BehaviourDocumentationGenerator.hxx"

namespace mfront {

  // map::at() is missing in ekopath standard library
  template <typename Map>
  static typename Map::mapped_type map_at(const Map& m,
                                          const typename Map::key_type& k) {
    const auto p = m.find(k);
    tfel::raise_if(p == m.end(), "map_at : unknown key '" + k + "'");
    return p->second;
  }

  static std::string getCodeBlocksDocumentation(const BehaviourDescription& mb,
                                                const bool standalone) {
    using tfel::material::ModellingHypothesis;
    const auto* const basic_title_level = standalone ? "" : "#";
    std::ostringstream out;
    const auto& mh = mb.getModellingHypotheses();
    const auto& dh = mb.getDistinctModellingHypotheses();
    if (dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS) != dh.end()) {
      const auto& d =
          mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      const auto& cn = d.getCodeBlockNames();
      for (const auto& n : cn) {
        const auto& c = d.getCodeBlock(n);
        if (!c.description.empty()) {
          out << basic_title_level << "## " << n << " description\n"  //
              << c.description << "\n\n";
        }
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          out << basic_title_level << "## " << n << " listing\n"
              << '\n'
              << "~~~~~~~ {.cpp}\n"
              << c.code << '\n'
              << "~~~~~~~ \n";
        }
      }
    }
    for (const auto& h : mh) {
      if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        const auto& d = mb.getBehaviourData(h);
        const auto& cn = d.getCodeBlockNames();
        for (const auto& cn_pcn : cn) {
          bool print = true;
          const auto& c = d.getCodeBlock(cn_pcn);
          if (dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS) != dh.end()) {
            const auto& duh =
                mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
            if (duh.hasCode(cn_pcn)) {
              const auto& cuh = duh.getCodeBlock(cn_pcn).code;
              print = c.code != cuh;
            }
          }
          if (print) {
            if (!c.description.empty()) {
              out << basic_title_level << "## " << cn_pcn << " description("
                  << ModellingHypothesis::toString(h) << ")\n"
                  << c.description << "\n\n";
            }
            if (getVerboseMode() >= VERBOSE_DEBUG) {
              out << basic_title_level << "## " << cn_pcn << " listing ("
                  << ModellingHypothesis::toString(h) << ")\n"
                  << '\n'
                  << "~~~~~~~ {.cpp} \n"
                  << c.code << '\n'
                  << "~~~~~~~ \n";
            }
          }
        }
      }
    }
    return out.str();
  }

  static void getData(
      std::vector<BehaviourDocumentationGenerator::VariableInformation>& data,
      const BehaviourDescription& mb,
      const VariableDescriptionContainer& (BehaviourData::*m)() const,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    const auto& d = mb.getBehaviourData(h);
    const auto& mdata = (d.*m)();
    for (auto pv = mdata.begin(); pv != mdata.end(); ++pv) {
      bool found = false;
      auto pd = data.begin();
      while ((pd != data.end()) && (!found)) {
        found = pd->name == pv->name;
        if (!found) {
          ++pd;
        }
      }
      if (pd == data.end()) {
        data.push_back(BehaviourDocumentationGenerator::VariableInformation());
        pd = data.end();
        --pd;
        pd->name = pv->name;
        pd->arraySize = pv->arraySize;
        pd->type = pv->type;
        pd->externalName = d.getExternalName(pv->name);
      } else {
        const auto uncompatible = [&d, &pd, &pv] {
          if ((pd->name != pv->name) ||
              (pd->externalName != d.getExternalName(pv->name)) ||
              (pd->arraySize != pv->arraySize)) {
            return true;
          }
          if (pd->type != pv->type) {
            if (!((tfel::utilities::starts_with(pd->type, "struct")) &&
                  (tfel::utilities::starts_with(pv->type, "struct")))) {
              return true;
            }
          }
          return false;
        }();
        //         if (uncompatible) {
        //           auto display = [](const auto a, const auto b) {
        //             if (a != b) {
        //               std::cout << "'" << a << "' '" << b << "' (failed)" <<
        //               std::endl;
        //             } else {
        //               std::cout << "'" << a << "' '" << b << "' (ok)" <<
        //               std::endl;
        //             }
        //           };
        //           display(pd->name, pv->name);
        //           display(pd->type, pv->type);
        //           display(pd->externalName, d.getExternalName(pv->name));
        //           display(pd->arraySize, pv->arraySize);
        //         }
        tfel::raise_if(uncompatible,
                       "getData: inconsistent data across "
                       "hypothesis for variable '" +
                           pd->name + "'");
      }
      if (!pv->description.empty()) {
        pd->descriptions[h] = pv->description;
      }
      pd->hypotheses.push_back(h);
    }
  }  // end of

  static std::vector<BehaviourDocumentationGenerator::VariableInformation>
  getData(const BehaviourDescription& mb,
          const VariableDescriptionContainer& (BehaviourData::*m)() const) {
    using namespace tfel::material;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    auto data =
        std::vector<BehaviourDocumentationGenerator::VariableInformation>{};
    const auto& dh = mb.getDistinctModellingHypotheses();
    for (const auto& h : dh) {
      getData(data, mb, m, h);
    }
    // description deserves a specific treatment
    for (auto& d : data) {
      if (glossary.contains(d.externalName)) {
        const auto& e = glossary.getGlossaryEntry(d.externalName);
        std::ostringstream os;
        os << e.getShortDescription();
        // const auto& cd = e.getDescription();
        // for(const auto & cd_pcd : cd){
        //   if(!cd_pcd.empty()){
        //     os << cd_pcd << '\n';
        //   }
        // }
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
      const BehaviourDescription& mb,
      const std::string& title,
      const std::vector<BehaviourDocumentationGenerator::VariableInformation>&
          data,
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
    const auto& dh = mb.getDistinctModellingHypotheses();
    auto cbnames = std::set<std::string>{};
    for (const auto& h : dh) {
      const auto& d = mb.getBehaviourData(h);
      const auto& cn = d.getCodeBlockNames();
      cbnames.insert(cn.begin(), cn.end());
    }
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
      if (d.hypotheses.size() != dh.size()) {
        os << "\t+ " << map_at(l, "defined for") << " ";
        for (auto pvh = d.hypotheses.begin(); pvh != d.hypotheses.end();) {
          os << ModellingHypothesis::toString(*pvh);
          if (++pvh != d.hypotheses.end()) {
            os << ", ";
          }
        }
        os << '\n';
      }
      if (!d.description.empty()) {
        os << "\t+ " << map_at(l, "description") << ": " << d.description
           << '\n';
      }
      if (!areParametersTreatedAsStaticVariables(mb)) {
        for (const auto& h : d.hypotheses) {
          if (mb.isParameterName(h, d.name)) {
            if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
              os << "\t+ " << map_at(l, "default value") << ": ";
            } else {
              os << "\t+ "
                 << map_at(l, "default value for") + " " +
                        ModellingHypothesis::toString(h) + ": ";
            }
            if (d.type == "int") {
              os << mb.getIntegerParameterDefaultValue(h, d.name);
            } else if (d.type == "ushort") {
              os << mb.getUnsignedShortParameterDefaultValue(h, d.name);
            } else {
              const auto& p =
                  mb.getBehaviourData(h).getParameters().getVariable(d.name);
              if (p.arraySize == 1u) {
                os << mb.getFloattingPointParameterDefaultValue(h, d.name);
              } else {
                for (unsigned short i = 0; i != p.arraySize;) {
                  os << mb.getFloattingPointParameterDefaultValue(h, d.name, i);
                  if (++i != p.arraySize) {
                    os << " ";
                  }
                }
              }
            }
            os << '\n';
          }
        }
      }
      // codes blocks referring to the current variable
      auto vcb =
          std::map<std::string, std::vector<ModellingHypothesis::Hypothesis>>{};
      auto dvcb =
          std::map<std::string, std::vector<ModellingHypothesis::Hypothesis>>{};
      for (const auto& cbname : cbnames) {
        for (const auto& h : dh) {
          const auto& bd = mb.getBehaviourData(h);
          const bool b = (bd.isIntegrationVariableName(d.name) ||
                          bd.isExternalStateVariableName(d.name));
          if (bd.hasCode(cbname)) {
            const auto& cb = bd.getCodeBlock(cbname);
            if (cb.members.find(d.name) != cb.members.end()) {
              vcb[cbname].push_back(h);
            }
            if (b) {
              if (cb.members.find("d" + d.name) != cb.members.end()) {
                dvcb[cbname].push_back(h);
              }
            }
          }
        }
      }
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        if (!vcb.empty()) {
          os << "\t+ used in ";
          for (auto pc = vcb.begin(); pc != vcb.end();) {
            os << pc->first;
            if (pc->second.size() != dh.size()) {
              os << " (";
              for (auto pvh = pc->second.begin(); pvh != pc->second.end();) {
                os << ModellingHypothesis::toString(*pvh);
                if (++pvh != pc->second.end()) {
                  os << ", ";
                }
              }
              os << ")";
            }
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
            if (pc->second.size() != dh.size()) {
              os << " (";
              for (auto pvh = pc->second.begin(); pvh != pc->second.end();) {
                os << ModellingHypothesis::toString(*pvh);
                if (++pvh != pc->second.end()) {
                  os << ", ";
                }
              }
              os << ")";
            }
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

  BehaviourDocumentationGenerator::BehaviourDocumentationGenerator(
      const int argc,
      const char* const* const argv,
      std::shared_ptr<AbstractBehaviourDSL> d,
      const std::string& f)

      : DocumentationGeneratorBase(argc, argv, f), dsl(d) {
    // registring interfaces
    if (!this->interfaces.empty()) {
      dsl->setInterfaces(this->interfaces);
    }
  }  // end of BehaviourDocumentationGenerator::BehaviourDocumentationGenerator

  void BehaviourDocumentationGenerator::exe() const {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "Treating file '" << this->file << "'\n";
    }
    // analysing the file
    this->dsl->analyseFile(this->file, this->ecmds, this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& mb = this->dsl->getBehaviourDescription();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "BehaviourDocumentationGenerator::exe : begin\n";
    }
    std::ofstream output_file;
    auto& out = [&output_file, &mb, this]() -> std::ostream& {
      if (this->std_output) {
        return std::cout;
      }
      const auto name = (!mb.getLibrary().empty())
                            ? mb.getLibrary() + mb.getClassName()
                            : mb.getClassName();
      output_file.open(name + ".md");
      output_file.exceptions(std::ios::badbit | std::ios::failbit);
      tfel::raise_if(!output_file,
                     "BehaviourDocumentationGenerator::exe: "
                     "could not open file 'src/" +
                         name + ".txt'");
      return output_file;
    }();
    DocumentationGeneratorBase::writeStandardLatexMacros(out);
    if (this->otype == FULL) {
      this->writeFullOutput(out, mb, fd);
    } else if (this->otype == WEB) {
      this->writeWebOutput(out, mb, fd);
    } else {
      tfel::raise(
          "BehaviourDocumentationGenerator::exe: "
          "unsupported output type");
    }
    if (!this->std_output) {
      output_file.close();
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDocumentationGenerator::exe : end\n";
    }
  }  // end of BehaviourDocumentationGenerator::exe

  void BehaviourDocumentationGenerator::writeWebOutput(
      std::ostream& out,
      const BehaviourDescription& mb,
      const FileDescription& fd) const {
    using namespace tfel::utilities;
    out << "# " << mb.getClassName() << " behaviour description\n\n"
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
    if (mb.hasAttribute(BehaviourData::algorithm)) {
      out << "* algorithm: "
          << mb.getAttribute<std::string>(BehaviourData::algorithm) << '\n';
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
      out << "No description specified\n";
    }
    //
    std::ifstream f(this->file);
    tfel::raise_if(!f,
                   "BehaviourDocumentationGenerator::writeWebOutput: "
                   "can't open file '" +
                       this->file + "'");
    out << '\n'
        << "## Source code\n"
        << '\n'
        << "~~~~{#" << mb.getClassName() << " .cpp .numberLines}\n"
        << f.rdbuf() << '\n'
        << "~~~~\n"
        << '\n';
  }  // end of BehaviourDocumentationGenerator::writeWebOutput

  void BehaviourDocumentationGenerator::writeFullOutput(
      std::ostream& out,
      const BehaviourDescription& mb,
      const FileDescription& fd) const {
    using namespace tfel::utilities;
    using namespace tfel::material;
    const auto* const basic_title_level = this->standalone ? "" : "#";
    if (this->standalone) {
      out << "---\n"
          << "title: Description of behaviour " << mb.getClassName() << '\n';
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
      out << "# " << mb.getClassName() << " behaviour description\n\n"
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
    out << "\n\n" << basic_title_level << "## List of supported Hypotheses\n\n";
    const auto& mh = mb.getModellingHypotheses();
    const auto& dh = mb.getDistinctModellingHypotheses();
    for (const auto& h : mh) {
      out << "* " << ModellingHypothesis::toString(h);
      if (dh.find(h) != dh.end()) {
        out << ", specialised";
      }
      out << '\n';
    }
    out << '\n'  //
        << basic_title_level << "# Variables\n\n";
    printData(out, mb, "Material properties",
              getData(mb, &BehaviourData::getMaterialProperties),
              this->standalone);
    out << '\n';
    printData(out, mb, "State variables",
              getData(mb, &BehaviourData::getPersistentVariables),
              this->standalone);
    out << '\n';
    printData(out, mb, "External state variables",
              getData(mb, &BehaviourData::getExternalStateVariables),
              this->standalone);
    out << '\n';
    if (!areParametersTreatedAsStaticVariables(mb)) {
      if (mb.hasParameters()) {
        printData(out, mb, "Parameters",
                  getData(mb, &BehaviourData::getParameters), this->standalone);
      }
    }
    out << '\n';
    printData(out, mb, "Local variables",
              getData(mb, &BehaviourData::getLocalVariables), this->standalone);
    out << '\n';
    const auto code = getCodeBlocksDocumentation(mb, this->standalone);
    if (!code.empty() != 0) {
      out << basic_title_level  //
          << "# Code documentation\n\n"
          << code << '\n';
    }
  }  // end of BehaviourDocumentationGenerator::writeFullOutput

  BehaviourDocumentationGenerator::~BehaviourDocumentationGenerator() = default;

  BehaviourDocumentationGenerator::VariableInformation::VariableInformation() =
      default;
  BehaviourDocumentationGenerator::VariableInformation::VariableInformation(
      VariableInformation&&) noexcept = default;
  BehaviourDocumentationGenerator::VariableInformation::VariableInformation(
      const VariableInformation&) = default;
  BehaviourDocumentationGenerator::VariableInformation&
  BehaviourDocumentationGenerator::VariableInformation::operator=(
      VariableInformation&&) = default;
  BehaviourDocumentationGenerator::VariableInformation&
  BehaviourDocumentationGenerator::VariableInformation::operator=(
      const VariableInformation&) = default;
  BehaviourDocumentationGenerator::VariableInformation::
      ~VariableInformation() noexcept = default;

}  // end of namespace mfront
