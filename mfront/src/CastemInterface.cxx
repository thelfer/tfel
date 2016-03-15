/*!
 * \file   mfront/src/CastemInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iterator>
#include<algorithm>

#include<sstream>
#include<fstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/CastemInterface.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront{

  static void
  writeUMATArguments(std::ostream& out,
		     const BehaviourDescription::BehaviourType& t)
  {
    out << "(castem::CastemReal *const STRESS,\n"
	<< " castem::CastemReal *const STATEV,\n"
	<< " castem::CastemReal *const DDSDDE,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n";
    if(t!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << " const castem::CastemReal *const STRAN,\n"
	  << " const castem::CastemReal *const DSTRAN,\n";
    } else {
      out << " const castem::CastemReal *const,\n"
	  << " const castem::CastemReal *const,\n";
    }
    out << " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const DTIME,\n"
	<< " const castem::CastemReal *const TEMP,\n"
	<< " const castem::CastemReal *const DTEMP,\n"
	<< " const castem::CastemReal *const PREDEF,\n"
	<< " const castem::CastemReal *const DPRED,\n"
	<< " const char           *const,\n"
	<< " const castem::CastemInt  *const NDI,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const NTENS,\n"
	<< " const castem::CastemInt  *const NSTATV,\n"
	<< " const castem::CastemReal *const PROPS,\n"
	<< " const castem::CastemInt  *const NPROPS,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const DROT,\n"
	<< "       castem::CastemReal *const PNEWDT,\n"
	<< " const castem::CastemReal *const,\n";
    if(t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << " const castem::CastemReal *const F0,\n"
	  << " const castem::CastemReal *const F1,\n";
    } else {
      out << " const castem::CastemReal *const,\n"
	  << " const castem::CastemReal *const,\n";
    }
    out << " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< "       castem::CastemInt  *const KINC,\n"
	<< "const int)";
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    out << "(castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const char           *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< "       castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemReal *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< " const castem::CastemInt  *const,\n"
	<< "       castem::CastemInt  *const,\n"
	<< "const int)";
  } // end of writeUMATArguments

  static int
  getCastemModellingHypothesisIndex(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    using namespace tfel::material;
    switch(h){
    case ModellingHypothesis::TRIDIMENSIONAL:
      return 2;
    case ModellingHypothesis::AXISYMMETRICAL:
      return 0;
    case ModellingHypothesis::PLANESTRAIN:
      return -1;
    case ModellingHypothesis::PLANESTRESS:
      return -2;
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      return -3;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      return 14;
    default:
      break;
    }
    std::ostringstream msg;
    msg << "mfront::getCastemModellingHypothesisIndex : "
	<< "unsupported hypothesis";
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      msg << " ('" << ModellingHypothesis::toString(h) << "')";
    }
    throw(std::runtime_error(msg.str()));
  } // end of getCastemModellingHypothesisIndex

  std::string
  CastemInterface::getName(void)
  {
    return "umat";
  }

  std::string
  CastemInterface::getLibraryName(const BehaviourDescription& mb) const
  {
    auto lib = std::string{};
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "Umat"+mb.getMaterialName();
      } else {
	lib = "UmatBehaviour";
      }
    } else {
      lib = "Umat"+mb.getLibrary();
    }
    return lib;
  } // end of CastemInterface::getLibraryName

  std::string
  CastemInterface::getInterfaceName(void) const
  {
    return "Castem";
  } // end of CastemInterface::getInterfaceName

  std::string
  CastemInterface::getFunctionName(const std::string& name) const
  {
    return "umat"+makeLowerCase(name);
  } // end of CastemInterface::getLibraryName

  std::string
  CastemInterface::getBehaviourName(const BehaviourDescription& mb) const
  {
    using namespace std;
    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();
    return name;
  } // end of CastemInterface::getBehaviourName

  std::string
  CastemInterface::getUmatFunctionName(const BehaviourDescription& mb) const
  {
    return "umat"+makeLowerCase(this->getBehaviourName(mb));
  } // end of CastemInterface::getUmatFunctionName

  CastemInterface::CastemInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CastemInterface::treatKeyword(const std::string& key,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CastemInterface::treatKeyword : "+m));}
    };
    if ((key=="@CastemGenerateMTestFileOnFailure")||
	(key=="@UMATGenerateMTestFileOnFailure")){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return {true,current};      
    } else if((key=="@CastemUseTimeSubStepping")||
	      (key=="@UMATUseTimeSubStepping")){
      this->useTimeSubStepping = this->readBooleanValue(key,current,end);
      return {true,current};
    } else if ((key=="@CastemMaximumSubStepping")||
	       (key=="@UMATMaximumSubStepping")){
      throw_if(!this->useTimeSubStepping,
	       "time sub stepping is not enabled at this stage.\n"
	       "Use the @CastemUseTimeSubStepping directive before "
	       "@CastemMaximumSubStepping");
      throw_if(current==end,"unexpected end of file");
      std::istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      throw_if(flux.fail(),"failed to read maximum substepping value.");
      throw_if(++current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+'\'');
      ++(current);
      return {true,current};
    } else if ((key=="@CastemDoSubSteppingOnInvalidResults")||
	       (key=="@UMATDoSubSteppingOnInvalidResults")){
      throw_if(!this->useTimeSubStepping,
	       "time sub stepping is not enabled at this stage.\n"
	       "Use the @CastemUseTimeSubStepping directive before "
	       "@CastemMaximumSubStepping");
      this->doSubSteppingOnInvalidResults = this->readBooleanValue(key,current,end);
      return {true,current};
    } else if ((key=="@CastemFiniteStrainStrategy")||
	       (key=="@UMATFiniteStrainStrategy")){
      throw_if(!this->finiteStrainStrategies.empty(),
	       "at least one strategy has already been defined");
      throw_if(current==end,"unexpected end of file");
      const auto& fss = current->value;
      if(fss=="FiniteRotationSmallStrain"){
	this->finiteStrainStrategies.push_back(FINITEROTATIONSMALLSTRAIN);
      } else if(fss=="MieheApelLambrechtLogarithmicStrain"){
	this->finiteStrainStrategies.push_back(MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN);
      } else if(fss=="LogarithmicStrain1D"){
	this->finiteStrainStrategies.push_back(LOGARITHMICSTRAIN1D);
      } else {
	throw_if(true,"unsupported strategy '"+fss+"'\n"
		 "The only supported strategies are "
		 "'FiniteRotationSmallStrain', "
		 "'MieheApelLambrechtLogarithmicStrain' and "
		 "'LogarithmicStrain1D'");
      }
      throw_if(++current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+'\'');
      ++(current);
      return {true,current};
    } else if ((key=="@CastemFiniteStrainStrategies")||
	       (key=="@UMATFiniteStrainStrategies")){
      throw_if(!this->finiteStrainStrategies.empty(),
	       "at least one strategy has already been defined");
      auto fss = std::vector<std::string>{};
      CxxTokenizer::readArray("CastemInterface::treatKeyword (@CastemFiniteStrainStrategies)",
			      fss,current,end);
      CxxTokenizer::readSpecifiedToken("CastemInterface::treatKeyword (@CastemFiniteStrainStrategies)",
				       ";",current,end);
      throw_if(fss.empty(),"no strategy defined");
      for(const auto& fs : fss){
	if(fs=="None"){
	  throw_if(find(this->finiteStrainStrategies.begin(),
			this->finiteStrainStrategies.end(),NONE)!=
		   this->finiteStrainStrategies.end(),
		   "strategy 'None' multiply defined");
	  this->finiteStrainStrategies.push_back(NONE);
	} else if(fs=="FiniteRotationSmallStrain"){
	  throw_if(find(this->finiteStrainStrategies.begin(),
			this->finiteStrainStrategies.end(),FINITEROTATIONSMALLSTRAIN)!=
		   this->finiteStrainStrategies.end(),
		   "strategy 'FiniteRotationSmallStrain' multiply defined");
	  this->finiteStrainStrategies.push_back(FINITEROTATIONSMALLSTRAIN);
	} else if(fs=="MieheApelLambrechtLogarithmicStrain"){
	  throw_if(find(this->finiteStrainStrategies.begin(),
			this->finiteStrainStrategies.end(),MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN)!=
		   this->finiteStrainStrategies.end(),
		   "strategy 'MieheApelLambrechtLogarithmicStrain' multiply defined");
	  this->finiteStrainStrategies.push_back(MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN);
	} else if(fs=="LogarithmicStrain1D"){
	  throw_if(find(this->finiteStrainStrategies.begin(),
			this->finiteStrainStrategies.end(),LOGARITHMICSTRAIN1D)!=
		   this->finiteStrainStrategies.end(),
		   "strategy 'LogarithmicStrain1D' multiply defined");
	  this->finiteStrainStrategies.push_back(LOGARITHMICSTRAIN1D);
	} else {
	  throw_if(true,"unsupported strategy '"+fs+"'\n"
		   "The only supported strategies are "
		   "'None', 'FiniteRotationSmallStrain', "
		   "'MieheApelLambrechtLogarithmicStrain' and "
		   "'LogarithmicStrain1D'");
	}
      }
      return {true,current};      
    }
    return {false,current};
  } // end of treatKeyword

  bool
  CastemInterface::usesGenericPlaneStressAlgorithm(const BehaviourDescription& mb)
  {
    if(mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)){
      return false;
    }
    return mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN);
  }

  std::string
  CastemInterface::treatScalar(const std::string& s)
  {
    return "'"+makeUpperCase(s.substr(0,4))+"'";
  }

  std::string
  CastemInterface::treatScalar(const std::string& s,
				   const unsigned short a)
  {
    std::ostringstream res;
    if(a<9){
      res << "'" << makeUpperCase(s.substr(0,3)) << a << "'";
    } else {
      res << "'" << makeUpperCase(s.substr(0,2)) << a << "'";
    }
    return res.str();
  }

  std::string
  CastemInterface::treatStensor(const Hypothesis h,
			      const std::string& s)
  {
    using namespace std;
    string res;
    string s2 = makeUpperCase(s.substr(0,2));
    switch(h){
    case ModellingHypothesis::TRIDIMENSIONAL:
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ' "+"'"+s2+"XY' "+"'"+s2+"XZ' "+"'"+s2+"YZ'";
      break;
    case ModellingHypothesis::AXISYMMETRICAL:
      res="'"+s2+"RR' "+"'"+s2+"ZZ' "+"'"+s2+"TT' "+"'"+s2+"RZ'";
      break;
    case ModellingHypothesis::PLANESTRAIN:
    case ModellingHypothesis::PLANESTRESS:
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ' "+"'"+s2+"XY'";
      break;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      res="'"+s2+"RR' "+"'"+s2+"ZZ' "+"'"+s2+"TT'";
      break;
    default:
      ostringstream msg;
      msg << "CastemInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::string
  CastemInterface::treatStensor(const Hypothesis h,
				    const std::string& s,
				    const unsigned short a)
  {
    using namespace std;
    ostringstream stmp;
    string res;
    stmp << a;
    string s2 = makeUpperCase(s.substr(0,1))+stmp.str();
    switch(h){
    case ModellingHypothesis::TRIDIMENSIONAL:
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ' "+"'"+s2+"XY' "+"'"+s2+"XZ' "+"'"+s2+"YZ'";
      break;
    case ModellingHypothesis::AXISYMMETRICAL:
      res="'"+s2+"RR' "+"'"+s2+"ZZ' "+"'"+s2+"TT' "+"'"+s2+"RZ'";
      break;
    case ModellingHypothesis::PLANESTRAIN:
    case ModellingHypothesis::PLANESTRESS:
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ' "+"'"+s2+"XY'";
      break;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      res="'"+s2+"RR' "+"'"+s2+"ZZ' "+"'"+s2+"TT'";
      break;
    default:
      ostringstream msg;
      msg << "CastemInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::string
  CastemInterface::treatTensor(const Hypothesis h,
				   const std::string& s)
  {
    using namespace std;
    string res;
    string s2 = makeUpperCase(s.substr(0,2));
    switch(h){
    case ModellingHypothesis::TRIDIMENSIONAL:
      res="'"+s2+"XX' '"+s2+"YY' '"+s2+"ZZ' '"+
	s2+"XY' '"+s2+"YX' '"+s2+"XZ' '"+s2+"ZX' '"+s2+"YZ' '"+s2+"ZY'";
      break;
    case ModellingHypothesis::AXISYMMETRICAL:
      res="'"+s2+"RR' '"+s2+"ZZ' '"+s2+"TT' '"+s2+"RZ' '"+s2+"ZR'";
      break;
    case ModellingHypothesis::PLANESTRAIN:
    case ModellingHypothesis::PLANESTRESS:
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      res="'"+s2+"XX' '"+s2+"YY' '"+s2+"ZZ' '"+s2+"XY' '"+s2+"YX'";
      break;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      res="'"+s2+"RR' '"+s2+"ZZ' '"+s2+"TT'";
      break;
    default:
      ostringstream msg;
      msg << "CastemInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::string
  CastemInterface::treatTensor(const Hypothesis h,
				   const std::string& s,
				   const unsigned short a)
  {
    using namespace std;
    ostringstream stmp;
    string res;
    stmp << a;
    string s2 = makeUpperCase(s.substr(0,1))+stmp.str();
    switch(h){
    case ModellingHypothesis::TRIDIMENSIONAL:
      res="'"+s2+"XX' '"+s2+"YY' '"+s2+"ZZ' '"+
	s2+"XY' '"+s2+"YX' '"+s2+"XZ' '"+s2+"ZX' '"+s2+"YZ' '"+s2+"ZY'";
      break;
    case ModellingHypothesis::AXISYMMETRICAL:
      res="'"+s2+"RR' '"+s2+"ZZ' '"+s2+"TT' '"+s2+"RZ' '"+s2+"ZR'";
      break;
    case ModellingHypothesis::PLANESTRAIN:
    case ModellingHypothesis::PLANESTRESS:
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      res="'"+s2+"XX' '"+s2+"YY' '"+s2+"ZZ' '"+s2+"XY' '"+s2+"YX'";
      break;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      res="'"+s2+"RR' '"+s2+"ZZ' '"+s2+"TT'";
      break;
    default:
      ostringstream msg;
      msg << "CastemInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::set<tfel::material::ModellingHypothesis::Hypothesis>
  CastemInterface::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    const Hypothesis sh[6u] = {ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ModellingHypothesis::AXISYMMETRICAL,
			       ModellingHypothesis::PLANESTRESS,
			       ModellingHypothesis::PLANESTRAIN,
			       ModellingHypothesis::GENERALISEDPLANESTRAIN,
			       ModellingHypothesis::TRIDIMENSIONAL};
    // treatment 
    set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    for(const Hypothesis *ph = sh;ph!=sh+6u;++ph){
      if(bh.find(*ph)!=bh.end()){
	h.insert(*ph);
      }
    }
    if(h.empty()){
      throw(runtime_error("CastemInterface::getModellingHypothesesToBeTreated : "
			  "no hypotheses selected. This means that the given beahviour "
			  "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
			  "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
			  "make sense to use the Castem interface"));
    }
    return h;
  } // end of CastemInterfaceModellingHypothesesToBeTreated

  void
  CastemInterface::writeGetOutOfBoundsPolicyFunctionImplementation(std::ostream& out,
								 const std::string& name) const
  {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy(void){\n"
	<< "using namespace castem;\n"
	<< "using namespace tfel::material;\n"
	<< "static OutOfBoundsPolicy policy = CastemOutOfBoundsPolicy::getCastemOutOfBoundsPolicy().getOutOfBoundsPolicy();\n"
	<< "return policy;\n"
	<< "}\n\n";
  } // end of MFrontCastemInterface::writeGetOutOfBoundsPolicyFunctionImplementation    
  
  void
  CastemInterface::writeSetOutOfBoundsPolicyFunctionImplementation2(std::ostream& out,
								  const std::string& name,
								  const std::string& name2) const
  {
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionName(name2) << "_setOutOfBoundsPolicy(const int p){\n"
	<< this->getFunctionName(name) << "_setOutOfBoundsPolicy(p);\n"
	<< "}\n\n";
  }
  
  void
  CastemInterface::endTreatment(const BehaviourDescription& mb,
			      const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    // get the modelling hypotheses to be treated
    const auto& h = this->getModellingHypothesesToBeTreated(mb);
    // some consistency checks
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      if(mb.getSymmetryType()!=mb.getElasticSymmetryType()){
	string msg("CastemInterface::endTreatment : ");
	msg += "the type of the behaviour (isotropic or orthotropic) does not ";
	msg += "match the the type of its elastic behaviour.\n";
	msg += "This is not allowed here :\n";
	msg += "- an isotropic behaviour must have an isotropic elastic behaviour\n";
	msg += "- an orthotropic behaviour must have an orthotropic elastic behaviour";
	throw(runtime_error(msg));
      }
    }
    if(this->useTimeSubStepping){
      if(this->maximumSubStepping==0u){
	string msg("CastemInterface::endTreatment : ");
	msg += "use of time sub stepping requested but MaximumSubStepping is zero.\n";
	msg += "Please use the @CastemMaximumSubStepping directive";
	throw(runtime_error(msg));
      }
    }
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Castem");

    vector<FiniteStrainStrategy>::const_iterator pfss;
    
    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();

    string fileName("umat");
    fileName += name;
    fileName += ".hxx";

    ofstream out("include/MFront/Castem/"+fileName);
    if(!out){
      string msg("CastemInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }
  
    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the umat interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";
    
    out << "#include\"castem.h\"\n";
    out << "#ifdef umat" << endl;
    out << "#undef umat" << endl;
    out << "#endif /* umat */" << endl << endl;

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Castem/Castem.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Castem/CastemTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Castem/CastemOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace castem{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(h)){
      this->writeUMATBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & elem : h){
      if(mb.hasSpecialisedMechanicalData(elem)){
	this->writeUMATBehaviourTraits(out,mb,elem);
      }
    }

    out << "} // end of namespace castem\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeSetParametersFunctionsDeclarations(out,name,mb);
	this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_frst",mb);
	    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name+"_frst");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_malls",mb);
	    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name+"_malls");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_log1D",mb);
	    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name+"_log1D");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==NONE){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_ss",mb);
	    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name+"_ss");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
	    }
	  } else {
	    string msg("CastemInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeSetParametersFunctionsDeclarations(out,name,mb);
	  this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
	}
      }
    } else {
      this->writeSetParametersFunctionsDeclarations(out,name,mb);
      this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    }

    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeCastemFunctionDeclaration(out,name);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeCastemFunctionDeclaration(out,name+"_frst");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeCastemFunctionDeclaration(out,name+"_malls");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeCastemFunctionDeclaration(out,name+"_log1D");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==NONE){
	    this->writeCastemFunctionDeclaration(out,name+"_ss");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunctionDeclaration(out,name);
	    }
	  } else {
	    string msg("CastemInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeCastemFunctionDeclaration(out,name);
	}
      }
    } else {
      this->writeCastemFunctionDeclaration(out,name);
    }

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName  = "umat";
    fileName += name;
    fileName += ".cxx";

    out.open("src/"+fileName);
    if(!out){
      string msg("CastemInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the umat interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    this->getExtraSrcIncludes(out,mb);
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if(this->generateMTestFile){
      if((mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	 (mb.getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
     	string msg("CastemInterface::endTreatment : "
		   "unsupported behaviour type");
	throw(runtime_error(msg));
      }
      out << "#include\"MFront/Castem/CastemGetModellingHypothesis.hxx\"\n";
    }
    if((find(this->finiteStrainStrategies.begin(),this->finiteStrainStrategies.end(),
	     FINITEROTATIONSMALLSTRAIN)!=this->finiteStrainStrategies.end())||
       (find(this->finiteStrainStrategies.begin(),this->finiteStrainStrategies.end(),
	     MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN)!=this->finiteStrainStrategies.end())){
      out << "#include\"MFront/Castem/CastemFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/Castem/CastemOutOfBoundsPolicy.hxx\"\n";
    out << "#include\"MFront/Castem/CastemInterface.hxx\"\n\n";
    out << "#include\"MFront/Castem/CastemStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/Castem/umat" << name << ".hxx\"\n\n";

    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    if(this->finiteStrainStrategies.size()==1){
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
	    } else {
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name+"_frst");
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    if(this->finiteStrainStrategies.size()==1){
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
	    } else {
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name+"_malls");
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    if(this->finiteStrainStrategies.size()==1){
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
	    } else {
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name+"_log1D");
	    }
	  } else if(*pfss==NONE){
	    if(this->finiteStrainStrategies.size()==1){
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
	    } else {
	      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name+"_ss");
	    }
	  } else {
	    throw(std::runtime_error("MFrontCastemInterface::endTreatment : "
				     "internal error, unsupported finite strain strategy"));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
	}
      }
    } else {
      this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
    }
    
    out << "extern \"C\"{\n\n";

    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	if(!mb.areAllMechanicalDataSpecialised(h)){
	  const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	  this->generateUMATxxSymbols(out,name,uh,mb,fd);
	}
	for(const auto & elem : h){
	  if(mb.hasSpecialisedMechanicalData(elem)){
	    this->generateUMATxxSymbols(out,name,elem,mb,fd);
	  }
	}
	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	    << "_BehaviourType = 1u;\n\n";
	out << "MFRONT_SHAREDOBJ unsigned short umat"
	    << makeLowerCase(name) << "_Interface = 1u;\n\n";
      } else {
	for(const auto fss : this->finiteStrainStrategies){
	  if(fss==FINITEROTATIONSMALLSTRAIN){
	    this->generateUMATxxGeneralSymbols(out,name+"_frst",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_frst",uh,mb,fd);
	    }
	    for(const auto & elem : h){
	      if(mb.hasSpecialisedMechanicalData(elem)){
		this->generateUMATxxSymbols(out,name+"_frst",elem,mb,fd);
	      }
	    }
	    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name+"_frst") 
		<< "_BehaviourType = 2u;\n\n";
	    out << "MFRONT_SHAREDOBJ unsigned short umat"
		<< makeLowerCase(name+"_frst") << "_Interface = 2u;\n\n";
	    if(this->finiteStrainStrategies.size()==1u){
	      this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	      if(!mb.areAllMechanicalDataSpecialised(h)){
		const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
		this->generateUMATxxSymbols(out,name,uh,mb,fd);
	      }
	      for(const auto & elem : h){
		if(mb.hasSpecialisedMechanicalData(elem)){
		  this->generateUMATxxSymbols(out,name,elem,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 2u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 2u;\n\n";
	    }
	  } else if(fss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	      this->generateUMATxxGeneralSymbols(out,name+"_malls",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_malls",uh,mb,fd);
	    }
	    for(const auto & elem : h){
	      if(mb.hasSpecialisedMechanicalData(elem)){
		this->generateUMATxxSymbols(out,name+"_malls",elem,mb,fd);
	      }
	    }
	    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name+"_malls") 
		<< "_BehaviourType = 2u;\n\n";
	    out << "MFRONT_SHAREDOBJ unsigned short umat"
		<< makeLowerCase(name+"_malls") << "_Interface = 2u;\n\n";
	    if(this->finiteStrainStrategies.size()==1u){
	      this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	      if(!mb.areAllMechanicalDataSpecialised(h)){
		const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
		this->generateUMATxxSymbols(out,name,uh,mb,fd);
	      }
	      for(const auto & elem : h){
		if(mb.hasSpecialisedMechanicalData(elem)){
		  this->generateUMATxxSymbols(out,name,elem,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 2u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 2u;\n\n";
	    }
	  } else if(fss==LOGARITHMICSTRAIN1D){
	      this->generateUMATxxGeneralSymbols(out,name+"_log1D",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_log1D",uh,mb,fd);
	    }
	    for(const auto & elem : h){
	      if(mb.hasSpecialisedMechanicalData(elem)){
		this->generateUMATxxSymbols(out,name+"_log1D",elem,mb,fd);
	      }
	    }
	    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name+"_log1D") 
		<< "_BehaviourType = 1u;\n\n";
	    out << "MFRONT_SHAREDOBJ unsigned short umat"
		<< makeLowerCase(name+"_log1D") << "_Interface = 1u;\n\n";
	    if(this->finiteStrainStrategies.size()==1u){
	      this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	      if(!mb.areAllMechanicalDataSpecialised(h)){
		const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
		this->generateUMATxxSymbols(out,name,uh,mb,fd);
	      }
	      for(const auto & elem : h){
		if(mb.hasSpecialisedMechanicalData(elem)){
		  this->generateUMATxxSymbols(out,name,elem,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 1u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 1u;\n\n";
	    }
	  } else if(fss==NONE){
	    this->generateUMATxxGeneralSymbols(out,name+"_ss",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_ss",uh,mb,fd);
	    }
	    for(const auto & elem : h){
	      if(mb.hasSpecialisedMechanicalData(elem)){
		this->generateUMATxxSymbols(out,name+"_ss",elem,mb,fd);
	      }
	    }
	    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name+"_ss") 
		<< "_BehaviourType = 1u;\n\n";
	    out << "MFRONT_SHAREDOBJ unsigned short umat"
		<< makeLowerCase(name+"_ss") << "_Interface = 1u;\n\n";
	    if(this->finiteStrainStrategies.size()==1u){
	      this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	      if(!mb.areAllMechanicalDataSpecialised(h)){
		const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
		this->generateUMATxxSymbols(out,name,uh,mb,fd);
	      }
	      for(const auto & elem : h){
		if(mb.hasSpecialisedMechanicalData(elem)){
		  this->generateUMATxxSymbols(out,name,elem,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 1u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 1u;\n\n";
	    }
	  } else {
	    string msg("CastemInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	  if(!mb.areAllMechanicalDataSpecialised(h)){
	    const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	    this->generateUMATxxSymbols(out,name,uh,mb,fd);
	  }
	  for(const auto & elem : h){
	    if(mb.hasSpecialisedMechanicalData(elem)){
	      this->generateUMATxxSymbols(out,name,elem,mb,fd);
	    }
	  }
	  out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	      << "_BehaviourType = 1u;\n\n";
	  out << "MFRONT_SHAREDOBJ unsigned short umat"
	      << makeLowerCase(name) << "_Interface = 1u;\n\n";
	}
      }
    } else {
      this->generateUMATxxGeneralSymbols(out,name,mb,fd);
      if(!mb.areAllMechanicalDataSpecialised(h)){
	const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	this->generateUMATxxSymbols(out,name,uh,mb,fd);
      }
      for(const auto & elem : h){
	if(mb.hasSpecialisedMechanicalData(elem)){
	  this->generateUMATxxSymbols(out,name,elem,mb,fd);
	}
      }
      if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){ 
	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	    << "_BehaviourType = 2u;\n\n";
	out << "MFRONT_SHAREDOBJ unsigned short umat"
	    << makeLowerCase(name) << "_Interface = 2u;\n\n";
      } else {
	  out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	      << "_BehaviourType = 3u;\n\n";
	  out << "MFRONT_SHAREDOBJ unsigned short umat"
	      << makeLowerCase(name) << "_Interface = 1u;\n\n";
      }
    }

    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeSetParametersFunctionsImplementations(out,name,mb);
	this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeSetParametersFunctionsImplementations(out,name+"_frst",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionImplementation2(out,name,name+"_frst");
	    } else {
	      this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name+"_frst");
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeSetParametersFunctionsImplementations(out,name+"_malls",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionImplementation2(out,name,name+"_malls");
	    } else {
	      this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name+"_malls");
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeSetParametersFunctionsImplementations(out,name+"_log1D",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionImplementation2(out,name,name+"_log1D");
	    } else {
	      this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name+"_log1D");
	    }
	  } else if(*pfss==NONE){
	    this->writeSetParametersFunctionsImplementations(out,name+"_ss",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	      this->writeSetOutOfBoundsPolicyFunctionImplementation2(out,name,name+"_ss");
	    } else {
	      this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name+"_ss");
	    }
	  } else {
	    string msg("CastemInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeSetParametersFunctionsImplementations(out,name,mb);
	  this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);
	}
      }
    } else {
      this->writeSetParametersFunctionsImplementations(out,name,mb);
      this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);
    }

    out << "static void \numat"
	<< makeLowerCase(name) << "_base" 
	<< "(const castem::CastemInt *const NTENS, const castem::CastemReal *const DTIME,\n"
	<< "const castem::CastemReal *const DROT,  castem::CastemReal *const DDSDDE,\n"
	<< "const castem::CastemReal *const STRAN, const castem::CastemReal *const DSTRAN,\n"
	<< "const castem::CastemReal *const TEMP,  const castem::CastemReal *const DTEMP,\n"
	<< "const castem::CastemReal *const PROPS, const castem::CastemInt    *const NPROPS,\n"
	<< "const castem::CastemReal *const PREDEF,const castem::CastemReal *const DPRED,\n"
	<< "castem::CastemReal *const STATEV,const castem::CastemInt    *const NSTATV,\n"
	<< "castem::CastemReal *const STRESS,castem::CastemReal *const PNEWDT,\n"
	<< "const castem::CastemInt *const NDI,\n"
	<< "castem::CastemInt  *const KINC,\n"
	<< "const castem::StressFreeExpansionHandler& sfeh)\n";
    out << "{\n";
    out << "tfel::material::OutOfBoundsPolicy op = "
	<< this->getFunctionName(name) << "_getOutOfBoundsPolicy();\n";
    out << "castem::CastemInterface<tfel::material::" << mb.getClassName()
	<< ">::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,\n"
	<< "TEMP,DTEMP,PROPS,NPROPS,\n"
	<< "PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "STRESS,PNEWDT,NDI,KINC,op,sfeh);\n";
    out << "}\n\n";

    
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeCastemFunction(out,name,"",mb,
				&CastemInterface::writeStandardCastemFunction);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeCastemFunction(out,name,"frst",mb,
				    &CastemInterface::writeFiniteRotationSmallStrainCastemFunction);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunction(out,name,"",mb,
				      &CastemInterface::writeFiniteRotationSmallStrainCastemFunction);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeCastemFunction(out,name,"malls",mb,
				    &CastemInterface::writeMieheApelLambrechtLogarithmicStrainCastemFunction);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunction(out,name,"",mb,
				      &CastemInterface::writeMieheApelLambrechtLogarithmicStrainCastemFunction);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeCastemFunction(out,name,"log1D",mb,
				    &CastemInterface::writeLogarithmicStrain1DCastemFunction);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunction(out,name,"",mb,
				      &CastemInterface::writeLogarithmicStrain1DCastemFunction);
	    }
	  } else if(*pfss==NONE){
	    this->writeCastemFunction(out,name,"ss",mb,
				    &CastemInterface::writeStandardCastemFunction);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeCastemFunction(out,name,"",mb,
				      &CastemInterface::writeStandardCastemFunction);
	    }
	  } else {
	    string msg("CastemInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeCastemFunction(out,name,"",mb,
				  &CastemInterface::writeStandardCastemFunction);
	}
      }
    } else {
      this->writeCastemFunction(out,name,"",mb,
			      &CastemInterface::writeStandardCastemFunction);
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      this->generateGibianeDeclaration(mb,fd);
    }
  } // end of CastemInterface::endTreatment

  void
  CastemInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "mg.setModellingHypothesis(castem::getModellingHypothesis(*NDI));\n";
  }
  
  void 
  CastemInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
						      const BehaviourDescription&) const
  {
    using namespace std;
    out << "#include\"MFront/Castem/Castem.hxx\"" << endl << endl;
  } // end of CastemInterface::writeInterfaceSpecificIncludes

  CastemInterface::~CastemInterface()
  {}

  void
  CastemInterface::getTargetsDescription(TargetsDescription& d,
				       const BehaviourDescription& bd)
  {
    using namespace std;
    const auto lib  = CastemInterface::getLibraryName(bd);
    const auto name = this->getBehaviourName(bd);
#ifdef _WIN32
    const string tfel_config = "tfel-config.exe";
#else /* WIN32 */
    const string tfel_config = "tfel-config";
#endif /* WIN32 */
    insert_if(d[lib].cppflags,"$(shell "+tfel_config+" --includes)");
#ifdef CASTEM_CPPFLAGS
    insert_if(d[lib].cppflags,CASTEM_CPPFLAGS);
#endif /* CASTEM_CPPFLAGS */
#ifndef LOCAL_CASTEM_HEADER
#ifdef CASTEM_ROOT
    char * castem_root = ::getenv("CASTEM_ROOT");
    if(castem_root!=nullptr){
      insert_if(d[lib].cppflags,"-I"+string(castem_root)+"/include");
    } else {
      insert_if(d[lib].cppflags,"-I"+string(CASTEM_ROOT)+"/include");
    }
#else /* CASTEM_ROOT */
    if(castem_root!=0){
      insert_if(d[lib].cppflags,"-I"+string(castem_root)+"/include");
    }
#endif /* CASTEM_ROOT */
#endif /* LOCAL_CASTEM_HEADER_FILE */
    insert_if(d[lib].sources,"umat"+name+".cxx");    
    insert_if(d.headers,"MFront/Castem/umat"+name+".hxx");
    insert_if(d[lib].ldflags,"-lCastemInterface");
    if(this->generateMTestFile){
      insert_if(d[lib].ldflags,"-lMTestFileGenerator");
    }
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    // entry points
    auto b = vector<string>{};
    const auto base = this->getUmatFunctionName(bd);
    if(bd.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	b.push_back(base);
	// b.push_back(name);
      } else {
	for(const auto& fss : this->finiteStrainStrategies){
	  if(fss==FINITEROTATIONSMALLSTRAIN){
	    b.push_back(base+"_frst");
	    b.push_back(name+"_frst");
	  } else if(fss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    b.push_back(base+"_malls");
	    b.push_back(name+"_malls");
	  } else if(fss==LOGARITHMICSTRAIN1D){
	    b.push_back(base+"_log1D");
	    b.push_back(name+"_log1D");
	  } else if(fss==NONE){
	    b.push_back(base+"_ss");
	    b.push_back(name+"_ss");
	  } else {
	    throw(runtime_error("CastemInterface::getGeneratedEntryPoints: "
				"internal error, unsupported finite strain strategy"));
	  }
	  if(this->finiteStrainStrategies.size()==1u){
	    b.push_back(base);
	    // b.push_back(name);
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  b.push_back(base);
	  b.push_back(name);
	}
      }
    } else {
      b.push_back(base);
      b.push_back(name);
    }
    insert_if(d[lib].epts,b);
  } // end of CastemInterface::getTargetsDescription

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  CastemInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
						   const Hypothesis h) const
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for(const auto & elem : ah){
	if(!mb.hasSpecialisedMechanicalData(elem)){
	  uh.insert(elem);
	}
      }
      if(uh.empty()){
	string msg("CastemInterface::endTreatment : ");
	msg += "internal error : the mechanical behaviour says that not "
	  "all handled mechanical data are specialised, but we found none.";
	throw(runtime_error(msg));
      }
      // material properties for all the selected hypothesis
      vector<pair<vector<UMATMaterialProperty>,
		  SupportedTypes::TypeSize> > mpositions;
      for(const auto & elem : uh){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,elem));
      }
      auto ph=uh.cbegin();
      auto pum = mpositions.cbegin();
      const auto& mfirst = *pum;
      ++ph;
      ++pum;
      for(;ph!=uh.end();++ph,++pum){
	const auto& d = mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const auto& mps = d.getMaterialProperties();
	for(const auto & mp : mps){
	  const auto& mp1 = findUMATMaterialProperty(mfirst.first,
						     mb.getExternalName(h,mp.name));
	  const auto& mp2 = findUMATMaterialProperty(pum->first,
						     mb.getExternalName(h,mp.name));
	  SupportedTypes::TypeSize o1 = mp1.offset;
	  o1+=pum->second;
	  SupportedTypes::TypeSize o2 = mp2.offset;
	  o2+=mfirst.second;
	  if(o1!=o2){
	    string msg("CastemInterface::buildMaterialPropertiesList : "
		       "incompatible offset for material property '"+mp.name+
		       "' (aka '"+mp1.name+"'). This is one pitfall of the umat interface. "
		       "To by-pass this limitation, you may want to explicitely "
		       "specialise some modelling hypotheses");
	    throw(runtime_error(msg));
	  }
	}
      }
      return mfirst;
    }
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> res;
    auto& mprops = res.first;
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus","youn",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio","nu",false);
	this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion","alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio12","nu12",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio23","nu22",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio13","nu13",false);
	  this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3","alp3",false);
	} else if(h==ModellingHypothesis::PLANESTRESS){
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio12","nu12",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	  this->appendToMaterialPropertiesList(mprops,"real","V1X","v1x",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V1Y","v1y",true);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio23","nu23",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio13","nu13",false);
	  this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	  this->appendToMaterialPropertiesList(mprops,"length","PlateWidth","epai",false);
	} else if((h==ModellingHypothesis::AXISYMMETRICAL)||
		  (h==ModellingHypothesis::PLANESTRAIN)||
		  (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio12","nu12",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio23","nu23",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio13","nu13",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	  this->appendToMaterialPropertiesList(mprops,"real","V1X","v1x",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V1Y","v1y",true);
	  this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3","alp3",false);
	} else if(h==ModellingHypothesis::TRIDIMENSIONAL){	    
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio12","nu12",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio23","nu23",false);
	  this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio13","nu13",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus23","g23",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus13","g13",false);
	  this->appendToMaterialPropertiesList(mprops,"real","V1X","v1x",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V1Y","v1y",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V1Z","v1z",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V2X","v2x",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V2Y","v2y",true);
	  this->appendToMaterialPropertiesList(mprops,"real","V2Z","v2z",true);
	  this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	  this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3","alp3",false);
	} else {
	  string msg("CastemInterface::buildMaterialPropertiesList : ");
	  msg += "unsupported modelling hypothesis.";
	  throw(runtime_error(msg));
	}
      } else {
	string msg("CastemInterface::buildMaterialPropertiesList : ");
	msg += "unsupported behaviour symmetry type.\n";
	msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
	throw(runtime_error(msg));
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	//! those are not the Cast3M conventions, switch is performed
	//! below the CastemInterface class
	this->appendToMaterialPropertiesList(mprops,"real","NormalStiffness","kn",false);
	this->appendToMaterialPropertiesList(mprops,"real","TangentialStiffness","kt",false);
	this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion",
					     "NormalThermalExpansion","ALPN",false);
      } else {
	string msg("CastemInterface::buildMaterialPropertiesList : ");
	msg += "unsupported symmetry type for cohesive zone modes.\n";
	throw(runtime_error(msg));
      }
    } else {
      string msg("CastemInterface::buildMaterialPropertiesList : ");
      msg += "unsupported behaviour type.\n";
      throw(runtime_error(msg));
    }
    if(!mprops.empty()){
      const auto& m = mprops.back();
      res.second  = m.offset;
      res.second += this->getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of CastemInterface::buildMaterialPropertiesList

  void
  CastemInterface::writeUMATxxBehaviourTypeSymbols(std::ostream&,
						       const std::string&,
						       const BehaviourDescription&) const
  {} // end of CastemInterface::writeUMATxxBehaviourTypeSymbols

  void
  CastemInterface::writeUMATxxAdditionalSymbols(std::ostream&,
						    const std::string&,
						    const Hypothesis,
						    const BehaviourDescription&,
						    const FileDescription&) const
  {} // end of CastemInterface::writeUMATxxAdditionalSymbols

  void
  CastemInterface::writeUMATxxSpecificSymbols(std::ostream& out,
						  const std::string& name,
						  const BehaviourDescription& mb,
						  const FileDescription&) const
  {
    if(CastemInterface::usesGenericPlaneStressAlgorithm(mb)){
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	  << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";    
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	  << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";    
    }
  }

  void
  CastemInterface::writeCastemFunctionDeclaration(std::ostream& out,
					      const std::string& name) const
  {
    out << "MFRONT_SHAREDOBJ void\numat"
    	<< makeLowerCase(name);
    writeUMATArguments(out);
    out << ";\n\n";
  } // end of CastemInterface::writeCastemFunctionDeclaration

  void
  CastemInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall(std::ostream& out,
									  const BehaviourDescription& mb,
									  const std::string& c,
									  const std::string& c2) const
  {
    using namespace std;
    if((mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS))||
       (this->usesGenericPlaneStressAlgorithm(mb))){
      out << "if(*NDI==" << getCastemModellingHypothesisIndex(ModellingHypothesis::PLANESTRESS) << "){\n";
      if(mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)){
	const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
	if(v.first){
	  out << "const CastemReal ezz = STATEV[" << v.second.getValueForDimension(2) << "];\n"
	      << "const CastemReal Fzz = " << c2 << ";" << endl;
	  out << c << ",Fzz);\n";	  
	} else {
	  // no axial strain
	  out << "std::cerr << \"no state variable standing for the axial strain (variable with the "
	      << "glossary name 'AxialStrain')\" << std::endl;\n";
	  out << "*KINC=-1;\n";
	  out << "return;\n";
	}
      } else {
	// generic algorithm, this means that the behaviour
	// can be called in generalised plane strain
	const auto& d = mb.getBehaviourData(ModellingHypothesis::GENERALISEDPLANESTRAIN);
	SupportedTypes::TypeSize s =  this->getTotalSize(d.getPersistentVariables());
	if(s.getValueForDimension(2)==0){
	  out << "const CastemReal ezz = STATEV[0];\n";
	} else {
	  out << "if(*NSTATV!=" << s.getValueForDimension(2)+1 << "){\n"
	      << "std::cerr << \"invalid number of internal state variables\" << std::endl;\n"
	      << "}\n";
	  out << "const CastemReal ezz = STATEV[" << s.getValueForDimension(2) << "];\n";
	}
	out << "const CastemReal Fzz = " << c2 << ";" << endl;
	out << c << ",Fzz);\n";
      }
      out << "} else {\n";
      out << c << ",0);\n";
      out << "}\n";
    } else {
      out << c << ",0);\n";
    }
  } // end of CastemInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall

  void
  CastemInterface::writeCastemFunction(std::ostream& out,
				   const std::string& name,
				   const std::string& suffix,
				   const BehaviourDescription& mb,
				   void  (CastemInterface:: *m)(std::ostream&,
							      const std::string&,
							      const std::string&,
							      const std::string&,
							      const BehaviourDescription&) const) const
  {
    const auto base  = suffix.empty() ? name : name+"_"+suffix;
    const auto fname2 = "umat"+makeLowerCase(base);
    //    (this->*m)(out,name,base,suffix,mb);
    (this->*m)(out,name,fname2,suffix,mb);
  }

  void
  CastemInterface::writeFiniteRotationSmallStrainCastemFunction(std::ostream& out,
							    const std::string& name,
							    const std::string& fname,
							    const std::string& suffix,
							    const BehaviourDescription& mb) const
  {
    using namespace std;
    if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("CastemInterface::writeFiniteRotationSmallStrainCastemFunction : "
		 "finite strain strategies shall be used with small strain behaviours");
      throw(runtime_error(msg));
    }
    out << "MFRONT_SHAREDOBJ void\n"
	<< fname;
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR);
    out << endl;
    out << "{\n";
    out << "using namespace castem;\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "// computing the Green Lagrange strains\n";
    out << "CastemReal eto[6];\n";
    out << "CastemReal deto[6];\n";
    out << "CastemInt  i;\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "{\n"
	  << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "CastemFiniteStrain::computeGreenLagrangeStrain(eto,F0,*NTENS,*NDI);\n";
    out << "CastemFiniteStrain::computeGreenLagrangeStrain(deto,F1,*NTENS,*NDI);\n";
    string c1 = "CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,F0,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c1,"std::sqrt(1+2*ezz)");
    out << "for(i=0;i!=*NTENS;++i){\n";
    out << "deto[i] -= eto[i];\n";
    out << "}\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
	<< "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "STRESS,PNEWDT,NDI,KINC,\n"
	<< "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    string c2 = "CastemFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c2,"std::sqrt(1+2*ezz)");
    out << "}\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
  }
  
  void
  CastemInterface::writeMieheApelLambrechtLogarithmicStrainCastemFunction(std::ostream& out,
								      const std::string& name,
								      const std::string& fname,
								      const std::string& suffix,
								      const BehaviourDescription& mb) const
  {
    using namespace std;
    if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("CastemInterface::writeMieheApelLambrechtLogarithmicStrainCastemFunction : "
		 "finite strain strategies shall be used with small strain behaviours");
      throw(runtime_error(msg));
    }
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR);
    out << endl;
    out << "{\n";
    out << "using namespace castem;\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "// computing the logarithmic strain\n";
    out << "CastemReal eto[6];\n";
    out << "CastemReal deto[6];\n";
    out << "CastemReal P0[36];\n";
    out << "CastemReal P1[36];\n";
    out << "CastemReal s[6];\n";
    out << "CastemInt  i;\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "{\n"
	  << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "CastemFiniteStrain::computeLogarithmicStrainAndDerivative(P0,eto ,F0,*NTENS,*NDI);\n";
    out << "CastemFiniteStrain::computeLogarithmicStrainAndDerivative(P1,deto,F1,*NTENS,*NDI);\n";
    string c1 = "CastemFiniteStrain::computeDualStressOfLogarithmicStrainFromCauchyStress(s,STRESS,P0,F0,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c1,"std::exp(ezz)");
    out << "for(i=0;i!=*NTENS;++i){\n";
    out << "deto[i] -= eto[i];\n";
    out << "}\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
	<< "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "s,PNEWDT,NDI,KINC,\n"
	<< "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    string c2 = "CastemFiniteStrain::computeCauchyStressFromDualStressOfLogarithmicStrain(STRESS,s,P1,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c2,"std::exp(ezz)");
    out << "}\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
       			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void
  CastemInterface::writeLogarithmicStrain1DCastemFunction(std::ostream& out,
						      const std::string& name,
						      const std::string& fname,
						      const std::string& suffix,
						      const BehaviourDescription& mb) const
  {
    using namespace std;
    if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("CastemInterface::writeLogarithmicStrain1DCastemFunction : "
    		 "finite strain strategies shall be used with small strain behaviours");
      throw(runtime_error(msg));
    }
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR);
    out << endl;
    out << "{\n";
    out << "using namespace castem;\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
    	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "bool k = std::abs(*DDSDDE)>0.5;\n";
    out << "// computing the logarithmic strain\n";
    out << "CastemReal eto[3];\n";
    out << "CastemReal deto[3];\n";
    out << "CastemReal s[3];\n";
    out << "CastemReal K[9];\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "{\n"
    	  << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
    	  << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "if(*NDI!=14){" << endl
	<< "*KINC=-7;" << endl
	<< "return;" << endl
	<< "}" << endl;
    out << "eto[0]=std::log(1+*STRAN);\n";
    out << "eto[1]=std::log(1+*(STRAN+1));\n";
    out << "eto[2]=std::log(1+*(STRAN+2));\n";
    out << "deto[0]=std::log(1+*STRAN+*DSTRAN)-eto[0];\n";
    out << "deto[1]=std::log(1+*(STRAN+1)+*(DSTRAN+1))-eto[1];\n";
    out << "deto[2]=std::log(1+*(STRAN+2)+*(DSTRAN+2))-eto[2];\n";
    out << "s[0]=(*STRESS)*(1+*STRAN);\n";
    out << "s[1]=(*(STRESS+1))*(1+*(STRAN+1));\n";
    out << "s[2]=(*(STRESS+2))*(1+*(STRAN+2));\n";
    out << "K[0]=*DDSDDE;\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
    	<< "_base(NTENS, DTIME,DROT,K,eto,deto,TEMP,DTEMP,\n"
    	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
    	<< "s,PNEWDT,NDI,KINC,\n"
    	<< "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
    	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    // First Piola-Kirchhoff stress
    out << "STRESS[0]=s[0]/(1+*STRAN+*DSTRAN);\n";
    out << "STRESS[1]=s[1]/(1+*(STRAN+1)+*(DSTRAN+1));\n";
    out << "STRESS[2]=s[2]/(1+*(STRAN+2)+*(DSTRAN+2));\n";
    // computation of the stiffness matrix
    out << "if(k){\n"
	<< "*DDSDDE     = (-STRESS[0]+K[0]/(1+STRAN[0]+DSTRAN[0]))/(1+STRAN[0]+DSTRAN[0]);\n"
	<< "*(DDSDDE+3) = K[3]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[0]+DSTRAN[0]));\n"
	<< "*(DDSDDE+6) = K[6]/((1+STRAN[2]+DSTRAN[2])*(1+STRAN[0]+DSTRAN[0]));\n"
	<< "*(DDSDDE+1) = K[1]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[1]+DSTRAN[1]));\n"
	<< "*(DDSDDE+4) = (-STRESS[1]+K[4]/(1+STRAN[1]+DSTRAN[1]))/(1+STRAN[1]+DSTRAN[1]);\n"
	<< "*(DDSDDE+7) = K[7]/((1+STRAN[2]+DSTRAN[2])*(1+STRAN[1]+DSTRAN[1]));\n"
	<< "*(DDSDDE+2) = K[2]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[2]+DSTRAN[2]));\n"
	<< "*(DDSDDE+5) = K[5]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[2]+DSTRAN[2]));\n"
	<< "*(DDSDDE+8) = (-STRESS[2]+K[8]/(1+STRAN[2]+DSTRAN[2]))/(1+STRAN[2]+DSTRAN[2]);\n"
	<< "}\n";
    out << "}\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
       			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void
  CastemInterface::writeStandardCastemFunction(std::ostream& out,
					   const std::string& name,
					   const std::string& fname,
					   const std::string& suffix,
					   const BehaviourDescription& mb) const
  {
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,mb.getBehaviourType());
    out << "\n{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "umat" << makeLowerCase(name)
	  << "_base(NTENS, DTIME,DROT,DDSDDE,F0,F1,TEMP,DTEMP,\n"
	  << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	  << "STRESS,PNEWDT,NDI,KINC,\n"
	  << "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    } else {
      out << "umat" << makeLowerCase(name)
	  << "_base(NTENS, DTIME,DROT,DDSDDE,STRAN,DSTRAN,TEMP,DTEMP,\n"
	  << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	  << "STRESS,PNEWDT,NDI,KINC,\n"
	  << "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    }
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,mb.getBehaviourType(),
       			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void
  CastemInterface::writeVariableDescriptionContainerToGibiane(std::ostream& out,
							      const Hypothesis h,
							      const VariableDescriptionContainer& v) const
  {
    for(const auto & elem : v){
      const auto flag = this->getTypeFlag(elem.type);
      std::string tmp;
      tmp += ' ';
      if(flag==SupportedTypes::Scalar){
	if(elem.arraySize==1){
	  tmp += treatScalar(elem.name);
	} else {
	  for(unsigned short j=0;j!=elem.arraySize;){
	    tmp += treatScalar(elem.name,j);
	    if(++j!=elem.arraySize){
	      tmp += ' ';
	    }
	  }
	}
      } else if(flag==SupportedTypes::Stensor){
	if(elem.arraySize==1){
	  tmp += treatStensor(h,elem.name);
	} else {
	  for(unsigned short j=0;j!=elem.arraySize;){
	    tmp += treatStensor(h,elem.name,j);
	    if(++j!=elem.arraySize){
	      tmp += ' ';
	    }
	  }
	}
      } else if(flag==SupportedTypes::Tensor){
	if(elem.arraySize==1){
	  tmp += treatTensor(h,elem.name);
	} else {
	  for(unsigned short j=0;j!=elem.arraySize;){
	    tmp += treatTensor(h,elem.name,j);
	    if(++j!=elem.arraySize){
	      tmp += ' ';
	    }
	  }
	}
      } else {
	throw(std::runtime_error("CastemInterface::writeVariableDescriptionContainerToGibiane: "
				 "internal error, tag unsupported"));
      }
      out << tmp;
    }
  }

  void
  CastemInterface::writeGibianeInstruction(std::ostream& out,
					 const std::string& i) const
  {
    using namespace std;
    istringstream in(i);
    string buffer;
    while(!in.eof()){
      string w;
      in >> w;
      if(buffer.size()+w.size()>70){
	out << buffer << endl;
	buffer.clear();
	if(w.size()>70){
	  out << w << endl;
	} else {
	  buffer = w;
	}
      } else {
	buffer+=' '+w;
      }
    }
    out << buffer << endl;
  } // end of CastemInterface::writeGibianeInstruction

  void
  CastemInterface::generateGibianeDeclaration(const BehaviourDescription& mb,
					      const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    std::map<ModellingHypothesis::Hypothesis,std::string> mo = {
      {ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,"'UNID' 'AXIS' 'AXGZ'"},
      {ModellingHypothesis::AXISYMMETRICAL,"'AXIS'"},
      {ModellingHypothesis::PLANESTRESS,"'PLAN' 'CONT'"},
      {ModellingHypothesis::PLANESTRAIN,"'PLAN' 'DEFO'"},
      {ModellingHypothesis::GENERALISEDPLANESTRAIN,"'PLAN' 'GENE'"},
      {ModellingHypothesis::TRIDIMENSIONAL,"'TRID'"}};
    const auto name((!mb.getLibrary().empty())?mb.getLibrary()+mb.getClassName():mb.getClassName());
    const auto fileName("castem/"+name+".dgibi");
    // opening output file
    systemCall::mkdir("castem");
    ofstream out;
    out.open(fileName);
    if(!out){
      throw(runtime_error("CastemInterface::generateGibianeDeclaration: "
			  "could not open file '"+fileName+"'"));
    }
    // header
    out << "*\n";
    out << "* \\file   "  << fd.fileName << endl;
    out << "* \\brief  example of how to use the " << mb.getClassName() << " behaviour law\n"
	<< "* in the Cast3M finite element solver\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*\n\n";
    // specific declaration
    string nonlin;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      nonlin = "'NON_LINEAIRE' 'UTILISATEUR'";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      nonlin = "'NON_LINEAIRE' 'UTILISATEUR' 'EPSILON' 'UTILISATEUR'";
    } else {
      throw(runtime_error("CastemInterface::generateGibianeDeclaration: "
			  "internal error, unsupported behaviour type"));
    }
    // loop over hypothesis
    const auto h = this->getModellingHypothesesToBeTreated(mb);
    for(const auto & elem : h){
      const auto& d = mb.getBehaviourData(elem);
      const auto& persistentVarsHolder = d.getPersistentVariables();
      const auto& externalStateVarsHolder = d.getExternalStateVariables();
      const auto mprops = this->buildMaterialPropertiesList(mb,elem);
      string tmp;
      out << "** " << ModellingHypothesis::toString(elem) << " example\n";
      if(mo.find(elem)!=mo.end()){
	out << "** 'OPTION' 'DIMENSION' " << getSpaceDimension(elem) << " 'MODELISER' " << mo[elem] << " ;\n\n";
      }
      ostringstream mcoel;
      mcoel << "coel = 'MOTS' ";
      for(auto pm=mprops.first.cbegin();pm!=mprops.first.cend();){
	auto flag = this->getTypeFlag(pm->type);
	if(flag!=SupportedTypes::Scalar){
	  throw(runtime_error("CastemInterface::generateGibianeDeclaration: "
			      "material properties shall be scalars"));
	}
	if(pm->arraySize==1){
	  mcoel << treatScalar(pm->var_name);
	} else {
	  for(unsigned short j=0;j!=pm->arraySize;){
	    mcoel << treatScalar(pm->var_name,j);
	    if(++j!=pm->arraySize){
	      mcoel << " ";
	    }
	  }
	}
	if(++pm!=mprops.first.end()){
	  mcoel << " ";
	}
      }
      mcoel << ";";
      writeGibianeInstruction(out,mcoel.str());
      out << endl;
      if(!persistentVarsHolder.empty()){
	ostringstream mstatev;
	mstatev << "statev = 'MOTS' ";
	this->writeVariableDescriptionContainerToGibiane(mstatev,elem,persistentVarsHolder);
	mstatev << ";";
	writeGibianeInstruction(out,mstatev.str());
	out << endl;
      }
      ostringstream mparam;
      mparam << "params = 'MOTS' 'T'";
      if(!externalStateVarsHolder.empty()){
	mparam << " ";
	this->writeVariableDescriptionContainerToGibiane(mparam,elem,externalStateVarsHolder);
      }
      mparam << ";";
      writeGibianeInstruction(out,mparam.str());
      out << endl;
      ostringstream mmod;
      mmod << "MO = 'MODELISER' v 'MECANIQUE' 'ELASTIQUE' ";
      mmod << nonlin << "\n"
	   << "'LIB_LOI' 'lib"+this->getLibraryName(mb)+".so'\n"
	   << "'FCT_LOI' '"+this->getUmatFunctionName(mb)+"'\n"
	   << "'C_MATERIAU' coel ";
      if(!persistentVarsHolder.empty()){
	mmod << "'C_VARINTER' statev ";
      }
      mmod << "'PARA_LOI'   params 'CONS' M;";
      writeGibianeInstruction(out,mmod.str());
      out << endl;
      ostringstream mi;
      mi << "MA = 'MATERIAU' MO ";
      for(auto pm=mprops.first.cbegin();pm!=mprops.first.cend();){
	auto flag = this->getTypeFlag(pm->type);
	if(flag!=SupportedTypes::Scalar){
	  throw(runtime_error("CastemInterface::generateGibianeDeclaration: "
			      "material properties shall be scalars"));
	}
	if(pm->arraySize==1){
	  tmp = treatScalar(pm->var_name);
	  mi << tmp << " x"<< makeLowerCase(pm->var_name);
	} else {
	  for(unsigned short j=0;j!=pm->arraySize;){
	    tmp = treatScalar(pm->var_name,j);
	    mi << tmp << " x"<< makeLowerCase(pm->var_name) << j;
	    if(++j!=pm->arraySize){
	      mi << " ";
	    }
	  }
	}
	if(++pm!=mprops.first.end()){
	  mi << " ";
	}
      }
      if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
	if((elem==ModellingHypothesis::PLANESTRESS)||
	   (elem==ModellingHypothesis::AXISYMMETRICAL)||
	   (elem==ModellingHypothesis::PLANESTRAIN)||
	   (elem==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  mi << " 'DIRECTION' (1 0) 'PARALLELE'";
	} else if(elem==ModellingHypothesis::TRIDIMENSIONAL){	    
	  mi << " 'DIRECTION' (1 0 0) (0 0 1) 'PARALLELE'";
	}
      }
      mi << ";";
      writeGibianeInstruction(out,mi.str());
      out << endl;
    }
    out.close();
  } // end of CastemInterface::generateGibianeDeclaration

  void
  CastemInterface::writeUMATBehaviourTraits(std::ostream& out,
						const BehaviourDescription& mb,
						const Hypothesis h) const
  {
    using namespace std;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb,h);
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(mb.useQt()){
	out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
      } else {
	out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
      }
      out << "struct CastemTraits<tfel::material::" << mb.getClassName() << "<H,Type,";
    } else {
      if(mb.useQt()){
	out << "template<typename Type,bool use_qt>\n";
      } else {
	out << "template<typename Type>\n";
      }
      out << "struct CastemTraits<tfel::material::" << mb.getClassName() 
	  << "<tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ",Type,";
    }
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "typedef tfel::material::ModellingHypothesis ModellingHypothesis;\n";
      out << "typedef tfel::material::ModellingHypothesisToSpaceDimension<ModellingHypothesis::" <<
	ModellingHypothesis::toUpperCaseString(h) << "> ModellingHypothesisToSpaceDimension;\n";
    } else {
      out << "typedef tfel::material::ModellingHypothesis ModellingHypothesis;\n";
      out << "typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;\n";
    }
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "static " << constexpr_c << " ModellingHypothesis::Hypothesis H = " 
	  << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
	  << ";" << endl;
    }
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " CastemBehaviourType btype  = SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " CastemBehaviourType btype  = FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL){
      out << "static " << constexpr_c << " CastemBehaviourType btype  = COHESIVEZONEMODEL;\n";
    } else {
      throw(runtime_error("CastemInterface::writeUMATBehaviourTraits: "
			  "unsupported behaviour type"));
    }
    out << "// space dimension\n";
    out << "static " << constexpr_c << " unsigned short N           = ModellingHypothesisToSpaceDimension::value;\n";
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c << " unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c << " unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array (STRAN)\n";
    out << "static " << constexpr_c << " unsigned short DrivingVariableSize = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c << " unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
    out << "static " << constexpr_c << " bool useTimeSubStepping = ";
    if(this->useTimeSubStepping){
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static " << constexpr_c << " bool doSubSteppingOnInvalidResults = ";
    if(this->doSubSteppingOnInvalidResults){
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static " << constexpr_c << " unsigned short maximumSubStepping = ";
    if(this->useTimeSubStepping){
      out << this->maximumSubStepping << ";\n";
    } else {
      out << "0u;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = true;\n";
      if(mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,false)){
	out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = true;\n";
      } else {
	out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = false;\n";
      }
    } else {
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    // computing material properties size
    SupportedTypes::TypeSize msize;
    if(!mprops.first.empty()){
      const auto& m = mprops.first.back();
      msize  = m.offset;
      msize += this->getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c << " unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " unsigned short propertiesOffset = 4u;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
	// something needs to be done here
	out << "static " << constexpr_c << " unsigned short propertiesOffset = CastemOrthotropicOffset<castem::SMALLSTRAINSTANDARDBEHAVIOUR,N>::value;\n";
      } else {
	throw(runtime_error("CastemInterface::writeUMATBehaviourTraits: "
			    "unsupported behaviour type"));
      }
    } else {
      throw(runtime_error("CastemInterface::writeUMATBehaviourTraits:"
			  "unsupported behaviour symmetry type.\n"
			  "The umat interface only support isotropic or "
			  "orthotropic behaviour at this time."));
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " CastemSymmetryType stype = castem::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " CastemSymmetryType stype = castem::ORTHOTROPIC;\n";
    } else {
      string msg("CastemInterface::writeUMATBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class CastemTraits\n\n";
  } // end of CastemInterface::writeUMATBehaviourTraits

  std::map<UMATInterfaceBase::Hypothesis,std::string>
  CastemInterface::gatherModellingHypothesesAndTests(const BehaviourDescription& mb) const
  {
    auto res = std::map<Hypothesis,std::string>{};
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      const auto h = this->getModellingHypothesesToBeTreated(mb);
      for(const auto & mh : h){
	res.insert({mh,this->getModellingHypothesisTest(mh)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  } // end of CastemInterface::gatherModellingHypothesesAndTests

  std::string
  CastemInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    std::ostringstream test;
    test << "*NDI==" << getCastemModellingHypothesisIndex(h);
    return test.str();
  }

} // end of namespace mfront
