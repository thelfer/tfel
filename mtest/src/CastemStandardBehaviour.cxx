/*!
 * \file   mtest/src/CastemStandardBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Castem/Castem.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CastemEvolution.hxx"
#include "MTest/CastemStandardBehaviour.hxx"

namespace mtest {

  static void setMaterialProperties(
      StandardBehaviourDescription& umb,
      const tfel::material::ModellingHypothesis::Hypothesis h,
      const CastemInterfaceVersion v) {
    using tfel::material::ModellingHypothesis;
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    umb.mpnames.clear();
    if (umb.stype == 0) {  // isotropic case
      if (h == ModellingHypothesis::PLANESTRESS) {
        umb.mpnames.insert(umb.mpnames.begin(),
                           {"YoungModulus", "PoissonRatio", "MassDensity",
                            "ThermalExpansion", "PlateWidth"});
      } else {
        umb.mpnames.insert(umb.mpnames.begin(),
                           {"YoungModulus", "PoissonRatio", "MassDensity",
                            "ThermalExpansion"});
      }
    } else {  // orthotropic case
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
        tfel::raise("setMaterialProperties: unsupported hypothesis");
      }
    }
    if (v == CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
      umb.mpnames.insert(umb.mpnames.end(),
                         {"ReferenceTemperatureForThermalExpansionCoefficient",
                          "ReferenceTemperatureForThermalExpansion"});
    }
    const auto mps = elm.getUMATMaterialPropertiesNames(
        umb.library, umb.behaviour, ModellingHypothesis::toString(h));
    umb.mpnames.insert(umb.mpnames.end(), mps.begin(), mps.end());
  }  // end of setMaterialProperties

  CastemStandardBehaviour::CastemStandardBehaviour(const Hypothesis h,
                                                   const std::string& l,
                                                   const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto i = elm.getInterface(l, b);
    tfel::raise_if((i != "Castem") && (i != "Castem21"),
                   "CastemStandardBehaviour::CastemStandardBehaviour: "
                   "invalid interface '" +
                       elm.getInterface(l, b) + "'");
    this->fct = elm.getCastemExternalBehaviourFunction(l, b);
    if (i == "Castem21") {
      setMaterialProperties(
          *this, h, CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021);
    } else {
      setMaterialProperties(
          *this, h, CastemInterfaceVersion::LEGACY_CASTEM_INTERFACE_VERSION);
    }
  }  // end of CastemStandardBehaviour

  CastemStandardBehaviour::CastemStandardBehaviour(
      const StandardBehaviourDescription& umb)
      : StandardBehaviourBase(umb) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    this->fct =
        elm.getCastemExternalBehaviourFunction(this->library, this->behaviour);
  }  // end of CastemStandardBehaviour

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
  }  // end of getRotationMatrix

  void CastemStandardBehaviour::allocateWorkSpace(
      BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
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
    this->allocateCurrentState(wk.cs);
  }  // end of allocateWorkSpace

  StiffnessMatrixType CastemStandardBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
  }  // end of getDefaultStiffnessMatrixType

  std::vector<std::string>
  CastemStandardBehaviour::getOptionalMaterialProperties() const {
    auto omps = std::vector<std::string>{};
    const auto h = this->getHypothesis();
    auto append_if = [this, &omps](const char* const n) {
      const auto& emps = this->elastic_material_properties_epts;
      if ((emps.empty()) || ((this->stype == 1) && (emps.size() != 9u))) {
        return;
      }
      omps.push_back(n);
    };
    // mass density
    omps.push_back("MassDensity");
    if (this->stype == 0) {
      append_if("YoungModulus");
      append_if("PoissonRatio");
      omps.push_back("ThermalExpansion");
    } else if (this->stype == 1) {
      append_if("YoungModulus1");
      append_if("YoungModulus2");
      append_if("YoungModulus3");
      append_if("PoissonRatio12");
      append_if("PoissonRatio23");
      append_if("PoissonRatio13");
      if ((h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        append_if("ShearModulus12");
        omps.push_back("V1X");
        omps.push_back("V1Y");
      }
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        append_if("ShearModulus23");
        append_if("ShearModulus13");
        omps.push_back("V1Z");
        omps.push_back("V2X");
        omps.push_back("V2Y");
        omps.push_back("V2Z");
      }
      omps.push_back("ThermalExpansion1");
      omps.push_back("ThermalExpansion2");
      omps.push_back("ThermalExpansion3");
    } else {
      tfel::raise(
          "CastemFiniteStrainBehaviour::"
          "getOptionalMaterialProperties: "
          "unsupported symmetry type");
    }
    if (h == ModellingHypothesis::PLANESTRESS) {
      omps.push_back("PlateWidth");
    }
    if (this->getCastemInterfaceVersion() ==
        CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
      omps.push_back("ReferenceTemperatureForThermalExpansionCoefficient");
      omps.push_back("ReferenceTemperatureForThermalExpansion");
    }
    return omps;
  }  // end of getOptionalMaterialProperties

  void CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    const auto h = this->getHypothesis();
    // elastic material properties
    auto set_emp = [this, &mp, &evm](
                       const char* const n,
                       const std::vector<std::string>::size_type pos) {
      const auto& emps = this->elastic_material_properties_epts;
      if ((emps.empty()) || ((this->stype == 1) && (emps.size() != 9u))) {
        return;
      }
      if (evm.find(n) != evm.end()) {
        mfront::getLogStream()
            << "CastemStandardBehaviour::"
            << "setOptionalMaterialPropertiesDefaultValues: "
            << " elastic material property '" << n
            << "' is already defined. This definition can be inconstent with "
            << "the material property used internally by the behaviour. "
            << "Remove its definition to use the property exported by the "
            << "behaviour and remove this warning.\n";
        return;
      }
      const auto cmp =
          std::make_shared<CastemEvolution>(this->library, emps[pos], evm);
      if (!mp.insert({n, cmp}).second) {
        tfel::raise(
            "CastemStandardBehaviour::"
            "setOptionalMaterialPropertiesDefaultValues: "
            "default value for material property '" +
            std::string(n) + "' already declared");
      }
    };
    if (this->stype == 0) {
      set_emp("YoungModulus", 0);
      set_emp("PoissonRatio", 1);
    } else if (this->stype == 1) {
      set_emp("YoungModulus1", 0);
      set_emp("YoungModulus2", 1);
      set_emp("YoungModulus3", 2);
      set_emp("PoissonRatio12", 3);
      set_emp("PoissonRatio23", 4);
      set_emp("PoissonRatio13", 5);
      if ((h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        set_emp("ShearModulus12", 6);
      }
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        set_emp("ShearModulus23", 7);
        set_emp("ShearModulus13", 8);
      }
    } else {
      tfel::raise(
          "CastemFiniteStrainBehaviour::"
          "setOptionalMaterialPropertiesDefaultValues : "
          "unsupported symmetry type");
    }
    // mass density
    Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "MassDensity",
                                                       0.);
    // thermal expansion
    if (this->stype == 0) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion", 0.);
    } else if (this->stype == 1) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion1", 0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion2", 0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion3", 0.);
    } else {
      tfel::raise(
          "CastemFiniteStrainBehaviour::"
          "setOptionalMaterialPropertiesDefaultValues : "
          "unsupported symmetry type");
    }
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
          tfel::raise_if(
              ((bv1x) && (!b)) || ((bv1y) && (!b)),
              "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
              "if one component of the orthotropic basis is defined, all "
              "the components must be defined.");
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
          tfel::raise_if(
              ((bv1x) && (!b)) || ((bv1y) && (!b)) || ((bv1z) && (!b)) ||
                  ((bv2x) && (!b)) || ((bv2y) && (!b)) || ((bv2z) && (!b)),
              "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
              "if one component of the orthotropic basis is defined, all "
              "the components must be defined.");
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
          tfel::raise(
              "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
              "unsupported hypothesis");
        }
      }
    }
    if (h == ModellingHypothesis::PLANESTRESS) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp, evm, "PlateWidth",
                                                         1.);
    }
    if (this->getCastemInterfaceVersion() ==
        CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ReferenceTemperatureForThermalExpansionCoefficient", 0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ReferenceTemperatureForThermalExpansion", 0.);
    }
  }  // end of setOptionalMaterialPropertiesDefaultValues

  void CastemStandardBehaviour::buildMaterialProperties(
      BehaviourWorkSpace& wk, const CurrentState& s) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "CastemSmallStrainBehaviour::buildMaterialProperties: " + m);
    };
    if (this->usesGenericPlaneStressAlgorithm) {
      const auto v = this->getCastemInterfaceVersion();
      if (this->stype == 0u) {
        throw_if(
            wk.mps.size() != s.mprops1.size() + 1,
            "temporary material properties vector was not allocated properly");
        if (v == CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
          throw_if(s.mprops1.size() < 6,
                   "invalid number of material properties");
        } else {
          throw_if(s.mprops1.size() < 4,
                   "invalid number of material properties");
        }
        wk.mps[0] = s.mprops1[0];
        wk.mps[1] = s.mprops1[1];
        wk.mps[2] = s.mprops1[2];
        wk.mps[3] = s.mprops1[3];
        // plate width
        wk.mps[4] = castem::CastemReal(1);
        if (v == CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
          wk.mps[5] = s.mprops1[4];
          wk.mps[6] = s.mprops1[5];
          std::copy(s.mprops1.begin() + 6u, s.mprops1.end(),
                    wk.mps.begin() + 7u);
        } else {
          std::copy(s.mprops1.begin() + 4u, s.mprops1.end(),
                    wk.mps.begin() + 5u);
        }
      } else if (this->stype == 1u) {
        throw_if(
            wk.mps.size() != s.mprops1.size(),
            "temporary material properties vector was not allocated properly");
        if (v == CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
          throw_if(s.mprops1.size() < 15,
                   "invalid number of material properties");
        } else {
          throw_if(s.mprops1.size() < 13,
                   "invalid number of material properties");
        }
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
        if (v == CastemInterfaceVersion::CASTEM_INTERFACE_VERSION_2021) {
          wk.mps[13] = s.mprops1[13];
          wk.mps[14] = s.mprops1[14];
          std::copy(s.mprops1.begin() + 15u, s.mprops1.end(),
                    wk.mps.begin() + 15u);
        } else {
          std::copy(s.mprops1.begin() + 13u, s.mprops1.end(),
                    wk.mps.begin() + 13u);
        }
      } else {
        throw_if(true, "unsupported symmetry type");
      }
    } else {
      throw_if(
          wk.mps.size() != s.mprops1.size(),
          "temporary material properties vector was not allocated properly");
      std::copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
    }
  }  // end of buildMaterialProperties

  CastemInterfaceVersion CastemStandardBehaviour::getCastemInterfaceVersion()
      const {
    return CastemInterfaceVersion::LEGACY_CASTEM_INTERFACE_VERSION;
  }  // end of getCastemInterfaceVersion

  CastemStandardBehaviour::~CastemStandardBehaviour() = default;

}  // namespace mtest
