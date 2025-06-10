/*!
 * \file   SingleStructureSchemeParser.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/ConstantExternalFunction.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/MTest.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/GenericEvolution.hxx"
#include "MTest/CastemEvolution.hxx"
#include "MTest/CyranoEvolution.hxx"
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

  void SingleStructureSchemeParser::handleHandleThermalExpansion(
      SingleStructureScheme& t, tokens_iterator& p) {
    this->checkNotEndOfLine(
        "SingleStructureSchemeParser::handleHandleThermalExpansion", p,
        this->tokens.end());
    const auto b = [&p] {
      if (p->value == "true") {
        return true;
      }
      if (p->value == "false") {
        return false;
      }
      tfel::raise(
          "SingleStructureSchemeParser::handleHandleThermalExpansion : "
          "unexpected token '" +
          p->value + "'");
    }();
    ++p;
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleHandleThermalExpansion", ";", p,
        this->tokens.end());
    t.setHandleThermalExpansion(b);
  }  // end of handleHandleThermalExpansion

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
      if ((p->value == "generic") || (p->value == "Generic")) {
        i = "Generic";
      }
#ifdef HAVE_CASTEM
      if ((p->value == "umat") || (p->value == "castem") ||
          (p->value == "Castem") || (p->value == "Cast3M")) {
        i = "castem";
      }
      if ((p->value == "castem21") || (p->value == "Castem21") ||
          (p->value == "Cast3M21")) {
        i = "castem21";
      }
      if ((p->value == "amitex") || (p->value == "Amitex")) {
        i = "amitex";
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
      if ((p->value == "aster") || (p->value == "Aster")) {
        i = p->value;
      }
#endif /* HAVE_ASTER */
#ifdef HAVE_ABAQUS
      if ((p->value == "Abaqus") || (p->value == "abaqus") ||
          (p->value == "abaqus_standard") || (p->value == "abaqus_umat")) {
        i = "abaqus";
      }
      if ((p->value == "AbaqusExplicit") || (p->value == "abaqus_explicit") ||
          (p->value == "abaqus_vumat")) {
        i = "abaqus_explicit";
      }
#endif /* HAVE_ABAQUS */
#ifdef HAVE_ANSYS
      if ((p->value == "Ansys") || (p->value == "ansys") ||
          (p->value == "ansys_usermat")) {
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
#ifdef HAVE_DIANAFEA
      if ((p->value == "dianafea") || (p->value == "DianaFEA")) {
        i = "DianaFEA";
      }
#endif /* HAVE_DIANAFEA */
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
    if (w.empty()) {
      t.setBehaviour(i, l, f, d);
    } else {
      t.setBehaviour(w, i, l, f, d);
    }
    const auto& b = *(t.getBehaviour());
    // adding parameters
    const auto& n = b.getBehaviourName();
    for (const auto& parameter : b.getParametersNames()) {
      const auto cste =
          std::make_shared<tfel::math::parser::ConstantExternalFunction>(
              b.getRealParameterDefaultValue(parameter));
      this->externalFunctions->insert({n + "::" + parameter, cste});
    }
  }  // end of handleBehaviour

  void SingleStructureSchemeParser::handleModel(SingleStructureScheme& t,
                                                tokens_iterator& p) {
    this->handleBehaviour(t, p);
    const auto& b = *(t.getBehaviour());
    if (b.getGradientsSize() != 0) {
      tfel::raise(
          "SingleStructureSchemeParser::handleModel: "
          "no gradient expected");
    }
  }  // end of handleModel

  void SingleStructureSchemeParser::handleMaterialProperty(
      SingleStructureScheme& t, tokens_iterator& p) {
    using namespace tfel::utilities;
    std::string i;
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleMaterialProperty", "<", p,
        this->tokens.end());
    this->checkNotEndOfLine(
        "SingleStructureSchemeParser::handleMaterialProperty", p,
        this->tokens.end());
    if ((p->value == "constant") || (p->value == "function") ||  //
        (p->value == "castem") || (p->value == "Castem") ||
        (p->value == "cyrano") || (p->value == "Cyrano") ||
        (p->value == "generic") || (p->value == "Generic")) {
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
      std::shared_ptr<Evolution> mpev;
      this->checkNotEndOfLine(
          "SingleStructureSchemeParser::handleMaterialProperty", p,
          this->tokens.end());
      const real v = this->readDouble(t, p);
      mpev = std::shared_ptr<Evolution>(new ConstantEvolution(v));
      t.setMaterialProperty(n, mpev, true);
    } else if (i == "function") {
      std::shared_ptr<Evolution> mpev;
      const auto f = this->readString(p, this->tokens.end());
      mpev = std::shared_ptr<Evolution>(
          new FunctionEvolution(f, t.getEvolutions()));
      t.setMaterialProperty(n, mpev, true);
    } else if ((i == "generic") || (i == "Generic")) {
      std::shared_ptr<Evolution> mpev;
      const auto l = this->readString(p, this->tokens.end());
      const auto f = this->readString(p, this->tokens.end());
      mpev = std::shared_ptr<Evolution>(
          new GenericEvolution(l, f, t.getEvolutions()));
      t.setMaterialProperty(n, mpev, true);
    } else if ((i == "castem") || (i == "Castem")) {
      std::shared_ptr<Evolution> mpev;
      const auto l = this->readString(p, this->tokens.end());
      const auto f = this->readString(p, this->tokens.end());
      mpev = std::shared_ptr<Evolution>(
          new CastemEvolution(l, f, t.getEvolutions()));
      t.setMaterialProperty(n, mpev, true);
    } else if ((i == "cyrano") || (i == "Cyrano")) {
      std::shared_ptr<Evolution> mpev;
      const auto l = this->readString(p, this->tokens.end());
      const auto f = this->readString(p, this->tokens.end());
      mpev = std::shared_ptr<Evolution>(
          new CyranoEvolution(l, f, t.getEvolutions()));
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
  }  // end of handleOutOfBoundsPolicy

  void SingleStructureSchemeParser::handleParameter(SingleStructureScheme& t,
                                                    tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    const real v = this->readDouble(t, p);
    t.setParameter(n, v);
    this->readSpecifiedToken("SingleStructureSchemeParser::handleParameter",
                             ";", p, this->tokens.end());
  }  // end of handleParameter

  void SingleStructureSchemeParser::handleIntegerParameter(
      SingleStructureScheme& t, tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    const int v = this->readInt(p, this->tokens.end());
    t.setIntegerParameter(n, v);
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleIntegerParameter", ";", p,
        this->tokens.end());
  }  // end of handleIntegerParameter

  void SingleStructureSchemeParser::handleUnsignedIntegerParameter(
      SingleStructureScheme& t, tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    const auto v = this->readUnsignedInt(p, this->tokens.end());
    t.setUnsignedIntegerParameter(n, v);
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleUnsignedIntegerParameter", ";", p,
        this->tokens.end());
  }  // end of handleUnsignedIntegerParameteru

  void SingleStructureSchemeParser::handleInternalStateVariable(
      SingleStructureScheme& t, tokens_iterator& p) {
    std::shared_ptr<Behaviour> b(t.getBehaviour());
    const auto n = this->readString(p, this->tokens.end());
    const auto& ivsnames = b->getInternalStateVariablesNames();
    auto ivs = std::vector<std::string>{};
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
      if (isScalarVariableType(type)) {
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
        const tokens_iterator p2 = p;
        for (auto pn = ivs.begin(); pn != ivs.end(); ++pn) {
          p = p2;
          this->setInternalStateVariableValue(t, p, *pn);
        }
      } else {
        this->readSpecifiedToken(
            "SingleStructureSchemeParser::handleInternalStateVariable", "{", p,
            this->tokens.end());
        for (auto pn = ivs.begin(); pn != ivs.end();) {
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
  }  // end of handleInternalStateVariable

  void SingleStructureSchemeParser::handleExternalStateVariable(
      SingleStructureScheme& t, tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    const auto& n = this->readString(p, this->tokens.end());
    const auto esvs = t.getBehaviour()->getExternalStateVariablesNames();
    const auto esvs2 = t.getBehaviour()->expandExternalStateVariablesNames();
    const auto p1 = std::find(esvs.begin(), esvs.end(), n);
    const auto p2 = std::find(esvs2.begin(), esvs2.end(), n);
    const auto b1 = p1 != esvs.end();
    const auto b2 = p2 != esvs2.end();
    if (b1) {
      // full name of the external state variable
      const auto type = t.getBehaviour()->getExternalStateVariableType(n);
      if (isScalarVariableType(type)) {
        t.setExternalStateVariable(n, this->parseEvolution(t, evt, p), true);
      } else {
        const auto& components =
            getVariableComponents(*(t.getBehaviour()), n, type);
        this->readSpecifiedToken(
            "SingleStructureSchemeParser::handleExternalStateVariable", "{", p,
            this->tokens.end());
        for (auto pc = components.begin(); pc != components.end();) {
          t.setExternalStateVariable(*pc, this->parseEvolution(t, evt, p),
                                     true);
          if (++pc != components.end()) {
            this->readSpecifiedToken(
                "SingleStructureSchemeParser::handleExternalStateVariable", ",",
                p, this->tokens.end());
          }
        }
        this->readSpecifiedToken(
            "SingleStructureSchemeParser::handleExternalStateVariable", "}", p,
            this->tokens.end());
      }
    } else if (b2) {
      // components
      t.setExternalStateVariable(n, this->parseEvolution(t, evt, p), true);
    } else {
      tfel::raise(
          "SingleStructureSchemeParser::handleExternalStateVariable: "
          "unknown external state '" +
          n + "'");
    }
    this->readSpecifiedToken(
        "SingleStructureSchemeParser::handleExternalStateVariable", ";", p,
        this->tokens.end());
  }  // end of handleExternalStateVariable

  void SingleStructureSchemeParser::setInternalStateVariableValue(
      SingleStructureScheme& t, tokens_iterator& p, const std::string& n) {
    const auto& b = *(t.getBehaviour());
    const auto type = b.getInternalStateVariableType(n);
    if (isScalarVariableType(type)) {
      t.setInternalStateVariableInitialValue(n, this->readDouble(t, p));
    } else {
      const auto s =
          getVariableSize(b.getInternalStateVariableType(n), b.getHypothesis());
      std::vector<real> v(s);
      this->readArrayOfSpecifiedSize(v, t, p);
      t.setInternalStateVariableInitialValue(n, v);
    }
  }  // end of setInternalStateVariableValue

  void SingleStructureSchemeParser::registerCallBack(
      const std::string& k, const SingleStructureSchemeParser::CallBack& p) {
    this->callbacks.insert({k, p});
  }  // end of registerCallBack

  void SingleStructureSchemeParser::registerCallBacks() {
    this->registerCallBack("@Behaviour",
                           &SingleStructureSchemeParser::handleBehaviour);
    this->registerCallBack("@Model", &SingleStructureSchemeParser::handleModel);
    this->registerCallBack(
        "@MaterialProperty",
        &SingleStructureSchemeParser::handleMaterialProperty);
    this->registerCallBack(
        "@InternalStateVariable",
        &SingleStructureSchemeParser::handleInternalStateVariable);
    this->registerCallBack(
        "@StateVariable",
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
  }  // end of registerCallBacks

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
  }  // end of treatKeyword

  std::vector<std::string> SingleStructureSchemeParser::getKeyWordsList()
      const {
    auto keys = std::vector<std::string>{};
    for (const auto& k : this->callbacks) {
      keys.push_back(k.first);
    }
    return keys;
  }  // end of getKeyWordsList

  SingleStructureSchemeParser::~SingleStructureSchemeParser() = default;

}  // end of namespace mtest
