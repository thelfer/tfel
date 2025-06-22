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
#include <iterator>
#include <stdexcept>
#include <algorithm>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

#include "MFront/InstallPath.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/DefaultDSL.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/PathSpecifier.hxx"
#include "MFront/GlobalDomainSpecificLanguageOptionsManager.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  std::shared_ptr<AbstractDSL> MFrontBase::getDSL(
      const tfel::utilities::CxxTokenizer::const_iterator ptb,
      const tfel::utilities::CxxTokenizer::const_iterator pte) {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MFrontBase::getDSL: " + m);
    };
    auto dsl_options = tfel::utilities::DataMap{};
    const auto& global_options =
        GlobalDomainSpecificLanguageOptionsManager::get();
    auto& dslFactory = DSLFactory::getDSLFactory();
    std::shared_ptr<AbstractDSL> dsl;
    std::string library, dslName;
    bool found = false;
    auto pt = ptb;
    while ((pt != pte) && (!found)) {
      if ((pt->value == "@Parser") || (pt->value == "@DSL")) {
        if (pt != ptb) {
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
        if (pt->value == "{") {
          const auto o = tfel::utilities::DataParsingOptions{};
          dsl_options = tfel::utilities::Data::read(pt, pte, o)
                            .get<tfel::utilities::DataMap>();
        }
        throw_if(pt == pte,
                 "unexpected end of file (exepected ';' or library name)");
        if (pt->value != ";") {
          library = pt->value;
          ++pt;
          throw_if(pt == pte,
                   "unexpected end of file (exepected library name)");
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
      auto& lm = ExternalLibraryManager::getExternalLibraryManager();
      try {
        if (!library.empty()) {
          lm.loadLibrary(library);
        }
      } catch (std::exception& e) {
        tfel::raise("MFrontBase::getDSL : error while loading library '" +
                    library + "'\n" + std::string(e.what()));
      }
      try {
        // first try to get the target type of the DSL
        dsl = dslFactory.createNewDSL(dslName, dsl_options);
        const auto t = dsl->getTargetType();
        if (t == AbstractDSL::MATERIALPROPERTYDSL) {
          dsl = dslFactory.createNewDSL(
              dslName, tfel::utilities::merge(
                           global_options.getMaterialPropertyDSLOptions(),
                           dsl_options, true));
        } else if (t == AbstractDSL::BEHAVIOURDSL) {
          dsl = dslFactory.createNewDSL(
              dslName,
              tfel::utilities::merge(global_options.getBehaviourDSLOptions(),
                                     dsl_options, true));
        } else if (t == AbstractDSL::MODELDSL) {
          dsl = dslFactory.createNewDSL(
              dslName,
              tfel::utilities::merge(global_options.getModelDSLOptions(),
                                     dsl_options, true));
        } else {
          tfel::raise("MFrontBase::getDSL: unsupported DSL target type");
        }
      } catch (std::exception& e) {
        tfel::raise("MFrontBase::getDSL: error while creating DSL '" + dslName +
                    "'\n" + std::string(e.what()));
      }
    } else {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream()
            << "MFrontBase::getDSL : no dsl specified, using default\n";
      }
      dsl = dslFactory.createNewDSL(DefaultDSL::getName(),
                                    global_options.getBehaviourDSLOptions());
    }
    return dsl;
  }  // end of getDSL

  std::shared_ptr<AbstractDSL> MFrontBase::getDSL(const std::string& f) {
    tfel::utilities::CxxTokenizer file;
    if ((tfel::utilities::starts_with(f, "madnex:")) ||
        (tfel::utilities::starts_with(f, "mdnx:")) ||
        (tfel::utilities::starts_with(f, "edf:"))) {
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
    auto dsl = MFrontBase::getDSL(file.cbegin(), file.cend());
    if ((tfel::utilities::starts_with(f, "madnex:")) ||
        (tfel::utilities::starts_with(f, "mdnx:")) ||
        (tfel::utilities::starts_with(f, "edf:"))) {
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
  }  // end of getDSL

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
  }  // end of finalizeArgumentsParsing

  void MFrontBase::treatMaterialIdentifier() {
    auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatMaterialIdentifier: "
                   "no material identifier given");
    tfel::raise_if(!this->material_identifier.empty(),
                   "MFrontBase::treatMaterialIdentifier: "
                   "material identifier already specified");
    this->material_identifier = o;
  }  // end of treatMaterialIdentifier

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
    this->material_property_identifier = o;
  }  // end of treatMaterialPropertyIdentifier

  void MFrontBase::treatAllMaterialProperties() {
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
    this->material_property_identifier = ".+";
  }  // end of treatAllMaterialProperties

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
  }  // end of treatBehaviourIdentifier

  void MFrontBase::treatAllBehaviours() {
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
    this->behaviour_identifier = ".+";
  }  // end of treatAllBehaviours

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
  }  // end of treatModelIdentifier

  void MFrontBase::treatAllModels() {
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
    this->model_identifier = ".+";
  }  // end of treatAllModels

  void MFrontBase::treatSearchPath() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(), "MFrontBase::treatSearchPath: no path given");
    SearchPathsHandler::addSearchPaths(o);
  }

  void MFrontBase::treatMadnexSearchPath() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "MFrontBase::treatMadnexSearchPath: "
                   "no path given");
    SearchPathsHandler::addMadnexSearchPath(o);
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
        if (std::count(s1.begin(), s1.end(), '@') != 2) {
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
    this->addInputPaths(an);
    return true;
  }  // end of treatUnknownArgument

  void MFrontBase::addInputPaths(const std::string& p) {
    const auto paths = getImplementationsPaths(
        p, this->material_identifier, this->material_property_identifier,
        this->behaviour_identifier, this->model_identifier);
    SearchPathsHandler::addSearchPathsFromImplementationPaths(paths);
    this->inputs.insert(paths.begin(), paths.end());
    this->material_identifier.clear();
    this->material_property_identifier.clear();
    this->behaviour_identifier.clear();
    this->model_identifier.clear();
  }  // end of addInputPaths

  void MFrontBase::treatVerbose() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if (o.empty()) {
      setVerboseMode(VERBOSE_LEVEL1);
    } else {
      setVerboseMode(o);
    }
  }  // end of treatVerbose

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
  }  // end of treatUnicodeOutput

  void MFrontBase::treatInstallPath() {
    setInstallPath(this->getCurrentCommandLineArgument().getOption());
  }  // end of treatInstallPath

  void MFrontBase::treatPedantic() { setPedanticMode(true); }

  void MFrontBase::treatWarning() {}

  void MFrontBase::treatDebug() { setDebugMode(true); }

  static std::pair<std::string, std::string> splitDSLOption(
      const std::string& o) {
    auto raise_invalid_options = [&o] {
      tfel::raise("splitDSLoption: invalid DSL option '" + o + "'");
    };
    const auto pos = o.find_first_of(':');
    if ((pos == std::string::npos) || (pos + 1 == o.size())) {
      raise_invalid_options();
    }
    return {o.substr(0, pos), o.substr(pos + 1)};
  }  // end of splitDSLOption

  void MFrontBase::addDSLOption(const std::string& o) {
    auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
    const auto& kv = splitDSLOption(o);
    g.addDSLOption(kv.first, kv.second);
  }

  void MFrontBase::addMaterialPropertyDSLOption(const std::string& o) {
    auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
    const auto& kv = splitDSLOption(o);
    g.addMaterialPropertyDSLOption(kv.first, kv.second);
  }

  void MFrontBase::addBehaviourDSLOption(const std::string& o) {
    auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
    const auto& kv = splitDSLOption(o);
    g.addBehaviourDSLOption(kv.first, kv.second);
  }

  void MFrontBase::addModelDSLOption(const std::string& o) {
    auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
    const auto& kv = splitDSLOption(o);
    g.addModelDSLOption(kv.first, kv.second);
  }

  template <typename CallBack>
  static void parseDSLOptionsFile(const CallBack& callback,
                                  const std::string& f) {
    std::ifstream ifs(f);
    if (!ifs) {
      tfel::raise("parseDSLOptionsFile: can't open file '" + f + "'");
    }
    tfel::utilities::CxxTokenizer t;
    t.parseString('{' + std::string{std::istreambuf_iterator<char>{ifs}, {}} +
                  '}');
    auto b = t.begin();
    const auto data = tfel::utilities::Data::read(b, t.end());
    for (const auto& d : data.get<tfel::utilities::DataMap>()) {
      callback(d.first, d.second);
    }
  }  // end of parseDSLOptionsFile

  void MFrontBase::parseDSLOptionsFile(const std::string& f) {
    const auto c = [](const std::string& k, const tfel::utilities::Data& d) {
      auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
      g.addDSLOption(k, d);
    };
    mfront::parseDSLOptionsFile(c, f);
  }

  void MFrontBase::parseMaterialPropertyDSLOptionsFile(const std::string& f) {
    const auto c = [](const std::string& k, const tfel::utilities::Data& d) {
      auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
      g.addMaterialPropertyDSLOption(k, d);
    };
    mfront::parseDSLOptionsFile(c, f);
  }

  void MFrontBase::parseBehaviourDSLOptionsFile(const std::string& f) {
    const auto c = [](const std::string& k, const tfel::utilities::Data& d) {
      auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
      g.addBehaviourDSLOption(k, d);
    };
    mfront::parseDSLOptionsFile(c, f);
  }

  void MFrontBase::parseModelDSLOptionsFile(const std::string& f) {
    const auto c = [](const std::string& k, const tfel::utilities::Data& d) {
      auto& g = GlobalDomainSpecificLanguageOptionsManager::get();
      g.addModelDSLOption(k, d);
    };
    mfront::parseDSLOptionsFile(c, f);
  }

  void MFrontBase::treatDSLOption() {
    MFrontBase::addDSLOption(this->getCurrentCommandLineArgument().getOption());
  }  // end of treatDSLOption

  void MFrontBase::treatMaterialPropertyDSLOption() {
    MFrontBase::addMaterialPropertyDSLOption(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatMaterialPropertyDSLOption

  void MFrontBase::treatBehaviourDSLOption() {
    MFrontBase::addBehaviourDSLOption(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatBehaviourDSLOption

  void MFrontBase::treatModelDSLOption() {
    MFrontBase::addModelDSLOption(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatModelDSLOption

  void MFrontBase::treatDSLOptionsFile() {
    MFrontBase::parseDSLOptionsFile(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatDSLOptionsFile

  void MFrontBase::treatMaterialPropertyDSLOptionsFile() {
    MFrontBase::parseMaterialPropertyDSLOptionsFile(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatMaterialPropertyDSLOptionsFile

  void MFrontBase::treatBehaviourDSLOptionsFile() {
    MFrontBase::parseBehaviourDSLOptionsFile(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatBehaviourDSLOptionsFile

  void MFrontBase::treatModelDSLOptionsFile() {
    MFrontBase::parseModelDSLOptionsFile(
        this->getCurrentCommandLineArgument().getOption());
  }  // end of treatModelDSLOptionsFile

  void MFrontBase::setInterface(const std::string& i) {
    tfel::raise_if(!this->interfaces.insert(i).second,
                   "MFrontBase::treatInterface : "
                   "the interface '" +
                       i +
                       "' has "
                       "already been specified");
  }  // end of setInterface

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
  }  // end of treatInterface

  MFrontBase::~MFrontBase() = default;

}  // end of namespace mfront
