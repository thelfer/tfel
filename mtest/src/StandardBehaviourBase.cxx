/*!
 * \file  mtest/src/StandardBehaviourBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/StandardBehaviourBase.hxx"

namespace mtest {

  static void updateComponentsList(std::vector<std::string>& c,
                                   const StandardBehaviourBase& b,
                                   const std::string& v,
                                   const int t) {
    auto push_back = [&c](const std::string& n) {
      if (std::find(c.cbegin(), c.cend(), n) != c.cend()) {
        tfel::raise(
            "StandardBehaviourBase::getGradientsComponents: "
            "components multiply declared");
      }
      c.push_back(n);
    };
    for (const auto& component : getVariableComponents(b, v, t)) {
      push_back(component);
    }
  }  // end of updateComponentsList

  StandardBehaviourDescription::StandardBehaviourDescription() = default;
  StandardBehaviourDescription::StandardBehaviourDescription(
      StandardBehaviourDescription&&) = default;
  StandardBehaviourDescription::StandardBehaviourDescription(
      const StandardBehaviourDescription&) = default;
  StandardBehaviourDescription& StandardBehaviourDescription::operator=(
      StandardBehaviourDescription&&) = default;
  StandardBehaviourDescription& StandardBehaviourDescription::operator=(
      const StandardBehaviourDescription&) = default;

  StandardBehaviourDescription::StandardBehaviourDescription(
      const std::string& l, const std::string& b, const std::string& h) {
    using namespace tfel::system;
    ExternalBehaviourData::operator=(ExternalBehaviourDescription(l, b, h));
  }  // end of StandardBehaviourDescription

  void StandardBehaviourBase::allocateCurrentState(CurrentState& s) const {
    s.behaviour = this->shared_from_this();
    // clear
    s.s_1.clear();
    s.s0.clear();
    s.s1.clear();
    s.e0.clear();
    s.e1.clear();
    s.e_th0.clear();
    s.e_th1.clear();
    s.mprops1.clear();
    s.iv_1.clear();
    s.iv0.clear();
    s.iv1.clear();
    s.esv0.clear();
    s.desv.clear();
    // resizing and initialization
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    s.s_1.resize(nth, 0.);
    s.s0.resize(nth, 0.);
    s.s1.resize(nth, 0.);
    s.e0.resize(ndv, 0.);
    s.e1.resize(ndv, 0.);
    s.e_th0.resize(ndv, 0.);
    s.e_th1.resize(ndv, 0.);
    s.mprops1.resize(this->getMaterialPropertiesNames().size());
    s.iv_1.resize(this->getInternalStateVariablesSize(), 0.);
    s.iv0.resize(s.iv_1.size(), 0.);
    s.iv1.resize(s.iv0.size(), 0.);
    s.se0 = 0;
    s.se1 = 0;
    s.de0 = 0;
    s.de1 = 0;
    const auto esvnames = this->expandExternalStateVariablesNames();
    s.esv0.resize(esvnames.size(), 0.);
    s.desv.resize(esvnames.size(), 0.);
  }  // end of allocateWorkSpaceCurrentState

  std::vector<std::string> StandardBehaviourBase::getVectorComponentsSuffixes(
      const Hypothesis h) {
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return {"X", "Y", "Z"};
    } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::PLANESTRESS) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return {"X", "Y"};
    } else if (h == ModellingHypothesis::AXISYMMETRICAL) {
      return {"R", "Z"};
    } else if ((h ==
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
               (h ==
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return {"R"};
    }
    tfel::raise(
        "StandardBehaviourBase::VectorComponentsSuffixes: "
        "unsupported modelling hypothesis");
  }

  std::vector<std::string> StandardBehaviourBase::getStensorComponentsSuffixes(
      const Hypothesis h) {
    auto c = std::vector<std::string>{};
    if ((h == ModellingHypothesis::TRIDIMENSIONAL) ||
        (h == ModellingHypothesis::PLANESTRAIN) ||
        (h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      c.insert(c.end(), {"XX", "YY", "ZZ", "XY"});
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        c.insert(c.end(), {"XZ", "YZ"});
      }
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h ==
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
               (h ==
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      c.insert(c.end(), {"RR", "ZZ", "TT"});
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        c.push_back("RZ");
      }
    } else {
      tfel::raise(
          "StandardBehaviourBase::getStensorComponentsSuffixes: "
          "unsupported modelling hypothesis");
    }
    return c;
  }  // end of getStensorComponentsSuffixes

  std::vector<std::string> StandardBehaviourBase::getTensorComponentsSuffixes(
      const Hypothesis h) {
    auto c = std::vector<std::string>{};
    if ((h == ModellingHypothesis::TRIDIMENSIONAL) ||
        (h == ModellingHypothesis::PLANESTRAIN) ||
        (h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      c.insert(c.end(), {"XX", "YY", "ZZ", "XY", "YX"});
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        c.insert(c.end(), {"XZ", "ZX", "YZ", "ZY"});
      }
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h ==
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
               (h ==
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      c.insert(c.end(), {"RR", "ZZ", "TT"});
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        c.insert(c.end(), {"RZ", "ZR"});
      }
    } else {
      tfel::raise(
          "StandardBehaviourBase::getTensorComponentsSuffixes: "
          "unsupported modelling hypothesis");
    }
    return c;
  }  // end of getTensorComponentsSuffixes

  StandardBehaviourBase::StandardBehaviourBase(const Hypothesis h,
                                               const std::string& l,
                                               const std::string& b)
      : StandardBehaviourDescription(l, b, ModellingHypothesis::toString(h)) {
    tfel::raise_if(this->stype >= 2u,
                   "StandardBehaviourBase::StandardBehaviourBase: "
                   "unsupported behaviour type "
                   "(neither isotropic nor orthotropic)");
    if (this->hasTemperatureBeenRemovedFromExternalStateVariables) {
      this->evnames.insert(this->evnames.begin(), "Temperature");
      this->evtypes.insert(this->evtypes.begin(), 0);
    }
  }  // end of StandardBehaviourBase

  StandardBehaviourBase::StandardBehaviourBase(
      const StandardBehaviourDescription& umb)
      : StandardBehaviourDescription(umb) {
    tfel::raise_if(this->stype >= 2u,
                   "StandardBehaviourBase::StandardBehaviourBase: "
                   "unsupported behaviour type "
                   "(neither isotropic nor orthotropic)");
  }  // end of StandardBehaviourBase

  std::string StandardBehaviourBase::getBehaviourName() const {
    return this->behaviour;
  }  // end of getBehaviourName

  StandardBehaviourBase::Hypothesis StandardBehaviourBase::getHypothesis()
      const {
    return ModellingHypothesis::fromString(this->hypothesis);
  }  // end of getHypothesis

  void StandardBehaviourBase::setOutOfBoundsPolicy(
      const tfel::material::OutOfBoundsPolicy p) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    elm.setOutOfBoundsPolicy(this->library, this->behaviour, p);
  }  // end of setOutOfBoundsPolicy

  bool StandardBehaviourBase::hasBounds(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasBounds(this->library, this->behaviour, this->hypothesis, v);
  }  // end of hasBounds

  bool StandardBehaviourBase::hasLowerBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of hasLowerBound

  bool StandardBehaviourBase::hasUpperBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of hasUpperBound

  long double StandardBehaviourBase::getLowerBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of getLowerBound

  long double StandardBehaviourBase::getUpperBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of getUpperBound

  bool StandardBehaviourBase::hasPhysicalBounds(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasPhysicalBounds(this->library, this->behaviour,
                                 this->hypothesis, v);
  }  // end of hasPhysicalBounds

  bool StandardBehaviourBase::hasLowerPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of hasLowerPhysicalBound

  bool StandardBehaviourBase::hasUpperPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of hasUpperPhysicalBound

  long double StandardBehaviourBase::getLowerPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of getLowerPhysicalBound

  long double StandardBehaviourBase::getUpperPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of getUpperPhysicalBound

  tfel::material::MechanicalBehaviourBase::BehaviourType
  StandardBehaviourBase::getBehaviourType() const {
    using namespace tfel::material;
    switch (this->btype) {
      case 0:
        return MechanicalBehaviourBase::GENERALBEHAVIOUR;
      case 1:
        return MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR;
      case 2:
        return MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR;
      case 3:
        return MechanicalBehaviourBase::COHESIVEZONEMODEL;
    }
    tfel::raise(
        "StandardBehaviourBase::getBehaviourType: "
        "unsupported behaviour type");
  }  // end of getBehaviourType

  tfel::material::MechanicalBehaviourBase::Kinematic
  StandardBehaviourBase::getBehaviourKinematic() const {
    using namespace tfel::material;
    switch (this->kinematic) {
      case 0:
        return MechanicalBehaviourBase::UNDEFINEDKINEMATIC;
      case 1:
        return MechanicalBehaviourBase::SMALLSTRAINKINEMATIC;
      case 2:
        return MechanicalBehaviourBase::COHESIVEZONEKINEMATIC;
      case 3:
        return MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY;
      case 4:
        return MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1;
      default:
        break;
    }
    tfel::raise(
        "StandardBehaviourBase::getBehaviourKinematic: "
        "unsupported behaviour type");
  }  // end of getBehaviourKinematic

  unsigned short StandardBehaviourBase::getGradientsSize() const {
    const auto h = this->getHypothesis();
    if (this->btype == 0) {
      return getVariablesSize(this->gtypes, this->getHypothesis());
    } else if ((this->btype == 1) ||
               ((this->btype == 2u) && (this->kinematic == 4u))) {
      // small strain behaviours
      return tfel::material::getStensorSize(h);
    } else if (this->btype == 2) {
      // finite strain behaviours
      return tfel::material::getTensorSize(h);
    } else if (this->btype == 3) {
      // cohesive zone models
      if ((h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        return tfel::material::getSpaceDimension(h);
      } else {
        tfel::raise(
            "StandardBehaviourBase::getGradientsSize: "
            "unsupported modelling hypothesis");
      }
    }
    tfel::raise(
        "StandardBehaviourBase::getGradientsSize: "
        "unsupported behaviour type");
  }  // end of getGradientsSize

  unsigned short StandardBehaviourBase::getThermodynamicForcesSize() const {
    const auto h = this->getHypothesis();
    if (this->btype == 0) {
      return getVariablesSize(this->thtypes, this->getHypothesis());
    } else if ((this->btype == 1) ||
               ((this->btype == 2u) && (this->kinematic == 4u))) {
      // small strain behaviours
      return tfel::material::getStensorSize(h);
    } else if (this->btype == 2) {
      // finite strain behaviours
      return tfel::material::getStensorSize(h);
    } else if (this->btype == 3) {
      // cohesive zone models
      if ((h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        return tfel::material::getSpaceDimension(h);
      } else {
        tfel::raise(
            "StandardBehaviourBase::getThermodynamicForcesSize: "
            "unsupported modelling hypothesis");
      }
    }
    tfel::raise(
        "StandardBehaviourBase::getThermodynamicForcesSize: "
        "unsupported behaviour type");
  }  // end of getThermodynamicForcesSize

  std::vector<std::string> StandardBehaviourBase::getVectorComponentsSuffixes()
      const {
    const auto h = this->getHypothesis();
    return StandardBehaviourBase::getVectorComponentsSuffixes(h);
  }  // end of getVectorComponentsSuffixes

  std::vector<std::string> StandardBehaviourBase::getStensorComponentsSuffixes()
      const {
    const auto h = this->getHypothesis();
    return StandardBehaviourBase::getStensorComponentsSuffixes(h);
  }  // end of getStensorComponentsSuffixes

  std::vector<std::string> StandardBehaviourBase::getTensorComponentsSuffixes()
      const {
    const auto h = this->getHypothesis();
    return StandardBehaviourBase::getTensorComponentsSuffixes(h);
  }  // end of getTensorComponentsSuffixes

  std::vector<std::string> StandardBehaviourBase::getGradientsComponents()
      const {
    const auto h = this->getHypothesis();
    auto c = std::vector<std::string>{};
    if (this->btype == 0) {
      for (decltype(this->gnames.size()) i = 0; i != this->gnames.size(); ++i) {
        updateComponentsList(c, *this, this->gnames[i], this->gtypes[i]);
      }
    } else if ((this->btype == 1) ||
               ((this->btype == 2u) && (this->kinematic == 4u))) {
      updateComponentsList(c, *this, "E", 1);
    } else if (this->btype == 2) {
      updateComponentsList(c, *this, "F", 3);
    } else if (this->btype == 3) {
      if ((h == ModellingHypothesis::TRIDIMENSIONAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        c.push_back("Un");
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          c.insert(c.end(), {"Ut1", "Ut2"});
        } else {
          c.push_back("Ut");
        }
      } else {
        tfel::raise(
            "StandardBehaviourBase::getGradientsComponents: "
            "unsupported modelling hypothesis");
      }
    } else {
      tfel::raise(
          "StandardBehaviourBase::getGradientsComponents: "
          "unsupported behaviour type");
    }
    return c;
  }  // end of getGradientsComponents

  std::vector<std::string>
  StandardBehaviourBase::getThermodynamicForcesComponents() const {
    const auto h = this->getHypothesis();
    auto c = std::vector<std::string>{};
    if (this->btype == 0) {
      for (decltype(this->thnames.size()) i = 0; i != this->thnames.size();
           ++i) {
        updateComponentsList(c, *this, this->thnames[i], this->thtypes[i]);
      }
    } else if ((this->btype == 1) || (this->btype == 2)) {
      updateComponentsList(c, *this, "S", 1);
    } else if (this->btype == 3) {
      if ((h == ModellingHypothesis::TRIDIMENSIONAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        c.push_back("Tn");
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          c.push_back("Tt1");
          c.push_back("Tt2");
        } else {
          c.push_back("Tt");
        }
      } else {
        tfel::raise(
            "StandardBehaviourBase::getThermodynamicForcesComponents: "
            "unsupported modelling hypothesis");
      }
    } else {
      tfel::raise(
          "StandardBehaviourBase::getThermodynamicForcesComponents: "
          "unsupported behaviour type");
    }
    return c;
  }  // end of getThermodynamicForcesComponents

  unsigned short StandardBehaviourBase::getGradientComponentPosition(
      const std::string& cname) const {
    const auto c = this->getGradientsComponents();
    auto p = std::find(c.begin(), c.end(), cname);
    if (p == c.end()) {
      std::ostringstream msg;
      msg << "StandardBehaviourBase::getGradientComponentPosition: "
             "component '"
          << cname << "' is not valid. Valid components are:\n";
      std::copy(c.begin(), c.end(),
                std::ostream_iterator<std::string>(msg, " "));
      tfel::raise(msg.str());
    }
    return static_cast<unsigned short>(p - c.begin());
  }  // end of getGradientComponentPosition

  unsigned short StandardBehaviourBase::getThermodynamicForceComponentPosition(
      const std::string& cname) const {
    const auto c = this->getThermodynamicForcesComponents();
    auto p = std::find(c.begin(), c.end(), cname);
    if (p == c.end()) {
      std::ostringstream msg;
      msg << "StandardBehaviourBase::getThermodynamicForceComponentPosition: "
             "component '"
          << cname << "' is not valid. Valid components are:\n";
      std::copy(c.begin(), c.end(),
                std::ostream_iterator<std::string>(msg, " "));
      tfel::raise(msg.str());
    }
    return static_cast<unsigned short>(p - c.begin());
  }  // end of getThermodynamicForceComponentPosition

  size_t StandardBehaviourBase::getTangentOperatorArraySize() const {
    auto r = size_t{};
    //     auto getVariableSize = [this](const std::string& n) {
    //       auto find = [&n](const std::vector<std::string>& variables) {
    //         return std::find(variables.cbegin(), variables.cend(), n);
    //       };
    //       const auto h = this->getHypothesis();
    //       auto pg = find(this->gnames);
    //       if (pg != this->gnames.cend()) {
    //         return mtest::getVariableSize(this->gtypes[pg -
    //         this->gnames.cbegin()],
    //                                       h);
    //       }
    //       auto pth = find(this->thnames);
    //       if (pth != this->thnames.cend()) {
    //         return mtest::getVariableSize(
    //             this->thtypes[pth - this->thnames.cbegin()], h);
    //       }
    //     };
    return r;
  }  // end of getTangentOperatorSize

  std::vector<std::pair<std::string, std::string>>
  StandardBehaviourBase::getTangentOperatorBlocks() const {
    return this->tangent_operator_blocks;
  }  // end of getTangentOperatorBlocks()

  unsigned short StandardBehaviourBase::getSymmetryType() const {
    if (this->stype == 0) {
      return 0u;
    } else if (this->stype == 1) {
      return 1u;
    }
    tfel::raise(
        "StandardBehaviourBase::getSymmetryType: "
        "unsupported behaviour type "
        "(neither isotropic nor orthotropic)");
  }  // end of getSymmetryType

  size_t StandardBehaviourBase::getMaterialPropertiesSize() const {
    return this->mpnames.size();
  }  // end of getMaterialPropertiesSize

  std::vector<std::string> StandardBehaviourBase::getMaterialPropertiesNames()
      const {
    return this->mpnames;
  }

  std::vector<std::string>
  StandardBehaviourBase::getInternalStateVariablesNames() const {
    return this->ivnames;
  }

  std::vector<std::string>
  StandardBehaviourBase::expandInternalStateVariablesNames() const {
    auto ivfullnames = std::vector<std::string>{};
    for (const auto& n : this->ivnames) {
      const auto components = getVariableComponents(
          *this, n, this->getInternalStateVariableType(n));
      ivfullnames.insert(ivfullnames.end(), components.begin(),
                         components.end());
    }
    return ivfullnames;
  }

  std::vector<std::string>
  StandardBehaviourBase::getExternalStateVariablesNames() const {
    return this->evnames;
  }

  std::vector<std::string>
  StandardBehaviourBase::expandExternalStateVariablesNames() const {
    auto evfullnames = std::vector<std::string>{};
    for (const auto& n : this->evnames) {
      const auto components = getVariableComponents(
          *this, n, this->getExternalStateVariableType(n));
      evfullnames.insert(evfullnames.end(), components.begin(),
                         components.end());
    }
    return evfullnames;
  }  // end of expandExternalStateVariablesNames

  std::vector<std::string> StandardBehaviourBase::getParametersNames() const {
    return this->pnames;
  }  // end of getParametersNames

  std::vector<std::string> StandardBehaviourBase::getIntegerParametersNames()
      const {
    return this->ipnames;
  }  // end of getIntegerParametersNames

  std::vector<std::string>
  StandardBehaviourBase::getUnsignedShortParametersNames() const {
    return this->upnames;
  }  // end of getUnsignedShortParametersNames

  double StandardBehaviourBase::getRealParameterDefaultValue(
      const std::string& p) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getRealParameterDefaultValue(this->library, this->behaviour,
                                            this->hypothesis, p);
  }

  int StandardBehaviourBase::getIntegerParameterDefaultValue(
      const std::string& p) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getIntegerParameterDefaultValue(this->library, this->behaviour,
                                               this->hypothesis, p);
  }

  unsigned short StandardBehaviourBase::getUnsignedShortParameterDefaultValue(
      const std::string& p) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUnsignedShortParameterDefaultValue(
        this->library, this->behaviour, this->hypothesis, p);
  }

  size_t StandardBehaviourBase::getInternalStateVariablesSize() const {
    return getVariablesSize(this->ivtypes, this->getHypothesis());
  }  // end of getInternalStateVariablesSize

  size_t StandardBehaviourBase::getExternalStateVariablesSize() const {
    return getVariablesSize(this->evtypes, this->getHypothesis());
  }  // end of getExternalStateVariablesSize

  unsigned short StandardBehaviourBase::getExternalStateVariableType(
      const std::string& n) const {
    auto p = find(this->evnames.begin(), this->evnames.end(), n);
    tfel::raise_if(p == this->evnames.end(),
                   "StandardBehaviourBase::getExternalStateVariableType: "
                   "no external variable named '" +
                       n + "' declared");
    tfel::raise_if(this->evnames.size() != this->evtypes.size(),
                   "StandardBehaviourBase::getExternalStateVariableType: "
                   "the number of external variables names and "
                   "the number of external variables types do not match");
    return this->evtypes[p - this->evnames.begin()];
  }

  unsigned short StandardBehaviourBase::getExternalStateVariablePosition(
      const std::string& n) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "StandardBehaviourBase::getExternalStateVariablePosition: " + m);
    };
    const auto h = this->getHypothesis();
    auto p = find(this->evnames.begin(), this->evnames.end(), n);
    throw_if(p == this->evnames.end(),
             "no external variable named '" + n + "' declared");
    throw_if(this->evnames.size() != this->evtypes.size(),
             "the number of external variables names and "
             "the number of external variables types do not match");
    std::vector<std::string>::size_type i = 0;
    std::vector<std::string>::size_type ie = p - this->evnames.begin();
    unsigned short s = 0u;
    while (i != ie) {
      s += getVariableSize(this->evtypes[i], h);
      ++i;
    }
    return s;
  }

  std::vector<std::string>
  StandardBehaviourBase::getInternalStateVariablesDescriptions() const {
    std::vector<std::string> desc;
    tfel::raise_if(
        this->ivnames.size() != this->ivtypes.size(),
        "StandardBehaviourBase::getInternalStateVariablesDescriptions: "
        "internal error (the number of internal state variables names "
        "dont match the number of internal state variables types)");
    auto p = this->ivtypes.begin();
    auto pn = this->ivnames.begin();
    for (; p != this->ivtypes.end(); ++p, ++pn) {
      if (*p == 0) {
        desc.push_back(*pn);
      } else {
        std::vector<std::string> nc = getVariableComponents(*this, *pn, *p);
        for (std::vector<std::string>::size_type i = 0; i != nc.size(); ++i) {
          const auto number = [&i]() -> std::string {
            const char* nvalues[9] = {"first",   "second", "third",
                                      "fourth",  "fifth",  "sixth",
                                      "seventh", "eight",  "ninth"};
            if (i < 9) {
              return nvalues[i];
            }
            return std::to_string(i + 1) + "th";
          }();
          desc.push_back(number + " component of internal variable '" + *pn +
                         "' (" + nc[i] + ")");
        }
      }
    }
    return desc;
  }  // end of getInternalStateVariablesDescriptions

  unsigned short StandardBehaviourBase::getInternalStateVariableType(
      const std::string& n) const {
    auto p = find(this->ivnames.begin(), this->ivnames.end(), n);
    tfel::raise_if(p == this->ivnames.end(),
                   "StandardBehaviourBase::getInternalStateVariableType: "
                   "no internal variable named '" +
                       n + "' declared");
    tfel::raise_if(this->ivnames.size() != this->ivtypes.size(),
                   "StandardBehaviourBase::getInternalStateVariableType: "
                   "the number of internal variables names and "
                   "the number of internal variables types do not match");
    return this->ivtypes[p - this->ivnames.begin()];
  }

  unsigned short StandardBehaviourBase::getInternalStateVariablePosition(
      const std::string& n) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "StandardBehaviourBase::getInternalStateVariablePosition: " + m);
    };
    const auto h = this->getHypothesis();
    auto p = find(this->ivnames.begin(), this->ivnames.end(), n);
    throw_if(p == this->ivnames.end(),
             "no internal variable named '" + n + "' declared");
    throw_if(this->ivnames.size() != this->ivtypes.size(),
             "the number of internal variables names and "
             "the number of internal variables types do not match");
    std::vector<std::string>::size_type i = 0;
    std::vector<std::string>::size_type ie = p - this->ivnames.begin();
    unsigned short s = 0u;
    while (i != ie) {
      s += getVariableSize(this->ivtypes[i], h);
      ++i;
    }
    return s;
  }

  void StandardBehaviourBase::setParameter(const std::string& n,
                                           const real v) const {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library, this->behaviour, this->hypothesis, n, v);
  }  // end of setParameter

  void StandardBehaviourBase::setIntegerParameter(const std::string& n,
                                                  const int v) const {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library, this->behaviour, this->hypothesis, n, v);
  }  // end of setIntegerParameter

  void StandardBehaviourBase::setUnsignedIntegerParameter(
      const std::string& n, const unsigned short v) const {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library, this->behaviour, this->hypothesis, n, v);
  }  // end of setUnsignedIntegerParameter

  std::vector<std::string>
  StandardBehaviourBase::getOptionalMaterialProperties() const {
    return {};
  }  // end of getOptionalMaterialProperties

  void StandardBehaviourBase::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager&, const EvolutionManager&) const {
  }  // end of setOptionalMaterialPropertiesDefaultValues

  void StandardBehaviourBase::initializeTangentOperator(
      tfel::math::matrix<real>& D,
      const StiffnessMatrixType ktype,
      const bool b) const {
    if (this->getGradientsSize() == 0) {
      D(0, 0) = real(0);
      return;
    }
    if (b) {
      if ((ktype == StiffnessMatrixType::NOSTIFFNESS) ||
          (ktype ==
           StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)) {
        // do nothing
      } else if (ktype == StiffnessMatrixType::ELASTIC) {
        D(0, 0) = real(1);
      } else if (ktype == StiffnessMatrixType::SECANTOPERATOR) {
        D(0, 0) = real(2);
      } else if (ktype == StiffnessMatrixType::TANGENTOPERATOR) {
        D(0, 0) = real(3);
      } else if (ktype == StiffnessMatrixType::CONSISTENTTANGENTOPERATOR) {
        D(0, 0) = real(4);
      } else {
        tfel::raise(
            "StandardBehaviourBase::initializeTangentOperator: "
            "invalid or unspecified stiffness matrix type");
      }
    } else {
      if (ktype == StiffnessMatrixType::ELASTIC) {
        D(0, 0) = real(-1);
      } else if (ktype == StiffnessMatrixType::SECANTOPERATOR) {
        D(0, 0) = real(-2);
      } else if (ktype == StiffnessMatrixType::TANGENTOPERATOR) {
        D(0, 0) = real(-3);
      } else {
        tfel::raise(
            "StandardBehaviourBase::initializeTangentOperator: "
            "invalid or unspecified stiffness matrix type");
      }
    }
  }  // end of initializeTangentOperator

  bool StandardBehaviourBase::doPackagingStep(CurrentState&,
                                              BehaviourWorkSpace&) const {
    return true;
  }  // end of doPackagingStep

  StandardBehaviourBase::~StandardBehaviourBase() = default;

}  // end of namespace mtest
