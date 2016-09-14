/*!
 * \file   mfront/mtest/CastemUmatSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/CastemUmatSmallStrainBehaviour.hxx"

namespace mtest
{

  static UmatBehaviourDescription
  readCastemUmatSmallStrainFile(const std::string& l,
				const std::string& f,
				const CastemUmatSmallStrainBehaviour::Parameters& parameters,
				const tfel::material::ModellingHypothesis::Hypothesis h){
    auto throw_if = [](const bool b,const std::string& msg){
      if(b){throw(std::runtime_error("readCastemUmatSmallStrainParameters: "+msg));};
    };
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    UmatBehaviourDescription md;
    md.library = l;
    md.behaviour = f;
    md.type=1u;
    // mandatory material properties names
    auto mmpnames = std::vector<std::string>{}
    // optional material properties names
    auto ompnames = std::vector<std::string>{}
    if(md.stype==1u){
      mmpnames.insert(mmpnames.begin(),{"YOUNG","NU"});
      ompnames.insert(ompnames.begin(),{"RHO","ALPA"});
    } else if (md.stype==2u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	mmpnames.insert(mmpnames.begin(),{
	  "YG1","YG2","YG3","NU12","NU23","NU13"});
	ompnames.insert(ompnames.begin(),{"RHO","ALP1",
	      "ALP2","ALP3"});
      } else if(h==ModellingHypothesis::PLANESTRESS){
	mmpnames.insert(mmpnames.begin(),{
	    "YG1","YG2","NU12","G12",
	      "V1X","V1Y","YG3","NU23","NU13"});
	ompnames.insert(ompnames.begin(),{"RHO","ALP1","ALP2","DIM3"});
      } else if((h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	mmpnames.insert(mmpnames.begin(),{
	    "YG1","YG2","YG3","NU12","NU23","NU13",
	      "G12","V1X","V1Y"});
	ompnames.insert(ompnames.begin(),{"RHO","ALP1","ALP2","ALP3"});
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	mmpnames.insert(mmpnames.begin(),{
	    "YG1","YG2","YG3","NU12","NU23","NU13",
	      "G12","G23","G13",
	      "V1X","V1Y","V1Z","V2X","V2Y","V2Z"});
	ompnames.insert(ompnames.begin(),{"RHO","ALP1","ALP2","ALP3"});
      }
    }
    throw_if(!parameters.is<std::map<std::string,Data>>(),
	     "invalid parameters type");
    const auto& mp = parameters.get<std::map<std::string,Data>>();
    auto get = [&mp,&throw_if](const std::string& n){
      auto p = mp.find(n);
      
    };
    md.mpnames = get("C_MATERIAU");
    md.ivnames = get("C_VARINTER");
    md.ivtypes.resize(md.ivnames.size(),0);
    md.evnames = get("PARA_LOI");
    return md;
  }

  std::shared_ptr<Behaviour>
  CastemUmatSmallStrainBehaviour::buildCastemUmatSmallStrainBehaviour(const std::string& l,
					  const std::string& f,
					  const CastemUmatSmallStrainBehaviour::Parameters& p,
					  const Hypothesis h)
  {
    const auto md = readCastemUmatSmallStrainFile(l,f,p,h);
    std::shared_ptr<CastemUmatSmallStrainBehaviour>
      ptr(new CastemUmatSmallStrainBehaviour(md,md.cvalues,
			       md.young_modulus,
			       md.poisson_ratio,
			       md.SENSIP1,md.SENSIP2,md.ICBASE,h));
    return ptr;    
  } // end of CastemUmatSmallStrainBehaviour::buildCastemUmatSmallStrainBehaviour
    
  CastemUmatSmallStrainBehaviour::CastemUmatSmallStrainBehaviour(const UmatBehaviourDescription& bd,
				     const std::vector<double>& cvalues,
				     const std::pair<double,double>& yg,
				     const std::pair<double,double>& nu,
				     const int SENSIP1_,
				     const int SENSIP2_,
				     const int ICBASE_,
				     const Hypothesis h)
    : CastemSmallStrainBehaviour(bd,h),
      parameters(cvalues),
      young_modulus(yg),
      poisson_ratio(nu),
      SENSIP1(SENSIP1_),
      SENSIP2(SENSIP2_),
      ICBASE(ICBASE_)
  {} // end of CastemUmatSmallStrainBehaviour::CastemUmatSmallStrainBehaviour

  void
  CastemUmatSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(EvolutionManager& mp,
							       const EvolutionManager& evm) const{
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("MTestStandardUmatBehaviour::setOptionalMaterialPropertiesDefaultValues: "+m));};
    };
    CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,evm);
    auto add_mp = [&mp,&evm,&throw_if](const std::string& n,
				       const std::shared_ptr<Evolution> m){
      if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL2){
	auto& log = mfront::getLogStream();
	log << "CastemUmatSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues: "
	<< "set material property '" << n << "' to default value\n";
      }
      throw_if(!mp.insert({n,m}).second,
	       "default value for material property "
	       "'"+n+"' already declared");
    };
    auto add_mp2 = [&evm,&add_mp](const std::string& n,
				  const std::shared_ptr<Evolution> m){
      if(evm.find(n)==evm.end()){
	add_mp(n,m);
      }
    };
    // elastic material properties
    const auto yg_f = std::to_string(young_modulus.first)+
      "+Temperature*"+std::to_string(young_modulus.second);
    const auto nu_f = std::to_string(poisson_ratio.first)+
      "+Temperature*"+std::to_string(poisson_ratio.second);
    const auto yg   =
      std::shared_ptr<Evolution>(new FunctionEvolution(yg_f,evm));
    const auto nu   =
      std::shared_ptr<Evolution>(new FunctionEvolution(nu_f,evm));
    const auto h = ModellingHypothesis::fromString(this->hypothesis);
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      add_mp2("YoungModulus",yg);
      add_mp2("PoissonRatio",nu);
    } else {
      const auto mu_f = "YoungModulus1/(2*(1+PoissonRatio12))";
      const auto mu   =
	std::shared_ptr<Evolution>(new FunctionEvolution(mu_f,evm));
      add_mp2("YoungModulus1",yg);
      add_mp2("YoungModulus2",yg);
      add_mp2("YoungModulus3",yg);
      add_mp2("PoissonRatio12",nu);
      add_mp2("PoissonRatio23",nu);
      add_mp2("PoissonRatio13",nu);
      add_mp2("ShearModulus12",mu);
      if (h==ModellingHypothesis::TRIDIMENSIONAL){
	add_mp2("ShearModulus23",mu);
	add_mp2("ShearModulus14",mu);
      }
    }
    // CastemUmatSmallStrain parameters
    for(decltype(this->parameters.size()) i=0;i!=this->parameters.size();++i){
      const auto idx = i+1;
      const auto n = ((idx<10) ? "P00" : ((idx<100) ? "P0" : "P"))+
	std::to_string(idx);
      add_mp2(n,make_evolution(parameters.at(i)));
    }
    add_mp2("SENSIP1",make_evolution(static_cast<real>(this->SENSIP1)));
    add_mp2("SENSIP2",make_evolution(static_cast<real>(this->SENSIP2)));
    add_mp2("ICBASE",make_evolution(static_cast<real>(this->ICBASE)));
  } // end of CastemUmatSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues

  const char*
  CastemUmatSmallStrainBehaviour::getBehaviourNameForUMATFunctionCall(void) const{
    return "              94";
  } // end of CastemUmatSmallStrainBehaviour::getBehaviourNameForUMATFunctionCall
  
  CastemUmatSmallStrainBehaviour::~CastemUmatSmallStrainBehaviour() = default;
  
} // end of namespace mtest

