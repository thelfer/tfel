/*!
 * \file   mtest/src/Behaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

#include "MFront/MFrontLogStream.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"

#include "MTest/GenericBehaviour.hxx"
#ifdef HAVE_CASTEM
#include "MTest/CastemSmallStrainBehaviour.hxx"
#include "MTest/CastemFiniteStrainBehaviour.hxx"
#include "MTest/CastemFiniteStrainBehaviour2.hxx"
#include "MTest/CastemCohesiveZoneModel.hxx"
#include "MTest/MistralBehaviour.hxx"
#include "MTest/CastemUmatSmallStrainBehaviour.hxx"
#include "MTest/CastemUmatFiniteStrainBehaviour.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include "MTest/AsterSmallStrainBehaviour.hxx"
#include "MTest/AsterFiniteStrainBehaviour.hxx"
#include "MTest/AsterCohesiveZoneModel.hxx"
#endif /* HAVE_ASTER  */
#ifdef HAVE_EUROPLEXUS
#include "MTest/EuroplexusFiniteStrainBehaviour.hxx"
#endif /* HAVE_EUROPLEXUS  */
#ifdef HAVE_ABAQUS
#include "MTest/AbaqusStandardBehaviour.hxx"
#include "MTest/AbaqusSmallStrainBehaviour.hxx"
#include "MTest/AbaqusFiniteStrainBehaviour.hxx"
#include "MTest/AbaqusExplicitBehaviour.hxx"
#endif /* HAVE_ABAQUS  */
#ifdef HAVE_ANSYS
#include "MTest/AnsysStandardBehaviour.hxx"
#include "MTest/AnsysSmallStrainBehaviour.hxx"
#include "MTest/AnsysFiniteStrainBehaviour.hxx"
#endif /* HAVE_ANSYS  */
#ifdef HAVE_CYRANO
#include "MTest/CyranoBehaviour.hxx"
#endif /* HAVE_CYRANO  */
#ifdef HAVE_CALCULIX
#include "MTest/CalculiXSmallStrainBehaviour.hxx"
#include "MTest/CalculiXFiniteStrainBehaviour.hxx"
#endif /* HAVE_CALCULIX  */

namespace mtest {

  std::shared_ptr<Behaviour> Behaviour::getBehaviour(const std::string& i,
                                                     const std::string& l,
                                                     const std::string& f,
                                                     const Parameters& d,
                                                     const Hypothesis h) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "Behaviour::getBehaviour: " + m);
    };
    auto check_no_parameters = [&throw_if, &d] {
      if (d.empty()) {
        return;
      }
      throw_if(!d.is<std::map<std::string, Parameters>>(),
               "unsupported parameters type");
      const auto& p = d.get<std::map<std::string, Parameters>>();
      throw_if(!p.empty(), "no parameter expected");
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    auto b = std::shared_ptr<Behaviour>{};
    auto in = i.empty() ? elm.getInterface(l, f) : i;
    if ((in == "generic") || (in == "Generic")) {
      check_no_parameters();
      b = std::make_shared<GenericBehaviour>(h, l, f);
    } else {
      throw_if(!elm.contains(l, f),
               "behaviour '" + f + "' not defined in library '" + l + "'");
    }
#ifdef HAVE_CASTEM
    if ((in == "castem") || (in == "umat") || (in == "Castem") ||
        (in == "Cast3M")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      const auto ktype = elm.getUMATBehaviourKinematic(l, f);
      if (type == 1u) {
        b = std::make_shared<CastemSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        if (ktype == 3u) {
          b = std::make_shared<CastemFiniteStrainBehaviour>(h, l, f);
        } else if (ktype == 4u) {
          b = std::make_shared<CastemFiniteStrainBehaviour2>(h, l, f);
        } else {
          throw_if(true, "unsupported kinematic");
        }
      } else if (type == 3u) {
        b = std::make_shared<CastemCohesiveZoneModel>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
    if (in == "mistral") {
      b = MistralBehaviour::buildMistralBehaviour(l, f, d, h);
    }
    if (in == "castem_umat_small_strain") {
      b = CastemUmatSmallStrainBehaviour::buildCastemUmatSmallStrainBehaviour(
          l, f, d, h);
    }
    if (in == "castem_umat_finite_strain") {
      b = CastemUmatFiniteStrainBehaviour::buildCastemUmatFiniteStrainBehaviour(
          l, f, d, h);
    }
#endif
#ifdef HAVE_ASTER
    if ((in == "aster") || (in == "Aster")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      if (type == 1u) {
        const auto ktype = elm.getUMATBehaviourKinematic(l, f);
        throw_if((ktype != 0u) && (ktype != 1u),
                 "unsupported behaviour kinematic");
        b = std::make_shared<AsterSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<AsterFiniteStrainBehaviour>(h, l, f);
      } else if (type == 3u) {
        b = std::make_shared<AsterCohesiveZoneModel>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_EUROPLEXUS
    if ((in == "europlexus") || (in == "epx") || (in == "Europlexus")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      if (type == 2u) {
        b = std::make_shared<EuroplexusFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_ABAQUS
    if ((in == "Abaqus") || (in == "abaqus") || (in == "abaqus_standard") ||
        (in == "abaqus_umat")) {
      check_no_parameters();
      const auto bn = AbaqusStandardBehaviour::getBehaviourName(f, h);
      const auto type = elm.getUMATBehaviourType(l, bn);
      if (type == 1u) {
        b = std::make_shared<AbaqusSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<AbaqusFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
    if ((in == "AbaqusExplicit") || (in == "abaqus_explicit") ||
        (in == "abaqus_vumat")) {
      const auto bn = AbaqusExplicitBehaviour::getBehaviourName(f, h);
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, bn);
      if (type == 2u) {
        b = std::make_shared<AbaqusExplicitBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_ANSYS
    if ((in == "Ansys") || (in == "ansys") || (in == "ansys_usermat")) {
      check_no_parameters();
      const auto bn = AnsysStandardBehaviour::getBehaviourName(f, h);
      const auto type = elm.getUMATBehaviourType(l, bn);
      if (type == 1u) {
        b = std::make_shared<AnsysSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<AnsysFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_CYRANO
    if ((in == "cyrano") || (in == "Cyrano")) {
      const auto btype = elm.getUMATBehaviourType(l, f);
      const auto ktype = elm.getUMATBehaviourKinematic(l, f);
      check_no_parameters();
      if (btype == 1u) {
        b = std::make_shared<CyranoBehaviour>(h, l, f);
      } else if (btype == 2u) {
        if (ktype == 4u) {
          b = std::make_shared<CyranoBehaviour>(h, l, f);
        } else {
          throw_if(true, "unsupported behaviour kinematic");
        }
      } else {
        throw_if(true, "unsupported behaviour type");
      }
    }
#endif
#ifdef HAVE_CALCULIX
    if ((in == "calculix") || (in == "CalculiX")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      if (type == 1u) {
        b = std::make_shared<CalculiXSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<CalculiXFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
    throw_if(b == nullptr, "unknown interface '" + in + "'");
    return b;
  }

  void Behaviour::setOptionalMaterialPropertyDefaultValue(
      EvolutionManager& mp,
      const EvolutionManager& evm,
      const std::string& n,
      const real v) {
    if (evm.find(n) == evm.end()) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
        auto& log = mfront::getLogStream();
        log << "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
            << "set material property '" << n << "' to default value\n";
      }
      tfel::raise_if(!mp.insert({n, make_evolution(v)}).second,
                     "Behaviour::setOptionalMaterialPropertiesDefaultValues: "
                     "default value for material property '" +
                         n + "' already declared");
    }
  }  // end of Behaviour::setOptionalMaterialPropertyDefaultValue

  Behaviour::~Behaviour() = default;

  bool isBehaviourVariable(const Behaviour& b, const std::string& n) {
    const auto enames = b.getGradientsComponents();
    auto p = std::find(enames.begin(), enames.end(), n);
    if (p != enames.end()) {
      return true;
    }
    const auto snames = b.getThermodynamicForcesComponents();
    p = std::find(snames.begin(), snames.end(), n);
    if (p != snames.end()) {
      return true;
    }
    const auto isvnames = b.expandInternalStateVariablesNames();
    p = std::find(isvnames.begin(), isvnames.end(), n);
    if (p != isvnames.end()) {
      return true;
    }
    const auto esvnames = b.getExternalStateVariablesNames();
    p = std::find(esvnames.begin(), esvnames.end(), n);
    if (p != esvnames.end()) {
      return true;
    }
    return false;
  }  // end of canValueBeExtracted

  std::function<real(const CurrentState&)> buildValueExtractor(
      const Behaviour& b, const std::string& n) {
    const auto enames = b.getGradientsComponents();
    auto p = std::find(enames.begin(), enames.end(), n);
    if (p != enames.end()) {
      const auto pos = static_cast<unsigned short>(p - enames.begin());
      return [pos](const CurrentState& s) { return s.e1[pos]; };
    }
    const auto snames = b.getThermodynamicForcesComponents();
    p = std::find(snames.begin(), snames.end(), n);
    if (p != snames.end()) {
      const auto pos = static_cast<unsigned short>(p - snames.begin());
      return [pos](const CurrentState& s) { return s.s1[pos]; };
    }
    const auto isvnames = b.expandInternalStateVariablesNames();
    p = std::find(isvnames.begin(), isvnames.end(), n);
    if (p != isvnames.end()) {
      const auto pos = static_cast<unsigned short>(p - isvnames.begin());
      return [pos](const CurrentState& s) { return s.iv1[pos]; };
    }
    const auto esvnames = b.getExternalStateVariablesNames();
    p = std::find(esvnames.begin(), esvnames.end(), n);
    if (p != esvnames.end()) {
      const auto pos = static_cast<unsigned short>(p - esvnames.begin());
      return [pos](const CurrentState& s) { return s.esv0[pos] + s.desv[pos]; };
    }
    tfel::raise("buildValueExtractor: no variable name '" + n + "'");
  }  // end of buildValueExtractor

}  // end of namespace mtest
