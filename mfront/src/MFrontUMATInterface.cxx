/*!
 * \file   mfront/src/MFrontUMATInterface.cxx
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
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MFrontUMATInterface.hxx"

namespace mfront{

  static void
  writeUMATArguments(std::ostream& out,
		     const MechanicalBehaviourDescription::BehaviourType& t,
		     const bool f)
  {
    using namespace std;
    if(f){
      out << "(umat::UMATReal *const STRESS," << endl
	  << " umat::UMATReal *const STATEV," << endl
	  << " umat::UMATReal *const DDSDDE," << endl
	  << " umat::UMATReal *const SSE," << endl
	  << " umat::UMATReal *const SPD," << endl
	  << " umat::UMATReal *const SCD," << endl
	  << " umat::UMATReal *const RPL," << endl
	  << " umat::UMATReal *const DDSDDT," << endl
	  << " umat::UMATReal *const DRPLDE," << endl
	  << " umat::UMATReal *const DRPLDT," << endl
	  << " const umat::UMATReal *const STRAN," << endl
	  << " const umat::UMATReal *const DSTRAN," << endl
	  << " const umat::UMATReal *const TIME," << endl
	  << " const umat::UMATReal *const DTIME," << endl
	  << " const umat::UMATReal *const TEMP," << endl
	  << " const umat::UMATReal *const DTEMP," << endl
	  << " const umat::UMATReal *const PREDEF," << endl
	  << " const umat::UMATReal *const DPRED," << endl
	  << " const char           *const CMNAME," << endl
	  << " const umat::UMATInt  *const NDI," << endl
	  << " const umat::UMATInt  *const NSHR," << endl
	  << " const umat::UMATInt  *const NTENS," << endl
	  << " const umat::UMATInt  *const NSTATV," << endl
	  << " const umat::UMATReal *const PROPS," << endl
	  << " const umat::UMATInt  *const NPROPS," << endl
	  << " const umat::UMATReal *const COORDS," << endl
	  << " const umat::UMATReal *const DROT," << endl
	  << " const umat::UMATReal *const PNEWDT," << endl
	  << " const umat::UMATReal *const CELENT," << endl
	  << " const umat::UMATReal *const DFGRD0," << endl
	  << " const umat::UMATReal *const DFGRD1," << endl
	  << " const umat::UMATInt  *const NOEL," << endl
	  << " const umat::UMATInt  *const NPT," << endl
	  << " const umat::UMATInt  *const LAYER," << endl
	  << " const umat::UMATInt  *const KSPT," << endl
	  << " const umat::UMATInt  *const KSTEP," << endl
	  << "       umat::UMATInt  *const KINC," << endl
	  << "const int size)";
    } else {
      out << "(umat::UMATReal *const STRESS," << endl
	  << " umat::UMATReal *const STATEV," << endl
	  << " umat::UMATReal *const DDSDDE," << endl
	  << " umat::UMATReal *const," << endl
	  << " umat::UMATReal *const," << endl
	  << " umat::UMATReal *const," << endl
	  << " umat::UMATReal *const," << endl
	  << " umat::UMATReal *const," << endl
	  << " umat::UMATReal *const," << endl
	  << " umat::UMATReal *const," << endl;
      if(t!=MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const umat::UMATReal *const STRAN," << endl
	    << " const umat::UMATReal *const DSTRAN," << endl;
      } else {
	out << " const umat::UMATReal *const," << endl
	    << " const umat::UMATReal *const," << endl;
      }
      out << " const umat::UMATReal *const," << endl
	  << " const umat::UMATReal *const DTIME," << endl
	  << " const umat::UMATReal *const TEMP," << endl
	  << " const umat::UMATReal *const DTEMP," << endl
	  << " const umat::UMATReal *const PREDEF," << endl
	  << " const umat::UMATReal *const DPRED," << endl
	  << " const char           *const," << endl
	  << " const umat::UMATInt  *const NDI," << endl
	  << " const umat::UMATInt  *const," << endl
	  << " const umat::UMATInt  *const NTENS," << endl
	  << " const umat::UMATInt  *const NSTATV," << endl
	  << " const umat::UMATReal *const PROPS," << endl
	  << " const umat::UMATInt  *const NPROPS," << endl
	  << " const umat::UMATReal *const," << endl
	  << " const umat::UMATReal *const DROT," << endl
	  << " const umat::UMATReal *const," << endl
	  << " const umat::UMATReal *const," << endl;
      if(t==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const umat::UMATReal *const F0," << endl
	    << " const umat::UMATReal *const F1," << endl;
      } else {
	out << " const umat::UMATReal *const," << endl
	    << " const umat::UMATReal *const," << endl;
      }
      out << " const umat::UMATInt  *const," << endl
	  << " const umat::UMATInt  *const," << endl
	  << " const umat::UMATInt  *const," << endl
	  << " const umat::UMATInt  *const," << endl
	  << " const umat::UMATInt  *const," << endl
	  << "       umat::UMATInt  *const KINC," << endl
	  << "const int)";
    }
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    using namespace std;
    out << "(umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const char           *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
        << " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATReal *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< " const umat::UMATInt  *const," << endl
	<< "       umat::UMATInt  *const,"
	<< "const int)";
  } // end of writeUMATArguments

  static int
  getUMATModellingHypothesisIndex(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    using namespace std;
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
      ostringstream msg;
      msg << "mfront::getUMATModellingHypothesisIndex : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return 0;
  } // end of getUMATModellingHypothesisIndex

  unsigned short
  MFrontUMATInterface::getStensorSize(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    using namespace std;
    using namespace tfel::material;
    switch(h){
    case ModellingHypothesis::TRIDIMENSIONAL:
      return 6u;
    case ModellingHypothesis::AXISYMMETRICAL:
    case ModellingHypothesis::PLANESTRAIN:
    case ModellingHypothesis::PLANESTRESS:
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      return 4u;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      return 3u;
    default:
      ostringstream msg;
      msg << "MFrontUMATInterface::getStensorSize : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return 0;
  } // end of MFrontUMATInterface::getStensorSize

  std::string
  MFrontUMATInterface::getName(void)
  {
    return "umat";
  }

  std::string
  MFrontUMATInterface::getLibraryName(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string lib;
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "libUmat"+mb.getMaterialName();
      } else {
	lib = "libUmatBehaviour";
      }
    } else {
      lib = "libUmat"+mb.getLibrary();
    }
    return lib;
  } // end of MFrontUMATInterface::getLibraryName

  std::string
  MFrontUMATInterface::getInterfaceName(void) const
  {
    return "Umat";
  } // end of MFrontUMATInterface::getInterfaceName

  std::string
  MFrontUMATInterface::getFunctionName(const std::string& name) const
  {
    return "umat"+makeLowerCase(name);
  } // end of MFrontUMATInterface::getLibraryName

  std::string
  MFrontUMATInterface::getBehaviourName(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();
    return name;
  } // end of MFrontUMATInterface::getBehaviourName

  std::string
  MFrontUMATInterface::getUmatFunctionName(const MechanicalBehaviourDescription& mb) const
  {
    return "umat"+makeLowerCase(this->getBehaviourName(mb));
  } // end of MFrontUMATInterface::getUmatFunctionName

  MFrontUMATInterface::MFrontUMATInterface()
  {
    this->reset();
  }

  void
  MFrontUMATInterface::reset(void)
  {
    MFrontUMATInterfaceBase::reset();
    this->finiteStrainStrategies.clear();
    this->useTimeSubStepping            = false;
    this->maximumSubStepping            = 0u;
    this->doSubSteppingOnInvalidResults = false;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontUMATInterface::treatKeyword(const std::string& key,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    using namespace tfel::utilities;
    if (key=="@UMATGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if(key=="@UMATUseTimeSubStepping"){
      this->useTimeSubStepping = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if (key=="@UMATMaximumSubStepping"){
      if(!this->useTimeSubStepping){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg += "time sub stepping is not enabled at this stage.\n";
	msg += "Use the @UMATUseTimeSubStepping directive before ";
	msg += "@UMATMaximumSubStepping";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      if(flux.fail()){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg+="failed to read maximum substepping value.\n";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("UmatInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@UMATDoSubSteppingOnInvalidResults"){
      if(!this->useTimeSubStepping){
	string msg("UmatInterface::treatKeyword (@UMATDoSubSteppingOnInvalidResults) : ");
	msg += "time sub stepping is not enabled at this stage.\n";
	msg += "Use the @UMATUseTimeSubStepping directive before ";
	msg += "@UMATMaximumSubStepping";
	throw(runtime_error(msg));
      }
      this->doSubSteppingOnInvalidResults = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if (key=="@UMATFiniteStrainStrategy"){
      if(!this->finiteStrainStrategies.empty()){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "at least one strategy has already been defined.\n";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      const string& fss = current->value;
      if(fss=="FiniteRotationSmallStrain"){
	this->finiteStrainStrategies.push_back(FINITEROTATIONSMALLSTRAIN);
      } else if(fss=="MieheApelLambrechtLogarithmicStrain"){
	this->finiteStrainStrategies.push_back(MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN);
      } else if(fss=="LogarithmicStrain1D"){
	this->finiteStrainStrategies.push_back(LOGARITHMICSTRAIN1D);
      } else {
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "unsupported strategy '"+fss+"'\n";
	msg += "The only supported strategies are "
	  "'FiniteRotationSmallStrain', 'MieheApelLambrechtLogarithmicStrain' "
	  "and 'LogarithmicStrain1D'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value!=";"){
	string msg("UmatInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@UMATFiniteStrainStrategies"){
      if(!this->finiteStrainStrategies.empty()){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	msg += "at least one strategy has already been defined.\n";
	throw(runtime_error(msg));
      }
      vector<string> fss;
      CxxTokenizer::readArray("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies)",
			      fss,current,end);
      CxxTokenizer::readSpecifiedToken("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies)",
				       ";",current,end);
      if(fss.empty()){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	msg += "no strategy defined.\n";
	throw(runtime_error(msg));
      }
      vector<string>::const_iterator pfss;
      for(pfss=fss.begin();pfss!=fss.end();++pfss){
	if(*pfss=="None"){
	  if(find(this->finiteStrainStrategies.begin(),
		  this->finiteStrainStrategies.end(),NONE)!=
	     this->finiteStrainStrategies.end()){
	    string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	    msg += "strategy 'None' multiply defined.\n";
	    throw(runtime_error(msg));
	  }
	  this->finiteStrainStrategies.push_back(NONE);
	} else if(*pfss=="FiniteRotationSmallStrain"){
	  if(find(this->finiteStrainStrategies.begin(),
		  this->finiteStrainStrategies.end(),FINITEROTATIONSMALLSTRAIN)!=
	     this->finiteStrainStrategies.end()){
	    string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	    msg += "strategy 'FiniteRotationSmallStrain' multiply defined.\n";
	    throw(runtime_error(msg));
	  }
	  this->finiteStrainStrategies.push_back(FINITEROTATIONSMALLSTRAIN);
	} else if(*pfss=="MieheApelLambrechtLogarithmicStrain"){
	  if(find(this->finiteStrainStrategies.begin(),
		  this->finiteStrainStrategies.end(),MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN)!=
	     this->finiteStrainStrategies.end()){
	    string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	    msg += "strategy 'MieheApelLambrechtLogarithmicStrain' multiply defined.\n";
	    throw(runtime_error(msg));
	  }
	  this->finiteStrainStrategies.push_back(MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN);
	} else if(*pfss=="LogarithmicStrain1D"){
	  if(find(this->finiteStrainStrategies.begin(),
		  this->finiteStrainStrategies.end(),LOGARITHMICSTRAIN1D)!=
	     this->finiteStrainStrategies.end()){
	    string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	    msg += "strategy 'LogarithmicStrain1D' multiply defined.\n";
	    throw(runtime_error(msg));
	  }
	  this->finiteStrainStrategies.push_back(LOGARITHMICSTRAIN1D);
	} else {
	  string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategies) : ");
	  msg += "unsupported strategy '"+*pfss+"'\n";
	  msg += "The only supported strategies are "
	    "'None', 'FiniteRotationSmallStrain', 'MieheApelLambrechtLogarithmicStrain' "
	    "and 'LogarithmicStrain1D'";
	  throw(runtime_error(msg));
	}
      }
      return make_pair(true,current);      
    }
    return make_pair(false,current);
  } // end of treatKeyword

  bool
  MFrontUMATInterface::usesGenericPlaneStressAlgorithm(const MechanicalBehaviourDescription& mb)
  {
    if(mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)){
      return false;
    }
    if(mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      return true;
    }
    return false;
  }

  std::string
  MFrontUMATInterface::treatScalar(const std::string& s)
  {
    using namespace std;
    string res("'");
    res += makeUpperCase(s.substr(0,4));
    res += "'";
    return res;
  }

  std::string
  MFrontUMATInterface::treatScalar(const std::string& s,
				   const unsigned short a)
  {
    using namespace std;
    ostringstream res;
    if(a<9){
      res << "'" << makeUpperCase(s.substr(0,3)) << a << "'";
    } else {
      res << "'" << makeUpperCase(s.substr(0,2)) << a << "'";
    }
    return res.str();
  }

  std::string
  MFrontUMATInterface::treatStensor(const Hypothesis h,
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
      msg << "MFrontUMATInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::string
  MFrontUMATInterface::treatStensor(const Hypothesis h,
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
      msg << "MFrontUMATInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::string
  MFrontUMATInterface::treatTensor(const Hypothesis h,
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
      msg << "MFrontUMATInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::string
  MFrontUMATInterface::treatTensor(const Hypothesis h,
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
      msg << "MFrontUMATInterface::treatStensor : "
	  << "unsupported hypothesis";
      if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " ('" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return res;
  }

  std::set<tfel::material::ModellingHypothesis::Hypothesis>
  MFrontUMATInterface::getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription& mb) const
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
    const set<Hypothesis>& bh = mb.getModellingHypotheses();
    for(const Hypothesis *ph = sh;ph!=sh+6u;++ph){
      if(bh.find(*ph)!=bh.end()){
	h.insert(*ph);
      }
    }
    if(h.empty()){
      string msg("MFrontUMATInterface::getModellingHypothesesToBeTreated : "
		 "no hypotheses selected. This means that the given beahviour "
		 "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
		 "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
		 "make sense to use the UMAT interface");
      throw(runtime_error(msg));
    }
    return h;
  } // end of MFrontUMATInterfaceModellingHypothesesToBeTreated

  void
  MFrontUMATInterface::endTreatment(const MechanicalBehaviourDescription& mb,
				     const MFrontFileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;

    // get the modelling hypotheses to be treated
    const set<Hypothesis>& h = this->getModellingHypothesesToBeTreated(mb);

    // some consistency checks
    if(mb.getSymmetryType()!=mb.getElasticSymmetryType()){
      string msg("MFrontUMATInterface::endTreatment : ");
      msg += "the type of the behaviour (isotropic or orthotropic) does not ";
      msg += "match the the type of its elastic behaviour.\n";
      msg += "This is not allowed here :\n";
      msg += "- an isotropic behaviour must have an isotropic elastic behaviour\n";
      msg += "- an orthotropic behaviour must have an orthotropic elastic behaviour";
      throw(runtime_error(msg));
    }
    if(this->useTimeSubStepping){
      if(this->maximumSubStepping==0u){
	string msg("MFrontUMATInterface::endTreatment : ");
	msg += "use of time sub stepping requested but MaximumSubStepping is zero.\n";
	msg += "Please use the @UMATMaximumSubStepping directive";
	throw(runtime_error(msg));
      }
    }

    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/UMAT");

    VariableDescriptionContainer::const_iterator p;    
    vector<FiniteStrainStrategy>::const_iterator pfss;
    
    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();

    string fileName("umat");
    fileName += name;
    fileName += ".hxx";

    ofstream out(("include/MFront/UMAT/"+fileName).c_str());
    if(!out){
      string msg("MFrontUMATInterface::endTreatment : ");
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
    out << "#include\"MFront/UMAT/UMAT.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/UMAT/UMATTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/UMAT/UMATOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);
    
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeUmatFortranFunctionDefine(out,name);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeUmatFortranFunctionDefine(out,name+"_frst");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFortranFunctionDefine(out,name);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeUmatFortranFunctionDefine(out,name+"_malls");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFortranFunctionDefine(out,name);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeUmatFortranFunctionDefine(out,name+"_log1D");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFortranFunctionDefine(out,name);
	    }
	  } else if(*pfss==NONE){
	    this->writeUmatFortranFunctionDefine(out,name+"_ss");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFortranFunctionDefine(out,name);
	    }
	  } else {
	    string msg("MFrontUMATInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeUmatFortranFunctionDefine(out,name);
	}
      }
    } else {
      this->writeUmatFortranFunctionDefine(out,name);
    }

    out << "#ifdef __cplusplus\n\n";

    out << "namespace umat{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(h)){
      this->writeUMATBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	this->writeUMATBehaviourTraits(out,mb,*ph);
      }
    }

    out << "} // end of namespace umat\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeSetParametersFunctionsDeclarations(out,name,mb);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_frst",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_malls",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_log1D",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	    }
	  } else if(*pfss==NONE){
	    this->writeSetParametersFunctionsDeclarations(out,name+"_ss",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsDeclarations(out,name,mb);
	    }
	  } else {
	    string msg("MFrontUMATInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeSetParametersFunctionsDeclarations(out,name,mb);
	}
      }
    } else {
      this->writeSetParametersFunctionsDeclarations(out,name,mb);
    }

    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeUmatFunctionDeclaration(out,name);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeUmatFunctionDeclaration(out,name+"_frst");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeUmatFunctionDeclaration(out,name+"_malls");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeUmatFunctionDeclaration(out,name+"_log1D");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFunctionDeclaration(out,name);
	    }
	  } else if(*pfss==NONE){
	    this->writeUmatFunctionDeclaration(out,name+"_ss");
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeUmatFunctionDeclaration(out,name);
	    }
	  } else {
	    string msg("MFrontUMATInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeUmatFunctionDeclaration(out,name);
	}
      }
    } else {
      this->writeUmatFunctionDeclaration(out,name);
    }

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName  = "umat";
    fileName += name;
    fileName += ".cxx";

    out.open(("src/"+fileName).c_str());
    if(!out){
      string msg("MFrontUMATInterface::endTreatment : ");
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
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "#include\"MFront/MFrontBehaviourProfiler.hxx\"\n\n";
    }
    if(this->generateMTestFile){
      if((mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	 (mb.getBehaviourType()!=MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
     	string msg("MFrontUMATInterface::treatKeyword : "
		   "unsupported behaviour type");
	throw(runtime_error(msg));
      }
      out << "#include\"MFront/UMAT/UMATGetModellingHypothesis.hxx\"\n";
    }
    if((find(this->finiteStrainStrategies.begin(),this->finiteStrainStrategies.end(),
	     FINITEROTATIONSMALLSTRAIN)!=this->finiteStrainStrategies.end())||
       (find(this->finiteStrainStrategies.begin(),this->finiteStrainStrategies.end(),
	     MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN)!=this->finiteStrainStrategies.end())){
      out << "#include\"MFront/UMAT/UMATFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/UMAT/UMATInterface.hxx\"\n\n";
    out << "#include\"MFront/UMAT/UMATStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/UMAT/umat" << name << ".hxx\"\n\n";
  
    out << "extern \"C\"{\n\n";

    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->generateUMATxxGeneralSymbols(out,name,mb,fd);
	if(!mb.areAllMechanicalDataSpecialised(h)){
	  const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	  this->generateUMATxxSymbols(out,name,uh,mb,fd);
	}
	for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	  if(mb.hasSpecialisedMechanicalData(*ph)){
	    this->generateUMATxxSymbols(out,name,*ph,mb,fd);
	  }
	}
	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	    << "_BehaviourType = 1u;\n\n";
	out << "MFRONT_SHAREDOBJ unsigned short umat"
	    << makeLowerCase(name) << "_Interface = 1u;\n\n";
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->generateUMATxxGeneralSymbols(out,name+"_frst",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_frst",uh,mb,fd);
	    }
	    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	      if(mb.hasSpecialisedMechanicalData(*ph)){
		this->generateUMATxxSymbols(out,name+"_frst",*ph,mb,fd);
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
	      for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
		if(mb.hasSpecialisedMechanicalData(*ph)){
		  this->generateUMATxxSymbols(out,name,*ph,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 2u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 2u;\n\n";
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	      this->generateUMATxxGeneralSymbols(out,name+"_malls",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_malls",uh,mb,fd);
	    }
	    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	      if(mb.hasSpecialisedMechanicalData(*ph)){
		this->generateUMATxxSymbols(out,name+"_malls",*ph,mb,fd);
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
	      for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
		if(mb.hasSpecialisedMechanicalData(*ph)){
		  this->generateUMATxxSymbols(out,name,*ph,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 2u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 2u;\n\n";
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	      this->generateUMATxxGeneralSymbols(out,name+"_log1D",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_log1D",uh,mb,fd);
	    }
	    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	      if(mb.hasSpecialisedMechanicalData(*ph)){
		this->generateUMATxxSymbols(out,name+"_log1D",*ph,mb,fd);
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
	      for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
		if(mb.hasSpecialisedMechanicalData(*ph)){
		  this->generateUMATxxSymbols(out,name,*ph,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 1u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 1u;\n\n";
	    }
	  } else if(*pfss==NONE){
	    this->generateUMATxxGeneralSymbols(out,name+"_ss",mb,fd);
	    if(!mb.areAllMechanicalDataSpecialised(h)){
	      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	      this->generateUMATxxSymbols(out,name+"_ss",uh,mb,fd);
	    }
	    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	      if(mb.hasSpecialisedMechanicalData(*ph)){
		this->generateUMATxxSymbols(out,name+"_ss",*ph,mb,fd);
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
	      for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
		if(mb.hasSpecialisedMechanicalData(*ph)){
		  this->generateUMATxxSymbols(out,name,*ph,mb,fd);
		}
	      }
	      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
		  << "_BehaviourType = 1u;\n\n";
	      out << "MFRONT_SHAREDOBJ unsigned short umat"
		  << makeLowerCase(name) << "_Interface = 1u;\n\n";
	    }
	  } else {
	    string msg("MFrontUMATInterface::endTreatment : "
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
	  for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	    if(mb.hasSpecialisedMechanicalData(*ph)){
	      this->generateUMATxxSymbols(out,name,*ph,mb,fd);
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
      for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
	if(mb.hasSpecialisedMechanicalData(*ph)){
	  this->generateUMATxxSymbols(out,name,*ph,mb,fd);
	}
      }
      if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){ 
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

    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeSetParametersFunctionsImplementations(out,name,mb);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeSetParametersFunctionsImplementations(out,name+"_frst",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeSetParametersFunctionsImplementations(out,name+"_malls",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeSetParametersFunctionsImplementations(out,name+"_log1D",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	    }
	  } else if(*pfss==NONE){
	    this->writeSetParametersFunctionsImplementations(out,name+"_ss",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeSetParametersFunctionsImplementations(out,name,mb);
	    }
	  } else {
	    string msg("MFrontUMATInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeSetParametersFunctionsImplementations(out,name,mb);
	}
      }
    } else {
      this->writeSetParametersFunctionsImplementations(out,name,mb);
    }

    out << "static void \numat"
	<< makeLowerCase(name) << "_base" 
	<< "(const umat::UMATInt *const NTENS, const umat::UMATReal *const DTIME,\n"
	<< "const umat::UMATReal *const DROT,  umat::UMATReal *const DDSDDE,\n"
	<< "const umat::UMATReal *const STRAN, const umat::UMATReal *const DSTRAN,\n"
	<< "const umat::UMATReal *const TEMP,  const umat::UMATReal *const DTEMP,\n"
	<< "const umat::UMATReal *const PROPS, const umat::UMATInt    *const NPROPS,\n"
	<< "const umat::UMATReal *const PREDEF,const umat::UMATReal *const DPRED,\n"
	<< "umat::UMATReal *const STATEV,const umat::UMATInt    *const NSTATV,\n"
	<< "umat::UMATReal *const STRESS,const umat::UMATInt    *const NDI,\n"
	<< "umat::UMATInt  *const KINC,\n"
	<< "const umat::StressFreeExpansionHandler& sfeh)\n";
    out << "{\n";
    out << "umat::UMATInterface<tfel::material::" << mb.getClassName()
	<< ">::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	<< "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC,sfeh);\n";
    out << "}\n\n";

    
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->finiteStrainStrategies.empty()){
	this->writeStandardUmatFunction(out,name,"",mb);
      } else {
	for(pfss=this->finiteStrainStrategies.begin();pfss!=this->finiteStrainStrategies.end();++pfss){
	  if(*pfss==FINITEROTATIONSMALLSTRAIN){
	    this->writeFiniteRotationSmallStrainUmatFunction(out,name,"frst",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeFiniteRotationSmallStrainUmatFunction(out,name,"",mb);
	    }
	  } else if(*pfss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeMieheApelLambrechtLogarithmicStrainUmatFunction(out,name,"malls",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeMieheApelLambrechtLogarithmicStrainUmatFunction(out,name,"",mb);
	    }
	  } else if(*pfss==LOGARITHMICSTRAIN1D){
	    this->writeLogarithmicStrain1DUmatFunction(out,name,"log1D",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeLogarithmicStrain1DUmatFunction(out,name,"",mb);
	    }
	  } else if(*pfss==NONE){
	    this->writeStandardUmatFunction(out,name,"ss",mb);
	    if(this->finiteStrainStrategies.size()==1u){
	      this->writeStandardUmatFunction(out,name,"",mb);
	    }
	  } else {
	    string msg("MFrontUMATInterface::endTreatment : "
		       "internal error, unsupported finite strain strategy");
	    throw(runtime_error(msg));
	  }
	}
	if((this->finiteStrainStrategies.size()!=1u)&&
	   (find(this->finiteStrainStrategies.begin(),
		 this->finiteStrainStrategies.end(),NONE)!=this->finiteStrainStrategies.end())){
	  this->writeStandardUmatFunction(out,name,"",mb);
	}
      }
    } else {
      this->writeStandardUmatFunction(out,name,"",mb);
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      this->generateGibianeDeclaration(mb,fd);
    }
  } // end of MFrontUMATInterface::endTreatment

  void
  MFrontUMATInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "mg.setModellingHypothesis(umat::getModellingHypothesis(*NDI));\n";
  }
  
  void 
  MFrontUMATInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
						      const MechanicalBehaviourDescription&) const
  {
    using namespace std;
    out << "#include\"MFront/UMAT/UMAT.hxx\"" << endl << endl;
  } // end of MFrontUMATInterface::writeInterfaceSpecificIncludes

  MFrontUMATInterface::~MFrontUMATInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getGlobalIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontUMATInterface::getLibraryName(mb);
    incs[lib].push_back("`tfel-config --includes`");
#ifdef CASTEM_CPPFLAGS
    incs[lib].push_back(CASTEM_CPPFLAGS);
#endif /* CASTEM_CPPFLAGS */
#ifndef LOCAL_CASTEM_HEADER
#ifdef CASTEM_ROOT
    char * castem_root = ::getenv("CASTEM_ROOT");
    if(castem_root!=0){
      incs[lib].push_back("-I"+string(castem_root)+"/include");
      incs[lib].push_back("-I"+string(castem_root)+"/include/c");
    } else {
      incs[lib].push_back("-I"+string(CASTEM_ROOT)+"/include");
      incs[lib].push_back("-I"+string(CASTEM_ROOT)+"/include/c");
    }
#else /* CASTEM_ROOT */
    if(castem_root!=0){
      incs[lib].push_back("-I"+string(castem_root)+"/include");
      incs[lib].push_back("-I"+string(castem_root)+"/include/c");
    }
#endif /* CASTEM_ROOT */
#endif /* LOCAL_CASTEM_HEADER_FILE */
    return incs;
  } // end of MFrontUMATInterface::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getGeneratedSources(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > src;
    const string lib = MFrontUMATInterface::getLibraryName(mb);
    if(!mb.getLibrary().empty()){
      src[lib].push_back("umat"+mb.getLibrary()+mb.getClassName()+".cxx");
    } else {
      src[lib].push_back("umat"+mb.getClassName()+".cxx");
    }
    return src;
  } // end of MFrontUMATInterface::getGeneratedSources

  std::vector<std::string>
  MFrontUMATInterface::getGeneratedIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    vector<string> incs;
    if(!mb.getLibrary().empty()){
      incs.push_back("MFront/UMAT/umat"+mb.getLibrary()+mb.getClassName()+".hxx");
    } else {
      incs.push_back("MFront/UMAT/umat"+mb.getClassName()+".hxx");
    }
    return incs;
  } // end of MFrontUMATInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getLibrariesDependencies(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontUMATInterface::getLibraryName(mb);
    deps[lib].push_back("-lUMATInterface");
#ifdef HAVE_CXX11
      deps[lib].push_back("`tfel-config --libs --material --mfront-profiling`");
#else 
      deps[lib].push_back("`tfel-config --libs --material`");
#endif
    return deps;
  } // end of MFrontUMATInterface::getLibrariesDependencies()

  std::pair<std::vector<MFrontUMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  MFrontUMATInterface::buildMaterialPropertiesList(const MechanicalBehaviourDescription& mb,
						   const Hypothesis h) const
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      const set<Hypothesis> ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for(set<Hypothesis>::const_iterator ph=ah.begin();ph!=ah.end();++ph){
	if(!mb.hasSpecialisedMechanicalData(*ph)){
	  uh.insert(*ph);
	}
      }
      if(uh.empty()){
	string msg("MFrontUMATInterface::endTreatment : ");
	msg += "internal error : the mechanical behaviour says that not "
	  "all handled mechanical data are specialised, but we found none.";
	throw(runtime_error(msg));
      }
      // material properties for all the selected hypothesis
      vector<pair<vector<UMATMaterialProperty>,
		  SupportedTypes::TypeSize> > mpositions;
      for(set<Hypothesis>::const_iterator ph=uh.begin();ph!=uh.end();++ph){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,*ph));
      }
      set<Hypothesis>::const_iterator ph=uh.begin();
      vector<pair<vector<UMATMaterialProperty>,
		  SupportedTypes::TypeSize> >::const_iterator pum = mpositions.begin();
      const pair<vector<UMATMaterialProperty>,
		 SupportedTypes::TypeSize>& mfirst = *pum;
      ++ph;
      ++pum;
      for(;ph!=uh.end();++ph,++pum){
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const VariableDescriptionContainer& mps = d.getMaterialProperties();
	for(VariableDescriptionContainer::const_iterator pm=mps.begin();pm!=mps.end();++pm){
	  const UMATMaterialProperty& mp1 = findUMATMaterialProperty(mfirst.first,
								     mb.getExternalName(h,pm->name));
	  const UMATMaterialProperty& mp2 = findUMATMaterialProperty(pum->first,
								     mb.getExternalName(h,pm->name));
	  SupportedTypes::TypeSize o1 = mp1.offset;
	  o1+=pum->second;
	  SupportedTypes::TypeSize o2 = mp2.offset;
	  o2+=mfirst.second;
	  if(o1!=o2){
	    string msg("MFrontUMATInterface::buildMaterialPropertiesList : "
		       "incompatible offset for material property '"+pm->name+
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
    vector<UMATMaterialProperty>& mprops = res.first;
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
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
	  string msg("MFrontUMATInterface::buildMaterialPropertiesList : ");
	  msg += "unsupported modelling hypothesis.";
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MFrontUMATInterface::buildMaterialPropertiesList : ");
	msg += "unsupported behaviour symmetry type.\n";
	msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
	throw(runtime_error(msg));
      }
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	//! those are not the Cast3M conventions, switch is performed
	//! below the UMATInterface class
	this->appendToMaterialPropertiesList(mprops,"real","NormalStiffness","kn",false);
	this->appendToMaterialPropertiesList(mprops,"real","TangentialStiffness","kt",false);
	this->appendToMaterialPropertiesList(mprops,"massdensity","MassDensity","rho",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion",
					     "NormalThermalExpansion","ALPN",false);
      } else {
	string msg("MFrontUMATInterface::buildMaterialPropertiesList : ");
	msg += "unsupported symmetry type for cohesive zone modes.\n";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MFrontUMATInterface::buildMaterialPropertiesList : ");
      msg += "unsupported behaviour type.\n";
      throw(runtime_error(msg));
    }
    if(!mprops.empty()){
      const UMATMaterialProperty& m = mprops.back();
      res.second  = m.offset;
      res.second += this->getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of MFrontUMATInterface::buildMaterialPropertiesList

  void
  MFrontUMATInterface::writeUMATxxBehaviourTypeSymbols(std::ostream&,
						       const std::string&,
						       const MechanicalBehaviourDescription&) const
  {} // end of MFrontUMATInterface::writeUMATxxBehaviourTypeSymbols

  void
  MFrontUMATInterface::writeUMATxxAdditionalSymbols(std::ostream&,
						    const std::string&,
						    const Hypothesis,
						    const MechanicalBehaviourDescription&,
						    const MFrontFileDescription&) const
  {} // end of MFrontUMATInterface::writeUMATxxAdditionalSymbols

  void
  MFrontUMATInterface::writeUMATxxSpecificSymbols(std::ostream& out,
						  const std::string& name,
						  const MechanicalBehaviourDescription& mb,
						  const MFrontFileDescription&) const
  {
    if(MFrontUMATInterface::usesGenericPlaneStressAlgorithm(mb)){
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	  << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";    
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	  << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";    
    }
  }

  void
  MFrontUMATInterface::writeUmatFortranFunctionDefine(std::ostream& out,
						      const std::string& name) const
  {
    out << "#define umat" << makeUpperCase(name) <<"_F77" << " F77_FUNC(umat"
	<< makeLowerCase(name) << ",UMAT"
	<< makeUpperCase(name) << ")\n\n";
  } // end of MFrontUMATInterface::writeUmatFortranFunctionDefine

  void
  MFrontUMATInterface::writeUmatFunctionDeclaration(std::ostream& out,
						    const std::string& name) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\numat"
    	<< makeLowerCase(name);
    writeUMATArguments(out);
    out << ";" << endl << endl;
    out << "MFRONT_SHAREDOBJ void\n" 
	<< "umat" << makeUpperCase(name) <<"_F77";
    writeUMATArguments(out);
    out << ";" << endl << endl;
  } // end of MFrontUMATInterface::writeUmatFunctionDeclaration

  std::pair<bool,SupportedTypes::TypeSize>
  MFrontUMATInterface::checkIfAxialStrainIsDefinedAndGetItsOffset(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(ModellingHypothesis::PLANESTRESS);
    const VariableDescriptionContainer& sv  = d.getPersistentVariables();
    SupportedTypes::TypeSize o;
    for(VariableDescriptionContainer::const_iterator p=sv.begin();p!=sv.end();++p){
      if(d.getExternalName(p->name)=="AxialStrain"){
	return make_pair(true,o);
      }
      o += this->getTypeSize(p->type,p->arraySize);
    }
    return make_pair(false,o);
  }

  void
  MFrontUMATInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall(std::ostream& out,
									  const MechanicalBehaviourDescription& mb,
									  const std::string& c,
									  const std::string& c2) const
  {
    using namespace std;
    if((mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS))||
       (this->usesGenericPlaneStressAlgorithm(mb))){
      out << "if(*NDI==" << getUMATModellingHypothesisIndex(ModellingHypothesis::PLANESTRESS) << "){\n";
      if(mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)){
	pair<bool,SupportedTypes::TypeSize> v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
	if(v.first){
	  out << "const UMATReal ezz = STATEV[" << v.second.getValueForDimension(2) << "];\n"
	      << "const UMATReal Fzz = " << c2 << ";" << endl;
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
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(ModellingHypothesis::GENERALISEDPLANESTRAIN);
	SupportedTypes::TypeSize s =  this->getTotalSize(d.getPersistentVariables());
	if(s.getValueForDimension(2)==0){
	  out << "const UMATReal ezz = STATEV[0];\n";
	} else {
	  out << "if(*NSTATV!=" << s.getValueForDimension(2)+1 << "){\n"
	      << "std::cerr << \"invalid number of internal state variables\" << std::endl;\n"
	      << "}\n";
	  out << "const UMATReal ezz = STATEV[" << s.getValueForDimension(2) << "];\n";
	}
	out << "const UMATReal Fzz = " << c2 << ";" << endl;
	out << c << ",Fzz);\n";
      }
      out << "} else {\n";
      out << c << ",0);\n";
      out << "}\n";
    } else {
      out << c << ",0);\n";
    }
  } // end of MFrontUMATInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall

  void
  MFrontUMATInterface::writeFiniteRotationSmallStrainUmatFunction(std::ostream& out,
								  const std::string& name,
								  const std::string& suffix,
								  const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string fname = name;
    if(!suffix.empty()){
      fname += "_"+suffix;
    }
    const string umatFortranFunctionName = "umat"+makeUpperCase(fname)+"_F77";
    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontUMATInterface::writeFiniteRotationSmallStrainUmatFunction : "
		 "finite strain strategies shall be used with small strain behaviours");
      throw(runtime_error(msg));
    }
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\numat"
	<< makeLowerCase(fname);
    writeUMATArguments(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,false);
    out << endl;
    out << "{\n";
    out << "using namespace umat;\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using namespace mfront::MFrontBehaviourProfiler;\n";
      out << "using namespace tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "MFrontBehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "// computing the Green Lagrange strains\n";
    out << "UMATReal eto[6];\n";
    out << "UMATReal deto[6];\n";
    out << "UMATInt  i;\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "{\n"
	  << "MFrontBehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "UMATFiniteStrain::computeGreenLagrangeStrain(eto,F0,*NTENS,*NDI);\n";
    out << "UMATFiniteStrain::computeGreenLagrangeStrain(deto,F1,*NTENS,*NDI);\n";
    string c1 = "UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,F0,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c1,"std::sqrt(1+2*ezz)");
    out << "for(i=0;i!=*NTENS;++i){\n";
    out << "deto[i] -= eto[i];\n";
    out << "}\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
	<< "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "STRESS,NDI,KINC,\n"
	<< "umat::UMATStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "MFrontBehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    string c2 = "UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c2,"std::sqrt(1+2*ezz)");
    out << "}\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n" << umatFortranFunctionName;
    writeUMATArguments(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,true);
    out << endl;
    out << "{\n";
    out << "umat" << makeLowerCase(fname)
	<< "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,"
	<< "DRPLDT,STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,"
	<< "CMNAME,NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,"
	<< "PNEWDT,CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,"
	<< "KINC,size);" << endl;
    out << "}" << endl << endl;
  }
  
  void
  MFrontUMATInterface::writeLogarithmicStrain1DUmatFunction(std::ostream& out,
							    const std::string& name,
							    const std::string& suffix,
							    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string fname = name;
    if(!suffix.empty()){
      fname += "_"+suffix;
    }
    const string umatFortranFunctionName = "umat"+makeUpperCase(fname)+"_F77";
    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontUMATInterface::writeLogarithmicStrain1DUmatFunction : "
    		 "finite strain strategies shall be used with small strain behaviours");
      throw(runtime_error(msg));
    }
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\numat"
    	<< makeLowerCase(fname);
    writeUMATArguments(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,false);
    out << endl;
    out << "{\n";
    out << "using namespace umat;\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
    	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "// computing the logarithmic strain\n";
    out << "UMATReal eto[3];\n";
    out << "UMATReal deto[3];\n";
    out << "UMATReal s[3];\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "{\n"
    	  << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
    	  << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "if(*NDI!=14){" << endl
	<< "*KINC=-7;" << endl
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
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
    	<< "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
    	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
    	<< "s,NDI,KINC,\n"
    	<< "umat::UMATStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
    	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "STRESS[0]=s[0]/(1+*STRAN+*DSTRAN);\n";
    out << "STRESS[1]=s[1]/(1+*(STRAN+1)+*(DSTRAN+1));\n";
    out << "STRESS[2]=s[2]/(1+*(STRAN+2)+*(DSTRAN+2));\n";
    out << "}\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
       			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n" << umatFortranFunctionName;
    writeUMATArguments(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,true);
    out << endl;
    out << "{\n";
    out << "umat" << makeLowerCase(fname)
    	<< "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,"
    	<< "DRPLDT,STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,"
    	<< "CMNAME,NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,"
    	<< "PNEWDT,CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,"
    	<< "KINC,size);" << endl;
    out << "}\n\n";
  }

  void
  MFrontUMATInterface::writeMieheApelLambrechtLogarithmicStrainUmatFunction(std::ostream& out,
									    const std::string& name,
									    const std::string& suffix,
									    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string fname = name;
    if(!suffix.empty()){
      fname += "_"+suffix;
    }
    const string umatFortranFunctionName = "umat"+makeUpperCase(fname)+"_F77";
    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontUMATInterface::writeMieheApelLambrechtLogarithmicStrainUmatFunction : "
		 "finite strain strategies shall be used with small strain behaviours");
      throw(runtime_error(msg));
    }
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\numat"
	<< makeLowerCase(fname);
    writeUMATArguments(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,false);
    out << endl;
    out << "{\n";
    out << "using namespace umat;\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using mfront::MFrontBehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "MFrontBehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "// computing the logarithmic strain\n";
    out << "UMATReal eto[6];\n";
    out << "UMATReal deto[6];\n";
    out << "UMATReal P0[36];\n";
    out << "UMATReal P1[36];\n";
    out << "UMATReal s[6];\n";
    out << "UMATInt  i;\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "{\n"
	  << "MFrontBehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "UMATFiniteStrain::computeLogarithmicStrainAndDerivative(P0,eto ,F0,*NTENS,*NDI);\n";
    out << "UMATFiniteStrain::computeLogarithmicStrainAndDerivative(P1,deto,F1,*NTENS,*NDI);\n";
    string c1 = "UMATFiniteStrain::computeDualStressOfLogarithmicStrainFromCauchyStress(s,STRESS,P0,F0,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c1,"std::exp(ezz)");
    out << "for(i=0;i!=*NTENS;++i){\n";
    out << "deto[i] -= eto[i];\n";
    out << "}\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
	<< "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "s,NDI,KINC,\n"
	<< "umat::UMATStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "MFrontBehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    string c2 = "UMATFiniteStrain::computeCauchyStressFromDualStressOfLogarithmicStrain(STRESS,s,P1,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out,mb,c2,"std::exp(ezz)");
    out << "}\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
       			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n" << umatFortranFunctionName;
    writeUMATArguments(out,MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,true);
    out << endl;
    out << "{\n";
    out << "umat" << makeLowerCase(fname)
	<< "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,"
	<< "DRPLDT,STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,"
	<< "CMNAME,NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,"
	<< "PNEWDT,CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,"
	<< "KINC,size);" << endl;
    out << "}\n\n";
  }

  void
  MFrontUMATInterface::writeStandardUmatFunction(std::ostream& out,
						 const std::string& name,
						 const std::string& suffix,
						 const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string fname = name;
    if(!suffix.empty()){
      fname += "_"+suffix;
    }
    const string umatFortranFunctionName = "umat"+makeUpperCase(fname)+"_F77";
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\numat"
	<< makeLowerCase(fname);
    writeUMATArguments(out,mb.getBehaviourType(),false);
    out << endl;
    out << "{\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using namespace mfront::MFrontBehaviourProfiler;\n";
      out << "using namespace tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "MFrontBehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "umat" << makeLowerCase(name)
	  << "_base(NTENS, DTIME,DROT,DDSDDE,F0,F1,TEMP,DTEMP,\n"
	  << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	  << "STRESS,NDI,KINC,\n"
	  << "umat::UMATStandardSmallStrainStressFreeExpansionHandler);\n";
    } else {
      out << "umat" << makeLowerCase(name)
	  << "_base(NTENS, DTIME,DROT,DDSDDE,STRAN,DSTRAN,TEMP,DTEMP,\n"
	  << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	  << "STRESS,NDI,KINC,\n"
	  << "umat::UMATStandardSmallStrainStressFreeExpansionHandler);\n";
    }
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,mb.getBehaviourType(),
       			       name,suffix,mb);
      out << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n" << umatFortranFunctionName;
    writeUMATArguments(out,mb.getBehaviourType(),true);
    out << endl;
    out << "{\n";
    out << "umat" << makeLowerCase(fname)
	<< "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,"
	<< "DRPLDT,STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,"
	<< "CMNAME,NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,"
	<< "PNEWDT,CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,"
	<< "KINC,size);" << endl;
    out << "}\n\n";
  }

  void
  MFrontUMATInterface::writeVariableDescriptionContainerToGibiane(std::ostream& out,
								  const Hypothesis h,
								  const VariableDescriptionContainer& v) const
  {
    using namespace std;
    for(VariableDescriptionContainer::const_iterator p=v.begin();p!=v.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      string tmp;
      tmp += ' ';
      if(flag==SupportedTypes::Scalar){
	if(p->arraySize==1){
	  tmp += treatScalar(p->name);
	} else {
	  for(unsigned short j=0;j!=p->arraySize;){
	    tmp += treatScalar(p->name,j);
	    if(++j!=p->arraySize){
	      tmp += ' ';
	    }
	  }
	}
      } else if(flag==SupportedTypes::Stensor){
	if(p->arraySize==1){
	  tmp += treatStensor(h,p->name);
	} else {
	  for(unsigned short j=0;j!=p->arraySize;){
	    tmp += treatStensor(h,p->name,j);
	    if(++j!=p->arraySize){
	      tmp += ' ';
	    }
	  }
	}
      } else if(flag==SupportedTypes::Tensor){
	if(p->arraySize==1){
	  tmp += treatTensor(h,p->name);
	} else {
	  for(unsigned short j=0;j!=p->arraySize;){
	    tmp += treatTensor(h,p->name,j);
	    if(++j!=p->arraySize){
	      tmp += ' ';
	    }
	  }
	}
      } else {
	string msg(" MFrontUMATInterface::writeVariableDescriptionContainerToGibiane : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      out << tmp;
    }
  }

  void
  MFrontUMATInterface::writeGibianeInstruction(std::ostream& out,
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
  } // end of MFrontUMATInterface::writeGibianeInstruction

  void
  MFrontUMATInterface::generateGibianeDeclaration(const MechanicalBehaviourDescription& mb,
						  const MFrontFileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    const string name((!mb.getLibrary().empty())?mb.getLibrary()+mb.getClassName():mb.getClassName());
    const string fileName("castem/"+name+".dgibi");
    // opening output file
    systemCall::mkdir("castem");
    ofstream out;
    out.open(fileName.c_str());
    if(!out){
      string msg("MFrontUMATInterface::generateGibianeDeclaration : ");
      msg += "could not open file ";
	msg += fileName;
	throw(runtime_error(msg));
    }
    // header
    out << "*\n";
    out << "* \\file   "  << fd.fileName << endl;
    out << "* \\brief  example of how to use the " << mb.getClassName() << " behaviour law\n"
	<< "* in the Cast3M finite element solver\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*\n\n";
    out << "* Note for Cast3M pleiades users, you may want to use\n"
	<< "* the syntaxe based on the LIB_LOI keyword. In this case,\n"
	<< "* you may want to use the following declaration : \n\n";
    out << "cmp = 'TABLE';\n";
    out << "cmp. 'LIB_LOI' = '" << this->getLibraryName(mb) << "';\n";
    out << "cmp. 'FCT_LOI' = '" << this->getUmatFunctionName(mb) << "';\n";
    out << "*\n\n";
    // specific declaration
    string nonlin;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      nonlin = "'NON_LINEAIRE' 'UTILISATEUR'";
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      nonlin = "'NON_LINEAIRE' 'UTILISATEUR' 'EPSILON' 'UTILISATEUR'";
    } else {
      string msg("MFrontUMATInterface::generateGibianeDeclaration : ");
      msg += "internal error, unsupported behaviour type";
      throw(runtime_error(msg));
    }
    // loop over hypothesis
    const set<Hypothesis> h = this->getModellingHypothesesToBeTreated(mb);
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      
      const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(*ph);
      const VariableDescriptionContainer& persistentVarsHolder     = d.getPersistentVariables();
      const VariableDescriptionContainer& externalStateVarsHolder  = d.getExternalStateVariables();
      pair<vector<UMATMaterialProperty>,
	   SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,*ph);
      string tmp;
      VariableDescriptionContainer::const_iterator p;
      out << "** " << ModellingHypothesis::toString(*ph) << " example\n\n";
      ostringstream mcoel;
      mcoel << "coel = 'MOTS' ";
      for(vector<UMATMaterialProperty>::const_iterator pm=mprops.first.begin();
	  pm!=mprops.first.end();){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(pm->type);
	if(flag!=SupportedTypes::Scalar){
	  string msg("MFrontUMATInterface::generateGibianeDeclaration : ");
	  msg += "material properties shall be scalars";
	  throw(runtime_error(msg));
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
      ostringstream mstatev;
      mstatev << "statev = 'MOTS' ";
      this->writeVariableDescriptionContainerToGibiane(mstatev,*ph,persistentVarsHolder);
      mstatev << ";";
      writeGibianeInstruction(out,mstatev.str());
      out << endl;
      ostringstream mparam;
      mparam << "params = 'MOTS' 'T'";
      if(!externalStateVarsHolder.empty()){
	mparam << " ";
	this->writeVariableDescriptionContainerToGibiane(mparam,*ph,externalStateVarsHolder);
      }
      mparam << ";";
      writeGibianeInstruction(out,mparam.str());
      out << endl;
      ostringstream mmod;
      mmod << "MO = 'MODELISER' v 'MECANIQUE' 'ELASTIQUE' ";
      mmod << nonlin << " 'NUME_LOI' 1 ";
      mmod << "'C_MATERIAU' coel ";
      mmod << "'C_VARINTER' statev ";
      mmod << "'PARA_LOI'   params 'CONS' M;";
      writeGibianeInstruction(out,mmod.str());
      out << endl;
      ostringstream mi;
      mi << "MA = 'MATERIAU' MO";
      for(vector<UMATMaterialProperty>::const_iterator pm=mprops.first.begin();
	  pm!=mprops.first.end();){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(pm->type);
	if(flag!=SupportedTypes::Scalar){
	  string msg("MFrontUMATInterface::generateGibianeDeclaration : ");
	  msg += "material properties shall be scalars";
	  throw(runtime_error(msg));
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
	if((*ph==ModellingHypothesis::PLANESTRESS)||
	   (*ph==ModellingHypothesis::AXISYMMETRICAL)||
	   (*ph==ModellingHypothesis::PLANESTRAIN)||
	   (*ph==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  mi << " 'DIRECTION' (1 0) 'PARALLELE'";
	} else if(*ph==ModellingHypothesis::TRIDIMENSIONAL){	    
	  mi << " 'DIRECTION' (1 0 0) (0 0 1) 'PARALLELE'";
	}
      }
      mi << ";";
      writeGibianeInstruction(out,mi.str());
      out << endl;
    }
    out.close();
  } // end of MFrontUMATInterface::generateGibianeDeclaration

  void
  MFrontUMATInterface::writeUMATBehaviourTraits(std::ostream& out,
						const MechanicalBehaviourDescription& mb,
						const Hypothesis h) const
  {
    using namespace std;
    pair<SupportedTypes::TypeSize,
	 SupportedTypes::TypeSize> mvs = mb.getMainVariablesSize();
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,h);
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(mb.useQt()){
	out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
      } else {
	out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
      }
      out << "struct UMATTraits<tfel::material::" << mb.getClassName() << "<H,Type,";
    } else {
      if(mb.useQt()){
	out << "template<typename Type,bool use_qt>\n";
      } else {
	out << "template<typename Type>\n";
      }
      out << "struct UMATTraits<tfel::material::" << mb.getClassName() 
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
      out << "static const ModellingHypothesis::Hypothesis H = " 
	  << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
	  << ";" << endl;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static const UMATBehaviourType btype  = SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static const UMATBehaviourType btype  = FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
      out << "static const UMATBehaviourType btype  = COHESIVEZONEMODEL;\n";
    } else {
      string msg("MFrontUMATInterface::writeUMATBehaviourTraits : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "// space dimension\n";
    out << "static const unsigned short N           = ModellingHypothesisToSpaceDimension::value;\n";
    out << "// tiny vector size\n";
    out << "static const unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static const unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static const unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array (STRAN)\n";
    out << "static const unsigned short DrivingVariableSize = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static const unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
    out << "static const bool useTimeSubStepping = ";
    if(this->useTimeSubStepping){
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static const bool doSubSteppingOnInvalidResults = ";
    if(this->doSubSteppingOnInvalidResults){
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static const unsigned short maximumSubStepping = ";
    if(this->useTimeSubStepping){
      out << this->maximumSubStepping << ";\n";
    } else {
      out << "0u;\n";
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      out << "static const bool requiresStiffnessTensor = true;\n";
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresUnAlteredStiffnessTensor,false)){
	out << "static const bool requiresUnAlteredStiffnessTensor = true;\n";
      } else {
	out << "static const bool requiresUnAlteredStiffnessTensor = false;\n";
      }
    } else {
      out << "static const bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static const bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    // computing material properties size
    SupportedTypes::TypeSize msize;
    if(!mprops.first.empty()){
      const UMATMaterialProperty& m = mprops.first.back();
      msize  = m.offset;
      msize += this->getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static const unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static const unsigned short propertiesOffset = 4u;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
	// something needs to be done here
	out << "static const unsigned short propertiesOffset = UMATOrthotropicOffset<umat::SMALLSTRAINSTANDARDBEHAVIOUR,N>::value;\n";
      } else {
	string msg("MFrontUMATInterface::writeUMATBehaviourTraits : "
		   "unsupported behaviour type");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MFrontUMATInterface::writeUMATBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static const UMATSymmetryType stype = umat::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const UMATSymmetryType stype = umat::ORTHOTROPIC;\n";
    } else {
      string msg("MFrontUMATInterface::writeUMATBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class UMATTraits\n\n";
  } // end of MFrontUMATInterface::writeUMATBehaviourTraits

  std::map<MFrontUMATInterfaceBase::Hypothesis,std::string>
  MFrontUMATInterface::gatherModellingHypothesesAndTests(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    typedef map<Hypothesis,string>::value_type MVType;
    map<Hypothesis,string> res;
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      set<Hypothesis> h = this->getModellingHypothesesToBeTreated(mb);
      for(set<Hypothesis>::const_iterator p=h.begin();p!=h.end();++p){
	res.insert(MVType(*p,this->getModellingHypothesisTest(*p)));
      }
      return res;
    }
    return MFrontUMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  } // end of MFrontUMATInterface::gatherModellingHypothesesAndTests

  std::string
  MFrontUMATInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    using namespace std;
    ostringstream test;
    test << "*NDI==" << getUMATModellingHypothesisIndex(h);
    return test.str();
  }

} // end of namespace mfront
