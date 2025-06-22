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
    if (t == 0) {
      push_back(v);
    } else if (t == 1) {
      for (const auto& e : b.getStensorComponentsSuffixes()) {
        push_back(v + e);
      }
    } else if (t == 2) {
      for (const auto& e : b.getVectorComponentsSuffixes()) {
        push_back(v + e);
      }
    } else if (t == 3) {
      for (const auto& e : b.getTensorComponentsSuffixes()) {
        push_back(v + e);
      }
    } else {
      tfel::raise(
          "StandardBehaviourBase::getGradientsComponents: "
          "unsupported type for variable '" +
          v + "'");
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
  }  // end of StandardBehaviourDescription::StandardBehaviourDescription

  StandardBehaviourBase::StandardBehaviourBase(const Hypothesis h,
                                               const std::string& l,
                                               const std::string& b)
      : StandardBehaviourDescription(l, b, ModellingHypothesis::toString(h)) {
    tfel::raise_if(this->stype >= 2u,
                   "StandardBehaviourBase::StandardBehaviourBase: "
                   "unsupported behaviour type "
                   "(neither isotropic nor orthotropic)");
    this->evnames.insert(this->evnames.begin(), "Temperature");
  }  // end of StandardBehaviourBase::StandardBehaviourBase

  StandardBehaviourBase::StandardBehaviourBase(
      const StandardBehaviourDescription& umb)
      : StandardBehaviourDescription(umb) {
    tfel::raise_if(this->stype >= 2u,
                   "StandardBehaviourBase::StandardBehaviourBase: "
                   "unsupported behaviour type "
                   "(neither isotropic nor orthotropic)");
  }  // end of StandardBehaviourBase::StandardBehaviourBase

  StandardBehaviourBase::Hypothesis StandardBehaviourBase::getHypothesis()
      const {
    return ModellingHypothesis::fromString(this->hypothesis);
  }  // end of StandardBehaviourBase::getHypothesis

  void StandardBehaviourBase::setOutOfBoundsPolicy(
      const tfel::material::OutOfBoundsPolicy p) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    elm.setOutOfBoundsPolicy(this->library, this->behaviour, p);
  }  // end of StandardBehaviourBase::setOutOfBoundsPolicy

  bool StandardBehaviourBase::hasBounds(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasBounds(this->library, this->behaviour, this->hypothesis, v);
  }  // end of StandardBehaviourBase::hasBounds

  bool StandardBehaviourBase::hasLowerBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of StandardBehaviourBase::hasLowerBound

  bool StandardBehaviourBase::hasUpperBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of StandardBehaviourBase::hasUpperBound

  long double StandardBehaviourBase::getLowerBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of StandardBehaviourBase::getLowerBound

  long double StandardBehaviourBase::getUpperBound(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of StandardBehaviourBase::getUpperBound

  bool StandardBehaviourBase::hasPhysicalBounds(const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasPhysicalBounds(this->library, this->behaviour,
                                 this->hypothesis, v);
  }  // end of StandardBehaviourBase::hasPhysicalBounds

  bool StandardBehaviourBase::hasLowerPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of StandardBehaviourBase::hasLowerPhysicalBound

  bool StandardBehaviourBase::hasUpperPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of StandardBehaviourBase::hasUpperPhysicalBound

  long double StandardBehaviourBase::getLowerPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of StandardBehaviourBase::getLowerPhysicalBound

  long double StandardBehaviourBase::getUpperPhysicalBound(
      const std::string& v) const {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of StandardBehaviourBase::getUpperPhysicalBound

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
  }  // end of StandardBehaviourBase::getBehaviourType

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
    }
    tfel::raise(
        "StandardBehaviourBase::getBehaviourKinematic: "
        "unsupported behaviour type");
  }  // end of StandardBehaviourBase::getBehaviourKinematic

  unsigned short StandardBehaviourBase::getGradientsSize() const {
    const auto h = this->getHypothesis();
    if (this->btype == 0) {
      return getVariablesSize(this->gtypes, this->getHypothesis());
    } else if ((this->btype == 1) ||
               ((this->btype == 2u) && (this->kinematic == 4u))) {
      // small strain behaviours
      if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        return 3u;
      } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL)) {
        return 4u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      } else {
        tfel::raise(
            "StandardBehaviourBase::getGradientsSize: "
            "unsupported modelling hypothesis");
      }
    } else if (this->btype == 2) {
      // finite strain behaviours
      if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        return 3u;
      } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL)) {
        return 5u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 9u;
      } else {
        tfel::raise(
            "StandardBehaviourBase::getGradientsSize: "
            "unsupported modelling hypothesis");
      }
    } else if (this->btype == 3) {
      // cohesive zone models
      if ((h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL)) {
        return 2u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 3u;
      } else {
        tfel::raise(
            "StandardBehaviourBase::getGradientsSize: "
            "unsupported modelling hypothesis");
      }
    }
    tfel::raise(
        "StandardBehaviourBase::getGradientsSize: "
        "unsupported behaviour type");
  }  // end of StandardBehaviourBase::getGradientsSize

  unsigned short StandardBehaviourBase::getThermodynamicForcesSize() const {
    const auto h = this->getHypothesis();
    if (this->btype == 0) {
      return getVariablesSize(this->thtypes, this->getHypothesis());
    } else if ((this->btype == 1) ||
               ((this->btype == 2u) && (this->kinematic == 4u))) {
      // small strain behaviours
      if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        return 3u;
      } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL)) {
        return 4u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      } else {
        tfel::raise(
            "StandardBehaviourBase::getThermodynamicForcesSize: "
            "unsupported modelling hypothesis");
      }
    } else if (this->btype == 2) {
      // finite strain behaviours
      if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        return 3u;
      } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL)) {
        return 4u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      } else {
        tfel::raise(
            "StandardBehaviourBase::getThermodynamicForcesSize: "
            "unsupported modelling hypothesis");
      }
    } else if (this->btype == 3) {
      // cohesive zone models
      if ((h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL)) {
        return 2u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 3u;
      } else {
        tfel::raise(
            "StandardBehaviourBase::getThermodynamicForcesSize: "
            "unsupported modelling hypothesis");
      }
    }
    tfel::raise(
        "StandardBehaviourBase::getThermodynamicForcesSize: "
        "unsupported behaviour type");
  }  // end of StandardBehaviourBase::getThermodynamicForcesSize

  std::vector<std::string> StandardBehaviourBase::getStensorComponentsSuffixes()
      const {
    const auto h = this->getHypothesis();
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
          "StandardBehaviourBase::getGradientsComponents: "
          "unsupported modelling hypothesis");
    }
    return c;
  }  // end of StandardBehaviourBase::getStensorComponentsSuffixes

  std::vector<std::string> StandardBehaviourBase::getVectorComponentsSuffixes()
      const {
    const auto h = this->getHypothesis();
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
  }  // end of StandardBehaviourBase::getVectorComponentsSuffixes

  std::vector<std::string> StandardBehaviourBase::getTensorComponentsSuffixes()
      const {
    const auto h = this->getHypothesis();
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
  }  // end of StandardBehaviourBase::getTensorComponentsSuffixes

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
  }  // end of StandardBehaviourBase::getGradientsComponents

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
  }  // end of StandardBehaviourBase::getThermodynamicForcesComponents

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
  }  // end of StandardBehaviourBase::getGradientComponentPosition

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
  }  // end of StandardBehaviourBase::getThermodynamicForceComponentPosition

  size_t StandardBehaviourBase::getTangentOperatorArraySize() const {
    auto r = size_t{};
    auto getVariableSize = [this](const std::string& n) {
      auto find = [&n](const std::vector<std::string>& variables) {
        return std::find(variables.cbegin(), variables.cend(), n);
      };
      const auto h = this->getHypothesis();
      auto pg = find(this->gnames);
      if (pg != this->gnames.cend()) {
        return mtest::getVariableSize(this->gtypes[pg - this->gnames.cbegin()],
                                      h);
      }
      auto pth = find(this->thnames);
      if (pth != this->thnames.cend()) {
        return mtest::getVariableSize(
            this->thtypes[pth - this->thnames.cbegin()], h);
      }
    };
    return r;
  }  // end of StandardBehaviourBase::getTangentOperatorSize

  std::vector<std::pair<std::string, std::string>>
  StandardBehaviourBase::getTangentOperatorBlocks() const {
    return this->tangent_operator_blocks;
  }  // end of StandardBehaviourBase::getTangentOperatorBlocks()

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
  }  // end of StandardBehaviourBase::getSymmetryType

  size_t StandardBehaviourBase::getMaterialPropertiesSize() const {
    return this->mpnames.size();
  }  // end of StandardBehaviourBase::getMaterialPropertiesSize

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
      const auto t = this->getInternalStateVariableType(n);
      if (t == 0) {
        ivfullnames.push_back(n);
      } else if (t == 1) {
        //! suffixes of stensor components
        const auto& sexts = this->getStensorComponentsSuffixes();
        for (decltype(sexts.size()) s = 0; s != sexts.size(); ++s) {
          const auto vn = n + sexts[s];
          ivfullnames.push_back(vn);
        }
      } else if (t == 3) {
        //! suffixes f stensor components
        const auto& exts = this->getTensorComponentsSuffixes();
        for (decltype(exts.size()) s = 0; s != exts.size(); ++s) {
          const auto vn = n + exts[s];
          ivfullnames.push_back(vn);
        }
      } else {
        tfel::raise(
            "StandardBehaviourBase::expandInternalStateVariablesNames: "
            "unsupported variable type for variable '" +
            n + "'");
      }
    }
    return ivfullnames;
  }

  std::vector<std::string>
  StandardBehaviourBase::getExternalStateVariablesNames() const {
    return this->evnames;
  }

  std::vector<std::string> StandardBehaviourBase::getParametersNames() const {
    return this->pnames;
  }  // end of StandardBehaviourBase::getParametersNames

  std::vector<std::string> StandardBehaviourBase::getIntegerParametersNames()
      const {
    return this->ipnames;
  }  // end of StandardBehaviourBase::getIntegerParametersNames

  std::vector<std::string>
  StandardBehaviourBase::getUnsignedShortParametersNames() const {
    return this->upnames;
  }  // end of StandardBehaviourBase::getUnsignedShortParametersNames

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
  }  // end of StandardBehaviourBase::getInternalStateVariablesSize

  size_t StandardBehaviourBase::getExternalStateVariablesSize() const {
    return this->evnames.size();
  }  // end of StandardBehaviourBase::getExternalStateVariablesSize

  std::vector<std::string>
  StandardBehaviourBase::getInternalStateVariablesDescriptions() const {
    using namespace std;
    const auto h = this->getHypothesis();
    vector<string> desc;
    vector<int>::const_iterator p;
    vector<string>::const_iterator pn;
    tfel::raise_if(
        this->ivnames.size() != this->ivtypes.size(),
        "StandardBehaviourBase::getInternalStateVariablesDescriptions: "
        "internal error (the number of internal state variables names "
        "dont match the number of internal state variables types)");
    for (p = this->ivtypes.begin(), pn = this->ivnames.begin();
         p != this->ivtypes.end(); ++p, ++pn) {
      if (*p == 0) {
        desc.push_back(*pn);
      } else if (*p == 1) {
        // symmetric tensor
        desc.push_back("first  component of internal variable '" + *pn + "'");
        desc.push_back("second component of internal variable '" + *pn + "'");
        desc.push_back("third  component of internal variable '" + *pn + "'");
        if (!((h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS))) {
          if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
              (h == ModellingHypothesis::PLANESTRESS) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
            desc.push_back("fourth  component of internal variable '" + *pn +
                           "'");
          } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
            desc.push_back("fourth  component of internal variable '" + *pn +
                           "'");
            desc.push_back("fifth   component of internal variable '" + *pn +
                           "'");
            desc.push_back("sixth   component of internal variable '" + *pn +
                           "'");
          } else {
            tfel::raise(
                "StandardBehaviourBase::getInternalStateVariablesDescriptions: "
                "invalid modelling hypothesis");
          }
        }
      } else if (*p == 3) {
        // unsymmetric tensor
        desc.push_back("first  component of internal variable '" + *pn + "'");
        desc.push_back("second component of internal variable '" + *pn + "'");
        desc.push_back("third  component of internal variable '" + *pn + "'");
        if (!((h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS))) {
          if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
              (h == ModellingHypothesis::PLANESTRESS) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
            desc.push_back("fourth  component of internal variable '" + *pn +
                           "'");
            desc.push_back("fifth   component of internal variable '" + *pn +
                           "'");
          } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
            desc.push_back("fourth  component of internal variable '" + *pn +
                           "'");
            desc.push_back("fifth   component of internal variable '" + *pn +
                           "'");
            desc.push_back("sixth   component of internal variable '" + *pn +
                           "'");
            desc.push_back("seventh component of internal variable '" + *pn +
                           "'");
            desc.push_back("eighth  component of internal variable '" + *pn +
                           "'");
            desc.push_back("ninth   component of internal variable '" + *pn +
                           "'");
          } else {
            tfel::raise(
                "StandardBehaviourBase::getInternalStateVariablesDescriptions: "
                "invalid modelling hypothesis");
          }
        }
      } else {
        tfel::raise(
            "StandardBehaviourBase::getInternalStateVariablesDescriptions: "
            "unsupported variable type");
      }
    }
    return desc;
  }  // end of StandardBehaviourBase::getInternalStateVariablesDescriptions

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
    const auto t = this->ivtypes[p - this->ivnames.begin()];
    if (t == 0) {
      return 0u;
    } else if (t == 1) {
      return 1u;
    } else if (t == 3) {
      return 3u;
    }
    tfel::raise(
        "StandardBehaviourBase::getInternalStateVariableType: "
        "unsupported internal variable type");
  }

  unsigned short StandardBehaviourBase::getInternalStateVariablePosition(
      const std::string& n) const {
    using namespace std;
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
    vector<string>::size_type i = 0;
    vector<string>::size_type ie = p - this->ivnames.begin();
    unsigned short s = 0;
    while (i != ie) {
      int t = this->ivtypes[i];
      if (t == 0) {
        s += 1;
      } else if (t == 1) {
        if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
          s += 3;
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRESS) ||
                   (h == ModellingHypothesis::PLANESTRAIN) ||
                   (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          s += 4;
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          s += 6;
        } else {
          throw_if(true, "invalid dimension");
        }
      } else if (t == 3) {
        if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
          s += 3;
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRESS) ||
                   (h == ModellingHypothesis::PLANESTRAIN) ||
                   (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          s += 5;
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          s += 9;
        } else {
          throw_if(true, "invalid dimension");
        }
      } else {
        throw_if(true, "unsupported internal variable type");
      }
      ++i;
    }
    return s;
  }

  void StandardBehaviourBase::setParameter(const std::string& n,
                                           const real v) const {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library, this->behaviour, this->hypothesis, n, v);
  }  // end of StandardBehaviourBase::setParameter

  void StandardBehaviourBase::setIntegerParameter(const std::string& n,
                                                  const int v) const {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library, this->behaviour, this->hypothesis, n, v);
  }  // end of StandardBehaviourBase::setIntegerParameter

  void StandardBehaviourBase::setUnsignedIntegerParameter(
      const std::string& n, const unsigned short v) const {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library, this->behaviour, this->hypothesis, n, v);
  }  // end of StandardBehaviourBase::setUnsignedIntegerParameter

  std::vector<std::string>
  StandardBehaviourBase::getOptionalMaterialProperties() const {
    return {};
  }  // end of StandardBehaviourBase::getOptionalMaterialProperties

  void StandardBehaviourBase::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager&, const EvolutionManager&) const {
  }  // end of StandardBehaviourBase::setOptionalMaterialPropertiesDefaultValues

  void StandardBehaviourBase::initializeTangentOperator(
      tfel::math::matrix<real>& D,
      const StiffnessMatrixType ktype,
      const bool b) const {
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
            "StandardBehaviourBase::call_behaviour: "
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
            "StandardBehaviourBase::call_behaviour: "
            "invalid or unspecified stiffness matrix type");
      }
    }
  }  // end of StandardBehaviourBase::initializeTangentOperator

  bool StandardBehaviourBase::doPackagingStep(CurrentState&,
                                              BehaviourWorkSpace&) const {
    return true;
  }  // end of StandardBehaviourBase::doPackagingStep

  StandardBehaviourBase::~StandardBehaviourBase() = default;

}  // end of namespace mtest
