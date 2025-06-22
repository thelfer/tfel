/*!
 * \file   mfront/mtest/CastemStandardBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Castem/Castem.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CastemStandardBehaviour.hxx"

namespace mtest {

  static void setMaterialProperties(
      UmatBehaviourDescription& umb,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using tfel::material::ModellingHypothesis;
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    umb.mpnames = elm.getUMATMaterialPropertiesNames(
        umb.library, umb.behaviour, ModellingHypothesis::toString(h));
    if (umb.stype == 0) {
      if (h == ModellingHypothesis::PLANESTRESS) {
        umb.mpnames.insert(umb.mpnames.begin(),
                           {"YoungModulus", "PoissonRatio", "MassDensity",
                            "ThermalExpansion", "PlateWidth"});
      } else {
        umb.mpnames.insert(umb.mpnames.begin(),
                           {"YoungModulus", "PoissonRatio", "MassDensity",
                            "ThermalExpansion"});
      }
    } else {
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        umb.mpnames.insert(
            umb.mpnames.begin(),
            {"YoungModulus1", "YoungModulus2", "YoungModulus3",
             "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
             "MassDensity", "ThermalExpansion1", "ThermalExpansion2",
             "ThermalExpansion3"});
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        umb.mpnames.insert(
            umb.mpnames.begin(),
            {"YoungModulus1", "YoungModulus2", "PoissonRatio12",
             "ShearModulus12", "V1X", "V1Y", "YoungModulus3", "PoissonRatio23",
             "PoissonRatio13", "MassDensity", "ThermalExpansion1",
             "ThermalExpansion2", "PlateWidth"});
      } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        umb.mpnames.insert(
            umb.mpnames.begin(),
            {"YoungModulus1", "YoungModulus2", "YoungModulus3",
             "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
             "ShearModulus12", "V1X", "V1Y", "MassDensity", "ThermalExpansion1",
             "ThermalExpansion2", "ThermalExpansion3"});
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        umb.mpnames.insert(
            umb.mpnames.begin(),
            {"YoungModulus1", "YoungModulus2", "YoungModulus3",
             "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
             "ShearModulus12", "ShearModulus23", "ShearModulus13", "V1X", "V1Y",
             "V1Z", "V2X", "V2Y", "V2Z", "MassDensity", "ThermalExpansion1",
             "ThermalExpansion2", "ThermalExpansion3"});
      } else {
        throw(
            std::runtime_error("setMaterialProperties: "
                               "unsupported hypothesis"));
      }
    }
  }  // end of setMaterialProperties

  CastemStandardBehaviour::CastemStandardBehaviour(const Hypothesis h,
                                                   const std::string& l,
                                                   const std::string& b)
      : UmatBehaviourBase(h, l, b) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    if (elm.getInterface(l, b) != "Castem") {
      throw(std::runtime_error(
          "CastemStandardBehaviour::CastemStandardBehaviour: "
          "invalid interface '" +
          elm.getInterface(l, b) + "'"));
    }
    this->fct = elm.getCastemExternalBehaviourFunction(l, b);
    setMaterialProperties(*this, h);
  }  // end of CastemStandardBehaviour::CastemStandardBehaviour

  CastemStandardBehaviour::CastemStandardBehaviour(
      const UmatBehaviourDescription& umb, const Hypothesis h)
      : UmatBehaviourBase(umb, h) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    this->fct =
        elm.getCastemExternalBehaviourFunction(this->library, this->behaviour);
  }  // end of CastemStandardBehaviour::CastemStandardBehaviour

  tfel::math::tmatrix<3u, 3u, real> CastemStandardBehaviour::getRotationMatrix(
      const tfel::math::vector<real>& mp,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    tfel::math::tmatrix<3u, 3u, real> nr(0.);
    const auto h = this->getHypothesis();
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      nr(0, 0) = nr(1, 1) = nr(2, 2) = 1.;  // identité
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
               (h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::PLANESTRESS)) {
      real V1X = 0;
      real V1Y = 0;
      if (h == ModellingHypothesis::PLANESTRESS) {
        V1X = mp[4];
        V1Y = mp[5];
      } else {
        V1X = mp[7];
        V1Y = mp[8];
      }
      nr(0, 0) = r(0, 0) * V1X + r(0, 1) * V1Y;
      nr(1, 0) = r(1, 0) * V1X + r(1, 1) * V1Y;
      nr(0, 1) = -nr(1, 0);
      nr(1, 1) = nr(0, 0);
      nr(2, 2) = 1;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      const real V1X = mp[9];
      const real V1Y = mp[10];
      const real V1Z = mp[11];
      const real V2X = mp[12];
      const real V2Y = mp[13];
      const real V2Z = mp[14];
      nr(0, 0) = r(0, 0) * V1X + r(0, 1) * V1Y + r(0, 2) * V1Z;
      nr(1, 0) = r(1, 0) * V1X + r(1, 1) * V1Y + r(1, 2) * V1Z;
      nr(2, 0) = r(2, 0) * V1X + r(2, 1) * V1Y + r(2, 2) * V1Z;
      nr(0, 1) = r(0, 0) * V2X + r(0, 1) * V2Y + r(0, 2) * V2Z;
      nr(1, 1) = r(1, 0) * V2X + r(1, 1) * V2Y + r(1, 2) * V2Z;
      nr(2, 1) = r(2, 0) * V2X + r(2, 1) * V2Y + r(2, 2) * V2Z;
      nr(0, 2) = nr(1, 0) * nr(2, 1) - nr(1, 1) * nr(2, 0);
      nr(1, 2) = nr(2, 0) * nr(0, 1) - nr(2, 1) * nr(0, 0);
      nr(2, 2) = nr(0, 0) * nr(1, 1) - nr(0, 1) * nr(1, 0);
    }
    return nr;
  }  // end of CastemStandardBehaviour::getRotationMatrix

  void CastemStandardBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getDrivingVariablesSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.D.resize(nth, nth);
    wk.k.resize(nth, ndv);
    wk.kt.resize(nth, ndv);
    wk.ivs.resize(nstatev == 0 ? 1u : nstatev, real(0));
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    if ((this->usesGenericPlaneStressAlgorithm) && (this->stype == 0u)) {
      wk.mps.resize(this->mpnames.size() + 1);
    } else {
      wk.mps.resize(this->mpnames.size());
    }
    mtest::allocate(wk.cs, this->shared_from_this());
  }

  StiffnessMatrixType CastemStandardBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
  }  // end of CastemStandardBehaviour::getDefaultStiffnessMatrixType

  void CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    const auto h = this->getHypothesis();
    Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "MassDensity",
                                                       0.);
    if (this->stype == 1) {
      if (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        // orthotropic behaviour
        const bool bv1x = evm.find("V1X") != evm.end();
        const bool bv1y = evm.find("V1Y") != evm.end();
        if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::PLANESTRESS) ||
            (h == ModellingHypothesis::PLANESTRAIN)) {
          const bool b = bv1x && bv1y;
          if (((bv1x) && (!b)) || ((bv1y) && (!b))) {
            throw(std::runtime_error(
                "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
                "if on component of the orthotropic basis is defined, all "
                "the components must be defined."));
          }
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V1X",
                                                             1.);
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V1Y",
                                                             0.);
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          const bool bv1z = evm.find("V1Z") != evm.end();
          const bool bv2x = evm.find("V2X") != evm.end();
          const bool bv2y = evm.find("V2Y") != evm.end();
          const bool bv2z = evm.find("V2Z") != evm.end();
          const bool b = bv1x && bv1y && bv1z && bv2x && bv2y && bv2z;
          if (((bv1x) && (!b)) || ((bv1y) && (!b)) || ((bv1z) && (!b)) ||
              ((bv2x) && (!b)) || ((bv2y) && (!b)) || ((bv2z) && (!b))) {
            throw(std::runtime_error(
                "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
                "if on component of the orthotropic basis is defined, all "
                "the components must be defined."));
          }
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V1X",
                                                             1.);
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V1Y",
                                                             0.);
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V1Z",
                                                             0.);
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V2X",
                                                             0.);
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V2Y",
                                                             1.);
          Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "V2Z",
                                                             0.);
        } else {
          throw(std::runtime_error(
              "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
              "unsupported hypothesis"));
        }
      }
    }
    Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "PlateWidth",
                                                       1.);
  }  // end of
     // CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues

  void CastemStandardBehaviour::buildMaterialProperties(
      BehaviourWorkSpace& wk, const CurrentState& s) const {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(std::runtime_error(
            "CastemSmallStrainBehaviour::buildMaterialProperties: " + m));
      }
    };
    if (this->usesGenericPlaneStressAlgorithm) {
      if (this->stype == 0u) {
        throw_if(
            wk.mps.size() != s.mprops1.size() + 1,
            "temporary material properties vector was not allocated properly");
        throw_if(s.mprops1.size() < 3, "invalid number of material properties");
        wk.mps[0] = s.mprops1[0];
        wk.mps[1] = s.mprops1[1];
        wk.mps[2] = s.mprops1[2];
        wk.mps[3] = s.mprops1[3];
        // plate width
        wk.mps[4] = castem::CastemReal(1);
        std::copy(s.mprops1.begin() + 4u, s.mprops1.end(), wk.mps.begin() + 5u);
      } else if (this->stype == 1u) {
        throw_if(
            wk.mps.size() != s.mprops1.size(),
            "temporary material properties vector was not allocated properly");
        throw_if(s.mprops1.size() < 13,
                 "invalid number of material properties");
        // YoungModulus1
        wk.mps[0] = s.mprops1[0];
        // YoungModulus2
        wk.mps[1] = s.mprops1[1];
        // PoissonRatio12
        wk.mps[2] = s.mprops1[3];
        // ShearModulus12
        wk.mps[3] = s.mprops1[6];
        // V1X
        wk.mps[4] = s.mprops1[7];
        // V1Y
        wk.mps[5] = s.mprops1[8];
        // YoungModulus3
        wk.mps[6] = s.mprops1[2];
        // PoissonRatio23
        wk.mps[7] = s.mprops1[4];
        // PoissonRatio13
        wk.mps[8] = s.mprops1[5];
        // MassDensity
        wk.mps[9] = s.mprops1[9];
        // ThermalExpansion1
        wk.mps[10] = s.mprops1[10];
        // ThermalExpansion2
        wk.mps[11] = s.mprops1[11];
        // ThermalExpansion3 (does not exists in mps)
        // plate width
        wk.mps[12] = castem::CastemReal(1);
        std::copy(s.mprops1.begin() + 13u, s.mprops1.end(),
                  wk.mps.begin() + 13u);
      } else {
        throw_if(true, "unsupported symmetry type");
      }
    } else {
      throw_if(
          wk.mps.size() != s.mprops1.size(),
          "temporary material properties vector was not allocated properly");
      std::copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
    }
  }  // end of CastemStandardBehaviour::buildMaterialProperties

  CastemStandardBehaviour::~CastemStandardBehaviour() = default;

}  // namespace mtest
