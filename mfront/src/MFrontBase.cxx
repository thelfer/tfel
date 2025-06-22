/*!
 * \file  MFrontBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

#include "MFront/InstallPath.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DefaultDSL.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  std::shared_ptr<AbstractDSL> MFrontBase::getDSL(const std::string& f) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MFrontBase::getDSL: " + m);
    };
    using namespace tfel::system;
    auto& dslFactory = DSLFactory::getDSLFactory();
    std::shared_ptr<AbstractDSL> dsl;
    std::string library, dslName;
    tfel::utilities::CxxTokenizer file(f);
    file.stripComments();
    auto pt = file.begin();
    const auto pte = file.end();
    bool found = false;
    while ((pt != pte) && (!found)) {
      if ((pt->value == "@Parser") || (pt->value == "@DSL")) {
        if (pt != file.begin()) {
          auto ptp = pt;
          --ptp;
          throw_if(ptp->value != ";",
                   "the keyword @DSL (or @Parser) does "
                   "not begin a new instruction.");
        }
        ++pt;
        throw_if(pt == pte, "unexpected end of file (exepected dsl name)");
        throw_if(pt->value == ";", "unexepected token '" + pt->value +
                                       "'.\n"
                                       "Error at line " +
                                       std::to_string(pt->line));
        dslName = pt->value;
        ++pt;
        throw_if(pt == pte, "unexpected end of file (exepected dsl name)");
        if (pt->value != ";") {
          library = pt->value;
          ++pt;
          throw_if(pt == pte, "unexpected end of file (exepected dsl name)");
          throw_if(pt->value == ";", "unexepected token '" + pt->value +
                                         "'.\n"
                                         "Error at line " +
                                         std::to_string(pt->line));
        }
        found = true;
      }
      ++pt;
    }
    if (found) {
      try {
        if (!library.empty()) {
          auto& lm = ExternalLibraryManager::getExternalLibraryManager();
          lm.loadLibrary(library);
        }
        dsl = dslFactory.createNewParser(dslName);
      } catch (std::runtime_error& r) {
        std::ostringstream msg;
        msg << "MFrontBase::getDSL : error while loading dsl " << dslName
            << " (" << r.what() << ")\n";
        msg << "Available dsls:\n";
        const auto& dsls = dslFactory.getRegistredParsers();
        std::copy(dsls.begin(), dsls.end(),
                  std::ostream_iterator<std::string>(msg, " "));
        tfel::raise(msg.str());
      }
    } else {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream()
            << "MFrontBase::getDSL : no dsl specified, using default\n";
      }
      dsl = dslFactory.createNewParser(DefaultDSL::getName());
    }
    return dsl;
  }  // end of MFrontBase::getAbstractDSL

  MFrontBase::MFrontBase() {
    using namespace tfel::system;
    // calling mfront plugins
    const auto libs = ::getenv("MFRONT_ADDITIONAL_LIBRARIES");
    if (libs != nullptr) {
      auto& lm = ExternalLibraryManager::getExternalLibraryManager();
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      const auto sep = ';';
#else
      const auto sep = ':';
#endif
      for (const auto& l : tfel::utilities::tokenize(libs, sep)) {
        lm.loadLibrary(l);
      }
    }
  }  // end of MFrontBase

  void MFrontBase::treatSearchPath() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatSearchPath: "
                   "no path given");
    SearchPathsHandler::addSearchPaths(o);
  }

  bool MFrontBase::treatUnknownArgumentBase() {
    using tfel::utilities::starts_with;
    const auto& a = this->getCurrentCommandLineArgument();
    const auto& an = a.as_string();
#ifdef _WIN32
    if (starts_with(an, "--@") || starts_with(an, "/@")) {
#else  /* _WIN32 */
    if (starts_with(an, "--@")) {
#endif /* _WIN32 */
      const auto& o = a.getOption();
      if (an.back() == '@') {
        if (o.empty()) {
          return false;
        }
#ifdef _WIN32
        const auto s1 = starts_with(an, "/@") ? an.substr(1) : an.substr(2);
#else  /* _WIN32 */
        const auto s1 = an.substr(2);
#endif /* _WIN32 */
        tfel::raise_if(std::count(s1.begin(), s1.end(), '@') != 2,
                       "MFrontBase::treatUnknownArgumentBase: "
                       "bad substitution pattern '" +
                           s1 + "'");
        if (s1.empty()) {
          return false;
        }
        if (getVerboseMode() >= VERBOSE_LEVEL2) {
          getLogStream() << "substituting '" << s1 << "' by '" << o << "'\n";
        }
        tfel::raise_if(!this->substitutions.insert({s1, o}).second,
                       "MFrontBase::treatUnknownArgumentBase: "
                       "a substitution for '" +
                           s1 +
                           "' has "
                           "already been defined");
      } else {
#ifdef _WIN32
        auto cmd = starts_with(an, "/@") ? an.substr(1) : an.substr(2);
#else  /* _WIN32 */
        auto cmd = an.substr(2);
#endif /* _WIN32 */
        if (!o.empty()) {
          cmd += ' ' + o;
        }
        cmd += ';';
        this->ecmds.push_back(cmd);
      }
      return true;
    }
    if (starts_with(an, "-")) {
      return false;
    }
    this->inputs.insert(an);
    return true;
  }  // end of MFrontBase::treatUnknownArgument

  void MFrontBase::treatVerbose() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if (o.empty()) {
      setVerboseMode(VERBOSE_LEVEL1);
    } else {
      setVerboseMode(o);
    }
  }  // end of MFrontBase::treatVerbose

  void MFrontBase::treatUnicodeOutput() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if (o.empty()) {
      setUnicodeOutputOption(true);
    } else {
      if (o == "true") {
        setUnicodeOutputOption(true);
      } else if (o == "false") {
        setUnicodeOutputOption(false);
      } else {
        tfel::raise(
            "MFrontBase::treatUnicodeOutput: "
            "unknown option '" +
            o + "'");
      }
    }
  }  // end of MFrontBase::treatUnicodeOutput

  void MFrontBase::treatInstallPath() {
    setInstallPath(this->getCurrentCommandLineArgument().getOption());
  }  // end of MFrontBase::treatInstallPath

  void MFrontBase::treatPedantic() { setPedanticMode(true); }

  void MFrontBase::treatWarning() {}

  void MFrontBase::treatDebug() { setDebugMode(true); }

  void MFrontBase::setInterface(const std::string& i) {
    tfel::raise_if(!this->interfaces.insert(i).second,
                   "MFrontBase::treatInterface : "
                   "the interface '" +
                       i +
                       "' has "
                       "already been specified");
  }  // end of MFrontBase::setInterface

  void MFrontBase::treatInterface() {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MFrontBase::treatInterface: " + m);
    };
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    throw_if(o.empty(), "no option given to the '--interface' argument");
    for (const auto& i : tfel::utilities::tokenize(o, ',')) {
      throw_if(i.empty(), "empty interface specified.");
      this->setInterface(i);
    }
  }  // end of MFrontBase::treatInterface

  MFrontBase::~MFrontBase() = default;

}  // end of namespace mfront
