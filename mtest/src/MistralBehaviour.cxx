/*!
 * \file   mfront/mtest/MistralBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/FunctionEvolution.hxx"
#include "MTest/MistralBehaviour.hxx"

namespace mtest {

  struct MistralBehaviourDescription : public UmatBehaviourDescription {
    //! Young modulus
    std::pair<double, double> young_modulus;
    //! Poisson ratio
    std::pair<double, double> poisson_ratio;
    //! values of the material properties
    std::vector<double> cvalues;
    std::string mname;
    int SENSIP1;
    int SENSIP2;
    int ICBASE;
  };  // end of MistralBehaviourDescription

  static MistralBehaviourDescription readMistralFile(
      const std::string& l,
      const std::string& f,
      const MistralBehaviour::Parameters& parameters,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    auto throw_if = [](const bool b, const std::string& msg) {
      if (b) {
        throw(std::runtime_error("readMistralParameters: " + msg));
      };
    };
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    MistralBehaviourDescription md;
    md.library = l;
    md.behaviour = f;
    md.btype = 1u;
    md.kinematic = 1u;
    throw_if(
        !parameters.is<std::map<std::string, MistralBehaviour::Parameters>>(),
        "invalid parameters type");
    const auto mp =
        parameters.get<std::map<std::string, MistralBehaviour::Parameters>>();
    auto get_int_parameter = [&mp, &throw_if](const std::string& n) {
      auto p = mp.find(n);
      if (p == mp.end()) {
        return 0;
      }
      const auto& d = p->second;
      throw_if(!d.is<int>(), "invalid type for parameter '" + n + "'");
      const auto v = d.get<int>();
      return v;
    };
    throw_if(mp.find("NUME_LOI") == mp.end(), "NUME_LOI not specified");
    const auto& d = mp.at("NUME_LOI");
    throw_if(!d.is<int>(), "parameter 'NUME_LOI' must be an integer");
    const auto& n = d.get<int>();
    throw_if((n < 0) || (n > 9999), "invalid value for parameter 'NUME_LOI' (" +
                                        std::to_string(n) + ")");
    const auto ns = std::to_string(n);
    throw_if(ns.size() > 4, "invalid convertion of parameter 'NUME_LOI'");
    md.mname = std::string(16, ' ');
    std::copy(ns.rbegin(), ns.rend(), md.mname.rbegin());
    // mistral parameters file
    const auto& m = [&mp, &throw_if]() {
      auto p = mp.find("mistral_parameters_file");
      throw_if(p == mp.end(),
               "no mistral parameters file defined. "
               "You shall defined a parameter named "
               "'mistral_parameters_file'");
      const auto& mf = p->second;
      throw_if(!mf.is<std::string>(),
               "invalid type for parameter 'mistral_parameters_file'");
      return mf.get<std::string>();
    }();
    // orthotropy management
    md.SENSIP1 = get_int_parameter("SENSIP1");
    md.SENSIP2 = get_int_parameter("SENSIP2");
    md.ICBASE = get_int_parameter("ICBASE");
    // number of user defined state variable
    const int NVIU = [&get_int_parameter, &throw_if]() {
      const auto v = get_int_parameter("NVIU");
      throw_if(v < 0, "invalid value for parameter 'NVIU'");
      return v;
    }();
    // symmetry type
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      md.stype = 0u;
    } else {
      md.stype = 1u;
    }
    CxxTokenizer tokens(m);
    auto p = tokens.begin();
    const auto pe = tokens.end();
    auto get_int = [&pe](CxxTokenizer::const_iterator& c) -> int {
      const auto v = CxxTokenizer::readInt(c, pe);
      return v;
    };
    auto get_double = [&pe](CxxTokenizer::const_iterator& c) -> double {
      const auto v = CxxTokenizer::readDouble(c, pe);
      return v;
    };
    auto get_array = [&pe, &get_int,
                      &get_double](CxxTokenizer::const_iterator& c) {
      const auto size = get_int(c);
      auto v = std::vector<double>{};
      v.reserve(size);
      for (int i = 0; i != size; ++i) {
        v.push_back(get_double(c));
      }
      return v;
    };
    const auto PARA = get_array(p);
    md.cvalues.push_back(PARA.size());
    md.cvalues.insert(md.cvalues.end(), PARA.begin(), PARA.end());
    const auto PCOEL = get_array(p);
    const auto NDPI = get_int(p);
    md.cvalues.push_back(NDPI);
    const auto NDVP = get_int(p);
    md.cvalues.push_back(NDVP);
    const auto NXX = get_int(p);
    md.cvalues.push_back(NXX);
    const auto NPSI = get_int(p);
    md.cvalues.push_back(NPSI);
    const auto NDP = NDPI + NDVP;
    if (NDP > 0) {
      const auto PCOHI = get_array(p);
      md.cvalues.push_back(PCOHI.size());
      md.cvalues.insert(md.cvalues.end(), PCOHI.begin(), PCOHI.end());
      if (NDVP > 0) {
        const auto PECOU = get_array(p);
        md.cvalues.push_back(PECOU.size());
        md.cvalues.insert(md.cvalues.end(), PECOU.begin(), PECOU.end());
      }
      const auto PEDIR = get_array(p);
      md.cvalues.push_back(PEDIR.size());
      md.cvalues.insert(md.cvalues.end(), PEDIR.begin(), PEDIR.end());
      const auto PRVCE = get_array(p);
      md.cvalues.push_back(PRVCE.size());
      md.cvalues.insert(md.cvalues.end(), PRVCE.begin(), PRVCE.end());
      if (NXX > 0) {
        const auto PECRX = get_array(p);
        md.cvalues.push_back(PECRX.size());
        md.cvalues.insert(md.cvalues.end(), PECRX.begin(), PECRX.end());
      }
      const auto PDVDI = get_array(p);
      md.cvalues.push_back(PDVDI.size());
      md.cvalues.insert(md.cvalues.end(), PDVDI.begin(), PDVDI.end());
    }
    const auto PCROI = get_array(p);
    md.cvalues.push_back(PCROI.size());
    md.cvalues.insert(md.cvalues.end(), PCROI.begin(), PCROI.end());
    const auto PINCR = get_array(p);
    md.cvalues.push_back(PINCR.size());
    md.cvalues.insert(md.cvalues.end(), PINCR.begin(), PINCR.end());
    throw_if(PCOEL.empty(), "no material property defined");
    const bool ISOEL = std::abs(PCOEL.at(0) - 1.) < 1.e-14;
    if (ISOEL) {
      md.young_modulus = {PCOEL.at(1), PCOEL.at(2)};
      md.poisson_ratio = {PCOEL.at(3), PCOEL.at(4)};
      throw_if(PCOEL.size() != 5u, "invalid number of material properties");
    } else {
      throw_if(true,
               "orthotropic elastic material properties are not supported yet");
    }
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      throw_if(!ISOEL,
               "orthotropic elastic material properties are not supported yet "
               "in 1D");
      md.mpnames.insert(md.mpnames.begin(),
                        {"YoungModulus", "PoissonRatio", "ThermalExpansion"});
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      md.mpnames.insert(
          md.mpnames.begin(),
          {"YoungModulus1", "YoungModulus2", "PoissonRatio12", "ShearModulus12",
           "V1X", "V1Y", "YoungModulus3", "PoissonRatio23", "PoissonRatio13",
           "ThermalExpansion1", "ThermalExpansion2"});
    } else if ((h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      md.mpnames.insert(
          md.mpnames.begin(),
          {"YoungModulus1", "YoungModulus2", "YoungModulus3", "PoissonRatio12",
           "PoissonRatio23", "PoissonRatio13", "ShearModulus12", "V1X", "V1Y",
           "ThermalExpansion1", "ThermalExpansion2", "ThermalExpansion3"});
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      md.mpnames.insert(
          md.mpnames.begin(),
          {"YoungModulus1", "YoungModulus2", "YoungModulus3", "PoissonRatio12",
           "PoissonRatio23", "PoissonRatio13", "ShearModulus12",
           "ShearModulus23", "ShearModulus13", "V1X", "V1Y", "V1Z", "V2X",
           "V2Y", "V2Z", "ThermalExpansion1", "ThermalExpansion2",
           "ThermalExpansion3"});
    }
    md.mpnames.insert(md.mpnames.end(), {"SENSIP1", "SENSIP2", "ICBASE"});
    for (decltype(md.cvalues.size()) i = 0; i != md.cvalues.size(); ++i) {
      const auto idx = i + 1;
      const auto mpn = ((idx < 10) ? "P00" : ((idx < 100) ? "P0" : "P")) +
                       std::to_string(idx);
      md.mpnames.push_back(std::move(mpn));
    }
    // internal state variables
    md.ivnames.insert(md.ivnames.end(),
                      {"ETH1", "ETH2", "ETH3", "ETH4", "ETH5", "ETH6"});
    md.ivnames.insert(md.ivnames.end(),
                      {"EEL1", "EEL2", "EEL3", "EEL4", "EEL5", "EEL6"});
    if (NDPI > 0) {
      md.ivnames.insert(md.ivnames.end(),
                        {"EP01", "EP02", "EP03", "EP04", "EP05", "EP06"});
    }
    if (NDVP > 0) {
      md.ivnames.insert(md.ivnames.end(),
                        {"EP11", "EP12", "EP13", "EP14", "EP15", "EP16"});
    }
    if (NDVP > 1) {
      md.ivnames.insert(md.ivnames.end(),
                        {"EP21", "EP22", "EP23", "EP24", "EP25", "EP26"});
    }
    if (NDVP > 2) {
      md.ivnames.insert(md.ivnames.end(),
                        {"EP31", "EP32", "EP33", "EP34", "EP35", "EP36"});
    }
    if (NDVP > 3) {
      md.ivnames.insert(md.ivnames.end(),
                        {"EP41", "EP42", "EP43", "EP44", "EP45", "EP46"});
    }
    throw_if(NDVP > 4, "unsupported case (NDVP > 4)");
    md.ivnames.insert(md.ivnames.end(),
                      {"ECR1", "ECR2", "ECR3", "ECR4", "ECR5", "ECR6"});
    if (NDPI > 0) {
      md.ivnames.insert(md.ivnames.end(), {"EP0E", "EP0Q"});
    }
    if (NDVP > 0) {
      md.ivnames.insert(md.ivnames.end(), {"EP1E", "EP1Q"});
    }
    if (NDVP > 1) {
      md.ivnames.insert(md.ivnames.end(), {"EP2E", "EP2Q"});
    }
    if (NDVP > 2) {
      md.ivnames.insert(md.ivnames.end(), {"EP3E", "EP3Q"});
    }
    if (NDVP > 3) {
      md.ivnames.insert(md.ivnames.end(), {"EP4E", "EP4Q"});
    }
    throw_if(NDVP > 4, "unsupported case (NDVP > 4)");
    if (NXX > 0) {
      md.ivnames.insert(md.ivnames.end(),
                        {"X11 ", "X12 ", "X13 ", "X14 ", "X15 ", "X16 "});
    }
    if (NXX > 1) {
      md.ivnames.insert(md.ivnames.end(),
                        {"X21 ", "X22 ", "X23 ", "X24 ", "X25 ", "X26 "});
    }
    if (NXX > 2) {
      md.ivnames.insert(md.ivnames.end(),
                        {"X31 ", "X32 ", "X33 ", "X34 ", "X35 ", "X36 "});
    }
    throw_if(NXX > 3, "unsupported case (NXX > 3)");
    md.ivnames.emplace_back("FIT");
    if (NDP > 0) {
      md.ivnames.emplace_back("PSI");
    }
    md.ivnames.emplace_back("DTA");
    if (NDPI > 0) {
      md.ivnames.insert(md.ivnames.end(), {"SEA0", "RRA0", "VEA0", "KPLA"});
    }
    if (NDP > 0) {
      md.ivnames.emplace_back("EPSE");
    }
    md.ivnames.insert(md.ivnames.end(),
                      {"VET1", "VET2", "VET3", "VET4", "VET5", "VET6"});
    if (NDP > 0) {
      md.ivnames.insert(md.ivnames.end(),
                        {"VEI1", "VEI2", "VEI3", "VEI4", "VEI5", "VEI6"});
    }
    if (NDPI > 0) {
      md.ivnames.emplace_back("VP0E");
    }
    if (NDVP > 0) {
      md.ivnames.emplace_back("VP1E");
    }
    if (NDVP > 1) {
      md.ivnames.emplace_back("VP2E");
    }
    if (NDVP > 2) {
      md.ivnames.emplace_back("VP3E");
    }
    if (NDVP > 3) {
      md.ivnames.emplace_back("VP4E");
    }
    throw_if(NDVP > 4, "unsupported case (NDVP > 4)");
    if (NVIU > 0) {
      throw_if(NVIU > 99, "unsupported case (NVIU > 99)");
      for (int i = 0; i != NVIU; ++i) {
        if (i < 10) {
          md.ivnames.push_back("VIU" + std::to_string(i));
        } else {
          md.ivnames.push_back("VI" + std::to_string(i));
        }
      }
    }
    md.ivtypes.resize(md.ivnames.size(), 0);
    md.evnames.emplace_back("Temperature");
    md.evnames.emplace_back("FI");
    return md;
  }

  std::shared_ptr<Behaviour> MistralBehaviour::buildMistralBehaviour(
      const std::string& l,
      const std::string& f,
      const MistralBehaviour::Parameters& p,
      const Hypothesis h) {
    const auto md = readMistralFile(l, f, p, h);
    return std::shared_ptr<MistralBehaviour>(new MistralBehaviour(
        md, md.mname, md.cvalues, md.young_modulus, md.poisson_ratio,
        md.SENSIP1, md.SENSIP2, md.ICBASE, h));
  }  // end of MistralBehaviour::buildMistralBehaviour

  MistralBehaviour::MistralBehaviour(const UmatBehaviourDescription& bd,
                                     const std::string& cn,
                                     const std::vector<double>& cvalues,
                                     const std::pair<double, double>& yg,
                                     const std::pair<double, double>& nu,
                                     const int SENSIP1_,
                                     const int SENSIP2_,
                                     const int ICBASE_,
                                     const Hypothesis h)
      : CastemSmallStrainBehaviour(bd, h),
        parameters(cvalues),
        young_modulus(yg),
        poisson_ratio(nu),
        SENSIP1(SENSIP1_),
        SENSIP2(SENSIP2_),
        ICBASE(ICBASE_) {
    if (cn.size() != 16u) {
      throw(
          std::runtime_error("MistralBehaviour::MistralBehaviour: "
                             "invalid size for the material name"));
    }
    std::copy(cn.begin(), cn.end(), this->mname);
  }  // end of MistralBehaviour::MistralBehaviour

  void MistralBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(
            std::runtime_error("MTestStandardUmatBehaviour::"
                               "setOptionalMaterialPropertiesDefaultValues: " +
                               m));
      };
    };
    CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,
                                                                           evm);
    auto add_mp = [&mp, &evm, &throw_if](const std::string& n,
                                         const std::shared_ptr<Evolution> m) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
        auto& log = mfront::getLogStream();
        log << "MistralBehaviour::setOptionalMaterialPropertiesDefaultValues: "
            << "set material property '" << n << "' to default value\n";
      }
      throw_if(!mp.insert({n, m}).second,
               "default value for material property "
               "'" +
                   n + "' already declared");
    };
    auto add_mp2 = [&evm, &add_mp](const std::string& n,
                                   const std::shared_ptr<Evolution> m) {
      if (evm.find(n) == evm.end()) {
        add_mp(n, m);
      }
    };
    // elastic material properties
    const auto yg_f = std::to_string(young_modulus.first) + "+Temperature*" +
                      std::to_string(young_modulus.second);
    const auto nu_f = std::to_string(poisson_ratio.first) + "+Temperature*" +
                      std::to_string(poisson_ratio.second);
    const auto yg =
        std::shared_ptr<Evolution>(new FunctionEvolution(yg_f, evm));
    const auto nu =
        std::shared_ptr<Evolution>(new FunctionEvolution(nu_f, evm));
    const auto h = ModellingHypothesis::fromString(this->hypothesis);
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      add_mp2("YoungModulus", yg);
      add_mp2("PoissonRatio", nu);
    } else {
      const auto mu_f = "YoungModulus1/(2*(1+PoissonRatio12))";
      const auto mu =
          std::shared_ptr<Evolution>(new FunctionEvolution(mu_f, evm));
      add_mp2("YoungModulus1", yg);
      add_mp2("YoungModulus2", yg);
      add_mp2("YoungModulus3", yg);
      add_mp2("PoissonRatio12", nu);
      add_mp2("PoissonRatio23", nu);
      add_mp2("PoissonRatio13", nu);
      add_mp2("ShearModulus12", mu);
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        add_mp2("ShearModulus23", mu);
        add_mp2("ShearModulus14", mu);
      }
    }
    // mistral parameters
    for (decltype(this->parameters.size()) i = 0; i != this->parameters.size();
         ++i) {
      const auto idx = i + 1;
      const auto n = ((idx < 10) ? "P00" : ((idx < 100) ? "P0" : "P")) +
                     std::to_string(idx);
      add_mp2(n, make_evolution(parameters.at(i)));
    }
    add_mp2("SENSIP1", make_evolution(static_cast<real>(this->SENSIP1)));
    add_mp2("SENSIP2", make_evolution(static_cast<real>(this->SENSIP2)));
    add_mp2("ICBASE", make_evolution(static_cast<real>(this->ICBASE)));
  }  // end of MistralBehaviour::setOptionalMaterialPropertiesDefaultValues

  const char* MistralBehaviour::getBehaviourNameForUMATFunctionCall(
      void) const {
    return this->mname;
  }  // end of MistralBehaviour::getBehaviourNameForUMATFunctionCall

  MistralBehaviour::~MistralBehaviour() = default;

}  // end of namespace mtest
