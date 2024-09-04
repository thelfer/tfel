/*!
 * \file  DocumentationGeneratorBase.cxx
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
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/DocumentationGeneratorBase.hxx"

namespace mfront {

  // map::at() is missing in ekopath standard library
  template <typename Map>
  static typename Map::mapped_type map_at(const Map& m,
                                          const typename Map::key_type& k) {
    const auto p = m.find(k);
    tfel::raise_if(p == m.end(), "map_at : unknown key '" + k + "'");
    return p->second;
  }

  void DocumentationGeneratorBase::writeStandardLatexMacros(std::ostream& os) {
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

  DocumentationGeneratorBase::DocumentationGeneratorBase(
      const int argc, const char* const* const argv, const std::string& f)
      : tfel::utilities::ArgumentParserBase<DocumentationGeneratorBase>(argc,
                                                                        argv),
        file(f),
        otype(DocumentationGeneratorBase::FULL) {
    this->registerCommandLineCallBacks();
    this->parseArguments();

  }  // end of DocumentationGeneratorBase::DocumentationGeneratorBase

  void DocumentationGeneratorBase::registerCommandLineCallBacks() {
    using Parser =
        tfel::utilities::ArgumentParserBase<DocumentationGeneratorBase>;
    Parser::registerNewCallBack("--verbose",
                                &DocumentationGeneratorBase::treatVerbose,
                                "set verbose output", true);
    this->registerNewCallBack(
        "--dsl-option", &DocumentationGeneratorBase::treatDSLOption,
        "allow to define options passed to domain specific languages", true);
    this->registerNewCallBack(
        "--material-property-dsl-option",
        &DocumentationGeneratorBase::treatMaterialPropertyDSLOption,
        "allow to define options passed to domain specific languages related "
        "to material properties",
        true);
    this->registerNewCallBack(
        "--behaviour-dsl-option",
        &DocumentationGeneratorBase::treatBehaviourDSLOption,
        "allow to define options passed to domain specific languages related "
        "to behaviours",
        true);
    this->registerNewCallBack(
        "--model-dsl-option", &DocumentationGeneratorBase::treatModelDSLOption,
        "allow to define options passed to domain specific languages related "
        "to models",
        true);
    this->registerNewCallBack(
        "--dsl-options-file", &DocumentationGeneratorBase::treatDSLOptionsFile,
        "allow to define options passed to domain specific languages thanks to "
        "an external file in a JSON-like format",
        true);
    this->registerNewCallBack(
        "--material-property-dsl-options-file",
        &DocumentationGeneratorBase::treatMaterialPropertyDSLOptionsFile,
        "allow to define options passed to domain specific languages related "
        "to material properties thanks to an external file in a JSON-like "
        "format",
        true);
    this->registerNewCallBack(
        "--behaviour-dsl-options-file",
        &DocumentationGeneratorBase::treatBehaviourDSLOptionsFile,
        "allow to define options passed to domain specific languages related "
        "to behaviours thanks to an external file in a JSON-like format",
        true);
    this->registerNewCallBack(
        "--model-dsl-options-file",
        &DocumentationGeneratorBase::treatModelDSLOptionsFile,
        "allow to define options passed to domain specific languages related "
        "to models thanks to an external file in a JSON-like format",
        true);
    Parser::registerNewCallBack("--unicode-output",
                                &DocumentationGeneratorBase::treatUnicodeOutput,
                                "allow/disallow unicode output", true);
    Parser::registerNewCallBack(
        "--include", "-I", &DocumentationGeneratorBase::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    Parser::registerNewCallBack(
        "--search-path", &DocumentationGeneratorBase::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    this->registerNewCallBack(
        "--madnex-search-path",
        &DocumentationGeneratorBase::treatMadnexSearchPath,
        "add a mandex file to the search paths", true);
    Parser::registerCallBack(
        "--standalone",
        CallBack(
            "generate a standalone document (false by default)",
            [this]() noexcept { this->standalone = true; }, false));
    Parser::registerNewCallBack("--web", &DocumentationGeneratorBase::treatWeb,
                                "output a web version of the file");
    Parser::registerCallBack(
        "--std-output", "--",
        CallBack(
            "print the output ont the standard output stream",
            [this]() noexcept { this->std_output = true; }, false));
  }  // end of DocumentationGeneratorBase::registerCommandLineCallBacks

  void DocumentationGeneratorBase::treatUnknownArgument() {
    if (!MFrontBase::treatUnknownArgumentBase()) {
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      ArgumentParserBase<DocumentationGeneratorBase>::treatUnknownArgument();
#else
      auto a = static_cast<const std::string&>(
          this->getCurrentCommandLineArgument());
      std::cerr << "mfront : unsupported option '" << a << '\'' << std::endl;
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  }  // end of DocumentationGeneratorBase::treatUnknownArgument

  const tfel::utilities::Argument&
  DocumentationGeneratorBase::getCurrentCommandLineArgument() const {
    return *(this->currentArgument);
  }

  std::string DocumentationGeneratorBase::getVersionDescription() const {
    return MFrontHeader::getHeader();
  }  // end of DocumentationGeneratorBase::getVersionDescription

  std::string DocumentationGeneratorBase::getUsageDescription() const {
    std::string usage("Usage : ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  }  // end of DocumentationGeneratorBase::getUsageDescription

  void DocumentationGeneratorBase::treatWeb() {
    tfel::raise_if(this->otype != FULL,
                   "DocumentationGeneratorBase::treatWeb: "
                   "output type already specified");
    this->otype = WEB;
  }  // end of DocumentationGeneratorBase::treatWeb

  DocumentationGeneratorBase::~DocumentationGeneratorBase() = default;

  DocumentationGeneratorBase::VariableInformationBase::
      VariableInformationBase() = default;
  DocumentationGeneratorBase::VariableInformationBase::VariableInformationBase(
      VariableInformationBase&&) noexcept = default;
  DocumentationGeneratorBase::VariableInformationBase::VariableInformationBase(
      const VariableInformationBase&) = default;
  DocumentationGeneratorBase::VariableInformationBase&
  DocumentationGeneratorBase::VariableInformationBase::operator=(
      VariableInformationBase&&) = default;
  DocumentationGeneratorBase::VariableInformationBase&
  DocumentationGeneratorBase::VariableInformationBase::operator=(
      const VariableInformationBase&) = default;
  DocumentationGeneratorBase::VariableInformationBase::
      ~VariableInformationBase() noexcept = default;
}  // end of namespace mfront
