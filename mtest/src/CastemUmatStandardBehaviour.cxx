/*!
 * \file   CastemUmatStandardBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 sept. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/StandardBehaviourBase.hxx"
#include "MTest/CastemUmatStandardBehaviour.hxx"
#include "MTest/CastemUmatSmallStrainBehaviour.hxx"
#include "MTest/CastemUmatFiniteStrainBehaviour.hxx"

namespace mtest {

  std::shared_ptr<Behaviour>
  CastemUmatStandardBehaviour::buildCastemUmatStandardBehaviour(
      const std::string& l,
      const std::string& f,
      const tfel::utilities::Data& p,
      const int t,
      const Hypothesis h) {
    auto throw_if = [](const bool b, const std::string& msg) {
      tfel::raise_if(
          b, "CastemUmatStandardBehaviour::buildCastemUmatStandardBehaviour: " +
                 msg);
    };
    StandardBehaviourDescription md;
    md.library = l;
    md.behaviour = f;
    md.hypothesis = ModellingHypothesis::toString(h);
    throw_if((t != 1u) && (t != 2u), "invalid behaviour");
    md.btype = t;
    if (t == 1u) {
      md.kinematic = 1u;
    } else {
      md.kinematic = 3u;
    }
    throw_if(!p.is<std::map<std::string, tfel::utilities::Data>>(),
             "invalid parameters type");
    const auto& mp = p.get<std::map<std::string, tfel::utilities::Data>>();
    //! name or number of the behaviour
    std::string mname(16, ' ');
    throw_if((mp.count("FCT_LOI") == 1u) && (mp.count("NUME_LOI") == 1u),
             "either 'FCT_LOI' or 'NUME_LOI' must be defined");
    throw_if((mp.count("FCT_LOI") != 1u) && (mp.count("NUME_LOI") != 1u),
             "'FCT_LOI' or 'NUME_LOI' must not be both defined");
    if (mp.count("FCT_LOI") == 1u) {
      const auto& d = mp.at("FCT_LOI");
      throw_if(!d.is<std::string>(), "parameter 'FCT_LOI' must be a string");
      const auto& fct = d.get<std::string>();
      throw_if(
          fct.size() > 16,
          "invalid parameter 'FCT_LOI': must be smaller than 16 characters");
      std::copy(fct.begin(), fct.end(), mname.begin());
    } else {
      const auto& d = mp.at("NUME_LOI");
      throw_if(!d.is<int>(), "parameter 'NUME_LOI' must be an integer");
      const auto& n = d.get<int>();
      throw_if(
          (n < 0) || (n > 9999),
          "invalid value for parameter 'NUME_LOI' (" + std::to_string(n) + ")");
      const auto ns = std::to_string(n);
      throw_if(ns.size() > 4, "invalid convertion of parameter 'NUME_LOI'");
      std::copy(ns.rbegin(), ns.rend(), mname.rbegin());
    }
    //! behaviour symmetry type
    throw_if(
        mp.count("SYMETRIE") != 1u,
        "the material symmetry is not defined, "
        "you shall use the 'SYMETRIE' (in french) parameter. The 'SYMETRIE' "
        "parameter must be string whose value is either 'ISOTROPE' or "
        "'ORTHOTROPE'");
    const auto& s = mp.at("SYMETRIE");
    throw_if(!s.is<std::string>(), "the 'SYMETRIE' parameter must be a string");
    const auto& ss = s.get<std::string>();
    if (ss == "ISOTROPE") {
      md.stype = 0u;
    } else if (ss == "ORTHOTROPE") {
      md.stype = 1u;
    } else {
      throw_if(true, "invalid value for the 'SYMETRIE' parameter");
    }
    // mandatory material properties names
    auto mmpnames = std::vector<std::string>{};
    // optional material properties names
    auto ompnames = std::vector<std::string>{};
    if (md.stype == 0u) {
      mmpnames.insert(mmpnames.begin(), {"YOUN", "NU"});
      ompnames.insert(ompnames.begin(), {"RHO", "ALPA"});
    } else if (md.stype == 1u) {
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        mmpnames.insert(mmpnames.begin(),
                        {"YG1", "YG2", "YG3", "NU12", "NU23", "NU13"});
        ompnames.insert(ompnames.begin(), {"RHO", "ALP1", "ALP2", "ALP3"});
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        mmpnames.insert(mmpnames.begin(), {"YG1", "YG2", "NU12", "G12", "V1X",
                                           "V1Y", "YG3", "NU23", "NU13"});
        ompnames.insert(ompnames.begin(), {"RHO", "ALP1", "ALP2", "DIM3"});
      } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        mmpnames.insert(mmpnames.begin(), {"YG1", "YG2", "YG3", "NU12", "NU23",
                                           "NU13", "G12", "V1X", "V1Y"});
        ompnames.insert(ompnames.begin(), {"RHO", "ALP1", "ALP2", "ALP3"});
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        mmpnames.insert(mmpnames.begin(), {"YG1", "YG2", "YG3", "NU12", "NU23",
                                           "NU13", "G12", "G23", "G13", "V1X",
                                           "V1Y", "V1Z", "V2X", "V2Y", "V2Z"});
        ompnames.insert(ompnames.begin(), {"RHO", "ALP1", "ALP2", "ALP3"});
      }
    }
    auto get = [&mp](const std::string& n) -> std::vector<std::string> {
      auto pd = mp.find(n);
      if (pd == mp.end()) {
        return {};
      }
      return tfel::utilities::convert<std::vector<std::string>>(pd->second);
    };
    const auto mpnames = get("C_MATERIAU");
    for (const auto& n : mpnames) {
      throw_if(n.size() > 4u, "invalid material property name '" + n + "'");
    }
    throw_if(mpnames.size() < mmpnames.size(),
             "invalid number of material properties");
    for (decltype(mpnames.size()) i = 0; i != mmpnames.size(); ++i) {
      throw_if(mpnames[i] != mmpnames[i], "invalid " + std::to_string(i) +
                                              "th material property: "
                                              "expected '" +
                                              mmpnames[i] + "', but '" +
                                              mpnames[i] + "' declared");
    }
    auto pmp = mpnames.begin() + mpnames.size();
    for (const auto& omp : ompnames) {
      if (pmp == mpnames.end()) {
        break;
      }
      throw_if(std::find(pmp, mpnames.end(), omp) != pmp,
               "invalid position for material property '" + omp + "'");
    }
    md.mpnames = mpnames;
    md.ivnames = get("C_VARINTER");
    md.ivtypes.resize(md.ivnames.size(), 0);
    auto evnames = get("PARA_LOI");
    if (!evnames.empty()) {
      if (evnames.front() == "T") {
        evnames.pop_back();
      }
    }
    evnames.insert(evnames.begin(), "Temperature");
    md.evnames = evnames;
    std::shared_ptr<Behaviour> ptr;
    if (t == 1u) {
      ptr = std::make_shared<CastemUmatSmallStrainBehaviour>(md, mname);
    } else {
      ptr = std::make_shared<CastemUmatFiniteStrainBehaviour>(md, mname);
    }
    return ptr;
  }

  void CastemUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp,
      const EvolutionManager& evm,
      const int stype,
      const Hypothesis h) {
    auto setOptionalMaterialPropertyDefaultValue =
        [&mp, &evm](const std::string& n, const real v) {
          if (evm.find(n) == evm.end()) {
            if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
              auto& log = mfront::getLogStream();
              log << "setOptionalMaterialPropertiesDefaultValues : "
                  << "set material property '" << n << "' to default value\n";
            }
            tfel::raise_if(!mp.insert({n, make_evolution(v)}).second,
                           "setOptionalMaterialPropertiesDefaultValues: "
                           "default value for material property '" +
                               n + "' already declared");
          }
        };
    auto throw_if = [](const bool b, const std::string& msg) {
      tfel::raise_if(b,
                     "CastemUmatStandardBehaviour::"
                     "setOptionalMaterialPropertiesDefaultValues: " +
                         msg);
    };
    setOptionalMaterialPropertyDefaultValue("RHO", 0.);
    if (stype == 1) {
      if (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        // orthotropic behaviour
        const bool bv1x = evm.find("V1X") != evm.end();
        const bool bv1y = evm.find("V1Y") != evm.end();
        if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::PLANESTRESS) ||
            (h == ModellingHypothesis::PLANESTRAIN)) {
          const bool b = bv1x && bv1y;
          throw_if(((bv1x) && (!b)) || ((bv1y) && (!b)),
                   "if on component of the orthotropic basis is defined, all "
                   "the components must be defined.");
          setOptionalMaterialPropertyDefaultValue("V1X", 1.);
          setOptionalMaterialPropertyDefaultValue("V1Y", 0.);
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          const bool bv1z = evm.find("V1Z") != evm.end();
          const bool bv2x = evm.find("V2X") != evm.end();
          const bool bv2y = evm.find("V2Y") != evm.end();
          const bool bv2z = evm.find("V2Z") != evm.end();
          const bool b = bv1x && bv1y && bv1z && bv2x && bv2y && bv2z;
          throw_if(((bv1x) && (!b)) || ((bv1y) && (!b)) || ((bv1z) && (!b)) ||
                       ((bv2x) && (!b)) || ((bv2y) && (!b)) || ((bv2z) && (!b)),
                   "if on component of the orthotropic basis is defined, all "
                   "the components must be defined.");
          setOptionalMaterialPropertyDefaultValue("V1X", 1.);
          setOptionalMaterialPropertyDefaultValue("V1Y", 0.);
          setOptionalMaterialPropertyDefaultValue("V1Z", 0.);
          setOptionalMaterialPropertyDefaultValue("V2X", 0.);
          setOptionalMaterialPropertyDefaultValue("V2Y", 1.);
          setOptionalMaterialPropertyDefaultValue("V2Z", 0.);
        } else {
          throw_if(true, "unsupported hypothesis");
        }
      }
    }
    if (stype == 0) {
      setOptionalMaterialPropertyDefaultValue("ALPH", 0.);
    } else if (stype == 1) {
      setOptionalMaterialPropertyDefaultValue("ALP1", 0.);
      setOptionalMaterialPropertyDefaultValue("ALP2", 0.);
      setOptionalMaterialPropertyDefaultValue("ALP3", 0.);
    } else {
      throw_if(true, "unsupported symmetry type");
    }
    if (h == ModellingHypothesis::PLANESTRESS) {
      setOptionalMaterialPropertyDefaultValue("DIM3", 0.);
    }
  }  // end of
     // CastemUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues

}  // end of namespace mtest
