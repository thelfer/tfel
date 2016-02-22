/*!
 * \file   mfront/src/BehaviourDSLCommon.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<iterator>
#include<vector>
#include<sstream>
#include<utility>
#include<stdexcept>
#include<limits>
#include<cctype>
#include<cmath>

#include"TFEL/System/System.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/PedanticMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/SearchFile.hxx"
#include"MFront/AbstractBehaviourInterface.hxx"
#include"MFront/MFrontMaterialPropertyInterface.hxx"
#include"MFront/PerformanceProfiling.hxx"
#include"MFront/BehaviourInterfaceFactory.hxx"
#include"MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx"
#include"MFront/AbstractBehaviourBrick.hxx"
#include"MFront/AbstractBehaviourBrickFactory.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/BehaviourDSLCommon.hxx"
#include"MFront/BehaviourBrick/Requirement.hxx"
#include"MFront/BehaviourBrick/RequirementManager.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99)
#include<sstream>
namespace std{
  template<typename T>
  std::string to_string(const T& v){
    std::ostringstream s;
    s << v;
    return s.str();
  }
}
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront{

  BehaviourDSLCommon::CodeBlockOptions::CodeBlockOptions()
    : p(BehaviourData::BODY),
      m(BehaviourData::CREATE)
  {
    this->hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
  } // end of BehaviourDSLCommon::CodeBlockOptions::CodeBlockOptions

  BehaviourDSLCommon::CodeBlockOptions::~CodeBlockOptions()
  {} // end of BehaviourDSLCommon::CodeBlockOptions::~CodeBlockOptions

  const BehaviourDescription&
  BehaviourDSLCommon::getBehaviourDescription(void) const{
    return this->mb;
  } // end of BehaviourDSLCommon::getBehaviourDescription

  std::string
  BehaviourDSLCommon::getClassName(void) const
  {
    return this->mb.getClassName();
  } // end of BehaviourDSLCommon::getClassName

  void
  BehaviourDSLCommon::addMaterialLaw(const std::string& m)
  {
    this->mb.addMaterialLaw(m);
  } // end of BehaviourDSLCommon::addMaterialLaw

  void
  BehaviourDSLCommon::appendToIncludes(const std::string& c)
  {
    this->mb.appendToIncludes(c);
  } // end of BehaviourDSLCommon::appendToIncludes

  void
  BehaviourDSLCommon::appendToMembers(const std::string& c)
  {
    this->mb.appendToMembers(c);
  } // end of BehaviourDSLCommon::appendToMembers

  void
  BehaviourDSLCommon::appendToPrivateCode(const std::string& c)
  {
    this->mb.appendToPrivateCode(c);
  } // end of BehaviourDSLCommon::appendToPrivateCode

  void
  BehaviourDSLCommon::appendToSources(const std::string& c)
  {
    this->mb.appendToSources(c);
  } // end of BehaviourDSLCommon::appendToSources

  void
  BehaviourDSLCommon::appendToHypothesesList(std::set<Hypothesis>& h,
					     const std::string& v) const
  {
    using namespace std;
    if(v==".+"){
      const auto& ash = ModellingHypothesis::getModellingHypotheses();
      for(const auto & elem : ash){
	this->appendToHypothesesList(h,ModellingHypothesis::toString(elem));
      }
    } else {
      const auto nh = ModellingHypothesis::fromString(v);
      if(!this->isModellingHypothesisSupported(nh)){
	this->throwRuntimeError("BehaviourDSLCommon::appendToHypothesesList",
				"hypothesis '"+v+"' is not supported by this parser");
      }
      if(this->mb.areModellingHypothesesDefined()){
	const auto& bh = this->mb.getModellingHypotheses();
	if(bh.find(nh)==bh.end()){
	  this->throwRuntimeError("BehaviourDSLCommon::appendToHypothesesList",
				  "hypothesis '"+v+"' is not supported by the "
				  "behaviour (This means that one of the "
				  "'@ModellingHypothesis' or '@ModellingHypotheses'"
				  "keyword was used earlier)");
	}
      }
      if(!h.insert(nh).second){
	this->throwRuntimeError("BehaviourDSLCommon::appendToHypothesesList",
				"hypothesis '"+v+"' multiply defined");
      }
    }
  } // end of BehaviourDSLCommon::appendToHypothesesList

  void
  BehaviourDSLCommon::readCodeBlockOptions(CodeBlockOptions& o,
					      const bool s)
  {
    using namespace tfel::utilities;
    using namespace tfel::material;
    auto cposition = false;
    auto cmode     = false;
    o.hypotheses.clear();
    if(this->current==this->fileTokens.end()){
      o.hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    if(this->current->value!="<"){
      o.hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    auto tokens = std::vector<Token>{};
    this->readList(tokens,"BehaviourDSLCommon::readCodeBlockOptions","<",">",true);
    for(const auto& t : tokens){
      if(t.value=="Append"){
	if(cmode){
	  this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
				  "insertion mode already specificed");
	}
	cmode = true;
	o.m=BehaviourData::CREATEORAPPEND;
      } else if(t.value=="Replace"){
	if(cmode){
	  this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
				  "insertion mode already specificed");
	}
	cmode = true;
	o.m=BehaviourData::CREATEORREPLACE;
      } else if(t.value=="Create"){
	if(cmode){
	  this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
				  "insertion mode already specificed");
	}
	cmode = true;
	o.m=BehaviourData::CREATE;
      } else if(t.value=="Body"){
	if(cposition){
	  this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
				  "insertion position already specificed");
	}
	cposition = true;
	o.p = BehaviourData::BODY;
      } else if(t.value=="AtBeginning"){
	if(cposition){
	  this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
				  "insertion position already specificed");
	}
	cposition = true;
	o.p = BehaviourData::AT_BEGINNING;
      } else if(t.value=="AtEnd"){
	if(cposition){
	  this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
				  "insertion position already specificed");
	}
	cposition = true;
	o.p = BehaviourData::AT_END;
      } else if((t.flag==Token::String)&&(t.value.substr(1,t.value.size()-2)=="+")){
	this->appendToHypothesesList(o.hypotheses,t.value.substr(1,t.value.size()-2));
      } else if(ModellingHypothesis::isModellingHypothesis(t.value)){
	this->appendToHypothesesList(o.hypotheses,t.value);
      } else {
	o.untreated.push_back(t);
      }
    }
    if(o.hypotheses.empty()){
      o.hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    // checks
    if(!s){
      if(o.hypotheses.size()!=1u){
	this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions : ",
				"specialisation is not allowed");
      }
      if(*(o.hypotheses.begin())!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions : ",
				"specialisation is not allowed");
      }
    }
  } // end of BehaviourDSLCommon::readCodeBlockOptions

  void
  BehaviourDSLCommon::treatUnsupportedCodeBlockOptions(const CodeBlockOptions& o)
  {
    if(!o.untreated.empty()){
      std::ostringstream msg;
      msg << "BehaviourDSLCommon::treatUnsupportedCodeBlockOptions : ";
      if(o.untreated.size()==1u){
	msg << "option '" << o.untreated[0].value << "' is invalid";
      } else {
	msg << "the";
	for(const auto & opt : o.untreated){
	  msg << " '" << opt.value << "'";
	}
	msg << " options are invalid";
      }
      throw(std::runtime_error(msg.str()));
    }
  } // end of BehaviourDSLCommon::treatUnsupportedCodeBlockOptions  

  void
  BehaviourDSLCommon::addStaticVariableDescription(const StaticVariableDescription& v)
  {
    this->mb.addStaticVariable(ModellingHypothesis::UNDEFINEDHYPOTHESIS,v);
  } // end of BehaviourDSLCommon::addStaticVariableDescription

  std::set<BehaviourDSLCommon::Hypothesis>
  BehaviourDSLCommon::getDefaultModellingHypotheses(void) const
  {
    using MH = ModellingHypothesis;
    return std::set<Hypothesis>{MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
	MH::AXISYMMETRICAL,
	MH::PLANESTRAIN,
	MH::GENERALISEDPLANESTRAIN,
	MH::TRIDIMENSIONAL};
  } // end of BehaviourDSLCommon::getDefaultModellingHypotheses

  bool
  BehaviourDSLCommon::isModellingHypothesisSupported(const Hypothesis) const
  {
    return true;
  } // end of BehaviourDSLCommon::isModellingHypothesesSupported

  void
  BehaviourDSLCommon::analyseMaterialProperty(const Hypothesis h,
					      const MaterialPropertyDescription& a)
  {
    for(const auto& v : a.inputs){
      if((a.glossaryNames.find(v.name)==a.glossaryNames.end())&&
    	 (a.entryNames.find(v.name)==a.entryNames.end())){
    	throw(std::runtime_error("BehaviourDSLCommon::analyseMaterialProperty : "
				 "no glossary name declared for variable '"+v.name+
				 "' used by the material property '"+a.law+"'"));
      }
      const auto& in = v.getExternalName(a.glossaryNames,a.entryNames);
      if(in!="Temperature"){
    	if(this->mb.isGlossaryNameUsed(h,in)||this->mb.isUsedAsEntryName(h,in)){
    	  // a variable with the good glossary name has been found
    	  const auto& n = this->mb.getVariableNameFromGlossaryNameOrEntryName(h,in);
	  if(!((this->mb.isMaterialPropertyName(h,n))||
	       (this->mb.isExternalStateVariableName(h,n))||
	       (this->mb.isParameterName(h,n))||
	       (this->mb.isStaticVariableName(h,n)))){
	    throw(std::runtime_error("BehaviourDSLCommon::analyseMaterialProperty : "
				     "variable '"+n+"' required for the material "
				     "property '"+a.law+"' has been declared but "
				     "is neither a material property nor an "
				     "external state variable nor a parameter "
				     "nor a static variable"));
	  } else {
    	    throw(std::runtime_error("BehaviourDSLCommon::analyseMaterialProperty : "
				     "internal error (name '"+n+"' has not been "
				     "declared but exists in the glossary"));
    	  }
    	} else {
    	  // trying to declared a new variable
    	  const auto nn = a.law+"_"+v.name;
    	  this->mb.addExternalStateVariable(h,VariableDescription("real",nn,1u,0u));
	  this->mb.setGlossaryName(h,nn,in);
    	}
      }
    }
  } // end of BehaviourDSLCommon::analyseMaterialProperty

  void
  BehaviourDSLCommon::analyseFile(const std::string& fileName_,
				  const std::vector<std::string>& ecmds)
  {
    this->importFile(fileName_,ecmds);
    // Adding some stuff
    this->endsInputFileProcessing();
    // setting the name of the output files
    this->behaviourFileName = "TFEL/Material/";
    this->behaviourFileName += this->mb.getClassName();
    this->behaviourFileName += ".hxx";
    this->behaviourDataFileName  = "TFEL/Material/";
    this->behaviourDataFileName += this->mb.getClassName();
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->integrationDataFileName  = "TFEL/Material/";
    this->integrationDataFileName += this->mb.getClassName();
    this->integrationDataFileName += "IntegrationData.hxx";
    this->srcFileName  = this->mb.getClassName();
    this->srcFileName += ".cxx";
    // targets description
    for(const auto & i : this->interfaces){
      i.second->getTargetsDescription(this->td,this->mb);
    }
    for(auto& l : this->td){
      insert_if(this->td[l.name].sources,this->srcFileName);
    }
    insert_if(this->td.headers,this->behaviourFileName);
    insert_if(this->td.headers,this->behaviourDataFileName);
    insert_if(this->td.headers,this->integrationDataFileName);
    this->completeTargetsDescription();
  }

  void
  BehaviourDSLCommon::endsInputFileProcessing(void)
  {
    using namespace std;
    using namespace mfront::bbrick;
    const auto& g = tfel::glossary::Glossary::getGlossary();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "BehaviourDSLCommon::endsInputFileProcessing : begin\n";
    }
    // defining modelling hypotheses
    if(!this->mb.areModellingHypothesesDefined()){
      this->mb.setModellingHypotheses(this->getDefaultModellingHypotheses());
    }
    const auto& h = this->mb.getModellingHypotheses();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "behaviour '" << this->mb.getClassName()
	  << "' supports the following hypotheses : \n";
      for(const auto & elem : h){
	log << " - " << ModellingHypothesis::toString(elem);
	if(this->mb.hasSpecialisedMechanicalData(elem)){
	  log << " (specialised)";
	}
	log << endl;	    
      }
    }
    // time step scaling factors
    if(!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			      "minimal_time_step_scaling_factor")){
      VariableDescription e("real","minimal_time_step_scaling_factor",1u,0u);
      e.description = "minimal value for the time step scaling factor";
      this->mb.addParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			    e,BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
					"minimal_time_step_scaling_factor",0.1);
      this->mb.setEntryName(ModellingHypothesis::UNDEFINEDHYPOTHESIS
			    ,"minimal_time_step_scaling_factor","minimal_time_step_scaling_factor");
    }
    if(!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			      "maximal_time_step_scaling_factor")){
      VariableDescription e("real","maximal_time_step_scaling_factor",1u,0u);
      e.description = "maximal value for the time step scaling factor";
      this->mb.addParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			    e,BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
					"maximal_time_step_scaling_factor",
					std::numeric_limits<double>::max());
      this->mb.setEntryName(ModellingHypothesis::UNDEFINEDHYPOTHESIS
			    ,"maximal_time_step_scaling_factor","maximal_time_step_scaling_factor");
    }
    // treating bricks
    if(!this->bricks.empty()){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDSLCommon::endsInputFileProcessing : "
	    << "treating bricks\n";
      }
      for(const auto mh: h){
	auto& d = this->mb.getBehaviourData(mh);
	RequirementManager r{d,this->mb.useQt()};
	// for(const auto& pb : this->bricks){
	//   pb->declareProviders(r,mh);
	// }
	for(const auto& pb : this->bricks){
	  pb->addRequirements(r,mh);
	}
	// unmatched requirements
	auto umrqs = vector<string>{};
	const auto& urs = r.getUnresolvedRequirements();
	for(const auto& n : urs){
	  const auto s = SupportedTypes{};
	  const auto ur = r.getRequirement(n);
	  if((s.getTypeFlag(ur.type)!=SupportedTypes::Scalar)||
	     (find(ur.aproviders.begin(),ur.aproviders.end(),
		   ProviderIdentifier::MATERIALPROPERTY)==ur.aproviders.end())){
	    umrqs.push_back(ur.name);	    
	  }
	}
	if(!umrqs.empty()){
	  auto msg = string{};
	  msg = "BehaviourDSLCommon::endsInputFileProcessing : "
	    "the following requirements can't be met : ";
	  for(const auto& umrq : umrqs){
	    msg += "\n- "+umrq;
	  }
	  throw(runtime_error(msg));
	}
	for(const auto& n : urs){
	  const auto ur = r.getRequirement(n);
	  this->mb.addMaterialProperty(mh,{ur.type,ur.name,ur.asize,0u});
	  if(!g.contains(ur.name)){
	    this->mb.setEntryName(mh,ur.name,ur.name);
	  }
	}
      }
    }
    for(const auto& pb : this->bricks){
      pb->endTreatment();
    }
    // check of stiffness tensor requirement
    if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      if((h.find(ModellingHypothesis::PLANESTRESS)!=h.end())||
	 (h.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)!=h.end())){
	if(this->mb.hasAttribute(BehaviourDescription::requiresStiffnessTensor)){
	  if(!this->mb.hasAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor)){
	    this->throwRuntimeError("BehaviourDSLCommon::endsInputFileProcessing",
				    "No option was given to the '@RequireStiffnessTensor' keyword.\n"
				    "For plane stress hypotheses, it is required to precise whether "
				    "the expected stiffness tensor is 'Altered' (the plane stress "
				    "hypothesis is taken into account) or 'UnAltered' (the stiffness "
				    "tensor is the same as in plane strain)");
	  }
	}
      }
    }
    // some checks on thermal expansions
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      const vector<shared_ptr<MaterialPropertyDescription> >& acs =
	this->mb.getThermalExpansionCoefficients();
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if(this->mb.getSymmetryType()==mfront::ISOTROPIC){
	if(acs.size()!=1u){
	  throw(runtime_error("BehaviourDSLCommon::endsInputFileProcessing : "
			      "unexpected number of thermal expansion coefficients"));
	}
	this->analyseMaterialProperty(uh,*(acs[0]));
      } else if(this->mb.getSymmetryType()==mfront::ORTHOTROPIC){
	if(acs.size()!=3u){
	  throw(runtime_error("BehaviourDSLCommon::endsInputFileProcessing : "
			      "unexpected number of thermal expansion coefficients"));
	}
	this->analyseMaterialProperty(uh,*(acs[0]));
	this->analyseMaterialProperty(uh,*(acs[1]));
	this->analyseMaterialProperty(uh,*(acs[2]));
      } else {
	throw(runtime_error("BehaviourDSLCommon::endsInputFileProcessing : "
			    "unexpected symmetry type"));
      }
      // check if the reference temperature is defined
      if(!this->mb.isParameterName(uh,"referenceTemperatureForThermalExpansion")){
	this->mb.addParameter(uh,VariableDescription("real","referenceTemperatureForThermalExpansion",
						     1u,0u));
      	this->mb.setParameterDefaultValue(uh,"referenceTemperatureForThermalExpansion",293.15);
      }
    }
    for(const auto& i  : this->interfaces){
      i.second->allowDynamicallyAllocatedArrays(this->areDynamicallyAllocatedVectorsAllowed());
    }
    if(getPedanticMode()){
      this->doPedanticChecks();
    }
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "BehaviourDSLCommon::endsInputFileProcessing : end\n";
    }
  } // end of BehaviourDSLCommon::endsInputFileProcessing

  /*!
   * \return the "true" integration variables (state variables are excluded)
   * \param[in] md : mechanical behaviour data
   */
  static VarContainer
  getIntegrationVariables(const BehaviourData& md)
  {
    const auto& ivs = md.getIntegrationVariables();
    VarContainer v;
    for(const auto & iv : ivs){
      if(!md.isStateVariableName(iv.name)){
	v.push_back(iv);
      }
    }
    return v;
  } // end of getIntegrationVariables

  /*!
   * \brief various checks
   * \param[in] v  : variables
   * \param[in] t  : variable type
   * \param[in] uv : list of all used variables
   * \param[in] b1 : check if the variable is used
   * \param[in] b2 : check if the variable increment (or rate) is used
   * \param[in] b3 : check if glossary name is declared
   * \param[in] b4 : check if variable is used in more than one code block (test for local variables)
   */
  static void
  performPedanticChecks(const BehaviourData& md,
			const VarContainer& v,
			const std::string& t,
			const std::map<std::string,
			unsigned short>& uv,
			const bool b1 = true,
			const bool b2 = true,
			const bool b3 = true,
			const bool b4 = false)
  {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    auto& log = getLogStream();
    for(const auto & elem : v){
      if(b1){
	const auto p = uv.find(elem.name);
	if(p==uv.end()){
	  log << "- " << t << " '" << elem.name << "' is unused.\n";
	} else {
	  if(b4&&p->second==1){
	    log << "- " << t << " '" << elem.name << "' is used in one code block only.\n";
	  }
	}
      }
      if(b2){
	if(uv.find("d"+elem.name)==uv.end()){
	  log << "- " << t << " increment 'd" << elem.name << "' is unused.\n";
	}
      }
      if(b3){
	if((!md.hasGlossaryName(elem.name))&&
	   (!md.hasEntryName(elem.name))){
	  log << "- " << t << " '" << elem.name << "' has no glossary name.\n";
	}
      }
      if(elem.description.empty()){
	auto hasDoc = false;
	if(md.hasGlossaryName(elem.name)){
	  const auto& e = glossary.getGlossaryEntry(md.getExternalName(elem.name));
	  hasDoc = (!e.getShortDescription().empty()) || (!e.getDescription().empty());
	}
	if(!hasDoc){
	  log << "- " << t << " '" << elem.name << "' has no description.\n";
	}
      }
    }
  } 

  /*!
   * \brief various checks on static variables
   * \param[in] v  : variables
   * \param[in] uv : list of all static variables
   */
  static void
  performPedanticChecks(const StaticVarContainer& v,
			const std::map<std::string,unsigned short>& uv)
  {
    auto& log = getLogStream();
    for(const auto & elem : v){
      if(uv.find(elem.name)==uv.end()){
	log << "- static variable '" << elem.name << "' is unused.\n";
      }
    }      
  }
  
  void
  BehaviourDSLCommon::doPedanticChecks(void) const
  {
    const auto& hs = this->mb.getDistinctModellingHypotheses();
    auto& log = getLogStream();
    log << "\n* Pedantic checks\n";
    for(auto h : hs){
      const auto& md = this->mb.getBehaviourData(h);
      // checks if variables are used
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	log << "\n** Beginning pedantic checks for default modelling hypothesis\n\n";
      } else {
	log << "\n** Beginning pedantic checks for modelling hypothesis '"
	    << ModellingHypothesis::toString(h) << "'\n\n";
      }
      // getting all used variables
      const auto& cbs = md.getCodeBlockNames();
      auto members  = std::map<std::string,unsigned short>{}; // variable names and counts
      auto smembers = std::map<std::string,unsigned short>{}; // static variable nanes and counts
      for(const auto & cbs_pcbs : cbs){
	const auto& cb = md.getCodeBlock(cbs_pcbs);
	if(cb.description.empty()){
	  log << "- code block '" << cbs_pcbs << "' has no description\n";
	}
	for(const auto & v : cb.members){
	  if(members.count(v)==0){
	    members[v] = 1;
	  } else{
	    ++(members[v]);
	  }
	}
	for(const auto & v : cb.staticMembers){
	  if(smembers.count(v)==0){
	    smembers[v] = 1;
	  } else{
	    ++(smembers[v]);
	  }
	}
      }
      performPedanticChecks(md,md.getMaterialProperties(),
			    "material property",members,true,false,true);
      const auto& ivs = getIntegrationVariables(md);
      performPedanticChecks(md,ivs,"integration variable",
			    members,false,true,false);
      performPedanticChecks(md,md.getStateVariables(),"state variable",members);
      performPedanticChecks(md,md.getAuxiliaryStateVariables(),
			    "auxiliary state variable",members,true,false);
      performPedanticChecks(md,md.getExternalStateVariables(),"external state variable",members);
      performPedanticChecks(md,md.getLocalVariables(),"local variable",members,true,
			    false,false,true);
      performPedanticChecks(md,md.getParameters(),"parameter",members,true,false);
      performPedanticChecks(md.getStaticVariables(),smembers);
    }
    log << "\n# End of pedantic checks\n";
  } // end of BehaviourDSLCommon::pedanticChecks

  void 
  BehaviourDSLCommon::generateOutputFiles(void)
  {
    using namespace std;
    tfel::system::systemCall::mkdir("src");
    tfel::system::systemCall::mkdir("include");
    tfel::system::systemCall::mkdir("include/TFEL/");
    tfel::system::systemCall::mkdir("include/TFEL/Material");
    this->behaviourFile.open("include/"+this->behaviourFileName);
    if(!this->behaviourFile){
      string msg("BehaviourDSLCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFile.open("include/"+this->behaviourDataFileName);
    if(!this->behaviourDataFile){
      string msg("BehaviourDSLCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFile.open("include/"+this->integrationDataFileName);
    if(!this->integrationDataFile){
      string msg("BehaviourDSLCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open("src/"+this->srcFileName);
    if(!this->srcFile){
      string msg("BehaviourDSLCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    // generate outpout files
    this->writeBehaviourDataFileBegin();
    this->writeIntegrationDataFileBegin();
    this->writeBehaviourFileBegin();
    // modelling hypotheses handled by the interfaces (if at least one
    // interface is defined), or by the behaviour
    set<Hypothesis> hh;
    if(this->interfaces.empty()){
      const auto& bh = this->mb.getModellingHypotheses();
      hh.insert(bh.begin(),bh.end());
    } else {
      // calling the interfaces
      for(const auto& i : this->interfaces){
	const auto& ih = i.second->getModellingHypothesesToBeTreated(this->mb);
	hh.insert(ih.begin(),ih.end());
      }
    }
    if(!this->mb.areAllMechanicalDataSpecialised(hh)){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDSLCommon::writeOutputFiles : "
	    << "treating default hypothesis\n";
      }
      const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      // Generating BehaviourData's outputClass
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour data "
	    << "for default hypothesis\n";
      }
      this->writeBehaviourDataClass(h);
      // Generating IntegrationData's outputClass
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDSLCommon::writeOutputFiles : writing integration data "
	    << "for default hypothesis\n";
      }
      this->writeIntegrationDataClass(h);
      // Generating Behaviour's outputFile
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour class "
	    << "for default hypothesis\n";
      }
      this->writeBehaviourClass(h);
    }
    for(const auto & elem : hh){
      if(mb.hasSpecialisedMechanicalData(elem)){
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  auto& log = getLogStream();
	  log << "BehaviourDSLCommon::writeOutputFiles : "
	      << "treating hypothesis '" << ModellingHypothesis::toString(elem)
	      << "'\n";
	}
	// Generating BehaviourData's outputClass
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  auto& log = getLogStream();
	  log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour data "
	      << "for hypothesis '" << ModellingHypothesis::toString(elem) << "'\n";
	}
	this->writeBehaviourDataClass(elem);
	// Generating IntegrationData's outputClass
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  auto& log = getLogStream();
	  log << "BehaviourDSLCommon::writeOutputFiles : writing integration data "
	      << "for hypothesis '" << ModellingHypothesis::toString(elem) << "'\n";
	}
	this->writeIntegrationDataClass(elem);
	// Generating behaviour's outputClass
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  auto& log = getLogStream();
	  log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour class "
	      << "for hypothesis '" << ModellingHypothesis::toString(elem) << "'\n";
	}
	this->writeBehaviourClass(elem);
      }
    }
    this->writeBehaviourDataFileEnd();
    this->writeIntegrationDataFileEnd();
    this->writeBehaviourFileEnd();
    // Generating behaviour's source file
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "BehaviourDSLCommon::writeOutputFiles : writing source file\n";
    }
    this->writeSrcFile();
    // calling the interfaces
    for(const auto& i : this->interfaces){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDSLCommon::writeOutputFiles : "
	    << "calling interface '" << i.first << "'\n";
      }
      i.second->endTreatment(this->mb,*this);
    }
  }

  void
  BehaviourDSLCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis h,
												const std::string& n)
  {
    if(!this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->mb.setUsableInPurelyImplicitResolution(h,false);
    }
    this->mb.declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,n);
  } // end of BehaviourDSLCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  std::string
  BehaviourDSLCommon::standardModifier(const Hypothesis h,
					  const std::string& var,
					  const bool addThisPtr)
  {
    if((this->mb.isExternalStateVariableIncrementName(h,var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of BehaviourDSLCommon::standardModifier
    
  std::string
  BehaviourDSLCommon::tangentOperatorVariableModifier(const Hypothesis h,
							 const std::string& var,
							 const bool addThisPtr)
  {
    return this->standardModifier(h,var,addThisPtr);
  } // end of BehaviourDSLCommon::tangentOperatorVariableModifier

  void BehaviourDSLCommon::treatBrick(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    auto& f = AbstractBehaviourBrickFactory::getFactory();
    AbstractBehaviourBrick::Parameters parameters;
    if(this->current->value=="<"){
      vector<Token> tokens;
      this->readList(tokens,"BehaviourDSLCommon::treatBehaviourBrick",
		     "<",">",true);
      for(vector<Token>::const_iterator p=tokens.begin();p!=tokens.end();++p){
	const auto& t = p->value;
	const auto pos = t.find('=');
	if(pos!=string::npos){
	  if(pos==0){
	    this->throwRuntimeError("BehaviourDSLCommon::treatBehaviourBrick",
				    "no parameter name given");
	  }
	  // extracting the name
	  const auto& n = t.substr(0,pos);
	  if(pos==t.size()){
	    this->throwRuntimeError("BehaviourDSLCommon::treatBehaviourBrick",
				    "no option given to the parameter '"+n+"'");
	  }
	  // extracting the option
	  const auto& o = t.substr(pos+1);
	  parameters.insert({n,o});
	} else {
	  parameters.insert({t,""});
	}
      }
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatIsTangentOperatorSymmetric : ",
  			    "Expected 'true' or 'false'.");
    const auto& b = this->readString("BehaviourDSLCommon::treatBehaviourBrick");
    this->readSpecifiedToken("BehaviourDSLCommon::treatBehaviourBrick",";");
    f.get(b,*this,this->mb,parameters);
  } // end of BehaviourDSLCommon::treatBrick
  
  void BehaviourDSLCommon::treatTangentOperator(void)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    CodeBlockOptions o;
    this->readCodeBlockOptions(o,true);
    if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      bool found = false;
      if(o.untreated.size()!=1u){
	ostringstream msg;
	msg << "BehaviourDSLCommon::treatTangentOperator : "
	    << "tangent operator type is undefined. Valid tanget operator type are :\n";
	for(const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()){
	  msg << "- " << convertFiniteStrainBehaviourTangentOperatorFlagToString(to) << " : "
	      << getFiniteStrainBehaviourTangentOperatorDescription(to) << '\n';
	}
	throw(runtime_error(msg.str()));
      }
      if(o.untreated[0].flag!=Token::Standard){
	throw(runtime_error("BehaviourDSLCommon::treatTangentOperator : "
			    "invalid option '"+o.untreated[0].value+"'"));
      }
      const auto& ktype = o.untreated[0].value;
      for(const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()){
	if(ktype==convertFiniteStrainBehaviourTangentOperatorFlagToString(to)){
	  found=true;
	  break;
	}
      }
      if(!found){
	ostringstream msg;
	msg << "BehaviourDSLCommon::treatTangentOperator : "
	    << "invalid tangent operator type '"+ktype+"'. Valid tanget operator type are :\n";
	for(const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()){
	  msg << "- " << convertFiniteStrainBehaviourTangentOperatorFlagToString(to) << " : "
	      << getFiniteStrainBehaviourTangentOperatorDescription(to) << endl;
	}
	throw(runtime_error(msg.str()));
      }
      this->readCodeBlock(*this,o,BehaviourData::ComputeTangentOperator+"-"+ktype,
			  &BehaviourDSLCommon::tangentOperatorVariableModifier,true);
      for(const auto & h : o.hypotheses){
	if(!this->mb.hasAttribute(h,BehaviourData::hasConsistentTangentOperator)){
	  this->mb.setAttribute(h,BehaviourData::hasConsistentTangentOperator,true);
	}
      }
    } else {
      this->treatUnsupportedCodeBlockOptions(o);
      this->readCodeBlock(*this,o,BehaviourData::ComputeTangentOperator,
			  &BehaviourDSLCommon::tangentOperatorVariableModifier,true);
      for(const auto & h : o.hypotheses){
	this->mb.setAttribute(h,BehaviourData::hasConsistentTangentOperator,true);
      }
    }
  } // end of BehaviourDSLCommon::treatTangentOperator

  void BehaviourDSLCommon::treatIsTangentOperatorSymmetric(void)
  {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatIsTangentOperatorSymmetric : ",
  			    "Expected 'true' or 'false'.");
    auto b = this->readBooleanValue("BehaviourDSLCommon::treatIsTangentOperatorSymmetric");
    this->readSpecifiedToken("BehaviourDSLCommon::treatIsTangentOperatorSymmetric",";");
    for(const auto & h : hs){
      this->mb.setAttribute(h,BehaviourData::isConsistentTangentOperatorSymmetric,b);
    }
  } // end of BehaviourDSLCommon::treatTangentOperator

  void
  BehaviourDSLCommon::treatMaterial(void)
  {
    const auto& m = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(m,true)){
      this->throwRuntimeError("BehaviourDSLCommon::treatMaterial",
			      "invalid material name '"+m+"'");
    }
    this->mb.setMaterialName(m);
    if(!isValidIdentifier(this->mb.getClassName())){
      this->throwRuntimeError("BehaviourDSLCommon::treatMaterial",
			      "resulting class name is not valid (read '"+
			      this->mb.getClassName()+"')");
    }
  } // end of BehaviourDSLCommon::treatMaterial

  void
  BehaviourDSLCommon::treatLibrary(void)
  {
    const auto& m = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(m,true)){
      this->throwRuntimeError("BehaviourDSLCommon::treatLibrary",
			      "invalid library name '"+m+"'");
    }
    this->mb.setLibrary(m);
  } // end of BehaviourDSLCommon::treatLibrary

  void
  BehaviourDSLCommon::treatComputeThermalExpansion(void)
  {
    using namespace std;
    const string m("BehaviourDSLCommon::treatComputeThermalExpansion");
    const auto& files = this->readStringOrArrayOfString(m);
    this->readSpecifiedToken(m,";");
    if((files.size()!=1u)&&(files.size()!=3u)){
      this->throwRuntimeError("BehaviourDSLCommon::treatComputeThermalExpansion",
			      "invalid number of file names given");
    }
    if(files.size()==1u){
      // the material shall have been declared isotropic
      if(this->mb.getSymmetryType()!=mfront::ISOTROPIC){
	this->throwRuntimeError("BehaviourDSLCommon::treatComputeThermalExpansion",
				"the mechanical behaviour must be isotropic to give more than "
				"one thermal expansion coefficient.");
      }
    }
    if(files.size()==3u){
      // the material shall have been declared orthotropic
      if(this->mb.getSymmetryType()!=mfront::ORTHOTROPIC){
	this->throwRuntimeError("BehaviourDSLCommon::treatComputeThermalExpansion",
				"the mechanical behaviour must be orthotropic to give more than "
				"one thermal expansion coefficient.");
      }
    }
    vector<shared_ptr<MaterialPropertyDescription> > acs;
    for(const auto & file : files){
      const auto& f = SearchFile::search(file);
      auto a = make_shared<MaterialPropertyDescription>(this->handleMaterialLaw(f));
      if(!a->staticVars.contains("ReferenceTemperature")){
	if(getVerboseMode()!=VERBOSE_QUIET){
	  auto& os = getLogStream();
	  os  << "no reference temperature in material property '";
	  if(a->material.empty()){
	    os << a->material << '_';
	  }
	  os << a->law << "'\n";
	}
      }
      if(!((a->inputs.size())||(a->inputs.size()!=1u))){
	this->throwRuntimeError("BehaviourDSLCommon::treatComputeThermalExpansion : ",
				"thermal expansion shall only depend on temperature");
      }
      if(a->inputs.size()==1u){
	const auto& v = a->inputs.front();
	const auto& vn = v.getExternalName(a->glossaryNames,a->entryNames);
	if(vn!="Temperature"){
	  this->throwRuntimeError("BehaviourDSLCommon::treatComputeThermalExpansion : ",
				  "thermal expansion shall only depend on temperature");
	}
      }
      acs.push_back(a);
    }
    if(acs.size()==1u){
      this->mb.setThermalExpansionCoefficient(acs.front());
    } else {
      this->mb.setThermalExpansionCoefficients(acs[0],acs[1],acs[2]);
    }
  } // end of BehaviourDSLCommon::treatComputeThermalExpansion

  void
  BehaviourDSLCommon::treatModellingHypothesis(void)
  {
    using namespace std;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypothesis");
    const Hypothesis h = ModellingHypothesis::fromString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypothesis");
    this->readSpecifiedToken("BehaviourDSLCommon::treatModellingHypothesis",";");
    if(!this->isModellingHypothesisSupported(h)){
      this->throwRuntimeError("BehaviourDSLCommon::treatModellingHypothesis",
			      "unsupported modelling hypothesis '"+
			      ModellingHypothesis::toString(h)+"'");
    }
    set<Hypothesis> hypotheses;
    hypotheses.insert(h);
    this->mb.setModellingHypotheses(hypotheses);
  } // end of BehaviourDSLCommon::treatModellingHypothesis

  void
  BehaviourDSLCommon::treatModellingHypotheses(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    set<Hypothesis> hypotheses;
    vector<Token> tokens;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypotheses");
    this->readList(tokens,"BehaviourDSLCommon::treatModellingHypotheses","{","}",false);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypotheses");
    this->readSpecifiedToken("BehaviourDSLCommon::treatModellingHypotheses",";");
    for(vector<Token>::const_iterator p=tokens.begin();p!=tokens.end();++p){
      if(p->flag==Token::String){
	this->appendToHypothesesList(hypotheses,p->value.substr(1,p->value.size()-2));
      } else {
	this->appendToHypothesesList(hypotheses,p->value);
      }
    }
    if(hypotheses.empty()){
      this->throwRuntimeError("BehaviourDSLCommon::treatModellingHypotheses",
			      "no hypothesis declared");
    }
    this->mb.setModellingHypotheses(hypotheses);
  } // end of BehaviourDSLCommon::treatModellingHypotheses

  void
  BehaviourDSLCommon::treatUsableInPurelyImplicitResolution(void)
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->readSpecifiedToken("BehaviourDSLCommon::treatUsableInPurelyImplicitResolution",";");
    if(this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->throwRuntimeError("BehaviourDSLCommon::treatUsableInPurelyImplicitResolution",
			      "keyword '@UsableInPurelyImplicitResolution' already called");
    }
    this->explicitlyDeclaredUsableInPurelyImplicitResolution = true;
    this->mb.setUsableInPurelyImplicitResolution(h,true);
  } // end of BehaviourDSLCommon::treatUsableInPurelyImplicitResolution

  void
  BehaviourDSLCommon::treatParameterMethod(const Hypothesis h)
  {
    using namespace std;
    using namespace tfel::utilities;
    const auto& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod",".");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
    if(this->current->value=="setDefaultValue"){
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod","(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("BehaviourDSLCommon::treatParameterMethod",
				"could not read default value for parameter '"+n+"'");
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod",")");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod",";");
      this->mb.setParameterDefaultValue(h,n,value);
    } else {
      --(this->current);
      --(this->current);
      this->treatVariableMethod(h);
    }
  } // end of BehaviourDSLCommon::treatParameterMethod

  bool
  BehaviourDSLCommon::isCallableVariable(const Hypothesis h,
					    const std::string& n) const
  {
    return ((this->mb.isMaterialPropertyName(h,n))||
	    (this->mb.isStateVariableName(h,n))||
	    (this->mb.isAuxiliaryStateVariableName(h,n))||
	    (this->mb.isExternalStateVariableName(h,n))||
	    (this->mb.isLocalVariableName(h,n))||
	    (this->mb.isStaticVariableName(h,n))||
	    (this->mb.isParameterName(h,n)));
  } // end of BehaviourDSLCommon::isCallableVariable

  void
  BehaviourDSLCommon::treatVariableMethod(const Hypothesis h)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    const auto& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod",".");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    if(this->current->value=="setGlossaryName"){
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      if(this->current->flag!=Token::String){
	string msg("BehaviourDSLCommon::treatVariableMethod : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      const auto& g = this->current->value.substr(1,this->current->value.size()-2);
      if(!glossary.contains(g)){
	string msg("BehaviourData::setGlossaryName : "
		   "'"+g+"' is not a glossary name");
	throw(runtime_error(msg));
      }
      this->mb.setGlossaryName(h,n,g);
      ++(this->current);
    } else if(this->current->value=="setEntryName"){
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      if(this->current->flag!=Token::String){
	string msg("BehaviourDSLCommon::treatVariableMethod : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      const auto& e = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->isValidIdentifier(e)){
	string msg("BehaviourDSLCommon::treatVariableMethod : ");
	msg += "invalid entry name '"+e+"'";
	throw(runtime_error(msg));
      }
      ++(this->current);
      this->mb.setEntryName(h,n,e);
    } else {
      this->treatUnknownVariableMethod(h,n);
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod",")");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod",";");
  } // end of BehaviourDSLCommon::treatVariableMethod

  void
  BehaviourDSLCommon::treatUnknownVariableMethod(const Hypothesis,
						    const std::string& n)
  {
    using namespace std;
    string msg("BehaviourDSLCommon::treatUnknownVariableMethod : ");
    msg += "unknown method '"+this->current->value+"' for variable '"+n+"', ";
    msg += "valid methods are 'setGlossaryName' or 'setEntryName'";
    throw(runtime_error(msg));
  } // end of BehaviourDSLCommon::treatUnknownVariableMethod

  void
  BehaviourDSLCommon::treatUnknownKeyword(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    pair<bool,CxxTokenizer::TokensContainer::const_iterator> p;
    TokensContainer::const_iterator p2;
    bool treated = false;
    string key;
    --(this->current);
    key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
	string t;
	if(this->current->flag==Token::String){
	  t = this->current->value.substr(1,this->current->value.size()-2);
	} else {
	  t = this->current->value;
	}
	if(this->interfaces.find(t)!=this->interfaces.end()){
	  s.insert(t);
	}
	++(this->current);
	if((this->current->value!="]")&&(this->current->value!=",")){
	  this->throwRuntimeError("BehaviourDSLCommon::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if(s.empty()){
	this->ignoreKeyWord(key);
	return;
      }
      for(const auto & elem : s){
	p = this->interfaces.at(elem)->treatKeyword(key,this->current,
						    this->fileTokens.end());
	if(!p.first){
	  string msg("BehaviourDSLCommon::treatUnknownKeyword : the keyword '");
	  msg += key;
	  msg += " has not been treated by interface '"+elem+"'";
	  throw(runtime_error(msg));
	  }
	if(treated){
	  if(p2!=p.second){
	    string msg("BehaviourDSLCommon::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += "' has been treated by two interfaces/analysers but";
	    msg += " results were differents";
	    throw(runtime_error(msg));
	  }
	}
	p2 = p.second;
	treated = true;
      }
    } else {
      for(const auto&i : this->interfaces){
	p = i.second->treatKeyword(key,this->current,
				 this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("BehaviourDSLCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces/analysers but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
      }
    }
    if(!treated){
      DSLBase::treatUnknownKeyword();
    }
    this->current = p2;
  } // end of BehaviourDSLCommon::treatUnknownKeyword

  void
  BehaviourDSLCommon::treatUseQt(void)
  {
    using namespace std;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatUseQt : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setUseQt(true);
    } else if(this->current->value=="false"){
      this->mb.setUseQt(false);
    } else {
      this->throwRuntimeError("BehaviourDSLCommon::treatUseQt",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatUseQt",";");
  } // end of BehaviourDSLCommon::treatUseQt

  void
  BehaviourDSLCommon::treatIsotropicBehaviour(void)
  {
    using namespace std;
    if(this->mb.getSymmetryType()!=mfront::ISOTROPIC){
      string msg("BehaviourDSLCommon::treatIsotropicBehaviour  : ");
      msg += "this behaviour has been declared orthotropic";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatIsotropicBehaviour",";");
  } // end of BehaviourDSLCommon::treatIsotropicBehaviour

  void
  BehaviourDSLCommon::treatOrthotropicBehaviour(void)
  {
    using namespace tfel::material;
    OrthotropicAxesConvention c = OrthotropicAxesConvention::DEFAULT;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatOrthotropicBehaviour");
    if(this->current->value=="<"){
      this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour","<");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatOrthotropicBehaviour");
      if(this->current->value=="Pipe"){
	c = OrthotropicAxesConvention::PIPE;
      } else {
	this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",
				 "Undefined");
      }
      this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",">");
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",";");
    this->mb.setSymmetryType(mfront::ORTHOTROPIC);
    this->mb.setOrthotropicAxesConvention(c);
    // by defaut the elastic behaviour is also orthotropic
    this->mb.setElasticSymmetryType(mfront::ORTHOTROPIC);
  } // end of BehaviourDSLCommon::treatOrthotropicBehaviour

  void
  BehaviourDSLCommon::treatIsotropicElasticBehaviour(void)
  {
    using namespace std;
    this->readSpecifiedToken("BehaviourDSLCommon::treatIsotropicElasticBehaviour",";");
    if(this->mb.getSymmetryType()!=mfront::ORTHOTROPIC){
      string msg("BehaviourDSLCommon::treatIsotropicElasticBehaviour  : ");
      msg += "this behaviour has not been declared orthotropic";
      throw(runtime_error(msg));
    }
    if(this->mb.getElasticSymmetryType()!=mfront::ORTHOTROPIC){
      string msg("BehaviourDSLCommon::treatIsotropicElasticBehaviour  : ");
      msg += "the elastic behaviour has already been declared isotropic";
      throw(runtime_error(msg));
    }
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
  } // end of BehaviourDSLCommon::treatIsotropicElasticBehaviour

  void
  BehaviourDSLCommon::treatRequireStiffnessOperator(void)
  {
    using namespace std;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      auto& log = getLogStream();
      log << "BehaviourDSLCommon::treatRequireStiffnessOperator : "
	  << "@RequireStiffnessOperator is deprecated\n"
	  << "You shall use @RequireStiffnessTensor instead\n";
    }
    this->treatRequireStiffnessTensor();
  } // end of BehaviourDSLCommon::treatRequireStiffnessOperator

  void
  BehaviourDSLCommon::treatRequireStiffnessTensor(void)
  {
    using tfel::material::OrthotropicAxesConvention;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatRequireStiffnessTensor");
    if(this->current->value=="<"){
      this->readSpecifiedToken("BehaviourDSLCommon::treatRequireStiffnessTensor","<");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatRequireStiffnessTensor");
      if(this->current->value=="UnAltered"){
	this->mb.setAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,true,false);
      } else if(this->current->value=="Altered"){
	this->mb.setAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,false,false);
      } else {
	this->throwRuntimeError("BehaviourDSLCommon::treatRequireStiffnessTensor : ",
				"expected 'Altered' or 'UnAltered' option "
				"(read '"+this->current->value+"')");
      }
      ++(this->current);
      this->readSpecifiedToken("BehaviourDSLCommon::treatRequireStiffnessTensor",">");
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatRequireStiffnessTensor",";");
    if(this->mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if(this->mb.getOrthotropicAxesConvention()!=OrthotropicAxesConvention::DEFAULT){
	this->throwRuntimeError("BehaviourDSLCommon::treatRequireStiffnessTensor : ",
				"RequireStiffnessTensor can only be used with the 'Default' "
				"orthotropic axes convention");
      }
    }
    this->mb.setAttribute(BehaviourDescription::requiresStiffnessTensor,true,false);
  } // end of BehaviourDSLCommon::treatRequireStiffnessTensor

  void
  BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor(void)
  {
    using namespace std;
    this->readSpecifiedToken("BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor",";");
    this->mb.setAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,true,false);
  } // end of BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor

  void BehaviourDSLCommon::treatBehaviour(void)
  {
    using namespace std;
    const auto& b = this->readOnlyOneToken();
    this->mb.setBehaviourName(b);
    if(!isValidIdentifier(this->mb.getClassName())){
      this->throwRuntimeError("BehaviourDSLCommon::treatBehaviour",
			      "resulting class name is not valid (read '"+
			      this->mb.getClassName()+"')");
    }
  } // end of BehaviourDSLCommon::treatMaterial

  void 
  BehaviourDSLCommon::readStringList(std::vector<std::string>& cont)
  {
    this->checkNotEndOfFile("BehaviourDSLCommon::readStringList",
			    "Cannot read interface name.");
    auto endOfTreatment=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatment)){
      const auto s = this->current->value;
      if(!isValidIdentifier(s)){
	--(this->current);
	this->throwRuntimeError("BehaviourDSLCommon::readStringList",
				"interface name is not valid (read '"+s+"')");
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::readStringList");
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatment=true;
	++(this->current);
      } else {
	this->throwRuntimeError("BehaviourDSLCommon::readStringList",
				"',' or ';' expected afer '"+s+"'");
      }
      if(find(cont.begin(),cont.end(),s)!=cont.end()){
	this->throwRuntimeError("BehaviourDSLCommon::readStringList",
				"'"+s+"' has already been registred.\n");
      } 
      cont.push_back(s);
    }
    if(!endOfTreatment){
      --(this->current);
      this->throwRuntimeError("BehaviourDSLCommon::readStringList",
			      "Expected ';' before end of file.");
    }
  }

  void
  BehaviourDSLCommon::readHypothesesList(std::set<Hypothesis>& h)
  {
    h.clear();
    if(this->current==this->fileTokens.end()){
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    if(this->current->value!="<"){
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    auto tokens = std::vector<tfel::utilities::Token>{};
    this->readList(tokens,"BehaviourDSLCommon::readHypothesesList","<",">",true);
    for(const auto& t : tokens){
      if(t.flag==tfel::utilities::Token::String){
	this->appendToHypothesesList(h,t.value.substr(1,t.value.size()-2));
      } else {
	this->appendToHypothesesList(h,t.value);
      }
    }
    if(h.empty()){
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
  } // end of BehaviourDSLCommon::readHypothesesList

  void
  BehaviourDSLCommon::readVariableList(VariableDescriptionContainer& v,
				       std::set<Hypothesis>& h,
				       void (BehaviourDescription::* m)(const Hypothesis,
									const VariableDescriptionContainer&,
									const BehaviourData::RegistrationStatus),
				       const bool b1,
				       const bool b2)
  {
    h.clear();
    v.clear();
    this->readHypothesesList(h);
    this->readVarList(v,b1);
    this->addVariableList(h,v,m,b2);
  } // end of BehaviourDSLCommon::readVariableList

  void
  BehaviourDSLCommon::addVariableList(const std::set<Hypothesis>& h,
				      const VariableDescriptionContainer& v,
				      void (BehaviourDescription::* m)(const Hypothesis,
								       const VariableDescriptionContainer&,
								       const BehaviourData::RegistrationStatus),
				      const bool b)
  {
    for(const auto & elem : h){
      if(!b){
	if(!this->mb.getAttribute<bool>(elem,BehaviourData::allowsNewUserDefinedVariables,true)){
	  const auto cbn = this->mb.getCodeBlockNames(elem);
	  if(cbn.empty()){
	    this->throwRuntimeError("BehaviourDSLCommon::readVariableList : ",
				    "no more variable can be defined. This may mean that "
				    "the parser does not expect you to add variables");
	  } else {
	    auto cbs = std::string{};
	    for(const auto& n : cbn){
	      cbs += "\n- "+n;
	    }
	    this->throwRuntimeError("BehaviourDSLCommon::readVariableList : ",
				    "no more variable can be defined. This may mean that "
				    "you already declared a block of code (or that the parser does not "
				    "expect you to add variables for whatever reason). This is the list of "
				    "code blocks defined :"+cbs);
	  }
	}
      }
      (this->mb.*m)(elem,v,BehaviourData::UNREGISTRED);
    }
  } // end of BehaviourDSLCommon::addVariableList

  void BehaviourDSLCommon::treatCoef(void)
  {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v,h,&BehaviourDescription::addMaterialProperties,true,false);
  } // end of BehaviourDSLCommon::treatCoef

  void
  BehaviourDSLCommon::treatLocalVar(void)
  {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v,h,&BehaviourDescription::addLocalVariables,true,true);
  } // end of BehaviourDSLCommon::treatLocalVar

  void BehaviourDSLCommon::treatInterface(void)
  {
    typedef BehaviourInterfaceFactory MBIF;
    auto& mbif = MBIF::getBehaviourInterfaceFactory();
    auto inames = std::vector<std::string>{};
    this->readStringList(inames);
    for(const auto& i : inames){
      if(this->interfaces.count(i)==0){
	this->interfaces.insert({i,mbif.getInterface(i)});
      }
    }
  } // end of BehaviourDSLCommon::treatInterface

  void
  BehaviourDSLCommon::setInterfaces(const std::set<std::string>& inames)
  {
    using namespace std;
    typedef BehaviourInterfaceFactory MBIF;
    auto& mbif = MBIF::getBehaviourInterfaceFactory();
    for(const auto& i : inames){
      if(this->interfaces.count(i)==0){
	this->interfaces.insert({i,mbif.getInterface(i)});
      }
    }
  } // end of BehaviourDSLCommon::setInterfaces

  void
  BehaviourDSLCommon::treatAPrioriTimeStepScalingFactor(void){
    this->readCodeBlock(*this,BehaviourData::APrioriTimeStepScalingFactor,
			&BehaviourDSLCommon::standardModifier,true,true);
  }
  
  void
  BehaviourDSLCommon::treatIntegrator(void)
  {
    this->readCodeBlock(*this,BehaviourData::Integrator,
			&BehaviourDSLCommon::standardModifier,true,true);
  } // end of BehaviourDSLCommon::treatIntegrator

  void
  BehaviourDSLCommon::treatAPosterioriTimeStepScalingFactor(void){
    this->readCodeBlock(*this,BehaviourData::APosterioriTimeStepScalingFactor,
			&BehaviourDSLCommon::standardModifier,true,true);
  }
    
  
  void BehaviourDSLCommon::treatStateVariable(void)
  {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v,h,&BehaviourDescription::addStateVariables,true,false);
  }

  void BehaviourDSLCommon::treatAuxiliaryStateVariable(void)
  {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v,h,&BehaviourDescription::addAuxiliaryStateVariables,true,false);
  }

  void BehaviourDSLCommon::treatExternalStateVariable(void)
  {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v,h,&BehaviourDescription::addExternalStateVariables,true,false);
  }

  void
  BehaviourDSLCommon::treatBounds(void)
  {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    auto b = current;
    for(const auto & h : hs){
      current = b;
      BoundsDescription d;
      d.category = BoundsDescription::Standard;
      this->treatBounds(d,h);
      this->mb.setBounds(h,d);
    }
  } // end of BehaviourDSLCommon::treatBounds

  void
  BehaviourDSLCommon::treatPhysicalBounds(void)
  {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    auto b = current;
    for(const auto & h : hs){
      current = b;
      BoundsDescription d;
      d.category = BoundsDescription::Physical;
      this->treatBounds(d,h);
      this->mb.setBounds(h,d);
    }
  } // end of BehaviourDSLCommon::treatPhysicalBounds

  void BehaviourDSLCommon::treatBounds(BoundsDescription& d,
					  const Hypothesis h)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    const auto& md = this->mb.getBehaviourData(h);
    bool found;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");

    d.lineNumber = this->current->line;
    d.name       = this->current->value;
    d.varName    = this->current->value;
    d.component  = -1;

    found = false;
    d.arraySize = 1u;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
      for(p3=this->mb.getMainVariables().begin();
	  p3!=this->mb.getMainVariables().end();++p3){
	// check if the driving variable match
	if(d.varName==p3->first.name){
	  found=true;
	  if(p3->first.increment_known){
	    d.varCategory = BoundsDescription::ExternalStateVariable;
	  } else {
	    d.varCategory = BoundsDescription::ExternalStateVariableII;
	  }
	  d.varType     = this->getTypeFlag(p3->first.type);
	}
	// check if the associated thermodynamic force match
	if(d.varName==p3->second.name){
	  found=true;
	  d.varCategory = BoundsDescription::StateVariable;
	  d.varType     = this->getTypeFlag(p3->second.type);
	}
      }
    }
    // temperature
    if(d.varName=="T"){
      found=true;
      d.varCategory = BoundsDescription::StateVariable;
      d.varType     = Scalar;
    }
    for(p   = md.getMaterialProperties().begin();
	(p != md.getMaterialProperties().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::MaterialProperty;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getLocalVariables().begin();
	(p != md.getLocalVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::LocalVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getStateVariables().begin();
	(p != md.getStateVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::StateVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getAuxiliaryStateVariables().begin();
	(p != md.getAuxiliaryStateVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::StateVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getExternalStateVariables().begin();
	(p != md.getExternalStateVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::ExternalStateVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    if(!found){
      this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
			      this->current->value+" is not a valid identifier.");
    }
    if ((d.varType==SupportedTypes::TVector)||
	(d.varType==SupportedTypes::Stensor)||
	(d.varType==SupportedTypes::Tensor)){
      ++(this->current);
      this->readSpecifiedToken("BehaviourDSLCommon::treatBounds : ","(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds : ");
      if(this->current->value!="*"){
	unsigned int component;
	istringstream converter(this->current->value);
	converter >> component;
	if(!converter||(!converter.eof())){
	  this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				  "could not read component number for variable '"+d.varName+"'");
	}
	d.component=component;
	d.varType=Scalar;
	d.varName+="(";
	d.varName+=to_string(component);
	d.varName+=")";
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");
      if(this->current->value!=")"){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"Expected ')' (read '"+this->current->value+"')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds","in");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");
      if(this->current->value!="*"){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      d.boundsType = BoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds",
			      "expected to read lower bound value for variable "+d.varName);
      istringstream converter(this->current->value);
      converter >> d.lowerBound;
      d.boundsType = BoundsDescription::LowerAndUpper;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"could not read lower bound value for variable '"+d.varName+"'");
      }
    } else {
      this->throwRuntimeError("BehaviourDSLCommon::treatBounds : ",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds",":");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds",
			    "expected to read upper bound value for variable "+d.varName);
    if(this->current->value=="*"){
      if(d.boundsType==BoundsDescription::Upper){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"upper and lower values bounds are both infinity. "
				"This is inconsistent.");
      }
      d.boundsType=BoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> d.upperBound;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"could not read upper bound value "
				"for variable '"+d.varName+"'");
      }
      if(d.boundsType==BoundsDescription::LowerAndUpper){
	if(d.lowerBound>d.upperBound){
	  this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				  "lower bound value is greater than upper "
				  "bound value for variable '"+
				  d.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds",
			      "Expected ']'.");
      if(this->current->value!="]"){
	this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
				"Expected ']' (read '"+this->current->value+"')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds",";");
  } // end of BehaviourDSLCommon::treatBounds
  
  void BehaviourDSLCommon::registerDefaultVarNames(void)
  {
    using namespace std;
    using namespace tfel::material;
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // all available tangent operators for finite strain behaviours
    const auto tos = getFiniteStrainBehaviourTangentOperatorFlags();

    this->mb.registerMemberName(h,"D");
    this->mb.registerMemberName(h,"Dt");

    this->reserveName("N");
    this->reserveName("Type");
    this->reserveName("use_qt");
    this->reserveName("src1");
    this->reserveName("src2");
    this->reserveName("policy_value");
    this->reserveName("integrate");
    this->mb.registerMemberName(h,"computeStress");
    this->mb.registerMemberName(h,"computeFinalStress");
    this->mb.registerMemberName(h,"computeStressFreeExpansion");
    this->mb.registerMemberName(h,"computeFdF");
    this->mb.registerMemberName(h,"updateIntegrationVariables");
    this->mb.registerMemberName(h,"updateStateVariables");
    this->mb.registerMemberName(h,"updateAuxiliaryStateVariables");
    this->mb.registerMemberName(h,"getTangentOperator");
    this->mb.registerMemberName(h,"getMinimalTimeStepScalingFactor");
    this->mb.registerMemberName(h,"computeAPrioriTimeStepScalingFactor");
    this->mb.registerMemberName(h,"computeAPrioriTimeStepScalingFactorII");
    this->mb.registerMemberName(h,"computeAPosterioriTimeStepScalingFactor");
    this->mb.registerMemberName(h,"computeAPosterioriTimeStepScalingFactorII");
    this->mb.registerMemberName(h,"computeConsistentTangentOperator");
    for(const auto& to : tos){
      const auto ktype = convertFiniteStrainBehaviourTangentOperatorFlagToString(to);
      this->mb.registerMemberName(h,ktype);
      this->mb.registerMemberName(h,"computeConsistentTangentOperator_"+ktype);
      this->mb.registerMemberName(h,"tangentOperator_"+ktype);
    }
    this->reserveName("tangentOperator_sk2");
    this->reserveName("computePredictionOperator");
    this->reserveName("computeTangentOperator_");
    this->reserveName("smt");
    this->reserveName("smflag");
    this->reserveName("dl_l0");
    this->reserveName("dl_l1");
    this->reserveName("dl_l01");
    this->reserveName("alpha_Ti");
    this->reserveName("alpha0_Ti");
    this->reserveName("alpha1_Ti");
    this->reserveName("alpha2_Ti");
    this->reserveName("alpha_T_t");
    this->reserveName("alpha_T_t_dt");
    this->reserveName("alpha0_T_t");
    this->reserveName("alpha0_T_t_dt");
    this->reserveName("alpha1_T_t");
    this->reserveName("alpha1_T_t_dt");
    this->reserveName("alpha2_T_t");
    this->reserveName("alpha2_T_t_dt");
    this->reserveName("TangentOperator");
    this->reserveName("StressFreeExpansionType");
    this->reserveName("behaviourData");
    this->reserveName("time_scaling_factor");
  } // end of BehaviourDSLCommon::registerDefaultVarNames

  BehaviourDSLCommon::BehaviourDSLCommon()
    : DSLBase(),
      useStateVarTimeDerivative(false),
      explicitlyDeclaredUsableInPurelyImplicitResolution(false)
  {
    // By default disable use of quantities
    this->mb.setUseQt(false);
    // By default, a behaviour can be used in a purely implicit resolution
    const Hypothesis h=ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setUsableInPurelyImplicitResolution(h,true);
    // reserve names
    for(const auto& v : DSLBase::getDefaultReservedNames()){
      this->mb.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,v);
    }
    // register behaviours specific names
    this->registerDefaultVarNames();
    this->reserveName("minimal_time_step_scaling_factor");
    this->reserveName("maximal_time_step_scaling_factor");
    this->reserveName("current_time_step_scaling_factor");
  } // end of BehaviourDSLCommon::MFrontParserCommon

  void BehaviourDSLCommon::reserveName(const std::string& n){
    this->mb.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,n);
  }
  
  void BehaviourDSLCommon::writeIncludes(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("BehaviourDSLCommon::writeIncludes : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    const auto& h = this->mb.getIncludes();
    if(!h.empty()){
      file << h << endl;
    }
  }

  void BehaviourDSLCommon::writeNamespaceBegin(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("BehaviourDSLCommon::writeNamespaceBegin : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "namespace tfel{\n";
    file << endl;
    file << "namespace material{\n";
    file << endl;
  }

  void BehaviourDSLCommon::writeNamespaceEnd(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("BehaviourDSLCommon::writeNamespaceEnd : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "} // end of namespace material\n";
    file << endl;
    file << "} // end of namespace tfel\n";
    file << endl;
  }

  void BehaviourDSLCommon::writeStandardTFELTypedefs(std::ofstream& file) 
  {
    using namespace std;
    if((!file)||(!file.good())){
      string msg("BehaviourDSLCommon::writeStandardTFELTypedefs : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "typedef unsigned short ushort;\n";
    if(this->mb.useQt()){        
      file << "typedef tfel::config::Types<N,Type,use_qt> Types;\n";
    } else {
      file << "typedef tfel::config::Types<N,Type,false> Types;\n";
    }
    file << "typedef typename Types::real                              real;\n";
    file << "typedef typename Types::time                              time;\n";
    file << "typedef typename Types::length                            length;\n";
    file << "typedef typename Types::frequency                         frequency;\n";
    file << "typedef typename Types::stress                            stress;\n";
    file << "typedef typename Types::strain                            strain;\n";
    file << "typedef typename Types::strainrate                        strainrate;\n";
    file << "typedef typename Types::stressrate                        stressrate;\n";
    file << "typedef typename Types::temperature                       temperature;\n";
    file << "typedef typename Types::thermalexpansion                  thermalexpansion;\n";
    file << "typedef typename Types::massdensity                       massdensity;\n";
    file << "typedef typename Types::TVector                           TVector;\n";
    file << "typedef typename Types::Stensor                           Stensor;\n";
    file << "typedef typename Types::Stensor4                          Stensor4;\n";
    file << "typedef typename Types::FrequencyStensor                  FrequencyStensor;\n";
    file << "typedef typename Types::ForceTVector                      ForceTVector;\n";
    file << "typedef typename Types::StressStensor                     StressStensor;\n";
    file << "typedef typename Types::StressRateStensor                 StressRateStensor;\n";
    file << "typedef typename Types::DisplacementTVector               DisplacementTVector;\n";
    file << "typedef typename Types::StrainStensor                     StrainStensor;\n";
    file << "typedef typename Types::StrainRateStensor                 StrainRateStensor;\n";
    file << "typedef typename Types::StiffnessTensor                   StiffnessTensor;\n";
    file << "typedef typename Types::Tensor                            Tensor;\n";
    file << "typedef typename Types::StressTensor                      StressTensor;\n";
    file << "typedef typename Types::ThermalExpansionCoefficientTensor ThermalExpansionCoefficientTensor;\n";
    file << "typedef typename Types::DeformationGradientTensor         DeformationGradientTensor;\n";
    // tangent operator
    file << "typedef " << this->mb.getTangentOperatorType() << " TangentOperator;\n";
  } // end of BehaviourDSLCommon::writeStandardTFELTypedefs

  BehaviourDSLCommon::~BehaviourDSLCommon()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();    
  } // end of BehaviourDSLCommon::~MFrontParserCommon

  void BehaviourDSLCommon::checkBehaviourDataFile() const {
    using namespace std;
    if((!this->behaviourDataFile)||
       (!this->behaviourDataFile.good())){
      string msg("BehaviourDSLCommon::checkBehaviourDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataFileHeader(){
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\file   " << this->behaviourDataFileName << endl;
    this->behaviourDataFile << "* \\brief  " << "this file implements the " 
			    << this->mb.getClassName() << "BehaviourData" << " class.\n";
    this->behaviourDataFile << "*         File generated by ";
    this->behaviourDataFile << MFrontHeader::getVersionName() << " ";
    this->behaviourDataFile << "version " << MFrontHeader::getVersionNumber();
    this->behaviourDataFile << endl;
    if(!this->authorName.empty()){
      this->behaviourDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourDataFile << "* \\date   " << this->date       << endl;
    }
    this->behaviourDataFile << " */\n";
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataFileHeaderBegin(){
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#ifndef LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->mb.getClassName());
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX\n";
    this->behaviourDataFile << "#define LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->mb.getClassName());
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX\n";
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataFileHeaderEnd()
  {
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "#endif /* LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->mb.getClassName());
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX */\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataStandardTFELIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#include<limits>\n";
    this->behaviourDataFile << "#include<string>\n";
    this->behaviourDataFile << "#include<sstream>\n";
    this->behaviourDataFile << "#include<iostream>\n";
    this->behaviourDataFile << "#include<stdexcept>\n";
    this->behaviourDataFile << "#include<algorithm>\n\n";
    this->behaviourDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Config/TFELTypes.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n";
    if(this->mb.useQt()){
      this->behaviourDataFile << "#include\"TFEL/Math/General/BaseCast.hxx\"\n";
    }
    this->mb.requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->behaviourDataFile << "#include\"TFEL/Math/tvector.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n";
    }
    if(b2){
      this->behaviourDataFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    this->behaviourDataFile << "#include\"TFEL/Math/stensor.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Math/Stensor/StensorConceptIO.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Math/tmatrix.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Math/st2tost2.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx\"\n";
    if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      this->behaviourDataFile << "#include\"TFEL/Math/tensor.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/Tensor/TensorConceptIO.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/t2tot2.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/T2toT2/T2toT2ConceptIO.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/t2tost2.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/T2toST2/T2toST2ConceptIO.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/st2tot2.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/ST2toT2/ST2toT2ConceptIO.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Math/ST2toST2/ConvertToTangentModuli.hxx\"\n";
      this->behaviourDataFile << "#include\"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx\"\n";
    }
    this->behaviourDataFile << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n";
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataDefaultMembers(void)
  {
    using namespace std;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p3;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "protected: \n\n";
    if(this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      this->behaviourDataFile << "StiffnessTensor D;\n";
    }
    if(this->mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      this->behaviourDataFile << "ThermalExpansionCoefficientTensor A;\n";
    }
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.type  << " " << p3->first.name << ";\n\n";
      } else {
	this->behaviourDataFile << p3->first.type  << " " << p3->first.name << "0;\n\n";
      }
      this->behaviourDataFile << p3->second.type << " " << p3->second.name << ";\n\n";
    }
    this->behaviourDataFile << "temperature T;\n";
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    this->behaviourDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourDataFile << "static " << constexpr_c << " unsigned short StensorSize = ";
    this->behaviourDataFile << "StensorDimeToSize::value;\n";
    this->behaviourDataFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->behaviourDataFile << "static " << constexpr_c << " unsigned short TensorSize = ";
    this->behaviourDataFile << "TensorDimeToSize::value;\n";
    this->behaviourDataFile << endl;
    this->writeStandardTFELTypedefs(this->behaviourDataFile);
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataDisabledConstructors(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
  }

  void BehaviourDSLCommon::writeBehaviourDataConstructors(const Hypothesis h)
  {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p3;
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\brief Default constructor\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData()\n";
    this->behaviourDataFile << "{}\n\n";
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\brief Copy constructor\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData(const ";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData& src)\n";
    this->behaviourDataFile << ": ";
    if(this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      this->behaviourDataFile << "D(src.D),\n";
    }
    if(this->mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      this->behaviourDataFile << "A(src.A),\n";
    }
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.name  << "(src." << p3->first.name << "),\n";
      } else {
	this->behaviourDataFile << p3->first.name  << "0(src." << p3->first.name << "0),\n";
      }
      this->behaviourDataFile << p3->second.name << "(src." << p3->second.name << "),\n";
    }
    this->behaviourDataFile << "T(src.T)";
    if(!md.getMaterialProperties().empty()){
      for(p=md.getMaterialProperties().begin();p!=md.getMaterialProperties().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!md.getStateVariables().empty()){
      for(p=md.getStateVariables().begin();p!=md.getStateVariables().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!md.getAuxiliaryStateVariables().empty()){
      for(p=md.getAuxiliaryStateVariables().begin();p!=md.getAuxiliaryStateVariables().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!md.getExternalStateVariables().empty()){
      for(p =md.getExternalStateVariables().begin();
	  p!=md.getExternalStateVariables().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    this->behaviourDataFile << endl;
    this->behaviourDataFile << "{}\n\n";
    // Creating constructor for external interfaces
    for(const auto& i : this->interfaces){
      if(i.second->isModellingHypothesisHandled(h,this->mb)){
	i.second->writeBehaviourDataConstructor(this->behaviourDataFile,h,this->mb);
      }
    }
  } // end of BehaviourDSLCommon::WriteBehaviourDataConstructors

  void BehaviourDSLCommon::writeBehaviourDataAssignementOperator(const Hypothesis h)
  {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p3;
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*\n";
    this->behaviourDataFile << "* \\brief Assignement operator\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData&\n";
    this->behaviourDataFile << "operator=(const " 
			    << this->mb.getClassName() << "BehaviourData& src){\n";
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << "this->" << p3->first.name  << " = src." << p3->first.name << ";\n";
      } else {
	this->behaviourDataFile << "this->" << p3->first.name  << "0 = src." << p3->first.name << "0;\n";
      }
      this->behaviourDataFile << "this->" << p3->second.name << " = src." << p3->second.name << ";\n";
    }
    if(!md.getMaterialProperties().empty()){
      for(p=md.getMaterialProperties().begin();p!=md.getMaterialProperties().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!md.getStateVariables().empty()){
      for(p=md.getStateVariables().begin();p!=md.getStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!md.getAuxiliaryStateVariables().empty()){
      for(p=md.getAuxiliaryStateVariables().begin();p!=md.getAuxiliaryStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!md.getExternalStateVariables().empty()){
      for(p =md.getExternalStateVariables().begin();
	  p!=md.getExternalStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    this->behaviourDataFile << "return *this;\n";
    this->behaviourDataFile << "}\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourAssignementOperator

  void BehaviourDSLCommon::writeBehaviourDataExport(const Hypothesis h)
  {
    this->checkBehaviourDataFile();
    for(const auto& i : this->interfaces){
      i.second->exportMechanicalData(this->behaviourDataFile,h,
				     this->mb);
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataPublicMembers(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    if(this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      this->behaviourDataFile << "StiffnessTensor&\n";
      this->behaviourDataFile << "getStiffnessTensor(void)\n";
      this->behaviourDataFile << "{\nreturn this->D;\n}\n\n";
      this->behaviourDataFile << "const StiffnessTensor&\n";
      this->behaviourDataFile << "getStiffnessTensor(void) const\n";
      this->behaviourDataFile << "{\nreturn this->D;\n}\n\n";
    }
    if(this->mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      this->behaviourDataFile << "ThermalExpansionCoefficientTensor&\n";
      this->behaviourDataFile << "getThermalExpansionCoefficientTensor(void)\n";
      this->behaviourDataFile << "{\nreturn this->A;\n}\n\n";
      this->behaviourDataFile << "const ThermalExpansionCoefficientTensor&\n";
      this->behaviourDataFile << "getThermalExpansionCoefficientTensor(void) const\n";
      this->behaviourDataFile << "{\nreturn this->A;\n}\n\n";
    }
  } // end of BehaviourDSLCommon::writeBehaviourDataPublicMembers

  void BehaviourDSLCommon::writeBehaviourDataClassHeader(void) 
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\class " << this->mb.getClassName() << "BehaviourData\n";
    this->behaviourDataFile << "* \\brief This class implements the " 
			    << this->mb.getClassName() << "BehaviourData" << " .\n";
    this->behaviourDataFile << "* \\param H, modelling hypothesis.\n";
    this->behaviourDataFile << "* \\param typename Type, numerical type.\n";
    this->behaviourDataFile << "* \\param bool use_qt, conditional saying if quantities are use.\n";
    if(!this->authorName.empty()){
      this->behaviourDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourDataFile << "* \\date   " << this->date << endl;
    }
    this->behaviourDataFile << "*/\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataForwardDeclarations(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "// Forward Declaration\n";
    this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
    this->behaviourDataFile << "class " << this->mb.getClassName() << "BehaviourData;\n\n";
    this->behaviourDataFile << "// Forward Declaration\n";
    this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
    this->behaviourDataFile << "class " << this->mb.getClassName() << "IntegrationData;\n\n";
    if(this->mb.useQt()){
      this->behaviourDataFile << "// Forward Declaration\n";
      this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->mb.getClassName() 
			      << "BehaviourData<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->behaviourDataFile << "// Forward Declaration\n";
      this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->mb.getClassName() 
			      << "BehaviourData<hypothesis,Type,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& h = this->mb.getModellingHypotheses();
    for(const auto & elem : h){
      if(this->mb.hasSpecialisedMechanicalData(elem)){
	if(this->mb.useQt()){
	  this->behaviourDataFile << "// Forward Declaration\n";
	  this->behaviourDataFile << "template<typename Type,bool use_qt>\n";
	  this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourDataFile << "const " << this->mb.getClassName() 
				  << "BehaviourData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(elem) << ",Type,use_qt>&);\n\n";
	} else {
	  this->behaviourDataFile << "// Forward Declaration\n";
	  this->behaviourDataFile << "template<typename Type>\n";
	  this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourDataFile << "const " << this->mb.getClassName() 
				  << "BehaviourData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(elem) << ",Type,false>&);\n\n";
	}
      }
    }
  }
    
  void BehaviourDSLCommon::writeBehaviourDataClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->behaviourDataFile << "class " << this->mb.getClassName() << "BehaviourData\n";
      } else {
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->behaviourDataFile << "class " << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,false>\n";
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<typename Type,bool use_qt>\n";
	this->behaviourDataFile << "class " << this->mb.getClassName()
				<< "BehaviourData<ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>\n";
      } else {
	this->behaviourDataFile << "template<typename Type>\n";
	this->behaviourDataFile << "class " << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ",Type,false>\n";
      }
    }
    this->behaviourDataFile << "{\n";
    this->behaviourDataFile << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->behaviourDataFile << "static " << constexpr_c << " ModellingHypothesis::Hypothesis hypothesis = " 
			      << "ModellingHypothesis::"
			      << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    this->behaviourDataFile << "static " << constexpr_c << " unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);\n";
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			    << "IsFundamentalNumericType<Type>::cond);\n";
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->behaviourDataFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData&);\n\n";
    this->behaviourDataFile << "/* integration data is declared friend to access"
			    << "   driving variables at the beginning of the time step */\n";
    if(this->mb.useQt()){
      this->behaviourDataFile << "friend class " << this->mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt>;\n\n";
    } else {
      this->behaviourDataFile << "friend class " << this->mb.getClassName() << "IntegrationData<hypothesis,Type,false>;\n\n";
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataClassEnd() {    
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "}; // end of " << this->mb.getClassName() << "BehaviourData" 
			    << "class\n";
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataMaterialProperties(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->mb.getBehaviourData(h).getMaterialProperties(),
				     "","",this->fileName,
				     false);

    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataStateVariables(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    const auto& d = this->mb.getBehaviourData(h);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     d.getStateVariables(),
				     "","",this->fileName,false);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     d.getAuxiliaryStateVariables(),
				     "","",this->fileName,false);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     d.getExternalStateVariables(),
				     "","",this->fileName,false);
    this->behaviourDataFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourDataOutputOperator(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p2;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->behaviourDataFile << "std::ostream&\n";
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,use_qt>& b)\n";
      } else {
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->behaviourDataFile << "std::ostream&\n";
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,false>& b)\n";
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<typename Type,bool use_qt>\n";
	this->behaviourDataFile << "std::ostream&\n";
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>& b)\n";
      } else {
	this->behaviourDataFile << "template<typename Type>\n";
	this->behaviourDataFile << "std::ostream&\n";
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>& b)\n";
      }
    }
    this->behaviourDataFile << "{\n";
    this->behaviourDataFile << "using namespace std;\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourDataFile << "os << \"" << p2->first.name  << " : \" << b." << p2->first.name  << " << '\\n';\n";
      } else {
	this->behaviourDataFile << "os << \"" << p2->first.name  << "0 : \" << b." << p2->first.name  << "0 << endl;\n";
      }
      this->behaviourDataFile << "os << \"" << p2->second.name << " : \" << b." << p2->second.name << " << '\\n';\n";
    }
    this->behaviourDataFile << "os << \"T : \" << b.T << endl;\n";
    for(p=d.getMaterialProperties().begin();p!=d.getMaterialProperties().end();++p){
      this->behaviourDataFile <<  "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << '\\n';\n";  
    }
    for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << '\\n';\n";  
    }    
    for(p=d.getAuxiliaryStateVariables().begin();
	p!=d.getAuxiliaryStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << '\\n';\n";  
    }    
    for(p=d.getExternalStateVariables().begin();
	p!=d.getExternalStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name << " << '\\n';\n";
    }
    this->behaviourDataFile << "return os;\n";
    this->behaviourDataFile << "}\n\n";
  } //  BehaviourDSLCommon::writeBehaviourDataOutputOperator

  void BehaviourDSLCommon::writeBehaviourDataFileBegin(void)
  {
    this->checkBehaviourDataFile();
    this->writeBehaviourDataFileHeader();
    this->writeBehaviourDataFileHeaderBegin();
    this->writeBehaviourDataStandardTFELIncludes();
    this->writeIncludes(this->behaviourDataFile);
    // includes specific to interfaces
    for(const auto& i : this->interfaces){
      i.second->writeInterfaceSpecificIncludes(this->behaviourDataFile,this->mb);
    }
    this->writeNamespaceBegin(this->behaviourDataFile);
    this->writeBehaviourDataForwardDeclarations();
  } // end of BehaviourDSLCommon::writeBehaviourDataFile

  void BehaviourDSLCommon::writeBehaviourDataClass(const Hypothesis h){
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeBehaviourDataClassBegin(h);
    this->writeBehaviourDataStandardTFELTypedefs();
    this->writeBehaviourDataDefaultMembers();
    this->writeBehaviourDataMaterialProperties(h);
    this->writeBehaviourDataStateVariables(h);
    this->behaviourDataFile << "public:\n\n";
    this->writeBehaviourDataDisabledConstructors();
    this->writeBehaviourDataConstructors(h);
    this->writeBehaviourDataMainVariablesSetters();
    this->writeBehaviourDataPublicMembers();
    this->writeBehaviourDataAssignementOperator(h);
    this->writeBehaviourDataExport(h);
    this->writeBehaviourDataClassEnd();
    this->writeBehaviourDataOutputOperator(h);
  }

  void BehaviourDSLCommon::writeBehaviourDataFileEnd(void)
  {
    this->writeNamespaceEnd(this->behaviourDataFile);
    this->writeBehaviourDataFileHeaderEnd();
  } // end of BehaviourDSLCommon::writeBehaviourDataFileEnd

  void BehaviourDSLCommon::checkBehaviourFile() const {
    using namespace std;
    if((!this->behaviourFile)||
       (!this->behaviourFile.good())){
      string msg("BehaviourDSLCommon::checkBehaviourDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void BehaviourDSLCommon::writeBehaviourForwardDeclarations(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "// Forward Declaration\n";
    this->behaviourFile << "template<ModellingHypothesis::Hypothesis,typename Type,bool use_qt>\n";
    this->behaviourFile << "class " << this->mb.getClassName() << ";\n\n";
    if(this->mb.useQt()){
      this->behaviourFile << "// Forward Declaration\n";
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
      this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->behaviourFile << "// Forward Declaration\n";
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
      this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& h = this->mb.getModellingHypotheses();
    for(const auto & elem : h){
      if(this->mb.hasSpecialisedMechanicalData(elem)){
	if(this->mb.useQt()){
	  this->behaviourFile << "// Forward Declaration\n";
	  this->behaviourFile << "template<typename Type,bool use_qt>\n";
	  this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourFile << "const " << this->mb.getClassName() 
			      << "<ModellingHypothesis::"
			      << ModellingHypothesis::toUpperCaseString(elem) << ",Type,use_qt>&);\n\n";
	} else {
	  this->behaviourFile << "// Forward Declaration\n";
	  this->behaviourFile << "template<typename Type>\n";
	  this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourFile << "const " << this->mb.getClassName() 
			      << "<ModellingHypothesis::"
			      << ModellingHypothesis::toUpperCaseString(elem) << ",Type,false>&);\n\n";
	}
      }
    }
  } // end of BehaviourDSLCommon::writeBehaviourClassForwardDeclarations

  void BehaviourDSLCommon::writeBehaviourClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\class " << this->mb.getClassName()    << endl;
    this->behaviourFile << "* \\brief This class implements the " 
			<< this->mb.getClassName() << " behaviour.\n";
    this->behaviourFile << "* \\param hypothesis, modelling hypothesis.\n";
    this->behaviourFile << "* \\param Type, numerical type.\n";
    if(this->mb.useQt()){    
      this->behaviourFile << "* \\param use_qt, conditional "
			  << "saying if quantities are use.\n";
    }
    if(!this->authorName.empty()){
      this->behaviourFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourFile << "* \\date   " << this->date << endl;
    }
    if(!this->description.empty()){
      this->behaviourFile << this->description << endl;
    }
    this->behaviourFile << "*/\n";
    const string btype = this->mb.getBehaviourTypeFlag();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->behaviourFile << "class " << this->mb.getClassName() << endl;
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>,\n";
	if(this->mb.getAttribute(BehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler,\n";
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt>,\n";
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt>";
	this->writeBehaviourParserSpecificInheritanceRelationship();
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->behaviourFile << "class " << this->mb.getClassName() 
			    << "<hypothesis,Type,false>\n";
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",hypothesis,Type,false>,\n";
	if(this->mb.getAttribute(BehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler,\n";
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false>,\n";
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<hypothesis,Type,false>";
	this->writeBehaviourParserSpecificInheritanceRelationship();
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourFile << "template<typename Type,bool use_qt>\n";
	this->behaviourFile << "class " << this->mb.getClassName()
			    << "<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>\n";
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>,\n";
	if(this->mb.getAttribute(BehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler,\n";
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>,\n";
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>";
	this->writeBehaviourParserSpecificInheritanceRelationship();
      } else {
	this->behaviourFile << "template<typename Type>\n";
	this->behaviourFile << "class " << this->mb.getClassName() 
			    << "<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>\n";
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>,\n";
	if(this->mb.getAttribute(BehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler,\n";
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>,\n";
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>";
      }
    }
    this->behaviourFile << "{\n";
    this->behaviourFile << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->behaviourFile << "static " << constexpr_c << " ModellingHypothesis::Hypothesis hypothesis = " 
			  << "ModellingHypothesis::"
			  << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    this->behaviourFile << "static " << constexpr_c << " unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->behaviourFile << endl;
    this->behaviourFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);\n";
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			<< "IsFundamentalNumericType<Type>::cond);\n";
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->behaviourFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourFile << this->mb.getClassName() << "&);\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourFileHeader(){
    using namespace std;

    this->checkBehaviourFile();

    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\file   " << this->behaviourFileName  << endl;
    this->behaviourFile << "* \\brief  " << "this file implements the " 
			<< this->mb.getClassName() << " Behaviour.\n";
    this->behaviourFile << "*         File generated by ";
    this->behaviourFile << MFrontHeader::getVersionName() << " ";
    this->behaviourFile << "version " << MFrontHeader::getVersionNumber();
    this->behaviourFile << endl;
    if(!this->authorName.empty()){
      this->behaviourFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourFile << "* \\date   " << this->date       << endl;
    }
    this->behaviourFile << " */\n";
    this->behaviourFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourFileHeaderBegin(){
    using namespace std;
    this->checkBehaviourFile();

    this->behaviourFile << "#ifndef LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->mb.getClassName());
    this->behaviourFile << "_HXX\n";
    this->behaviourFile << "#define LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->mb.getClassName());
    this->behaviourFile << "_HXX\n";
    this->behaviourFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourFileHeaderEnd() {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "#endif /* LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->mb.getClassName());
    this->behaviourFile << "_HXX */\n";
  }

  void BehaviourDSLCommon::writeBehaviourClassEnd() {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "}; // end of " << this->mb.getClassName() 
			<< " class\n";
    this->behaviourFile << endl;
  }
  
  void
  BehaviourDSLCommon::treatUpdateAuxiliaryStateVariables(void)
  {
    this->readCodeBlock(*this,BehaviourData::UpdateAuxiliaryStateVariables,
			&BehaviourDSLCommon::standardModifier,true,true);
  } // end of BehaviourDSLCommon::treatUpdateAuxiliaryStateVarBase

  void
  BehaviourDSLCommon::writeBehaviourUpdateIntegrationVariables(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update internal variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateIntegrationVariables(void)";
    if(!d.getIntegrationVariables().empty()){
      this->behaviourFile << "{\n";
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	if(!d.isStateVariableName(p->name)){
	  if(d.isMemberUsedInCodeBlocks(p->name)){
	    this->behaviourFile << "this->"  << p->name << " += ";
	    this->behaviourFile << "this->d" << p->name << ";\n";
	  }
	}
      }
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  } // end of BehaviourDSLCommon::writeBehaviourUpdateIntegrationVariables

  void
  BehaviourDSLCommon::writeBehaviourUpdateStateVariables(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update internal variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateStateVariables(void)";
    if(!d.getStateVariables().empty()){
      this->behaviourFile << "{\n";
      for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
	this->behaviourFile << "this->"  << p->name << " += ";
	this->behaviourFile << "this->d" << p->name << ";\n";
      }
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  } // end of BehaviourDSLCommon::writeBehaviourUpdateStateVariables

  void
  BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables(const Hypothesis h) 
  {
    using namespace std;
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update auxiliary state variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateAuxiliaryStateVariables(void)";
    if(this->mb.hasCode(h,BehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      writeMaterialLaws("BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables",
			this->behaviourFile,this->mb.getMaterialLaws());		      
      this->behaviourFile << this->mb.getCode(h,BehaviourData::UpdateAuxiliaryStateVariables) << endl;
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  } // end of  BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables

  bool
  BehaviourDSLCommon::hasUserDefinedTangentOperatorCode(const Hypothesis h) const
  {
    using tfel::material::getFiniteStrainBehaviourTangentOperatorFlags;
    if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      // all available tangent operators for finite strain behaviours
      const auto tos = getFiniteStrainBehaviourTangentOperatorFlags();
      // search tangent operators defined by the user
      for(auto pt=tos.cbegin();pt!=tos.cend();++pt){
	const auto ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	if(this->mb.hasCode(h,BehaviourData::ComputeTangentOperator+'-'+ktype)){
	  return true;
	}
      }
    } else {
      if(this->mb.hasCode(h,BehaviourData::ComputeTangentOperator)){
	return true;
      }
    }
    return false;
  } // end of BehaviourDSLCommon::hasUserDefinedTangentOperatorCode

  void BehaviourDSLCommon::writeBehaviourIntegrator(const Hypothesis h) {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    const auto& md = this->mb.getBehaviourData(h);
    vector<BoundsDescription>::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour  over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult\n";
    this->behaviourFile << "integrate(const SMFlag smflag, const SMType smt) override{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("BehaviourDSLCommon::writeBehaviourIntegrator",
		      this->behaviourFile,this->mb.getMaterialLaws());		      
    if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL)){
      if(this->mb.useQt()){
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n"
			    << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
			    << "}\n";
      } else {
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n"
			    << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
			    << "}\n";
      }
    }
    this->behaviourFile << "bool computeTangentOperator_ = smt!=NOSTIFFNESSREQUESTED;\n";
    if(this->mb.hasCode(h,BehaviourData::ComputePredictor)){
      this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputePredictor) << endl;
    }
    if(this->mb.hasCode(h,BehaviourData::Integrator)){
      this->behaviourFile << this->mb.getCode(h,BehaviourData::Integrator) << endl;
    }
    this->behaviourFile << "this->updateIntegrationVariables();\n";
    this->behaviourFile << "this->updateStateVariables();\n";
    this->behaviourFile << "this->updateAuxiliaryStateVariables();\n";
    for(p  = md.getBounds().begin();p !=md.getBounds().end();++p){
      if(p->varCategory==BoundsDescription::StateVariable){
    	p->writeBoundsChecks(this->behaviourFile);
      }
    }
    if(this->hasUserDefinedTangentOperatorCode(h)){
      this->behaviourFile << "if(computeTangentOperator_){\n";
      if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	this->behaviourFile << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
      } else {
	this->behaviourFile << "if(!this->computeConsistentTangentOperator(smt)){\n";
      }
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}\n";
      this->behaviourFile << "}\n";
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDisabledConstructors(void)
  {    
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Default constructor (disabled)\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << this->mb.getClassName() << "();\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Copy constructor (disabled)\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << this->mb.getClassName() << "(const ";
    this->behaviourFile << this->mb.getClassName()<< "&);\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Assignement operator (disabled)\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << this->mb.getClassName() << "& operator = (const ";
    this->behaviourFile << this->mb.getClassName()<< "&);\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourSetOutOfBoundsPolicy(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief set the policy for \"out of bounds\" conditions\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\nsetOutOfBoundsPolicy(const OutOfBoundsPolicy policy_value){\n";
    this->behaviourFile << "this->policy = policy_value;\n";
    this->behaviourFile << "} // end of setOutOfBoundsPolicy\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourOutOfBoundsEnumeration(void)

  void BehaviourDSLCommon::writeBehaviourCheckBounds(const Hypothesis h)
  {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief check bounds\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\ncheckBounds(void) const{\n";
    for(const auto& b : md.getBounds()){
      b.writeBoundsChecks(this->behaviourFile);
    }      
    this->behaviourFile << "} // end of checkBounds\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourCheckBounds(void)

  std::string
  BehaviourDSLCommon::getBehaviourConstructorsInitializers(const Hypothesis h)
  {
    const auto& md = this->mb.getBehaviourData(h);
    // variable initialisation
    auto init = this->getIntegrationVariablesIncrementsInitializers(md.getIntegrationVariables(),
								    this->useStateVarTimeDerivative);    
    if(!this->localVariablesInitializers.empty()){
      if(!init.empty()){
	init += ",\n";
      }
      init += this->localVariablesInitializers;
    }
    return init;
  } // end of BehaviourDSLCommon::getBehaviourConstructorsInitializers

  void BehaviourDSLCommon::writeBehaviourConstructors(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    // initializers
    const auto& init = this->getBehaviourConstructorsInitializers(h);;
    // writing constructors
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Constructor\n";
    this->behaviourFile << "*/\n";
    if(this->mb.useQt()){        
      this->behaviourFile << this->mb.getClassName() << "("
			  << "const " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,use_qt>& src1,\n"
			  << "const " << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,use_qt>& src2)"
			  << endl;
      this->behaviourFile << ": " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,use_qt>(src1),\n";
      this->behaviourFile << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,use_qt>(src2)";
    } else {
      this->behaviourFile << this->mb.getClassName() << "("
			  << "const " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,false>& src1,\n"
			  << "const " << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,false>& src2)"
			  << endl; 
      this->behaviourFile << ": " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,false>(src1),\n";
      this->behaviourFile << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,false>(src2)";
    }
    if(!init.empty()){
      this->behaviourFile << ",\n" <<init;
    }
    this->behaviourFile << "\n{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws("BehaviourDSLCommon::writeBehaviourConstructors",
		      this->behaviourFile,this->mb.getMaterialLaws());
    this->writeBehaviourParameterInitialisation(h);
    this->writeBehaviourLocalVariablesInitialisation(h);
    this->behaviourFile << "}\n\n";
    // constructor specific to interfaces
    for(const auto& i : this->interfaces){
      if(i.second->isModellingHypothesisHandled(h,this->mb)){
	i.second->writeBehaviourConstructor(this->behaviourFile,this->mb,init);
	this->behaviourFile << "\n{\n";
	this->behaviourFile << "using namespace std;\n";
	this->behaviourFile << "using namespace tfel::math;\n";
	this->behaviourFile << "using std::vector;\n";
	writeMaterialLaws("BehaviourDSLCommon::writeBehaviourConstructors",
			  this->behaviourFile,this->mb.getMaterialLaws());		      
	this->writeBehaviourParameterInitialisation(h);
	this->writeBehaviourLocalVariablesInitialisation(h);
	this->behaviourFile << "}\n\n";
      }
    }
  }

  static void
  BehaviourDSLCommonWriteThermalExpansionCoefficientComputation(std::ostream& out,
								   const MaterialPropertyDescription& a,
								   const std::string& T,
								   const std::string& i,
								   const std::string& suffix)
  {
    using namespace std;
    MFrontMaterialPropertyInterface minterface;
    const auto& mname = minterface.getFunctionName(a.material,a.law);
    out << "const thermalexpansion alpha" << suffix;
    if(!i.empty()){
      out << "_" << i;
    }
    out <<  "  = " << mname << "(";
    if(!a.inputs.empty()){
      if(a.inputs.size()!=1u){
	string msg("BehaviourDSLCommonWriteThermalExpansionCoefficientComputation : "
		   "a thermal expansion shall only depend on the temperature");
	throw(runtime_error(msg));
      }
      const auto& v = a.inputs.front();
      const auto& in = v.getExternalName(a.glossaryNames,a.entryNames);
      if(in!="Temperature"){
	string msg("BehaviourDSLCommonWriteThermalExpansionCoefficientComputation : "
		   "a thermal expansion shall only depend on the temperature");
	throw(runtime_error(msg));
      }
      out << T;
    }
    out << ");\n";
  } // end of BehaviourDSLCommonWriteThermalExpansionCoefficientComputation

  static void
  BehaviourDSLCommonWriteThermalExpansionCoefficientsComputations(std::ostream& out,
								     const MaterialPropertyDescription& a,
								     const std::string& suffix = "")
  {
    using namespace std;
    if(a.inputs.empty()){
      MFrontMaterialPropertyInterface minterface;
      const auto& mname = minterface.getFunctionName(a.material,a.law);
      out << "const thermalexpansion alpha" << suffix << "_Ti        = " << mname << "();\n";
      out << "const thermalexpansion alpha" << suffix << "_T_t       = " << mname << "();\n";
      out << "const thermalexpansion alpha" << suffix << "_T_t_dt    = " << mname << "();\n";
    } else {
      BehaviourDSLCommonWriteThermalExpansionCoefficientComputation(out,a,"this->referenceTemperatureForThermalExpansion",
								       "",suffix+"_Ti");
      BehaviourDSLCommonWriteThermalExpansionCoefficientComputation(out,a,"this->T","t",suffix+"_T");
      BehaviourDSLCommonWriteThermalExpansionCoefficientComputation(out,a,"this->T+this->dT",
								       "t_dt",suffix+"_T");
    }
  } // end of BehaviourDSLCommonWriteThermalExpansionCoefficientComputation

  static void
  BehaviourDSLCommonWriteThermalExpansionComputation(std::ostream& out,
							const MaterialPropertyDescription& a,
							const std::string& t,
							const std::string& c,
							const std::string& suffix = "")
  {
    using namespace std;
    string Tref;
    string T;
    if(a.staticVars.contains("ReferenceTemperature")){
      ostringstream Tref_value;
      Tref_value << a.staticVars.get("ReferenceTemperature").value;
      Tref = Tref_value.str();
    } else {
      Tref = "293.15";
    }
    if(t=="t"){
      out << "dl_l0";
      T = "this->T";
    } else {
      out << "dl_l1";
      T = "this->T+this->dT";
    }
    out << "[" << c << "] = 1/(1+alpha" << suffix << "_Ti * (this->referenceTemperatureForThermalExpansion-" << Tref << "))*("
	<< "alpha" << suffix << "_T_"  << t << " * (" << T << "-" << Tref << ")-"
	<< "alpha" << suffix << "_Ti * (this->referenceTemperatureForThermalExpansion-" << Tref << "));\n";
  }
  
  void BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion(void)
  {    
    using namespace std;
    bool b = this->mb.areThermalExpansionCoefficientsDefined();
    if(b){
      if(!((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	   (this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
	this->throwRuntimeError("BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion",
				"only finite strain or small strain behaviour are supported");
      }
      this->checkBehaviourFile();
      this->behaviourFile << "void\n"
			  << "computeStressFreeExpansion(std::pair<StressFreeExpansionType,StressFreeExpansionType>& dl_l01)\n{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion",
			this->behaviourFile,this->mb.getMaterialLaws());		      
      this->behaviourFile << "auto& dl_l0 = dl_l01.first;\n";
      this->behaviourFile << "auto& dl_l1 = dl_l01.second;\n";
      this->behaviourFile << "dl_l0 = StressFreeExpansionType(typename StressFreeExpansionType::value_type(0));\n";
      this->behaviourFile << "dl_l1 = StressFreeExpansionType(typename StressFreeExpansionType::value_type(0));\n";
      const vector<shared_ptr<MaterialPropertyDescription> >& acs =
	this->mb.getThermalExpansionCoefficients();
      if(this->mb.getSymmetryType()==mfront::ISOTROPIC){
	if(acs.size()!=1u){
	  this->throwRuntimeError("BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion",
				  "invalid number of thermal expansion coefficients");
	}
	BehaviourDSLCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
									   *(acs.front()));
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs.front()),"t","0");
	this->behaviourFile << "dl_l0[1] = dl_l0[0];\n"
			    << "dl_l0[2] = dl_l0[0];\n";
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs.front()),"t_dt","0");
	this->behaviourFile << "dl_l1[1] = dl_l1[0];\n"
			    << "dl_l1[2] = dl_l1[0];\n";
      } else if(this->mb.getSymmetryType()==mfront::ORTHOTROPIC){
	if(acs.size()!=3u){
	  this->throwRuntimeError("BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion",
				  "invalid number of thermal expansion coefficients");
	}
	BehaviourDSLCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
									   *(acs[0]),"0");
	BehaviourDSLCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
									   *(acs[1]),"1");
	BehaviourDSLCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
									   *(acs[2]),"2");
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[0]),"t","0","0");
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[0]),"t_dt","0","0");
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[1]),"t","1","1");
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[1]),"t_dt","1","1");
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[2]),"t","2","2");
	BehaviourDSLCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[2]),"t_dt","2","2");
      } else {
	this->throwRuntimeError("BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion",
				"unsupported behaviour symmetry");
      }
      this->behaviourFile << "}\n\n";
    }
  } // end of BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion

  void BehaviourDSLCommon::writeBehaviourInitializeMethod(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n"
			<< " * \\ brief initialize the behaviour with user code\n"
			<< " */\n"
			<< "void initialize(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws("BehaviourDSLCommon::writeBehaviourInitializeMethod",
		      this->behaviourFile,this->mb.getMaterialLaws());		      
    if(this->mb.hasCode(h,BehaviourData::BeforeInitializeLocalVariables)){
      if(this->mb.getAttribute(BehaviourData::profiling,false)){
	writeStandardPerformanceProfilingBegin(this->behaviourFile,this->mb.getClassName(),
					       BehaviourData::BeforeInitializeLocalVariables,
					       "binit");
      }
      this->behaviourFile << this->mb.getCodeBlock(h,BehaviourData::BeforeInitializeLocalVariables).code
			  << endl;
      if(this->mb.getAttribute(BehaviourData::profiling,false)){
	writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if(this->mb.hasCode(h,BehaviourData::InitializeLocalVariables)){
      if(this->mb.getAttribute(BehaviourData::profiling,false)){
	writeStandardPerformanceProfilingBegin(this->behaviourFile,this->mb.getClassName(),
					       BehaviourData::InitializeLocalVariables,"init");
      }
      this->behaviourFile << this->mb.getCodeBlock(h,BehaviourData::InitializeLocalVariables).code
			  << endl;
      if(this->mb.getAttribute(BehaviourData::profiling,false)){
	writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if(this->mb.hasCode(h,BehaviourData::AfterInitializeLocalVariables)){
      if(this->mb.getAttribute(BehaviourData::profiling,false)){
	writeStandardPerformanceProfilingBegin(this->behaviourFile,this->mb.getClassName(),
					       BehaviourData::AfterInitializeLocalVariables,"ainit");
      }
      this->behaviourFile << this->mb.getCodeBlock(h,BehaviourData::AfterInitializeLocalVariables).code
			  << endl;
      if(this->mb.getAttribute(BehaviourData::profiling,false)){
	writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    this->writeBehaviourParserSpecificInitializeMethodPart(h);
    this->behaviourFile << "}\n\n";
  } // end of void BehaviourDSLCommon::writeBehaviourInitializeMethod

  void
  BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(const Hypothesis h)
  {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourFile();
    for(p=md.getLocalVariables().begin();p!=md.getLocalVariables().end();++p){
      if(this->useDynamicallyAllocatedVector(p->arraySize)){
	this->behaviourFile << "this->" << p->name << ".resize(" << p->arraySize << ");\n";
      }
    }  
  }
  
  void BehaviourDSLCommon::writeBehaviourParameterInitialisation(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    const auto& d = this->mb.getBehaviourData(h);
    const auto& params = d.getParameters();
    VariableDescriptionContainer::const_iterator p;
    for(p=params.begin();p!=params.end();++p){
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	this->behaviourFile << "this->" << p->name << " = " << this->mb.getClassName() 
			    << "ParametersInitializer::get()." << p->name << ";\n";  
      } else {
	if(this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)){
	  this->behaviourFile << "this->" << p->name << " = " << this->mb.getClassName() 
			      << "ParametersInitializer::get()." << p->name << ";\n";  
	} else {
	  this->behaviourFile << "this->" << p->name << " = " << this->mb.getClassName() 
			      << ModellingHypothesis::toString(h) 
			      << "ParametersInitializer::get()." << p->name << ";\n";
	}
      }
    }
  } // end of BehaviourDSLCommon::writeBehaviourParameterInitialisation

  void BehaviourDSLCommon::writeBehaviourDataMainVariablesSetters(void)
  {
    using std::endl;
    this->checkBehaviourDataFile();
    for(const auto& i : this->interfaces){
      i.second->writeBehaviourDataMainVariablesSetters(this->behaviourDataFile,
						       this->mb);
      this->behaviourDataFile << endl;
    }
  } // end of BehaviourDSLCommon::writeBehaviourDataMainVariablesSetters

  void BehaviourDSLCommon::writeIntegrationDataMainVariablesSetters(void)
  {
    using std::endl;
    this->checkIntegrationDataFile();
    for(const auto& i : this->interfaces){
      i.second->writeIntegrationDataMainVariablesSetters(this->integrationDataFile,
							 this->mb);
      this->integrationDataFile << endl;
    }
  } // end of BehaviourDSLCommon::writeIntegrationDataMainVariablesSetters

  void BehaviourDSLCommon::writeBehaviourGetModellingHypothesis()
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief set the policy for \"out of bounds\" conditions\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "ModellingHypothesis::Hypothesis\ngetModellingHypothesis(void) const{\n";
    this->behaviourFile << "return hypothesis;\n";
    this->behaviourFile << "} // end of getModellingHypothesis\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourGetModellingHypothesis();

  void BehaviourDSLCommon::writeBehaviourLocalVariables(const Hypothesis h)
  {    
    this->checkBehaviourFile();
    const auto& md = this->mb.getBehaviourData(h);
    this->writeVariablesDeclarations(this->behaviourFile,
				     md.getLocalVariables(),
				     "","",this->fileName,false);
    this->behaviourFile << '\n';
  }

  void
  BehaviourDSLCommon::writeBehaviourIntegrationVariables(const Hypothesis h)
  {
    this->checkBehaviourFile();
    const auto& md = this->mb.getBehaviourData(h);
    for(const auto& v : md.getIntegrationVariables()){
      if(!md.isStateVariableName(v.name)){
	if(md.isMemberUsedInCodeBlocks(v.name)){
	  this->writeVariableDeclaration(this->behaviourFile,
					 v,"","",this->fileName,false);
	}
      }
    }
    this->behaviourFile << '\n';
  } // end od BehaviourDSLCommon::writeBehaviourIntegrationVariables

  void BehaviourDSLCommon::writeBehaviourParameters(const Hypothesis h)
  {    
    this->checkBehaviourFile();
    const auto& d = this->mb.getBehaviourData(h);
    for(const auto& v : d.getParameters()){
      if(!getDebugMode()){
	if(v.lineNumber!=0u){
	  this->behaviourFile << "#line " << v.lineNumber << " \"" 
			      << this->fileName << "\"\n";
	}
      }
      this->behaviourFile << v.type << " " << v.name << ";\n";  
    }
    this->behaviourFile << '\n';
  }

  
  void BehaviourDSLCommon::writeBehaviourPolicyVariable(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "//! policy for treating out of bounds conditions\n";
    this->behaviourFile << "OutOfBoundsPolicy policy = None;\n";
  } // end of BehaviourDSLCommon::writeBehaviourPolicyVariable

  void BehaviourDSLCommon::writeBehaviourStaticVariables(const Hypothesis h)
  {    
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    for(const auto& v : md.getStaticVariables()){
      if(!getDebugMode()){
	if(v.lineNumber!=0u){
	  this->behaviourFile << "#line " << v.lineNumber << " \"" 
			      << this->fileName << "\"\n";
	}
      }
      if(v.type=="int"){
	this->behaviourFile << "static constexpr " << v.type << " " << v.name
			    << " = " << v.value << ";\n";
      } else {
	this->behaviourFile << "static const " << v.type << " " << v.name << ";\n";
      }
    }
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourIntegrationVariablesIncrements(const Hypothesis h)
  {    
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(this->behaviourFile,
				     md.getIntegrationVariables(),
				     "d","",this->fileName,
				     this->useStateVarTimeDerivative);
    this->behaviourFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourOutputOperator(const Hypothesis h)
  {    
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->behaviourFile << "std::ostream&\n";
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,use_qt>& b)\n";
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->behaviourFile << "std::ostream&\n";
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,false>& b)\n";
      }
    } else {
      if(this->mb.useQt()){        
	this->behaviourFile << "template<typename Type,bool use_qt>\n";
	this->behaviourFile << "std::ostream&\n";
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName()
			    << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>& b)\n";
      } else {
	this->behaviourFile << "template<typename Type>\n";
	this->behaviourFile << "std::ostream&\n";
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName() 
			    << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>& b)\n";
      }
    }
    this->behaviourFile << "{\n";
    this->behaviourFile << "using namespace std;\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourFile << "os << \""  << p2->first.name << " : \" << b." << p2->first.name << " << '\\n';\n";
	this->behaviourFile << "os << \"d" << p2->first.name << " : \" << b.d" << p2->first.name << " << '\\n';\n";
      } else {
	this->behaviourFile << "os << \"" << p2->first.name << "0 : \" << b." << p2->first.name << "0 << endl;\n";
	this->behaviourFile << "os << \"" << p2->first.name << "1 : \" << b." << p2->first.name << "1 << endl;\n";
      }
      this->behaviourFile << "os << \"" << p2->second.name << " : \" << b." << p2->second.name << " << '\\n';\n";
    }
    this->behaviourFile << "os << \"dt : \" << b.dt << endl;\n";
    this->behaviourFile << "os << \"T : \" << b.T << endl;\n";
    this->behaviourFile << "os << \"dT : \" << b.dT << endl;\n";
    for(p=md.getMaterialProperties().begin();p!=md.getMaterialProperties().end();++p){
      this->behaviourFile <<  "os << \"" << p->name << " : \" << b." << p->name <<  " << '\\n';\n";  
    }
    for(p=md.getStateVariables().begin();
	p!=md.getStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << '\\n';\n";  
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name <<  " << '\\n';\n";  
    }    
    for(p=md.getAuxiliaryStateVariables().begin();
	p!=md.getAuxiliaryStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << '\\n';\n";  
    }
    for(p=md.getExternalStateVariables().begin();
	p!=md.getExternalStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name << " << '\\n';\n";
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name << " << '\\n';\n";  
    }
    for(p=md.getLocalVariables().begin();p!=md.getLocalVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << '\\n';\n";  
    }
    for(p=md.getParameters().begin();p!=md.getParameters().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name << " << '\\n';\n";  
    }
    this->behaviourFile << "return os;\n";
    this->behaviourFile << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDestructor(void) 
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Destructor\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "~" << this->mb.getClassName() << "()\n" << "{}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourUpdateExternalStateVariables(const Hypothesis h) 
  {    
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    this->behaviourFile << "void updateExternalStateVariables(void){\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourFile << "this->" << p2->first.name  << "  += this->d" << p2->first.name << ";\n";
      } else {
	this->behaviourFile << "this->" << p2->first.name  << "0  = this->"  << p2->first.name << "1;\n";
      }
    }
    this->behaviourFile << "this->T   += this->dT;\n";
    for(p=md.getExternalStateVariables().begin();p!=md.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << " += this->d" << p->name << ";\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourIncludes(void) {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "#include<string>\n";
    this->behaviourFile << "#include<iostream>\n";
    this->behaviourFile << "#include<limits>\n";
    this->behaviourFile << "#include<stdexcept>\n";
    this->behaviourFile << "#include<algorithm>\n\n";
    this->behaviourFile << "#include\"TFEL/Config/TFELConfig.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Config/TFELTypes.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/MaterialException.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/MechanicalBehaviour.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/MechanicalBehaviourTraits.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/BoundsCheck.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/Lame.hxx\"\n";
    if(this->mb.getSymmetryType()==ORTHOTROPIC){
      this->behaviourFile << "#include\"TFEL/Material/Hill.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Material/OrthotropicAxesConvention.hxx\"\n";
    }
    this->behaviourFile << "#include\"" << this->behaviourDataFileName << "\"\n";
    this->behaviourFile << "#include\"" << this->integrationDataFileName << "\"\n";
    if(this->mb.getAttribute<bool>(BehaviourData::profiling,false)){
      this->behaviourFile << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    this->behaviourFile << endl;
  }

  void BehaviourDSLCommon::writeBehaviourMembersFunc(void) {    
    using namespace std;
    const auto& m = this->mb.getMembers();
    this->checkBehaviourFile();
    if(!m.empty()){
      this->behaviourFile << m << endl << endl;
    }
  }

  void BehaviourDSLCommon::writeBehaviourPrivate(void) {    
    using namespace std;
    const auto& c = this->mb.getPrivateCode();
    this->checkBehaviourFile();
    if(!c.empty()){
      this->behaviourFile << c << endl << endl;
    }
  } // end of void BehaviourDSLCommon::writeBehaviourPrivate

  void BehaviourDSLCommon::writeBehaviourStandardTFELTypedefs(void)
  {
    using namespace std;
    using namespace tfel::material;
    this->checkBehaviourFile();
    const string btype = this->mb.getBehaviourTypeFlag();
    this->behaviourFile << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static " << constexpr_c << " unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n";
    this->behaviourFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->behaviourFile << "static " << constexpr_c << " unsigned short TensorSize = ";
    this->behaviourFile << "TensorDimeToSize::value;\n";
    this->behaviourFile << endl;
    this->writeStandardTFELTypedefs(this->behaviourFile);
    this->behaviourFile << endl;
    this->behaviourFile << "public :\n\n";
    const string qt = this->mb.useQt() ? "use_qt" : "false";
    this->behaviourFile << "typedef " << this->mb.getClassName() 
			<< "BehaviourData<hypothesis,Type," << qt << "> BehaviourData;\n";
    this->behaviourFile << "typedef " << this->mb.getClassName() 
			<< "IntegrationData<hypothesis,Type," << qt << "> IntegrationData;\n";
    this->behaviourFile << "typedef typename MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SMFlag SMFlag;\n";
    this->behaviourFile << "typedef typename MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SMType SMType;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::ELASTIC;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SECANTOPERATOR;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::TANGENTOPERATOR;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::CONSISTENTTANGENTOPERATOR;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::NOSTIFFNESSREQUESTED;\n";
    if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL)){
      this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::STANDARDTANGENTOPERATOR;\n";
    } else if (this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      const vector<FiniteStrainBehaviourTangentOperatorBase::Flag> toflags = getFiniteStrainBehaviourTangentOperatorFlags();
      for(const auto & toflag : toflags){
	this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::"
			    << convertFiniteStrainBehaviourTangentOperatorFlagToString(toflag) << ";\n";
      }
    }
    this->behaviourFile << "typedef typename MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::IntegrationResult IntegrationResult;\n\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SUCCESS;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::FAILURE;\n";
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::UNRELIABLE_RESULTS;\n\n";
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      this->behaviourFile << "typedef " << this->mb.getStressFreeExpansionType()  << " StressFreeExpansionType;\n\n";
    }
    this->behaviourFile << "private :\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourStandardTFELTypedefs

  void BehaviourDSLCommon::writeBehaviourTraits(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    const auto& ah = ModellingHypothesis::getModellingHypotheses();
    // writing partial specialisations
    if(this->mb.getModellingHypotheses().size()>=4u){
      // on définit toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(ModellingHypothesis::UNDEFINEDHYPOTHESIS,true);
      // unsupported hypothesis
      vector<ModellingHypothesis::Hypothesis>::const_iterator p;
      for(p=ah.begin();p!=ah.end();++p){
    	if(this->mb.isModellingHypothesisSupported(*p)){
	  if(this->mb.hasSpecialisedMechanicalData(*p)){
	    this->writeBehaviourTraitsSpecialisation(*p,true);
	  }
    	} else {
    	  this->writeBehaviourTraitsSpecialisation(*p,false);
	}
      }
    } else {
      // on exclut toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(ModellingHypothesis::UNDEFINEDHYPOTHESIS,false);
      // unsupported hypothesis
      set<ModellingHypothesis::Hypothesis>::const_iterator p;
      for(p=this->mb.getModellingHypotheses().begin();p!=this->mb.getModellingHypotheses().end();++p){
    	this->writeBehaviourTraitsSpecialisation(*p,true);
      }
    }
  }

  void
  BehaviourDSLCommon::writeBehaviourTraitsSpecialisation(const tfel::material::ModellingHypothesis::Hypothesis h,
							 const bool b)
  {
    using namespace std;
    using tfel::material::OrthotropicAxesConvention;
    SupportedTypes::TypeSize coefSize;
    SupportedTypes::TypeSize stateVarsSize;
    SupportedTypes::TypeSize externalStateVarsSize;
    if(b){
      const auto& d = this->mb.getBehaviourData(h);
      for(const auto& m : d.getMaterialProperties()){
	coefSize+=this->getTypeSize(m.type,m.arraySize);
      }
      for(const auto& v : d.getPersistentVariables()){
	stateVarsSize+=this->getTypeSize(v.type,v.arraySize);
      }
      for(const auto& v : d.getExternalStateVariables()){
	externalStateVarsSize+=this->getTypeSize(v.type,v.arraySize);
      }
    }
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* Partial specialisation for ";
    this->behaviourFile << this->mb.getClassName() << ".\n";
    this->behaviourFile << "*/\n";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<hypothesis,Type,use_qt> >\n";
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<hypothesis,Type,false> >\n";
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourFile << "template<typename Type,bool use_qt>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt> >\n";
      } else {
	this->behaviourFile << "template<typename Type>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false> >\n";
      }
    }
    this->behaviourFile << "{\n";
    if(b){
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	this->behaviourFile << "static " << constexpr_c << " unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
      } else {
	this->behaviourFile << "static " << constexpr_c << " unsigned short N = ModellingHypothesisToSpaceDimension<"
			    << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ">::value;\n";
      }
      this->behaviourFile << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
      this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
      this->behaviourFile << "static " << constexpr_c << " unsigned short StensorSize = ";
      this->behaviourFile << "StensorDimeToSize::value;\n";
      this->behaviourFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
      this->behaviourFile << "static " << constexpr_c << " unsigned short TensorSize = ";
      this->behaviourFile << "TensorDimeToSize::value;\n";
    }
    this->behaviourFile << "public:\n";
    if(b){
      this->behaviourFile << "static " << constexpr_c << " bool is_defined = true;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c << " bool is_defined = false;\n";
    }
    if(this->mb.useQt()){
      this->behaviourFile << "static " << constexpr_c << " bool use_quantities = use_qt;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c << " bool use_quantities = false;\n";
    }
    if(this->mb.getSymmetryType()==mfront::ORTHOTROPIC){
      this->behaviourFile << "//! orthotropic axes convention\n";
      if(this->mb.getOrthotropicAxesConvention()==OrthotropicAxesConvention::DEFAULT){
	this->behaviourFile << "static " << constexpr_c
			    << " OrthotropicAxesConvention oac = OrthotropicAxesConvention::DEFAULT;\n";
      } else if(this->mb.getOrthotropicAxesConvention()==OrthotropicAxesConvention::PIPE){
	this->behaviourFile << "static " << constexpr_c
			    << " OrthotropicAxesConvention oac = OrthotropicAxesConvention::PIPE;\n";
      } else {
	throw(runtime_error("BehaviourDSLCommon::writeBehaviourTraitsSpecialisation : "
			    "internal error : unsupported orthotropic axes convention"));
      }
    }
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      this->behaviourFile << "static " << constexpr_c << " bool hasStressFreeExpansion = true;\n";    
    } else {
      this->behaviourFile << "static " << constexpr_c << " bool hasStressFreeExpansion = false;\n";
    }
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      this->behaviourFile << "static " << constexpr_c << " bool handlesThermalExpansion = true;\n";    
    } else {
      this->behaviourFile << "static " << constexpr_c << " bool handlesThermalExpansion = false;\n";
    }
    if(b){
      this->behaviourFile << "static " << constexpr_c << " unsigned short dimension = N;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c << " unsigned short dimension = 0u;\n";
    }
    this->behaviourFile << "typedef Type NumType;\n";
    this->behaviourFile << "static " << constexpr_c << " unsigned short material_properties_nb = ";
    this->behaviourFile << coefSize << ";\n";
    this->behaviourFile << "static " << constexpr_c << " unsigned short internal_variables_nb  = ";
    this->behaviourFile << stateVarsSize << ";\n";
    this->behaviourFile << "static " << constexpr_c << " unsigned short external_variables_nb  = ";
    this->behaviourFile << externalStateVarsSize << ";\n";
    this->behaviourFile << "static " << constexpr_c << " bool hasConsistentTangentOperator = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,BehaviourData::hasConsistentTangentOperator,false)){
	this->behaviourFile << "true;\n";
      } else {
	this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c << " bool isConsistentTangentOperatorSymmetric = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,BehaviourData::isConsistentTangentOperatorSymmetric,false)){
	this->behaviourFile << "true;\n";
      } else {
	this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c << " bool hasPredictionOperator = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,BehaviourData::hasPredictionOperator,false)){
	this->behaviourFile << "true;\n";
      } else {
	this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c << " bool hasAPrioriTimeStepScalingFactor = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,BehaviourData::hasAPrioriTimeStepScalingFactor,false)){
	this->behaviourFile << "true;\n";
      } else {
	this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c << " bool hasAPosterioriTimeStepScalingFactor = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,BehaviourData::hasAPosterioriTimeStepScalingFactor,false)){
	this->behaviourFile << "true;\n";
      } else {
	this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "/*!\n"
			<< "* \\return the name of the class.\n"
			<< "*/\n"
			<< "static const char*\n"
			<< "getName(void){\n"
			<< "return \"" << this->mb.getClassName() << "\";\n"
			<< "}\n\n";
    this->behaviourFile << "};\n\n";
  }

  void 
  BehaviourDSLCommon::writeBehaviourParserSpecificInheritanceRelationship(void)
  {
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourParserSpecificTypedefs(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourParserSpecificMembers(const Hypothesis)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourParserSpecificIncludes(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourParametersInitializers(void)
  {
    if(!this->mb.hasParameters()){
      return;
    }
    auto h = this->mb.getDistinctModellingHypotheses();
    h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for(const auto p : h){
      if(this->mb.hasParameters(p)){
	this->writeBehaviourParametersInitializer(p);
      }
    }
  } // end of BehaviourDSLCommon::writeBehaviourParametersInitializers

  void 
  BehaviourDSLCommon::writeBehaviourParametersInitializer(const Hypothesis h)
  {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    const auto& params = md.getParameters();
    string cname(this->mb.getClassName());
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    VariableDescriptionContainer::const_iterator p;
    bool rp = false;
    bool ip = false;
    bool up = false;
    bool rp2 = false;
    bool ip2 = false;
    bool up2 = false;
    this->checkBehaviourFile();
    this->behaviourFile << "struct " << cname << endl
			<< "{\n"
			<< "static " << cname << "&\n"
			<< "get();\n\n";
    for(p=params.begin();p!=params.end();++p){
      if(p->type=="real"){
	rp = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  rp2=true;
	  this->behaviourFile << "double " << p->name << ";\n"; 
	}
      } else if(p->type=="int"){
	ip = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  ip2 = true;
	  this->behaviourFile << "double " << p->name << ";\n"; 
	}
      } else  if(p->type=="ushort"){
	up = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  up2 = true;
	  this->behaviourFile << "unsigned short " << p->name << ";\n"; 
	}
      } else {
	string msg("BehaviourDSLCommon::writeBehaviourParametersInitializer : ");
	msg += "invalid type for parameter '"+p->name+"' ('"+p->type+"')";
	throw(runtime_error(msg));
      }
    }
    if(!params.empty()){
      this->behaviourFile << endl; 
    }
    if(rp){
      this->behaviourFile << "void set(const char* const,const double);\n\n";
    }
    if(ip){
      this->behaviourFile << "void set(const char* const,const int);\n\n";
    }
    if(up){
      this->behaviourFile << "void set(const char* const,const unsigned short);\n\n";
    }
    if(rp2){
      this->behaviourFile << "/*!\n"
			  << " * \\brief convert a string to double\n"
			  << " * \\param[in] p : parameter\n"
			  << " * \\param[in] v : value\n"
			  << " */\n"
			  << "static double getDouble(const std::string&,const std::string&);\n";
    }
    if(ip2){
      this->behaviourFile << "/*!\n"
			  << " * \\brief convert a string to int\n"
			  << " * \\param[in] p : parameter\n"
			  << " * \\param[in] v : value\n"
			  << " */\n"
			  << "static int getInt(const std::string&,const std::string&);\n";
    }
    if(up2){
      this->behaviourFile << "/*!\n"
			  << " * \\brief convert a string to unsigned short\n"
			  << " * \\param[in] p : parameter\n"
			  << " * \\param[in] v : value\n"
			  << " */\n"
			  << "static unsigned short getUnsignedShort(const std::string&,const std::string&);\n";
    }
    this->behaviourFile << "private :\n\n"
			<< cname << "();\n\n"
			<< cname << "(const " << cname << "&);\n\n"
			<< cname << "&\n"
			<< "operator=(const " << cname << "&);\n"
			<< "/*!\n"
			<< " * \\brief read the parameters from the given file\n"
			<< " * \\param[out] pi : parameters initializer\n"
			<< " * \\param[in]  fn : file name\n"
			<< " */\n"
			<< "static void readParameters(" << cname << "&,const char* const);\n";
    this->behaviourFile << "};\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourParametersInitializer

  void BehaviourDSLCommon::writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void
  BehaviourDSLCommon::writeBehaviourFileBegin(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->writeBehaviourFileHeader();
    this->writeBehaviourFileHeaderBegin();
    this->writeBehaviourIncludes();
    this->writeBehaviourParserSpecificIncludes();
    this->writeIncludes(this->behaviourFile);
    // includes specific to interfaces
    for(const auto& i : this->interfaces){
      i.second->writeInterfaceSpecificIncludes(this->behaviourFile,this->mb);
    }
    this->writeNamespaceBegin(this->behaviourFile);
    this->writeBehaviourParametersInitializers();
    this->writeBehaviourForwardDeclarations();
    this->writeBehaviourProfiler();
  } // end of BehaviourDSLCommon::writeBehaviourFileBegin

  void BehaviourDSLCommon::writeBehaviourProfiler()
  {
    using namespace std;
    if(this->mb.getAttribute(BehaviourData::profiling,false)){
      this->checkBehaviourFile();
      this->behaviourFile << "/*!\n"
			  << " * " << this->mb.getClassName() << " profiler\n"
			  << " */\n"
			  << "struct " << this->mb.getClassName() << "Profiler\n"
			  << "{\n"
			  << "//! return the profiler associated with the behaviour\n"
			  << "static mfront::BehaviourProfiler&\n"
			  << "getProfiler(void);\n"
			  << "}; // end of struct " << this->mb.getClassName() << "Profiler"
			  << endl << endl;
    }
  } // end of BehaviourDSLCommon::writeBehaviourProfiler

  void BehaviourDSLCommon::writeBehaviourClass(const Hypothesis h)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->writeBehaviourClassBegin(h);
    this->writeBehaviourStandardTFELTypedefs();
    this->writeBehaviourParserSpecificTypedefs();
    this->writeBehaviourStaticVariables(h);
    this->writeBehaviourIntegrationVariables(h);
    this->writeBehaviourIntegrationVariablesIncrements(h);
    this->writeBehaviourLocalVariables(h);
    this->writeBehaviourParameters(h);
    this->writeBehaviourParserSpecificMembers(h);
    this->writeBehaviourUpdateIntegrationVariables(h);
    this->writeBehaviourUpdateStateVariables(h);
    this->writeBehaviourUpdateAuxiliaryStateVariables(h);
    this->writeBehaviourMembersFunc();
    this->writeBehaviourPrivate();
    this->writeBehaviourDisabledConstructors();
    // from this point, all is public
    this->behaviourFile << "public:\n\n";
    this->writeBehaviourConstructors(h);
    this->writeBehaviourComputeStressFreeExpansion();
    this->writeBehaviourInitializeMethod(h);
    this->writeBehaviourSetOutOfBoundsPolicy();
    this->writeBehaviourGetModellingHypothesis();
    this->writeBehaviourCheckBounds(h);
    this->writeBehaviourComputePredictionOperator(h);
    this->writeBehaviourGetTimeStepScalingFactor();
    this->writeBehaviourComputeAPrioriTimeStepScalingFactor();
    this->writeBehaviourIntegrator(h);
    this->writeBehaviourComputeAPosterioriTimeStepScalingFactor();
    this->writeBehaviourComputeTangentOperator(h);
    this->writeBehaviourGetTangentOperator();
    this->writeBehaviourUpdateExternalStateVariables(h);
    this->writeBehaviourDestructor();
    this->checkBehaviourFile();
    this->behaviourFile << "private:\n\n";
    this->writeBehaviourComputeAPrioriTimeStepScalingFactorII(h);
    this->writeBehaviourComputeAPosterioriTimeStepScalingFactorII(h);
    this->writeBehaviourTangentOperator();
    this->writeBehaviourPolicyVariable();
    this->writeBehaviourClassEnd();
    this->writeBehaviourOutputOperator(h);
  }

  void
  BehaviourDSLCommon::writeBehaviourFileEnd(void)
  {
    this->checkBehaviourFile();
    this->writeBehaviourTraits();
    this->writeNamespaceEnd(this->behaviourFile);
    this->writeBehaviourFileHeaderEnd();
  } // end of BehaviourDSLCommon::writeBehaviourFileBegin

  void BehaviourDSLCommon::writeBehaviourComputePredictionOperator(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    if((!this->mb.getAttribute<bool>(h,BehaviourData::hasPredictionOperator,false))&&
       (this->mb.hasCode(h,BehaviourData::ComputePredictionOperator))){
      this->throwRuntimeError("BehaviourDSLCommon::writeBehaviourComputePredictionOperator : ",
			      "attribute 'hasPredictionOperator' is set but no associated code defined");
    }
    if(this->mb.hasCode(h,BehaviourData::ComputePredictionOperator)){
      this->behaviourFile << "IntegrationResult\n"
			  << "computePredictionOperator(const SMFlag smflag,const SMType smt) override{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      writeMaterialLaws("BehaviourDSLCommon::writeBehaviourComputePredictionOperator",
			this->behaviourFile,this->mb.getMaterialLaws());
      if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL)){
	if(mb.useQt()){
	  this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			      << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n"
			      << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
			      << "}\n";
	} else {
	  this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			      << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n"
			      << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
			      << "}\n";
	}
      }
      this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputePredictionOperator);
      this->behaviourFile << "return SUCCESS;\n";
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "IntegrationResult computePredictionOperator(const SMFlag,const SMType) override{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::computePredictionOperator : \");\n";
      this->behaviourFile << "msg +=\"unimplemented feature\";\n";
      this->behaviourFile << "throw(runtime_error(msg));\n";
      this->behaviourFile << "return FAILURE;\n";
      this->behaviourFile << "}\n\n";
    }
  } // end of BehaviourDSLCommon::writeBehaviourComputePredictionOperator(void)

  void BehaviourDSLCommon::writeBehaviourComputeTangentOperator(const Hypothesis h)
  {
    using namespace std;
    using namespace tfel::material;
    if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      // all available tangent operators for finite strain behaviours
      const vector<FiniteStrainBehaviourTangentOperatorBase::Flag> tos(getFiniteStrainBehaviourTangentOperatorFlags());
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pt;
      // all known converters
      const vector<FiniteStrainBehaviourTangentOperatorConversion> converters =
	FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions();
      // tangent operators defined by the user
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for(pt=tos.begin();pt!=tos.end();++pt){
	const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	if(this->mb.hasCode(h,BehaviourData::ComputeTangentOperator+'-'+ktype)){
	  ktos.push_back(*pt);
	}
      }
      if(!ktos.empty()){
	// computing all the conversion paths starting from user defined ones
	vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
	vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pk;
	for(pk=ktos.begin();pk!=ktos.end();++pk){
	  const vector<FiniteStrainBehaviourTangentOperatorConversionPath> kpaths = 
	    FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(*pk,ktos,converters);
	  paths.insert(paths.end(),kpaths.begin(),kpaths.end());
	}
	for(pt=tos.begin();pt!=tos.end();++pt){
	  const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	  if(find(ktos.begin(),ktos.end(),*pt)!=ktos.end()){
	    this->behaviourFile << "bool computeConsistentTangentOperator_" << ktype << "(const SMType smt){\n";
	    this->behaviourFile << "using namespace std;\n";
	    this->behaviourFile << "using namespace tfel::math;\n";
	    this->behaviourFile << "using std::vector;\n";
	    writeMaterialLaws("BehaviourDSLCommon::writeBehaviourComputeTangentOperator",
			      this->behaviourFile,this->mb.getMaterialLaws());
	    this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeTangentOperator+"-"+ktype) << '\n';
	    this->behaviourFile << "return true;\n";
	    this->behaviourFile << "}\n\n";
	  } else {
	    const FiniteStrainBehaviourTangentOperatorConversionPath path =
	      FiniteStrainBehaviourTangentOperatorConversionPath::getShortestPath(paths,*pt);
	    if(path.empty()){
	      this->behaviourFile << "bool computeConsistentTangentOperator_" << ktype << "(const SMType){\n";
	      this->behaviourFile << "using namespace std;\n";
	      this->behaviourFile << "string msg(\"" << this->mb.getClassName()
				  << "::computeConsistentTangentOperator_" << ktype << " : \");\n"
				  << "msg += \"computing the tangent operator '"
				  << ktype << "' is not supported\";\n"
				  << "throw(runtime_error(msg));\n";
	      this->behaviourFile << "return false;\n";
	      this->behaviourFile << "}\n\n";
	    } else {
	      this->behaviourFile << "bool computeConsistentTangentOperator_" << ktype << "(const SMType smt){\n";
	      FiniteStrainBehaviourTangentOperatorConversionPath::const_iterator pc = path.begin();
	      this->behaviourFile << "using namespace tfel::math;\n";
	      this->behaviourFile << "// computing " << convertFiniteStrainBehaviourTangentOperatorFlagToString(pc->from()) << endl;
	      const string k = convertFiniteStrainBehaviourTangentOperatorFlagToString(pc->from());
	      this->behaviourFile << "this->computeConsistentTangentOperator_" << k << "(smt);\n";
	      this->behaviourFile << "const " << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from()) << "<N,stress>"
				  << " tangentOperator_" << convertFiniteStrainBehaviourTangentOperatorFlagToString(pc->from())
				  << " = this->Dt.template get<"
				  << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from())
				  << "<N,stress> >();\n";
	      for(;pc!=path.end();){
		const FiniteStrainBehaviourTangentOperatorConversion converter = *pc;
		if(++pc==path.end()){
		  this->behaviourFile << converter.getFinalConversion() << endl;
		} else {
		  this->behaviourFile << converter.getIntermediateConversion() << endl;
		}
	      }
	      this->behaviourFile << "return true;\n";
	      this->behaviourFile << "}\n\n";
	    }
	  }
	}
	this->behaviourFile << "bool computeConsistentTangentOperator(const SMFlag& smflag,const SMType smt){\n";
	this->behaviourFile << "using namespace std;\n";
	this->behaviourFile << "switch(smflag){\n";
	for(pt=tos.begin();pt!=tos.end();++pt){
	  const auto ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	  this->behaviourFile << "case " << ktype << ":\n";
	  this->behaviourFile << "return this->computeConsistentTangentOperator_" << ktype << "(smt);\n";
	}
	this->behaviourFile << "}\n";
	this->behaviourFile << "string msg(\"" << this->mb.getClassName()
			    << "::computeConsistentTangentOperator : \");\n"
			    << "msg += \"unsupported tangent operator flag\";\n"
			    << "throw(runtime_error(msg));\n";
	this->behaviourFile << "return false;\n";
	this->behaviourFile << "}\n\n";
      }
    } else {
      if(this->mb.hasCode(h,BehaviourData::ComputeTangentOperator)){
	this->behaviourFile << "bool computeConsistentTangentOperator(const SMType smt){\n";
	this->behaviourFile << "using namespace std;\n";
	this->behaviourFile << "using namespace tfel::math;\n";
	this->behaviourFile << "using std::vector;\n";
	writeMaterialLaws("BehaviourDSLCommon::writeBehaviourComputeTangentOperator",
			  this->behaviourFile,this->mb.getMaterialLaws());
	this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeTangentOperator) << '\n';
	this->behaviourFile << "return true;\n";
	this->behaviourFile << "}\n\n";
      }
    }
  } // end of BehaviourDSLCommon::writeBehaviourComputeTangentOperator

  void BehaviourDSLCommon::writeBehaviourGetTangentOperator()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "const TangentOperator&\n";
    this->behaviourFile << "getTangentOperator(void) const{\n";
    this->behaviourFile << "return this->Dt;\n";
    this->behaviourFile << "}\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourComputeTangentOperator(void)

  void BehaviourDSLCommon::writeBehaviourGetTimeStepScalingFactor()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "real\n"
      "getMinimalTimeStepScalingFactor(void) const override{\n"
      "  return this->minimal_time_step_scaling_factor;\n"
      "}\n\n";
  }
  
  void BehaviourDSLCommon::writeBehaviourComputeAPrioriTimeStepScalingFactor()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "std::pair<bool,real>\n"
      "computeAPrioriTimeStepScalingFactor(const real current_time_step_scaling_factor) const override{\n"
      "const auto time_scaling_factor = this->computeAPrioriTimeStepScalingFactorII();\n"
      "return {time_scaling_factor.first,\n"
      "        std::min(std::min(std::max(time_scaling_factor.second,\n"
      "                                   this->minimal_time_step_scaling_factor),\n"
      "                          this->maximal_time_step_scaling_factor),\n"
      "                  current_time_step_scaling_factor)};\n"
      "}\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourComputeAPrioriTimeStepScalingFactor(void)

  void BehaviourDSLCommon::writeBehaviourComputeAPrioriTimeStepScalingFactorII(const Hypothesis h){
    this->checkBehaviourFile();
    this->behaviourFile << "std::pair<bool,real>\n"
			<< "computeAPrioriTimeStepScalingFactorII(void) const{\n";
    if(this->mb.hasCode(h,BehaviourData::APrioriTimeStepScalingFactor)){
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("BehaviourDSLCommon::writeBehaviourComputeTangentOperator",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,BehaviourData::APrioriTimeStepScalingFactor) << '\n';
    }
    this->behaviourFile << "return {true,this->maximal_time_step_scaling_factor};\n";
    this->behaviourFile << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactor()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "std::pair<bool,real>\n"
      "computeAPosterioriTimeStepScalingFactor(const real current_time_step_scaling_factor) const override{\n"
      "const auto time_scaling_factor = this->computeAPosterioriTimeStepScalingFactorII();\n"
      "return {time_scaling_factor.first,\n"
      "        std::min(std::min(std::max(time_scaling_factor.second,\n"
      "                                   this->minimal_time_step_scaling_factor),\n"
      "                          this->maximal_time_step_scaling_factor),\n"
      "                 current_time_step_scaling_factor)};\n"
      "}\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactor(void)
  
  void BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactorII(const Hypothesis h)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "std::pair<bool,real>\n";
    this->behaviourFile << "computeAPosterioriTimeStepScalingFactorII(void) const{\n";
    if(this->mb.hasCode(h,BehaviourData::APosterioriTimeStepScalingFactor)){
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("BehaviourDSLCommon::writeBehaviourComputeTangentOperator",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,BehaviourData::APosterioriTimeStepScalingFactor) << '\n';
    }
    this->behaviourFile << "return {true,this->maximal_time_step_scaling_factor};\n";
    this->behaviourFile << "}\n\n";
  } // end of BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactor(void)
  
  void BehaviourDSLCommon::writeBehaviourTangentOperator()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "//! Tangent operator;\n";
    this->behaviourFile << "TangentOperator Dt;\n";
  } // end of BehaviourDSLCommon::writeBehaviourTangentOperator()

  void BehaviourDSLCommon::checkIntegrationDataFile() const {
    using namespace std;
    if((!this->integrationDataFile)||
       (!this->integrationDataFile.good())){
      string msg("BehaviourDSLCommon::checkIntegrationDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataFileHeader(){
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\file   " << this->integrationDataFileName << endl;
    this->integrationDataFile << "* \\brief  " << "this file implements the " 
			      << this->mb.getClassName() << "IntegrationData" << " class.\n";
    this->integrationDataFile << "*         File generated by ";
    this->integrationDataFile << MFrontHeader::getVersionName() << " ";
    this->integrationDataFile << "version " << MFrontHeader::getVersionNumber();
    this->integrationDataFile << endl;
    if(!this->authorName.empty()){
      this->integrationDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->integrationDataFile << "* \\date   " << this->date       << endl;
    }
    this->integrationDataFile << " */\n";
    this->integrationDataFile << endl;
  }

  void BehaviourDSLCommon::writeIntegrationDataFileHeaderBegin(){
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "#ifndef LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->mb.getClassName());
    this->integrationDataFile << "_INTEGRATION_DATA_HXX\n";
    this->integrationDataFile << "#define LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->mb.getClassName());
    this->integrationDataFile << "_INTEGRATION_DATA_HXX\n";
    this->integrationDataFile << endl;
  }

  void BehaviourDSLCommon::writeIntegrationDataFileHeaderEnd()
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "#endif /* LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->mb.getClassName());
    this->integrationDataFile << "_INTEGRATION_DATA_HXX */\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataStandardTFELIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "#include<string>\n";
    this->integrationDataFile << "#include<iostream>\n";
    this->integrationDataFile << "#include<limits>\n";
    this->integrationDataFile << "#include<stdexcept>\n";
    this->integrationDataFile << "#include<algorithm>\n\n";
    this->integrationDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Config/TFELTypes.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsScalar.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/Promote.hxx\"\n";
    this->mb.requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->integrationDataFile << "#include\"TFEL/Math/tvector.hxx\"\n";
      this->integrationDataFile << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n";
    }
    if(b2){
      this->integrationDataFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    this->integrationDataFile << "#include\"TFEL/Math/stensor.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Math/st2tost2.hxx\"\n";
    if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      this->integrationDataFile << "#include\"TFEL/Math/tensor.hxx\"\n";
      this->integrationDataFile << "#include\"TFEL/Math/t2tot2.hxx\"\n";
      this->integrationDataFile << "#include\"TFEL/Math/t2tost2.hxx\"\n";
      this->integrationDataFile << "#include\"TFEL/Math/st2tot2.hxx\"\n";
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataDefaultMembers(void)
  {
    using namespace std;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "protected: \n\n";
    for(p=this->mb.getMainVariables().begin();p!=this->mb.getMainVariables().end();++p){
      if(p->first.increment_known){
	this->integrationDataFile << "/*!\n";
	this->integrationDataFile << " * \\brief " << p->first.name << " increment\n";
	this->integrationDataFile << " */\n";
	this->integrationDataFile << p->first.type  << " d" << p->first.name << ";\n\n";
      } else {
	this->integrationDataFile << "/*!\n";
	this->integrationDataFile << " * \\brief " << p->first.name << " at the end of the time step\n";
	this->integrationDataFile << " */\n";
	this->integrationDataFile << p->first.type  << " " << p->first.name << "1;\n\n";
      }
    }
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << " * \\brief time increment\n";
    this->integrationDataFile << " */\n";
    this->integrationDataFile << "time dt;\n";
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << " * \\brief temperature increment\n";
    this->integrationDataFile << " */\n";
    this->integrationDataFile << "temperature dT;\n";
    this->integrationDataFile << endl;
  }

  void BehaviourDSLCommon::writeIntegrationDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    this->integrationDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->integrationDataFile << "static " << constexpr_c << " unsigned short StensorSize = ";
    this->integrationDataFile << "StensorDimeToSize::value;\n";
    this->integrationDataFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->integrationDataFile << "static " << constexpr_c << " unsigned short TensorSize = ";
    this->integrationDataFile << "TensorDimeToSize::value;\n";
    this->integrationDataFile << endl;
    this->writeStandardTFELTypedefs(this->integrationDataFile);
    this->integrationDataFile << endl;
  }

  void BehaviourDSLCommon::writeIntegrationDataDisabledConstructors(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
  }

  void BehaviourDSLCommon::writeIntegrationDataConstructors(const Hypothesis h)
  {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\brief Default constructor\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData()\n";
    this->integrationDataFile << "{}\n\n";
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\brief Copy constructor\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData(const ";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData& src)\n";
    this->integrationDataFile << ": ";
    for(auto p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "d" <<p2->first.name  << "(src.d" << p2->first.name << "),\n";
      } else {
	this->integrationDataFile << p2->first.name  << "1(src." << p2->first.name << "1),\n";
      }
    }
    this->integrationDataFile << "dt(src.dt),\n";
    this->integrationDataFile << "dT(src.dT)";
    if(!md.getExternalStateVariables().empty()){
      for(auto p =md.getExternalStateVariables().begin();
	  p!=md.getExternalStateVariables().end();++p){
	this->integrationDataFile << ",\n";
	this->integrationDataFile << "d" << p->name << "(src.d" << p->name << ")";
      }
    }
    this->integrationDataFile << "\n{}\n\n";
    // Creating constructor for external interfaces
    for(const auto& i : this->interfaces){
      if(i.second->isModellingHypothesisHandled(h,this->mb)){
	i.second->writeIntegrationDataConstructor(this->integrationDataFile,h,this->mb);
      }
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataScaleOperators(const Hypothesis h)
  {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p2;
    bool iknown = true;
    for(p2=this->mb.getMainVariables().begin();(p2!=this->mb.getMainVariables().end())&&(iknown);++p2){
      iknown = p2->first.increment_known;
    }
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*\n";
    this->integrationDataFile << "* Multiplication by a scalar.\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << "template<typename Scal>\n";
    this->integrationDataFile << "typename std::enable_if<\n";
    this->integrationDataFile << "tfel::typetraits::IsFundamentalNumericType<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::typetraits::IsScalar<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::typetraits::IsReal<Scal>::cond&&\n";
    this->integrationDataFile << "std::is_same<Type," 
			      << "typename tfel::typetraits::Promote"
			      << "<Type,Scal>::type>::value,\n";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData&\n"
			      << ">::type\n";
    if(!iknown){
      if(this->mb.useQt()){
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt>& behaviourData, const Scal time_scaling_factor){\n";
      } else {
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false>& behaviourData, const Scal time_scaling_factor){\n";
      }
    } else {
      if(this->mb.useQt()){
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt>&, const Scal time_scaling_factor){\n";
      } else {
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false>&, const Scal time_scaling_factor){\n";
      }
    }
    this->integrationDataFile << "this->dt   *= time_scaling_factor;\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "this->d" <<p2->first.name  << " *= time_scaling_factor;\n";
      } else {
	this->integrationDataFile << "this->" <<p2->first.name  << "1 = (1-time_scaling_factor)*(behaviourData." <<p2->first.name  << "0)+time_scaling_factor*(this->" <<p2->first.name  << "1);\n";
      }
    }
    this->integrationDataFile << "this->dT   *= time_scaling_factor;\n";
    for(p=md.getExternalStateVariables().begin();p!=md.getExternalStateVariables().end();++p){
      this->integrationDataFile << "this->d" << p->name << " *= time_scaling_factor;\n";
    }
    this->integrationDataFile << "return *this;\n";
    this->integrationDataFile << "}\n\n";
  } // end of BehaviourDSLCommon::writeIntegrationDataScaleOpeartors

  void BehaviourDSLCommon::writeIntegrationDataClassHeader(void) 
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\class " << this->mb.getClassName() << "IntegrationData\n";
    this->integrationDataFile << "* \\brief This class implements the " 
			      << this->mb.getClassName() << "IntegrationData" << " behaviour.\n";
    this->integrationDataFile << "* \\param unsigned short N, space dimension.\n";
    this->integrationDataFile << "* \\param typename Type, numerical type.\n";
    this->integrationDataFile << "* \\param bool use_qt, conditional saying if quantities are use.\n";
    if(!this->authorName.empty()){
      this->integrationDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->integrationDataFile << "* \\date   " << this->date << endl;
    }
    this->integrationDataFile << "*/\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataForwardDeclarations(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "// Forward Declaration\n";
    this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
    this->integrationDataFile << "class " << this->mb.getClassName() << "IntegrationData;\n\n";

    if(this->mb.useQt()){
      this->integrationDataFile << "// Forward Declaration\n";
      this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
      this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->integrationDataFile << "const " << this->mb.getClassName() 
				<< "IntegrationData<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->integrationDataFile << "// Forward Declaration\n";
      this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
      this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->integrationDataFile << "const " << this->mb.getClassName() 
				<< "IntegrationData<hypothesis,Type,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& h = this->mb.getModellingHypotheses();
    for(const auto & elem : h){
      if(this->mb.hasSpecialisedMechanicalData(elem)){
	if(this->mb.useQt()){
	  this->integrationDataFile << "// Forward Declaration\n";
	  this->integrationDataFile << "template<typename Type,bool use_qt>\n";
	  this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->integrationDataFile << "const " << this->mb.getClassName() 
				    << "IntegrationData<ModellingHypothesis::"
				    << ModellingHypothesis::toUpperCaseString(elem) << ",Type,use_qt>&);\n\n";
	} else {
	  this->integrationDataFile << "// Forward Declaration\n";
	  this->integrationDataFile << "template<typename Type>\n";
	  this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->integrationDataFile << "const " << this->mb.getClassName() 
				    << "IntegrationData<ModellingHypothesis::"
				    << ModellingHypothesis::toUpperCaseString(elem) << ",Type,false>&);\n\n";
	}
      }
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkIntegrationDataFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->integrationDataFile << "class " << this->mb.getClassName() << "IntegrationData\n";
      } else {
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->integrationDataFile << "class " << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,false>\n";
      }
    } else {
      if(this->mb.useQt()){
	this->integrationDataFile << "template<typename Type,bool use_qt>\n";
	this->integrationDataFile << "class " << this->mb.getClassName()
				  << "IntegrationData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>\n";
      } else {
	this->integrationDataFile << "template<typename Type>\n";
	this->integrationDataFile << "class " << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>\n";
      }
    }
    this->integrationDataFile << "{\n";
    this->integrationDataFile << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->integrationDataFile << "static " << constexpr_c << " ModellingHypothesis::Hypothesis hypothesis = " 
				<< "ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    this->integrationDataFile << "static " << constexpr_c << " unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->integrationDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);\n";
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			      << "IsFundamentalNumericType<Type>::cond);\n";
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->integrationDataFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData&);\n\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataOutputOperator(const Hypothesis h)
  {    
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->integrationDataFile << "std::ostream&\n";
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,use_qt>& b)\n";
      } else {
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->integrationDataFile << "std::ostream&\n";
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,false>& b)\n";
      }
    } else {
      if(this->mb.useQt()){        
	this->integrationDataFile << "template<typename Type,bool use_qt>\n";
	this->integrationDataFile << "std::ostream&\n";
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>& b)\n";
      } else {
	this->integrationDataFile << "template<typename Type>\n";
	this->integrationDataFile << "std::ostream&\n";
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>& b)\n";
      }
    }
    this->integrationDataFile << "{\n";
    this->integrationDataFile << "using namespace std;\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "os << \"d" << p2->first.name << " : \" << b.d" << p2->first.name << " << '\\n';\n";
      } else {
	this->integrationDataFile << "os << \"" << p2->first.name << "1 : \" << b." << p2->first.name << "1 << endl;\n";
      }
    }
    this->integrationDataFile << "os << \"dt : \" << b.dt << endl;\n";
    this->integrationDataFile << "os << \"dT : \" << b.dT << endl;\n";
    for(p=md.getExternalStateVariables().begin();p!=md.getExternalStateVariables().end();++p){
      this->integrationDataFile << "os << \"d" << p->name << " : \" << b.d" 
				<< p->name << " << '\\n';\n";  
    }
    this->integrationDataFile << "return os;\n";
    this->integrationDataFile << "}\n\n";
  } // end of BehaviourDSLCommon::writeIntegrationDataOutputOperator()

  void BehaviourDSLCommon::writeIntegrationDataClassEnd() {    
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "}; // end of " << this->mb.getClassName() << "IntegrationData" 
			      << "class\n";
    this->integrationDataFile << endl;
  }

  void BehaviourDSLCommon::writeIntegrationDataExternalStateVariables(const Hypothesis h) {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    this->checkIntegrationDataFile();
    this->writeVariablesDeclarations(this->integrationDataFile,
				     md.getExternalStateVariables(),
				     "d","",this->fileName,false);
  }

  void BehaviourDSLCommon::writeIntegrationDataFileBegin(void)
  {
    this->checkIntegrationDataFile();
    this->writeIntegrationDataFileHeader();
    this->writeIntegrationDataFileHeaderBegin();
    this->writeIntegrationDataStandardTFELIncludes();
    this->writeIncludes(this->integrationDataFile);
    // includes specific to interfaces
    for(const auto& i : this->interfaces){
      i.second->writeInterfaceSpecificIncludes(this->integrationDataFile,this->mb);
    }
    this->writeNamespaceBegin(this->integrationDataFile);
    this->writeIntegrationDataForwardDeclarations();
  } // end of BehaviourDSLCommon::writeIntegrationDataFile

  void BehaviourDSLCommon::writeIntegrationDataClass(const Hypothesis h){
    using namespace std;
    this->checkIntegrationDataFile();
    this->writeIntegrationDataClassBegin(h);
    this->writeIntegrationDataStandardTFELTypedefs();
    this->writeIntegrationDataDefaultMembers();
    this->writeIntegrationDataExternalStateVariables(h);
    this->writeIntegrationDataDisabledConstructors();
    this->integrationDataFile << "public:\n\n";
    this->writeIntegrationDataConstructors(h);
    this->writeIntegrationDataMainVariablesSetters();
    this->writeIntegrationDataScaleOperators(h);
    this->writeIntegrationDataClassEnd();
    this->writeIntegrationDataOutputOperator(h);
  }

  void BehaviourDSLCommon::writeIntegrationDataFileEnd(void)
  {
    this->checkIntegrationDataFile();
    this->writeNamespaceEnd(this->integrationDataFile);
    this->writeIntegrationDataFileHeaderEnd();
  } // end of BehaviourDSLCommon::writeIntegrationDataFileEnd

  void BehaviourDSLCommon::checkSrcFile(void) const {
    using namespace std;
    if((!this->integrationDataFile)||
       (!this->integrationDataFile.good())){
      string msg("BehaviourDSLCommon::checkSrcOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void BehaviourDSLCommon::writeSrcFileHeader(void){
    using namespace std;
    this->checkSrcFile();
    this->srcFile << "/*!\n";
    this->srcFile << "* \\file   " << this->srcFileName << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << this->mb.getClassName() << " Behaviour.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!this->authorName.empty()){
      this->srcFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->srcFile << "* \\date   " << this->date       << endl;
    }
    this->srcFile << " */\n";
    this->srcFile << endl;
    if(this->mb.hasParameters()){
      this->srcFile << "#include<string>\n";
      this->srcFile << "#include<cstring>\n";
      this->srcFile << "#include<sstream>\n";
      this->srcFile << "#include<fstream>\n";
      this->srcFile << "#include<stdexcept>\n";
      this->srcFile << endl;
    }
    this->srcFile << "#include\"" << this->behaviourDataFileName   << "\"\n";
    this->srcFile << "#include\"" << this->integrationDataFileName << "\"\n";
    this->srcFile << "#include\"" << this->behaviourFileName       << "\"\n";
    this->srcFile << endl;
  } // end of BehaviourDSLCommon::writeSrcFileHeader()

  void BehaviourDSLCommon::writeSrcFileStaticVariables(const Hypothesis h){
    const auto& md = this->mb.getBehaviourData(h);
    const auto m = "tfel::material::ModellingHypothesis::"+
      ModellingHypothesis::toUpperCaseString(h);
    this->checkSrcFile();
    for(const auto& v : md.getStaticVariables()){
      if(v.type=="int"){
	continue;
      }
      if(this->mb.useQt()){
	this->srcFile << "template<>\n";
	this->srcFile << "const " << this->mb.getClassName() << "<" << m << ",float,true>::" 
		      << v.type << '\n' << this->mb.getClassName() 
		      << "<" << m << ",float,true>::" 
		      << v.name << " = " << this->mb.getClassName() 
		      << "<" << m << ",float,true>::" << v.type 
		      << "(static_cast<float>(" << v.value <<"));\n\n";
      }
      this->srcFile << "template<>\n";
      this->srcFile << "const " << this->mb.getClassName() << "<" << m << ",float,false>::" 
		    << v.type << '\n' << this->mb.getClassName() 
		    << "<" << m << ",float,false>::" 
		    << v.name << " = " << this->mb.getClassName() 
		    << "<" << m << ",float,false>::" << v.type 
		    << "(static_cast<float>(" << v.value <<"));\n\n";
      if(this->mb.useQt()){
	this->srcFile << "template<>\n";
	this->srcFile << "const " << this->mb.getClassName() << "<" << m << ",double,true>::" 
		      << v.type << '\n' << this->mb.getClassName() 
		      << "<" << m << ",double,true>::" 
		      << v.name << " = " << this->mb.getClassName() 
		      << "<" << m << ",double,true>::" << v.type 
		      << "(static_cast<double>(" << v.value <<"));\n\n";
      }
      this->srcFile << "template<>\n";
      this->srcFile << "const " << this->mb.getClassName() << "<" << m << ",double,false>::" 
		    << v.type << '\n' << this->mb.getClassName() 
		    << "<" << m << ",double,false>::" 
		    << v.name << " = " << this->mb.getClassName() 
		    << "<" << m << ",double,false>::" << v.type 
		    << "(static_cast<double>(" << v.value <<"));\n\n";
      if(this->mb.useQt()){
	this->srcFile << "template<>\n";
	this->srcFile << "const " << this->mb.getClassName() << "<" << m << ",long double,true>::" 
		      << v.type << '\n' << this->mb.getClassName() 
		      << "<" << m << ",long double,true>::" 
		      << v.name << " = " << this->mb.getClassName() << "<" 
		      << m << ",long double,true>::" << v.type 
		      << "(static_cast<long double>(" << v.value <<"));\n\n";
      }
      this->srcFile << "template<>\n";
      this->srcFile << "const " << this->mb.getClassName() << "<" << m << ",long double,false>::" 
		    << v.type << '\n' << this->mb.getClassName() 
		    << "<" << m << ",long double,false>::" 
		    << v.name << " = " << this->mb.getClassName() 
		    << "<" << m << ",long double,false>::" << v.type 
		    << "(static_cast<long double>(" << v.value <<"));\n\n";
    }
  } // end of BehaviourDSLCommon::writeSrcFileStaticVariables
  
  void
  BehaviourDSLCommon::writeSrcFileUserDefinedCode(void)
  {
    const auto& s = this->mb.getSources();
    this->checkBehaviourFile();
    if(!s.empty()){
      this->srcFile << s << "\n\n";
    }
  } // end of BehaviourDSLCommon::writeSrcFileUserDefinedCode

  void
  BehaviourDSLCommon::writeSrcFileParametersInitializers(void)
  {
    if(!this->mb.hasParameters()){
      return;
    }
    auto hs = this->mb.getDistinctModellingHypotheses();
    hs.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for(const auto & h : hs){
      if(this->mb.hasParameters(h)){
	this->writeSrcFileParametersInitializer(h);
      }
    }
  } // end of BehaviourDSLCommon::writeSrcFileParametersInitializer

  static void
  BehaviourDSLCommon_writeConverter(std::ostream& f,
				    const std::string& cname,
				    const std::string& type,
				    const std::string& type2)
  {
    f << type  << '\n'
      << cname << "::get" << type2 << "(const std::string& n,\n"
      <<                              "const std::string& v)\n"
      << "{\n"
      << "using namespace std;\n"
      << type << " value;\n"
      << "istringstream converter(v);\n"
      << "converter >> value;\n"
      << "if(!converter||(!converter.eof())){\n"
      << "string msg(\"" << cname << "::get" << type2 << " : \"\n"
      << "           \"can't convert '\"+v+\"' to " << type
      << " for parameter '\"+ n+\"'\");\n"
      << "throw(runtime_error(msg));\n"
      << "}\n"
      << "return value;\n"
      << "}\n\n";
  }

  void
  BehaviourDSLCommon::writeSrcFileParametersInitializer(const Hypothesis h)
  {
    using namespace std;
    this->checkBehaviourFile();
    // treating the default case
    bool rp  = false; // real    parameter found
    bool ip  = false; // integer parameter found
    bool up  = false; // unsigned short parameter found
    bool rp2 = false; // real    parameter found
    bool ip2 = false; // integer parameter found
    bool up2 = false; // unsigned short parameter found
    VariableDescriptionContainer::const_iterator p;
    string cname(this->mb.getClassName());
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    string dcname(this->mb.getClassName()+"ParametersInitializer");
    this->srcFile << cname << "&\n"
		  << cname << "::get()\n"
		  <<"{\n"
		  << "static " << cname << " i;\n"
		  << "return i;\n"
		  << "}\n\n";
    this->srcFile << cname << "::" 
		  << cname << "()\n"
		  <<"{\n";
    const VariableDescriptionContainer& params =
      this->mb.getBehaviourData(h).getParameters();
    for(p=params.begin();p!=params.end();++p){
      if(p->type=="real"){
	rp = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  rp2=true;
	  this->srcFile << "this->" << p->name << " = " 
			<< this->mb.getFloattingPointParameterDefaultValue(h,p->name) << ";\n"; 
	}
      } else if(p->type=="int"){
	ip = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  ip2=true;
	  this->srcFile << "this->" << p->name << " = " 
			<< this->mb.getIntegerParameterDefaultValue(h,p->name) << ";\n"; 
	}
      } else if(p->type=="ushort"){
	up = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  up2=true;
	  this->srcFile << "this->" << p->name << " = " 
			<< this->mb.getUnsignedShortParameterDefaultValue(h,p->name) << ";\n"; 
	}
      }
    }
    this->srcFile << "// Reading parameters from a file\n";
    this->srcFile << cname << "::readParameters(*this,\"" << this->mb.getClassName() << "-parameters.txt\");\n";
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->srcFile << cname << "::readParameters(*this,\"" << this->mb.getClassName() << ModellingHypothesis::toString(h) << "-parameters.txt\");\n";
    }
    this->srcFile <<"}\n\n";
    if(rp){
      this->srcFile <<"void\n"
		    << cname << "::set(const char* const key,\nconst double v)" 
		    << "{\n"
		    << "using namespace std;\n";
      bool first = true;
      for(p=params.begin();p!=params.end();++p){
	if(p->type=="real"){
	  if(first){
	    this->srcFile << "if(";
	    first = false;
	  } else {
	    this->srcFile << "} else if(";
	  }
	  this->srcFile << "::strcmp(\""+this->mb.getExternalName(h,p->name)+"\",key)==0){\n";
	  if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	     ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	      (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	    this->srcFile << "this->" << p->name << " = v;\n";
	  } else {
	    this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",v);\n";
	  }
	}
      }
      this->srcFile << "} else {\n";
      this->srcFile << "string msg(\"" << cname << "::set : \");\n"
		    << "msg += \" no parameter named '\";\n"
		    << "msg += key;\n"
		    << "msg += \"'\";\n"
		    << "throw(runtime_error(msg));\n"
		    << "}\n"
		    << "}\n\n";
    }
    if(ip){
      this->srcFile <<"void\n"
		    << cname << "::set(const char* const key,\nconst int v)" 
		    << "{\n"
		    << "using namespace std;\n";
      bool first = true;
      for(p=params.begin();p!=params.end();++p){
	if(p->type=="int"){
	  if(first){
	    this->srcFile << "if(";
	    first = false;
	  } else {
	    this->srcFile << "} else if(";
	  }
	  this->srcFile << "::strcmp(\""+this->mb.getExternalName(h,p->name)+"\",key)==0){\n";
	  if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	     ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	      (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	    this->srcFile << "this->" << p->name << " = v;\n";
	  } else {
	    this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",v);\n";
	  }
	}
      }
      this->srcFile << "} else {\n";
      this->srcFile << "string msg(\"" << cname << "::set : \");\n"
		    << "msg += \" no parameter named '\";\n"
		    << "msg += key;\n"
		    << "msg += \"'\";\n"
		    << "throw(runtime_error(msg));\n"
		    << "}\n"
		    << "}\n\n";
    }
    if(up){
      this->srcFile <<"void\n"
		    << cname << "::set(const char* const key,\nconst unsigned short v)" 
		    << "{\n"
		    << "using namespace std;\n";
      bool first = true;
      for(p=params.begin();p!=params.end();++p){
	if(p->type=="ushort"){
	  if(first){
	    this->srcFile << "if(";
	    first = false;
	  } else {
	    this->srcFile << "} else if(";
	  }
	  this->srcFile << "::strcmp(\""+this->mb.getExternalName(h,p->name)+"\",key)==0){\n";
	  if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	     ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	      (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	    this->srcFile << "this->" << p->name << " = v;\n";
	  } else {
	    this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",v);\n";
	  }
	}
      }
      this->srcFile << "} else {\n";
      this->srcFile << "string msg(\"" << cname << "::set : \");\n"
		    << "msg += \" no parameter named '\";\n"
		    << "msg += key;\n"
		    << "msg += \"'\";\n"
		    << "throw(runtime_error(msg));\n"
		    << "}\n"
		    << "}\n\n";
    }
    if(rp2){
      BehaviourDSLCommon_writeConverter(this->srcFile,cname,"double","Double");
    }
    if(ip2){
      BehaviourDSLCommon_writeConverter(this->srcFile,cname,"int","Int");
    }
    if(up2){
      BehaviourDSLCommon_writeConverter(this->srcFile,cname,"unsigned short","UnsignedShort");
    }
    this->srcFile << "void\n"
		  << cname << "::readParameters(" << cname << "&";
    if(rp2||ip2||up2){
      this->srcFile << " pi";
    }
    this->srcFile << ",const char* const fn)" 
		  << "{\n"
		  << "using namespace std;\n"
		  << "ifstream f(fn);\n"
		  << "if(!f){\n"
      		  << "return;\n"
      		  << "}\n"
		  << "while(!f.eof()){\n"
		  << "string p;\n"
		  << "string v;\n"
		  << "f >> p;\n"
		  << "if(p.empty()){\n"
		  << "if(!f.eof()){\n"
		  << "string msg(\"" << cname << "::readParameters : \");\n"
		  << "msg+=\"Error while parsing file '\";\n"
		  << "msg+=fn;\n"
		  << "msg+=\"'\";\n"
		  << "throw(runtime_error(msg));\n"
		  << "}\n"
		  << "break;\n"
		  << "}\n"
		  << "f >> v;\n";
      for(p=params.begin();p!=params.end();++p){
	if(p==params.begin()){
	  this->srcFile << "if(";
	} else {
	  this->srcFile << "} else if(";
	}
	this->srcFile << "\"" <<  this->mb.getExternalName(h,p->name) << "\"==p){\n";
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  this->srcFile << "pi." << p->name << " = ";
	  if(p->type=="real"){
	    this->srcFile <<  cname << "::getDouble(p,v);\n";
	  } else if(p->type=="int"){
	    this->srcFile << cname << "::getInt(p,v);\n";
	  } else if(p->type=="ushort"){
	    this->srcFile << cname << "::getUnsignedShort(p,v);\n";
	  } else {
	    this->throwRuntimeError("BehaviourDSLCommon::writeSrcFileParametersInitializer",
				    "invalid parameter type '"+p->type+"'");
	  }
	} else {
	  this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",\n";
	  if(p->type=="real"){
	    this->srcFile << dcname << "::getDouble(p,v)";
	  } else if(p->type=="int"){
	    this->srcFile << dcname << "::getInt(p,v)";
	  } else if(p->type=="ushort"){
	    this->srcFile << dcname << "::getUnsignedShort(p,v)";
	  } else {
	    this->throwRuntimeError("BehaviourDSLCommon::writeSrcFileParametersInitializer",
				    "invalid parameter type '"+p->type+"'");
	  }
	  this->srcFile << ");\n";
	}
      }
      this->srcFile << "} else {\n"
		    << "string msg(\"" << cname << "::readParameters : \");\n"
		    << "msg+=\"Error while parsing file '\";\n"
		    << "msg+=fn;\n"
		    << "msg+=\"'. Invalid parameter '\"+p+\"'\";\n"
		    << "throw(runtime_error(msg));\n"
		    << "}\n"
		    << "}\n"
		    << "}\n\n";
  } // end of BehaviourDSLCommon::writeSrcFileParametersInitializer

  void
  BehaviourDSLCommon::writeSrcFileBehaviourProfiler(void)
  {
    if(this->mb.getAttribute(BehaviourData::profiling,false)){
      this->checkSrcFile();
      this->srcFile << "mfront::BehaviourProfiler&\n"
		    << this->mb.getClassName() << "Profiler::getProfiler()\n"
		    << "{\n"
		    << "static mfront::BehaviourProfiler profiler(\""
		    << this->mb.getClassName() << "\");\n;"
		    << "return profiler;\n"
		    << "}\n\n";
    }
  } // end of BehaviourDSLCommon::writeSrcFileBehaviourProfiler

  void BehaviourDSLCommon::writeSrcFile(void)
  {
    using namespace std;
    this->writeSrcFileHeader();
    this->writeSrcFileUserDefinedCode();
    this->writeNamespaceBegin(this->srcFile);
    this->writeSrcFileBehaviourProfiler();
    this->writeSrcFileParametersInitializers();
    // modelling hypotheses handled by the behaviour
    const auto& h = this->mb.getModellingHypotheses();
    for(const auto & elem : h){
      this->writeSrcFileStaticVariables(elem);
    }
    this->writeNamespaceEnd(this->srcFile);
  } // end of BehaviourDSLCommon::writeSrcFile(void)

  std::string
  BehaviourDSLCommon::predictionOperatorVariableModifier(const Hypothesis h,
							    const std::string& var,
							    const bool addThisPtr)
  {
    if(this->mb.isIntegrationVariableIncrementName(h,var)){
      this->throwRuntimeError("BehaviourDSLCommon::predictionOperatorVariableModifier : ",
			      "integration variable '"+var+"' can't be used in @PredictionOperator");
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of BehaviourDSLCommon::predictionOperatorVariableModifier

  void BehaviourDSLCommon::treatProfiling(void)
  {
    const bool b = 
      this->readBooleanValue("BehaviourDSLCommon::treatProfiling");
    this->readSpecifiedToken("BehaviourDSLCommon::treatProfiling",";");
    this->mb.setAttribute(BehaviourData::profiling,b,false);
  } // end of BehaviourDSLCommon::treatProfiling

  void BehaviourDSLCommon::treatPredictionOperator(void)
  {
    using namespace std;
    const auto& o = this->readCodeBlock(*this,BehaviourData::ComputePredictionOperator,
						    &BehaviourDSLCommon::predictionOperatorVariableModifier,
						    true,true);
    for(const auto & elem : o.hypotheses){
      this->mb.setAttribute(elem,BehaviourData::hasPredictionOperator,true);
    }
  } // end of BehaviourDSLCommon::treatPredictionOperator

  void
  BehaviourDSLCommon::treatParameter(void)
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    bool endOfTreatment=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatment)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("DSLBase::handleParameter : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      const auto n = this->current->value;
      const auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::handleParameter");
      if((this->current->value=="=")||
	 (this->current->value=="{")||
	 (this->current->value=="(")){
	string ci; // closing initializer
	double value;
	if(this->current->value=="{"){
	  ci="}";
	}
	if(this->current->value=="("){
	  ci=")";
	}
	++(this->current);
	this->checkNotEndOfFile("DSLBase::handleParameter");
	istringstream converter(this->current->value);
	converter >> value;
	if(!converter||(!converter.eof())){
	  this->throwRuntimeError("DSLBase::handleParameter",
				  "could not read default value for parameter '"+n+"'");
	}
	++(this->current);
	this->checkNotEndOfFile("DSLBase::handleParameter");
	if(!ci.empty()){
	  this->readSpecifiedToken("DSLBase::handleParameter",ci);
	}
	for(const auto & elem : h){
	  this->mb.addParameter(elem,VariableDescription("real",n,1u,lineNumber));
	  this->mb.setParameterDefaultValue(elem,n,value);
	}
      } else {
	for(const auto & elem : h){
	  this->mb.addParameter(elem,VariableDescription("real",n,1u,lineNumber));
	}
      }
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatment=true;
	++(this->current);
      } else {
	this->throwRuntimeError("DSLBase::handleParameter",
				", or ; expected afer '"+n+"'");
      }
    }
    if(!endOfTreatment){
      --(this->current);
      this->throwRuntimeError("DSLBase::handleParameter",
			      "Expected ';' before end of file");
    }
  } // end of BehaviourDSLCommon::treatParameter

  void
  BehaviourDSLCommon::treatInitLocalVariables(void)
  {
    this->readCodeBlock(*this,BehaviourData::InitializeLocalVariables,
			&BehaviourDSLCommon::standardModifier,true,true);
  } // end of BehaviourDSLCommon:treatInitLocalVariables

  void
  BehaviourDSLCommon::treatMinimalTimeStepScalingFactor(void){
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double r_dt;
    this->checkNotEndOfFile("ImplicitDSLBase::treatMinimalTimeStepScalingFactor",
			    "Cannot read value.");
    std::istringstream flux(current->value);
    flux >> r_dt;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatMinimalTimeStepScalingFactor",
			      "Failed to read value.");
    }
    if(r_dt<10*std::numeric_limits<double>::min()){
      this->throwRuntimeError("ImplicitDSLBase::treatMinimalTimeStepScalingFactor",
			      "minimal time step scaling factor either too "
			      "low value or negative.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatMinimalTimeStepScalingFactor",";");
    VariableDescription e("real","minimal_time_step_scaling_factor",1u,0u);
    e.description = "minimal value for the time step scaling factor";
    this->mb.addParameter(h,e,BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h,"minimal_time_step_scaling_factor",r_dt);
    this->mb.setEntryName(h,"minimal_time_step_scaling_factor","minimal_time_step_scaling_factor");
  } // end of BehaviourDSLCommon::treatMinimalTimeStepScalingFactor

  void
  BehaviourDSLCommon::treatMaximalTimeStepScalingFactor(void){
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double r_dt;
    this->checkNotEndOfFile("ImplicitDSLBase::treatMaximalTimeStepScalingFactor",
			    "Cannot read value.");
    std::istringstream flux(current->value);
    flux >> r_dt;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatMaximalTimeStepScalingFactor",
			      "Failed to read value.");
    }
    if(r_dt<1){
      this->throwRuntimeError("ImplicitDSLBase::treatMaximalTimeStepScalingFactor",
			      "maximal time step scaling factor value either too "
			      "low or negative.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatMaximalTimeStepScalingFactor",";");
    VariableDescription e("real","maximal_time_step_scaling_factor",1u,0u);
    e.description = "maximal value for the time step scaling factor";
    this->mb.addParameter(h,e,BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h,"maximal_time_step_scaling_factor",r_dt);
    this->mb.setEntryName(h,"maximal_time_step_scaling_factor","maximal_time_step_scaling_factor");
  } // end of BehaviourDSLCommon::treatMaximalTimeStepScalingFactor
  
  void
  BehaviourDSLCommon::disableVariableDeclaration(const Hypothesis h)
  {
    if(!this->mb.hasAttribute(h,BehaviourData::allowsNewUserDefinedVariables)){
      this->mb.setAttribute(h,BehaviourData::allowsNewUserDefinedVariables,false);
    }
  }

  void
  BehaviourDSLCommon::setMinimalTangentOperator(void)
  {
    using namespace std;
    if(this->mb.getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      const auto& mh = this->mb.getDistinctModellingHypotheses();
      for(const auto & elem : mh){
	// basic check
	if(this->mb.hasAttribute(elem,BehaviourData::hasConsistentTangentOperator)){
	  if(!this->mb.hasCode(elem,BehaviourData::ComputeTangentOperator)){
	    this->throwRuntimeError("BehaviourDSLCommon::setMinimalTangentOperator",
				    "behaviour has attribute 'hasConsistentTangentOperator' but "
				    "no associated code");
	  }
	}
      }
      if(this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	if(this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	  const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	  // if the user provided a tangent operator, it won't be
	  // overriden
	  CodeBlock tangentOperator;
	  ostringstream code; 
	  code << "if(smt==ELASTIC){\n"
	       << "this->Dt = this->D;\n"
	       << "} else {\n"
	       << "return false;\n"
	       << "}\n";
	  tangentOperator.code = code.str();
	  this->mb.setCode(h,BehaviourData::ComputeTangentOperator,
			   tangentOperator,
			   BehaviourData::CREATEBUTDONTREPLACE,
			   BehaviourData::BODY);
	  this->mb.setAttribute(h,BehaviourData::hasConsistentTangentOperator,true,true);
	}
      }
    }
  } // end of BehaviourDSLCommon::setMinimalTangentOperator

  void
  BehaviourDSLCommon::setComputeFinalStressFromComputeFinalStressCandidateIfNecessary(void)
  {
    using namespace std;
    const auto& mh = this->mb.getDistinctModellingHypotheses();
    // first treating specialised mechanical data
    for(const auto & elem : mh){
      if(elem!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	if(!this->mb.hasCode(elem,BehaviourData::ComputeFinalStress)){
	  if(this->mb.hasCode(elem,BehaviourData::ComputeFinalStressCandidate)){
	    this->mb.setCode(elem,BehaviourData::ComputeFinalStress,
			     this->mb.getCodeBlock(elem,BehaviourData::ComputeFinalStressCandidate),
			     BehaviourData::CREATE,
			     BehaviourData::BODY);
	  }
	}
      }
    }
    // now treating the default hypothesis case
    if(!this->mb.areAllMechanicalDataSpecialised()){
      const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if(!this->mb.hasCode(h,BehaviourData::ComputeFinalStress)){
	if(this->mb.hasCode(h,BehaviourData::ComputeFinalStressCandidate)){
	  this->mb.setCode(h,BehaviourData::ComputeFinalStress,
			   this->mb.getCodeBlock(h,BehaviourData::ComputeFinalStressCandidate),
			   BehaviourData::CREATEBUTDONTREPLACE,
			   BehaviourData::BODY);
	}
      }
    }
  } // end of BehaviourDSLCommon::setComputeFinalStressFromComputeFinalStressCandidateIfNecessary

} // end of namespace mfront
