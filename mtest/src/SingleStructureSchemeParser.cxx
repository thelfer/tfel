/*!
 * \file   SingleStructureSchemeParser.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/MTest.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/CastemEvolution.hxx"
#include "MTest/FunctionEvolution.hxx"
#include "MTest/SingleStructureSchemeParser.hxx"

namespace mtest {

  static void selectVariables(std::vector<std::string>& r,
                              const std::vector<std::string>& names,
                              const std::string& n) {
    r.clear();
    if (find(names.begin(), names.end(), n) != names.end()) {
      r.push_back(n);
    } else {
      // checking for an array of internal state variables
      for (const auto& vn : names) {
        if (vn.compare(0, n.length(), n) == 0) {
          if (!(vn.size() >= n.length() + 3u)) {
            continue;
          }
          auto pn = vn.crbegin();
          const auto pne = vn.crbegin() + (vn.size() - n.size() - 1);
          if ((vn[n.size()] != '[') || (*pn != ']')) {
            continue;
          }
          ++pn;
          bool ok = true;
          while ((pn != pne) && (ok)) {
            ok = ::isdigit(*pn) != 0;
            ++pn;
          }
          if (ok) {
            r.push_back(vn);
          }
        }
      }
    }
  }  // end of selectVariables

  static unsigned short getSTensorSize(const unsigned short d) {
    if (d == 1) {
      return 3;
    } else if (d == 2) {
      return 4;
    } else if (d == 3) {
      return 6;
    }
    tfel::raise("mfront::getSTensorSize: invalid dimension");
  }

  static unsigned short getTensorSize(const unsigned short d) {
    if (d == 1) {
      return 3;
    } else if (d == 2) {
      return 5;
    } else if (d == 3) {
      return 9;
    }
    tfel::raise("mfront::getTensorSize: invalid dimension");
  }

  void SingleStructureSchemeParser::handleHandleThermalExpansion(
      SingleStructureScheme& t, tokens_iterator& p) {
    bool b;
    this->checkNotEndOfLine(
        "SingleStructureSchemeParser::handleHandleThermalExpansion", p,
        this->tokens.end());
    if (p->value == "true") {
      b = true;
    } else if (p->value == "false") {
      b = false;
    } else {
      tfel::raise(
          "SingleStructureSchemeParser::handleHandleThermalExpansion : "
          "unexpected token '" +
          p->value + "'");
    }
    ++p;
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleHandleThermalExpansion", ";", p,
        this->tokens.end());
    t.setHandleThermalExpansion(b);
  }

  void SingleStructureSchemeParser::handleBehaviour(SingleStructureScheme& t,
                                                    tokens_iterator& p) {
    auto i = std::string{};  // interface
    auto w = std::string{};  // wrapper
    this->checkNotEndOfLine("SingleStructureSchemeParser::handleBehaviour", p,
                            this->tokens.end());
    if (p->value == "<") {
      this->readSpecifiedToken("SingleStructureSchemeParser::handleBehaviour",
                               "<", p, this->tokens.end());
      this->checkNotEndOfLine("SingleStructureSchemeParser::handleBehaviour", p,
                              this->tokens.end());
#ifdef HAVE_CASTEM
      if ((p->value == "umat") || (p->value == "castem") ||
          (p->value == "Castem") || (p->value == "Cast3M")) {
        i = "castem";
      }
      if (p->value == "mistral") {
        i = "mistral";
      }
      if ((p->value == "mistral") || (p->value == "castem_umat_small_strain") ||
          (p->value == "castem_umat_finite_strain")) {
        i = p->value;
      }
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
      if (p->value == "aster") {
        i = p->value;
      }
#endif /* HAVE_ASTER */
#ifdef HAVE_EUROPLEXUS
      if ((p->value == "europlexus") || (p->value == "epx")) {
        i = p->value;
      }
#endif /* HAVE_EUROPLEXUS */
#ifdef HAVE_ABAQUS
      if ((p->value == "abaqus") || (p->value == "abaqus_standard") ||
          (p->value == "abaqus_umat")) {
        i = "abaqus";
      }
      if ((p->value == "abaqus_explicit") || (p->value == "abaqus_vumat")) {
        i = "abaqus_explicit";
      }
#endif /* HAVE_ABAQUS */
#ifdef HAVE_ANSYS
      if ((p->value == "ansys") || (p->value == "ansys_usermat")) {
        i = "ansys";
      }
#endif /* HAVE_ANSYS */
#ifdef HAVE_CYRANO
      if (p->value == "cyrano") {
        i = p->value;
      }
#endif /* HAVE_CYRANO */
#ifdef HAVE_CALCULIX
      if ((p->value == "calculix") || (p->value == "CalculiX")) {
        i = "CalculiX";
      }
#endif /* HAVE_CALCULIX */
      tfel::raise_if(i.empty(),
                     "SingleStructureSchemeParser::handleBehaviour: "
                     "unknown interface '" +
                         p->value + "'");
      ++p;
      this->checkNotEndOfLine("SingleStructureSchemeParser::handleBehaviour", p,
                              this->tokens.end());
      if (p->value == ",") {
        this->readSpecifiedToken("SingleStructureSchemeParser::handleBehaviour",
                                 ",", p, this->tokens.end());
        w = p->value;
        ++p;
      }
      this->readSpecifiedToken("SingleStructureSchemeParser::handleBehaviour",
                               ">", p, this->tokens.end());
    }
    const auto& l = this->readString(p, this->tokens.end());
    const auto& f = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine("SingleStructureSchemeParser::handleBehaviour", p,
                            this->tokens.end());
    tfel::utilities::Data d;
    if (p->value == "{") {
      d = tfel::utilities::Data::read(p, this->tokens.end());
    }
    this->readSpecifiedToken("SingleStructureSchemeParser::handleBehaviour",
                             ";", p, this->tokens.end());
    mfront::getLogStream() << l << " " << f << std::endl;
    if (w.empty()) {
      t.setBehaviour(i, l, f, d);
    } else {
      t.setBehaviour(w, i, l, f, d);
    }
  }  // end of SingleStructureSchemeParser::handleBehaviour

  void SingleStructureSchemeParser::handleMaterialProperty(
      SingleStructureScheme& t, tokens_iterator& p) {
    using namespace std;
    using namespace tfel::utilities;
    string i;
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleMaterialProperty", "<", p,
        this->tokens.end());
    this->checkNotEndOfLine(
        "SingleStructureSchemeParser::handleMaterialProperty", p,
        this->tokens.end());
    if ((p->value == "constant") || (p->value == "castem") ||
        (p->value == "function")) {
      i = p->value;
    } else {
      tfel::raise(
          "SingleStructureSchemeParser::handleMaterialProperty: "
          "unknown interface '" +
          p->value + "'");
    }
    ++p;
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleMaterialProperty", ">", p,
        this->tokens.end());
    const auto& n = this->readString(p, this->tokens.end());
    if (i == "constant") {
      shared_ptr<Evolution> mpev;
      this->checkNotEndOfLine(
          "SingleStructureSchemeParser::handleMaterialProperty", p,
          this->tokens.end());
      const real v = this->readDouble(t, p);
      mpev = shared_ptr<Evolution>(new ConstantEvolution(v));
      t.setMaterialProperty(n, mpev, true);
    } else if (i == "function") {
      shared_ptr<Evolution> mpev;
      const string f = this->readString(p, this->tokens.end());
      mpev = shared_ptr<Evolution>(new FunctionEvolution(f, t.getEvolutions()));
      t.setMaterialProperty(n, mpev, true);
    } else if (i == "castem") {
      shared_ptr<Evolution> mpev;
      const string l = this->readString(p, this->tokens.end());
      const string f = this->readString(p, this->tokens.end());
      mpev =
          shared_ptr<Evolution>(new CastemEvolution(l, f, t.getEvolutions()));
      t.setMaterialProperty(n, mpev, true);
    } else {
      tfel::raise(
          "SingleStructureSchemeParser::handleMaterialProperty: "
          "unknown interface '" +
          i + "'");
    }
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleMaterialProperty", ";", p,
        this->tokens.end());
  }

  void SingleStructureSchemeParser::handleOutOfBoundsPolicy(
      SingleStructureScheme& t, tokens_iterator& p) {
    const std::string& s = this->readString(p, this->tokens.end());
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handlePredictionPolicy", ";", p,
        this->tokens.end());
    if (s == "None") {
      t.setOutOfBoundsPolicy(tfel::material::None);
    } else if (s == "Warning") {
      t.setOutOfBoundsPolicy(tfel::material::Warning);
    } else if (s == "Strict") {
      t.setOutOfBoundsPolicy(tfel::material::Strict);
    } else {
      tfel::raise(
          "SingleStructureSchemeParser::handleOutOfBoundsPolicy: "
          "unsupported policy '" +
          s + "'");
    }
  }  // end of SingleStructureSchemeParser::handleOutOfBoundsPolicy

  void SingleStructureSchemeParser::handleParameter(SingleStructureScheme& t,
                                                    tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    const real v = this->readDouble(t, p);
    t.setParameter(n, v);
    this->readSpecifiedToken("SingleStructureSchemeParser::handleParameter",
                             ";", p, this->tokens.end());
  }  // end of SingleStructureSchemeParser::handleParameter

  void SingleStructureSchemeParser::handleIntegerParameter(
      SingleStructureScheme& t, tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    const int v = this->readInt(p, this->tokens.end());
    t.setIntegerParameter(n, v);
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleIntegerParameter", ";", p,
        this->tokens.end());
  }  // end of SingleStructureSchemeParser::handleIntegerParameter

  void SingleStructureSchemeParser::handleUnsignedIntegerParameter(
      SingleStructureScheme& t, tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    const auto v = this->readUnsignedInt(p, this->tokens.end());
    t.setUnsignedIntegerParameter(n, v);
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleUnsignedIntegerParameter", ";", p,
        this->tokens.end());
  }  // end of SingleStructureSchemeParser::handleUnsignedIntegerParameteru

  void SingleStructureSchemeParser::handleInternalStateVariable(
      SingleStructureScheme& t, tokens_iterator& p) {
    using namespace std;
    shared_ptr<Behaviour> b(t.getBehaviour());
    const string& n = this->readString(p, this->tokens.end());
    const vector<string>& ivsnames = b->getInternalStateVariablesNames();
    vector<string> ivs;
    selectVariables(ivs, ivsnames, n);
    tfel::raise_if(ivs.empty(),
                   "SingleStructureSchemeParser::handleInternalStateVariable: "
                   "the behaviour does not declare an internal state "
                   "variable named '" +
                       n + "'");
    if (ivs.size() == 1) {
      this->setInternalStateVariableValue(t, p, ivs[0]);
    } else {
      const int type = b->getInternalStateVariableType(ivs[0]);
      bool uniform = false;
      if (type == 0) {
        uniform = p->value != "{";
      } else {
        tfel::raise_if(
            p->value != "{",
            "SingleStructureSchemeParser::handleInternalStateVariable: "
            "unexpected token '" +
                n + "'");
        ++p;
        this->checkNotEndOfLine(
            "SingleStructureSchemeParser::handleInternalStateVariable", p,
            this->tokens.end());
        uniform = p->value != "{";
        --p;
      }
      if (uniform) {
        vector<string>::const_iterator pn;
        const tokens_iterator p2 = p;
        for (pn = ivs.begin(); pn != ivs.end(); ++pn) {
          p = p2;
          this->setInternalStateVariableValue(t, p, *pn);
        }
      } else {
        this->readSpecifiedToken(
            "SingleStructureSchemeParser::handleInternalStateVariable", "{", p,
            this->tokens.end());
        vector<string>::const_iterator pn;
        for (pn = ivs.begin(); pn != ivs.end();) {
          this->setInternalStateVariableValue(t, p, *pn);
          if (++pn != ivs.end()) {
            this->readSpecifiedToken(
                "SingleStructureSchemeParser::handleInternalStateVariable", ",",
                p, this->tokens.end());
          }
        }
        this->readSpecifiedToken(
            "SingleStructureSchemeParser::handleInternalStateVariable", "}", p,
            this->tokens.end());
      }
    }
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleInternalStateVariable", ";", p,
        this->tokens.end());
  }

  void SingleStructureSchemeParser::handleExternalStateVariable(
      SingleStructureScheme& t, tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    const auto& n = this->readString(p, this->tokens.end());
    t.setExternalStateVariable(n, this->parseEvolution(t, evt, p), true);
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleExternalStateVariable", ";", p,
        this->tokens.end());
  }

  void SingleStructureSchemeParser::setInternalStateVariableValue(
      SingleStructureScheme& t, tokens_iterator& p, const std::string& n) {
    using namespace std;
    const int type = t.getBehaviour()->getInternalStateVariableType(n);
    if (type == 0) {
      t.setScalarInternalStateVariableInitialValue(n, this->readDouble(t, p));
    } else if (type == 1) {
      const unsigned short N = getSTensorSize(t.getDimension());
      vector<real> v(N);
      this->readArrayOfSpecifiedSize(v, t, p);
      t.setStensorInternalStateVariableInitialValues(n, v);
    } else if (type == 3) {
      const unsigned short N = getTensorSize(t.getDimension());
      vector<real> v(N);
      this->readArrayOfSpecifiedSize(v, t, p);
      t.setTensorInternalStateVariableInitialValues(n, v);
    } else {
      tfel::raise(
          "SingleStructureSchemeParser::setInternalStateVariableValue : "
          "unsupported state variable type for "
          "internal state variable '" +
          n + "'");
    }
  }

  void SingleStructureSchemeParser::registerCallBack(
      const std::string& k, const SingleStructureSchemeParser::CallBack& p) {
    this->callbacks.insert({k, p});
  }  // end of SingleStructureSchemeParser::registerCallBack

  void SingleStructureSchemeParser::registerCallBacks() {
    this->registerCallBack("@Behaviour",
                           &SingleStructureSchemeParser::handleBehaviour);
    this->registerCallBack(
        "@MaterialProperty",
        &SingleStructureSchemeParser::handleMaterialProperty);
    this->registerCallBack(
        "@InternalStateVariable",
        &SingleStructureSchemeParser::handleInternalStateVariable);
    this->registerCallBack(
        "@ExternalStateVariable",
        &SingleStructureSchemeParser::handleExternalStateVariable);
    this->registerCallBack(
        "@OutOfBoundsPolicy",
        &SingleStructureSchemeParser::handleOutOfBoundsPolicy);
    this->registerCallBack("@Parameter",
                           &SingleStructureSchemeParser::handleParameter);
    this->registerCallBack(
        "@IntegerParameter",
        &SingleStructureSchemeParser::handleIntegerParameter);
    this->registerCallBack(
        "@UnsignedIntegerParameter",
        &SingleStructureSchemeParser::handleUnsignedIntegerParameter);
    this->registerCallBack(
        "@HandleThermalExpansion",
        &SingleStructureSchemeParser::handleHandleThermalExpansion);
  }  // end of SingleStructureSchemeParser::registerCallBacks

  bool SingleStructureSchemeParser::treatKeyword(SingleStructureScheme& t,
                                                 tokens_iterator& p) {
    auto pc = this->callbacks.find(p->value);
    if (pc == this->callbacks.end()) {
      return false;
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      auto& log = mfront::getLogStream();
      log << "SingleStructureSchemeParser::execute : treating keyword '"
          << p->value << "' at line '" << p->line << "'\n";
    }
    ++p;
    auto line = p->line;
    try {
      (this->*(pc->second))(t, p);
    } catch (std::exception& e) {
      std::ostringstream msg;
      msg << "SingleStructureSchemeParser::SingleStructureScheme : error while "
          << "parsing file '" << this->file << "' at line '" << line << "'.\n"
          << e.what();
      tfel::raise(msg.str());
    }
    return true;
  }  // end of SingleStructureSchemeParser::treatKeyword

  std::vector<std::string> SingleStructureSchemeParser::getKeyWordsList()
      const {
    auto keys = std::vector<std::string>{};
    for (const auto& k : this->callbacks) {
      keys.push_back(k.first);
    }
    return keys;
  }  // end of SingleStructureSchemeParser::getKeyWordsList

  SingleStructureSchemeParser::~SingleStructureSchemeParser() = default;

}  // end of namespace mtest
