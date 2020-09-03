/*!
 * \file  MFrontBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <regex>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/MFrontDataBase.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

#include "MFront/InstallPath.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLUtilities.hxx"
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
    tfel::utilities::CxxTokenizer file;
    if (tfel::utilities::starts_with(f, "madnex:")) {
#ifdef MFRONT_HAVE_MADNEX
      const auto path = decomposeImplementationPathInMadnexFile(f);
      const auto impl =
          madnex::getMFrontImplementation(std::get<0>(path), std::get<1>(path),
                                          std::get<2>(path), std::get<3>(path));
      file.parseString(impl.source);
#else  /* HAVE_MANDEX */
      tfel::raise("DSLBase::openFile: madnex support was not enabled");
#endif /* HAVE_MANDEX */
    } else {
      file.openFile(f);
    }
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
    if (tfel::utilities::starts_with(f, "madnex:")) {
#ifdef MFRONT_HAVE_MADNEX
      const auto type = std::get<1>(decomposeImplementationPathInMadnexFile(f));
      if (type == "material_property") {
        if (dsl->getTargetType() != AbstractDSL::MATERIALPROPERTYDSL) {
          tfel::raise(
              "MFrontBase::getDSL: ill-formed madnex file, "
              "expected to read the implementation of "
              "a material property at location '" +
              f + "'");
        }
      }
      if (type == "behaviour") {
        if (dsl->getTargetType() != AbstractDSL::BEHAVIOURDSL) {
          tfel::raise(
              "MFrontBase::getDSL: ill-formed madnex file, "
              "expected to read the implementation of "
              "a behaviour at location '" +
              f + "'");
        }
      }
      if (type == "model") {
        if (dsl->getTargetType() != AbstractDSL::MODELDSL) {
          tfel::raise(
              "MFrontBase::getDSL: ill-formed madnex file, "
              "expected to read the implementation of "
              "a model at location '" +
              f + "'");
        }
      }
#else  /* HAVE_MANDEX */
      tfel::raise("DSLBase::openFile: madnex support was not enabled");
#endif /* HAVE_MANDEX */
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

  void MFrontBase::finalizeArgumentsParsing() {
    auto check = [](const std::string& s, const char* const t) {
      if (!s.empty()) {
        tfel::raise(
            "MFrontBase::finalizeArgumentsParsing: a " + std::string(t) +
            "' identifier has been specified after the last input file");
      }
    };
    check(this->material_property_identifier, "material property");
    check(this->behaviour_identifier, "behaviour");
    check(this->model_identifier, "model");
  }  // end of MFrontBase::finalizeArgumentsParsing

  void MFrontBase::treatMaterialIdentifier() {
    auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatMaterialIdentifier: "
                   "no material identifier given");
    tfel::raise_if(!this->material_identifier.empty(),
                   "MFrontBase::treatMaterialIdentifier: "
                   "material identifier already specified");
    this->material_identifier = o;
  }  // end of MFrontBase::treatMaterialIdentifier

  void MFrontBase::treatMaterialPropertyIdentifier() {
    auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatMaterialPropertyIdentifier: "
                   "no material property identifier given");
    tfel::raise_if(!this->material_property_identifier.empty(),
                   "MFrontBase::treatMaterialPropertyIdentifier: "
                   "material property identifier already specified");
    tfel::raise_if(!this->behaviour_identifier.empty(),
                   "MFrontBase::treatMaterialPropertyIdentifier: "
                   "can't specify a material property identifier and "
                   "a behaviour identifier");
    tfel::raise_if(!this->model_identifier.empty(),
                   "MFrontBase::treatMaterialPropertyIdentifier: "
                   "can't specify a behaviour identifier and "
                   "a model identifier");
    this->behaviour_identifier = o;
  }  // end of MFrontBase::treatMaterialPropertyIdentifier

  void MFrontBase::treatBehaviourIdentifier() {
    auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatBehaviourIdentifier: "
                   "no behaviour identifier given");
    tfel::raise_if(!this->behaviour_identifier.empty(),
                   "MFrontBase::treatBehaviourIdentifier: "
                   "behaviour identifier already specified");
    tfel::raise_if(!this->material_property_identifier.empty(),
                   "MFrontBase::treatBehaviourIdentifier: "
                   "can't specify a behaviour identifier and "
                   "a material property identifier");
    tfel::raise_if(!this->model_identifier.empty(),
                   "MFrontBase::treatBehaviourIdentifier: "
                   "can't specify a behaviour identifier and "
                   "a model identifier");
    this->behaviour_identifier = o;
  }  // end of MFrontBase::treatBehaviourIdentifier

  void MFrontBase::treatModelIdentifier() {
    auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatModelIdentifier: "
                   "no model identifier given");
    tfel::raise_if(!this->model_identifier.empty(),
                   "MFrontBase::treatModelIdentifier: "
                   "model identifier already specified");
    tfel::raise_if(!this->material_property_identifier.empty(),
                   "MFrontBase::treatModelIdentifier: "
                   "can't specify a model identifier and "
                   "a material property identifier");
    tfel::raise_if(!this->behaviour_identifier.empty(),
                   "MFrontBase::treatModelIdentifier: "
                   "can't specify a model identifier and "
                   "a behaviour identifier");
    this->model_identifier = o;
  }  // end of MFrontBase::treatModelIdentifier

  void MFrontBase::treatSearchPath() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(), "MFrontBase::treatSearchPath: no path given");
    SearchPathsHandler::addSearchPaths(o);
  }

  bool MFrontBase::treatUnknownArgumentBase() {
    using tfel::utilities::starts_with;
    auto raise = [](const std::string& m) {
      tfel::raise("MFrontBase::treatUnknownArgumentBase: " + m);
    };
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
        if(std::count(s1.begin(), s1.end(), '@') != 2){
          raise("bad substitution pattern '" + s1 + "'");
        }
        if (s1.empty()) {
          return false;
        }
        if (getVerboseMode() >= VERBOSE_LEVEL2) {
          getLogStream() << "substituting '" << s1 << "' by '" << o << "'\n";
        }
        if (!this->substitutions.insert({s1, o}).second) {
          raise("a substitution for '" + s1 + "' has already been defined");
        }
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
    const auto ext = [&an]() -> std::string {
      const auto p = an.find(".");
      if (p != std::string::npos) {
        return an.substr(p + 1);
      }
      return "";
    }();
    if (ext == "madnex") {
#ifdef MFRONT_HAVE_MADNEX
      this->treatMadnexFile(an);
#else  /* MFRONT_HAVE_MADNEX */
      raise("madnex support has not been enabled");
#endif /* MFRONT_HAVE_MADNEX */
    } else {
      auto check = [&ext, &raise](const std::string& s, const char* const t) {
        if (s.empty()) {
          return;
        }
        raise("specifiying a " + std::string(t) +
              "' identifier is not valid for files with the extension '" + ext +
              "'");
      };
      check(this->material_identifier, "material");
      check(this->material_property_identifier, "material property");
      check(this->behaviour_identifier, "behaviour");
      check(this->model_identifier, "model");
      this->inputs.insert(an);
    }
    this->material_identifier.clear();
    this->material_property_identifier.clear();
    this->behaviour_identifier.clear();
    this->model_identifier.clear();
    return true;
  }  // end of MFrontBase::treatUnknownArgument

#ifdef MFRONT_HAVE_MADNEX
  void MFrontBase::treatMadnexFile(const std::string& f) {
    auto db = madnex::MFrontDataBase{f};
    auto raise = [](const std::string& m) {
      tfel::raise("MFrontBase::treatMadnexFile: " + m);
    };
    if ((this->material_property_identifier.empty()) &&
        (this->behaviour_identifier.empty()) &&
        (this->model_identifier.empty())) {
      raise("no material property or behaviour or model identifier specified ");
    }
    if (this->material_identifier.empty()) {
      auto filter = [](const std::map<std::string, std::vector<std::string>>&
                           available_implementations,
                       const std::string& p) {
        auto res = std::map<std::string, std::vector<std::string>>{};
        std::regex r(p);
        for (const auto& i : available_implementations) {
          auto impls = std::vector<std::string>{};
          std::copy_if(
              i.second.begin(), i.second.end(), std::back_inserter(impls),
              [&r](const std::string& n) { return std::regex_match(n, r); });
          if (!impls.empty()) {
            res[i.first] = impls;
          }
        }
        return res;
      };  // end of filter
      auto append = [this, &f](
          const std::map<std::string, std::vector<std::string>>&
              selected_implementations,
          const std::string& t) {
        for (const auto& mi : selected_implementations) {
          for (const auto& i : mi.second) {
            this->inputs.insert("madnex:" + f + ":" + t + ":" + mi.first + ":" +
                                i);
          }
        }
      };
      if (!this->material_property_identifier.empty()) {
        const auto mps = filter(db.getAvailableMaterialProperties(),
                                this->material_property_identifier);
        if (mps.empty()) {
          raise("no material property matching '" +
                this->material_property_identifier + "' in file '" + f + "'");
        }
        append(mps, "material_property");
      }
      if (!this->behaviour_identifier.empty()) {
        const auto bs =
            filter(db.getAvailableBehaviours(), this->behaviour_identifier);
        if (bs.empty()) {
          raise("no behaviour matching '" + this->behaviour_identifier +
                "' in file '" + f + "'");
        }
        append(bs, "behaviour");
      }
      if (!this->model_identifier.empty()) {
        const auto ms = filter(db.getAvailableModels(), this->model_identifier);
        if (ms.empty()) {
          raise("no model matching '" + this->model_identifier + "' in file '" +
                f + "'");
        }
        append(ms, "model");
      }
    } else {
      auto filter = [](const std::vector<std::string>& names,
                       const std::string& p) {
        auto res = std::vector<std::string>{};
        std::regex r(p);
        std::copy_if(
            names.begin(), names.end(), std::back_inserter(res),
            [&r](const std::string& n) { return std::regex_match(n, r); });
        return res;
      };  // end of filter
      auto append = [this, &f](
          const std::vector<std::string>& selected_implementations,
          const std::string& t) {
        for (const auto& i : selected_implementations) {
          this->inputs.insert("madnex:" + f + ":" + t + ":" +
                              this->material_identifier + ":" + i);
        }
      };
      if (!this->material_property_identifier.empty()) {
        const auto mps =
            filter(db.getAvailableMaterialProperties(this->material_identifier),
                   this->material_property_identifier);
        if (mps.empty()) {
          raise("no material property matching '" +
                this->material_property_identifier + "' in file '" + f +
                "' for material '" + this->material_identifier + "'");
        }
        append(mps, "material_property");
      }
      if (!this->behaviour_identifier.empty()) {
        const auto bs =
            filter(db.getAvailableBehaviours(this->material_identifier),
                   this->behaviour_identifier);
        if (bs.empty()) {
          raise("no behaviour matching '" + this->behaviour_identifier +
                "' in file '" + f + "' for material '" +
                this->material_identifier + "'");
        }
        append(bs, "behaviour");
      }
      if (!this->model_identifier.empty()) {
        const auto ms = filter(db.getAvailableModels(this->material_identifier),
                               this->model_identifier);
        if (ms.empty()) {
          raise("no model matching '" + this->model_identifier + "' in file '" +
                f + "' for material '" + this->material_identifier + "'");
        }
        append(ms, "model");
      }
    }
  }  // end of MFrontBase::treatMadnexFile
#endif /* MFRONT_HAVE_MADNEX */

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
