/*!
 * \file   MechanicalBehaviourData.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#include<sstream>
#include<stdexcept>

#include"MFront/MechanicalBehaviourData.hxx"

namespace mfront{

  const std::string
  MechanicalBehaviourData::FlowRule("FlowRule");
  const std::string
  MechanicalBehaviourData::BeforeInitializeLocalVariables("BeforeInitializeLocalVariables");
  const std::string
  MechanicalBehaviourData::InitializeLocalVariables("InitializeLocalVariables");
  const std::string
  MechanicalBehaviourData::AfterInitializeLocalVariables("AfterInitializeLocalVariables");
  const std::string
  MechanicalBehaviourData::ComputePredictor("ComputePredictor");
  const std::string
  MechanicalBehaviourData::ComputeStress("ComputeStress");
  const std::string
  MechanicalBehaviourData::ComputeFinalStress("ComputeFinalStress");
  const std::string
  MechanicalBehaviourData::ComputeFinalStressCandidate("ComputeFinalStressCandidate");
  const std::string
  MechanicalBehaviourData::Integrator("Integrator");
  const std::string
  MechanicalBehaviourData::ComputeDerivative("ComputeDerivative");
  const std::string
  MechanicalBehaviourData::UpdateAuxiliaryStateVariables("UpdateAuxiliaryStateVariables");
  const std::string
  MechanicalBehaviourData::ComputePredictionOperator("ComputePredictionOperator");
  const std::string
  MechanicalBehaviourData::ComputeTangentOperator("ComputeTangentOperator");
  const std::string
  MechanicalBehaviourData::InitializeJacobian("InitializeJacobian");

  const std::string
  MechanicalBehaviourData::profiling("profiling");
  const std::string
  MechanicalBehaviourData::hasTimeStepScalingFactor("hasTimeStepScalingFactor");
  const std::string
  MechanicalBehaviourData::hasConsistentTangentOperator("hasConsistentTangentOperator");
  const std::string
  MechanicalBehaviourData::isConsistentTangentOperatorSymmetric("isConsistentTangentOperatorSymmetric");
  const std::string
  MechanicalBehaviourData::hasPredictionOperator("hasPredictionOperator");
  const std::string
  MechanicalBehaviourData::useCast3MAccelerationAlgorithm("useCast3MAccelerationAlgorithm");
  const std::string
  MechanicalBehaviourData::useRelaxationAlgorithm("useRelaxationAlgorithm");
  const std::string
  MechanicalBehaviourData::compareToNumericalJacobian("compareToNumericalJacobian");
  const std::string
  MechanicalBehaviourData::allowsNewUserDefinedVariables("allowsNewUserDefinedVariables");
  const std::string
  MechanicalBehaviourData::algorithm("algorithm");
  const std::string
  MechanicalBehaviourData::numberOfEvaluations("numberOfEvaluations");

  /*!
   * check if the given name has already been used as value in the
   * glossary map or in the entry map
   * \param[in] gm : glossray names
   * \param[in] em : entry names
   * \param[in] n  : glossary name
   */
  static void
  MechanicalBehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& gm,
								 const std::map<std::string,std::string>& em,
								 const std::string& n)
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=gm.begin();p!=gm.end();++p){
      if(p->second==n){
	string msg("MechanicalBehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as a glossary name";
	throw(runtime_error(msg));
      }
    }
    for(p=em.begin();p!=em.end();++p){
      if(p->second==n){
	string msg("MechanicalBehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as an entry name";
	throw(runtime_error(msg));
      }
    }
  }

  /*!
   * \brief associate a glossary name or an entry name to a variable
   * \param[in] m  : map to be updated
   * \param[in] gm : glossary names
   * \param[in] em : entry names
   * \param[in] n  : variable name
   * \param[in] g  : glossray name or entry name
   */
  static void
  MechanicalBehaviourDataAddToGlossaryOrEntryNames(std::map<std::string,std::string>& m,
						   const std::map<std::string,std::string>& gm,
						   const std::map<std::string,std::string>& em,
						   const std::string& n,
						   const std::string& g)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    MechanicalBehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName(gm,em,g);
    if(em.find(n)!=em.end()){
      string msg("MechanicalBehaviourDataAddToGlossaryOrEntryNames : ");
      msg += "an entry name has already been specified for variable '"+n+"'";
      throw(runtime_error(msg));
    }
    if(gm.find(n)!=gm.end()){
      string msg("MechanicalBehaviourDataAddToGlossaryOrEntryNames : ");
      msg += "an entry name has already been specified for variable '"+n+"'";
      throw(runtime_error(msg));
    }
    if(!m.insert(MVType(n,g)).second){
      string msg("MechanicalBehaviourDataAddToGlossaryOrEntryNames : ");
      msg += "glossary name for variable '"+n+"' already specified";
      throw(runtime_error(msg));
    }
  }
  
  void
  MechanicalBehaviourData::throwUndefinedAttribute(const std::string& n)
  {
    using namespace std;
    string msg("MechanicalBehaviourData::getAttribute : "
	       "no attribute named '"+n+"'");
    throw(runtime_error(msg));
  } // end of MechanicalBehaviourData::throwUndefinedAttribute
  
  MechanicalBehaviourData::CodeBlocksAggregator::CodeBlocksAggregator()
    : get_already_called(false)
  {} // end of MechanicalBehaviourData::CodeBlocksAggregator::CodeBlocksAggregator

  void
  MechanicalBehaviourData::CodeBlocksAggregator::update(void)
  {
    this->cblock.code  = cblock_begin;
    if(!this->cblock_body.empty()){
      if(!this->cblock.code.empty()){
	if(*(this->cblock.code.rbegin())!='\n'){
	  this->cblock.code.push_back('\n');
	}
      }
    }
    this->cblock.code += cblock_body;
    if(!this->cblock_end.empty()){
      if(!this->cblock.code.empty()){
	if(*(this->cblock.code.rbegin())!='\n'){
	  this->cblock.code.push_back('\n');
	}
      }
    }
    this->cblock.code += cblock_end;
  } // end of MechanicalBehaviourData::update

  void
  MechanicalBehaviourData::CodeBlocksAggregator::set(const CodeBlock& c,
						    const Position p)
  {
    this->check();
    this->cblock.static_variables.insert(c.static_variables.begin(),
					 c.static_variables.end());
    this->cblock.variables.insert(c.variables.begin(),
				  c.variables.end());
    switch(p){
    case AT_BEGINNING:
      if(!this->cblock_begin.empty()){
	this->cblock_begin += '\n';
      }
      this->cblock_begin += c.code;
      break;
    case BODY:
      if(!this->cblock_body.empty()){
	this->cblock_body += '\n';
      }
      this->cblock_body  += c.code;
      break;
    case AT_END:
      if(!this->cblock_end.empty()){
	this->cblock_end += '\n';
      }
      this->cblock_end   += c.code;
      break; 
    }
    this->update();
  } // end of MechanicalBehaviourData::CodeBlocksAggregator::set

  void
  MechanicalBehaviourData::CodeBlocksAggregator::replace(const CodeBlock& c,
							const Position p)
  {
    this->check();
    this->cblock_begin.clear();
    this->cblock_body.clear();
    this->cblock_end.clear();
    this->cblock.code.clear();
    this->cblock.variables.clear();
    this->cblock.static_variables.clear();
    this->set(c,p);
  } // end of MechanicalBehaviourData::CodeBlocksAggregator::set

  void
  MechanicalBehaviourData::CodeBlocksAggregator::check(void) const
  {
    using namespace std;
    if(this->get_already_called){
      string msg("MechanicalBehaviourData::CodeBlocksAggregator::set : "
		 "can't modifiy a code block");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourData::CodeBlocksAggregator::check

  const CodeBlock&
  MechanicalBehaviourData::CodeBlocksAggregator::get(void) const
  {
    this->get_already_called = true;
    return this->cblock;
  } // end of MechanicalBehaviourData::CodeBlocksAggregator::get

  MechanicalBehaviourData::MechanicalBehaviourData()
    : usableInPurelyImplicitResolution(false)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    this->glossaryNames.insert(MVType("T","Temperature"));
  } // end of MechanicalBehaviourData::MechanicalBehaviourData()

  void
  MechanicalBehaviourData::addStaticVariable(const StaticVariableDescription& v)
  {
    this->registerVariable(v.name);
    this->staticVars.push_back(v);
  } // end of MechanicalBehaviourData::addStaticVariable

  const StaticVariableDescriptionContainer&
  MechanicalBehaviourData::getStaticVariables(void) const
  {
    return this->staticVars;
  } // end of MechanicalBehaviourData::getStaticVariables

  const std::vector<BoundsDescription>&
  MechanicalBehaviourData::getBounds(void) const
  {
    return this->bounds;
  } // end of MechanicalBehaviourData::getBoundsDescriptions

  const VariableDescription&
  MechanicalBehaviourData::getPersistentVariableHandler(const std::string& v) const
  {
    return this->getVariableHandler(this->getPersistentVariables(),v);
  } // end of MechanicalBehaviourData::getPersistentVariableHandler

  const VariableDescription&
  MechanicalBehaviourData::getIntegrationVariableHandler(const std::string& v) const
  {
    return this->getVariableHandler(this->getIntegrationVariables(),v);
  } // end of MechanicalBehaviourData::getIntegrationVariableHandler

  const VariableDescription&
  MechanicalBehaviourData::getStateVariableHandler(const std::string& v) const
  {
    return this->getVariableHandler(this->getStateVariables(),v);
  } // end of MechanicalBehaviourData::getStateVariableHandler

  const VariableDescription&
  MechanicalBehaviourData::getVariableHandler(const VariableDescriptionContainer& cont,
					      const std::string& v) const
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for(p=cont.begin();p!=cont.end();++p){
      if(p->name==v){
	return *p;
      }
    }
    string msg("MechanicalBehaviourData::getVariableHandler : ");
    msg += "no state variable '"+v+"'";
    throw(runtime_error(msg));
    return *(static_cast<VariableDescription*>(0));
  } // end of MechanicalBehaviourData::getVariableHandler

  void
  MechanicalBehaviourData::addMaterialProperty(const VariableDescription& v)
  {
    this->addVariable(this->materialProperties,v);
  } // end of MechanicalBehaviourData::addMaterialProperty

  void
  MechanicalBehaviourData::addIntegrationVariable(const VariableDescription& v)
  {
    this->addVariable(this->integrationVariables,v);
  } // end of MechanicalBehaviourData::addIntegrationVariable

  void
  MechanicalBehaviourData::addStateVariable(const VariableDescription& v)
  {
    using namespace std;
    VariableDescriptionContainer::iterator p;
    this->addVariable(this->stateVariables,v);
    this->addVariable(this->integrationVariables,v,false);
    /*!
     * for compatibility reasons with previous mfront versions
     * (<2.0), auxiliary state variables shall be put after
     * state variables.
     */
    bool found = false;
    p=this->persistentVariables.begin();
    while((p!=this->persistentVariables.end())&&(!found)){
      if(this->isAuxiliaryStateVariableName(p->name)){
	this->persistentVariables.insert(p,v);
	found = true;
      } else {
	++p;
      }
    }
    if(!found){
      this->persistentVariables.push_back(v);
    }
  } // end of MechanicalBehaviourData::addStateVariable

  void
  MechanicalBehaviourData::addAuxiliaryStateVariable(const VariableDescription& v)
  {
    this->addVariable(this->auxiliaryStateVariables,v);
    this->addVariable(this->persistentVariables,v,false);
  } // end of MechanicalBehaviourData::addAuxiliaryStateVariable

  void
  MechanicalBehaviourData::addExternalStateVariable(const VariableDescription& v)
  {
    this->addVariable(this->externalStateVariables,v);
  } // end of MechanicalBehaviourData::addExternalStateVariable

  void
  MechanicalBehaviourData::addLocalVariable(const VariableDescription& v)
  {
    this->addVariable(this->localVariables,v);
  } // end of MechanicalBehaviourData::addLocalVariable

  void
  MechanicalBehaviourData::addParameter(const VariableDescription& v)
  {
    this->addVariable(this->parameters,v);
  } // end of MechanicalBehaviourData::addParameter

  bool
  MechanicalBehaviourData::hasParameter(const std::string& n) const
  {
    return this->parameters.contains(n);
  }

  bool
  MechanicalBehaviourData::hasParameters(void) const
  {
    return !this->parameters.empty();
  }

  bool
  MechanicalBehaviourData::isVariableUsedInCodeBlocks(const std::string& v) const
  {
    using namespace std;
    map<string,CodeBlocksAggregator>::const_iterator pc;
    for(pc=this->cblocks.begin();pc!=this->cblocks.end();++pc){
      const CodeBlock& c = pc->second.get();
      if(c.variables.find(v)!=c.variables.end()){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourData::isVariableUsedInCodeBlocks

  bool
  MechanicalBehaviourData::isMaterialPropertyName(const std::string& n) const
  {
    return this->getMaterialProperties().contains(n);
  } // end of MechanicalBehaviourData::isMaterialPropertyName

  bool
  MechanicalBehaviourData::isStaticVariableName(const std::string& n) const
  {
    return this->getStaticVariables().contains(n);
  } // end of MechanicalBehaviourData::isStaticVariableName

  bool
  MechanicalBehaviourData::isLocalVariableName(const std::string& n) const
  {
    return this->getLocalVariables().contains(n);
  } // end of MechanicalBehaviourData::isLocalVariableName

  bool
  MechanicalBehaviourData::isPersistentVariableName(const std::string& n) const
  {
    return this->getPersistentVariables().contains(n);
  } // end of MechanicalBehaviourData::isPersistentVariableName

  bool
  MechanicalBehaviourData::isIntegrationVariableName(const std::string& n) const
  {
    return this->getIntegrationVariables().contains(n);
  } // end of MechanicalBehaviourData::isIntegrationVariableName

  bool
  MechanicalBehaviourData::isIntegrationVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getIntegrationVariables().contains(n.substr(1));
  } // end of MechanicalBehaviourData::isIntegrationVariableName

  bool
  MechanicalBehaviourData::isStateVariableName(const std::string& n) const
  {
    return this->getStateVariables().contains(n);
  } // end of MechanicalBehaviourData::isStateVariableName

  bool
  MechanicalBehaviourData::isStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getStateVariables().contains(n.substr(1));
  } // end of MechanicalBehaviourData::isStateVariableName

  bool
  MechanicalBehaviourData::isAuxiliaryStateVariableName(const std::string& n) const
  {
    return this->getAuxiliaryStateVariables().contains(n);
  } // end of MechanicalBehaviourData::isStateVariableName
  
  bool
  MechanicalBehaviourData::isExternalStateVariableName(const std::string& n) const
  {
    return this->getExternalStateVariables().contains(n);
  } // end of MechanicalBehaviourData::isExternalStateVariableName

  bool
  MechanicalBehaviourData::isExternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getExternalStateVariables().contains(n.substr(1));
  } // end of MechanicalBehaviourData::isExternalStateVariableName

  bool
  MechanicalBehaviourData::isParameterName(const std::string& n) const
  {
    return this->getParameters().contains(n);
  } // end of MechanicalBehaviourData::isParameterName

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getMaterialProperties(void) const
  {
    return this->materialProperties;
  } // end of MechanicalBehaviourData::getMaterialProperties

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getPersistentVariables(void) const
  {
    return this->persistentVariables;
  } // end of MechanicalBehaviourData::getPersistentVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getIntegrationVariables(void) const
  {
    return this->integrationVariables;
  } // end of MechanicalBehaviourData::getIntegrationVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getStateVariables(void) const
  {
    return this->stateVariables;
  } // end of MechanicalBehaviourData::getStateVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getAuxiliaryStateVariables(void) const
  {
    return this->auxiliaryStateVariables;
  } // end of MechanicalBehaviourData::getAuxiliaryStateVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getExternalStateVariables(void) const
  {
    return this->externalStateVariables;
  } // end of MechanicalBehaviourData::getExternalStateVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getLocalVariables(void) const
  {
    return this->localVariables;
  } // end of MechanicalBehaviourData::getLocalVariables
  
  bool
  MechanicalBehaviourData::isUsableInPurelyImplicitResolution(void) const
  {
    return this->usableInPurelyImplicitResolution;
  } // end of MechanicalBehaviourData::isUsableInPurelyImplicitResolution

  void
  MechanicalBehaviourData::setUsableInPurelyImplicitResolution(const bool b)
  {
    this->usableInPurelyImplicitResolution = b;
  } // end of MechanicalBehaviourData::setUsableInPurelyImplicitResolution

  void
  MechanicalBehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    this->pupirv.insert(n);
  } // end of MechanicalBehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  const std::set<std::string>&
  MechanicalBehaviourData::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const
  {
    return this->pupirv;
  } // end of MechanicalBehaviourData::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution

  void
  MechanicalBehaviourData::requiresTVectorOrVectorIncludes(bool& b1,
							   bool& b2) const
  {
    VariableDescriptionContainer::const_iterator ps;
    b1 = false;
    b2 = false;
    for(ps=this->getMaterialProperties().begin();
	(ps!=this->getMaterialProperties().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getIntegrationVariables().begin();
	(ps!=this->getIntegrationVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getStateVariables().begin();
	(ps!=this->getStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getAuxiliaryStateVariables().begin();
	(ps!=this->getAuxiliaryStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getLocalVariables().begin();
	(ps!=this->getLocalVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getExternalStateVariables().begin();
	(ps!=this->getExternalStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
  } // end of MechanicalBehaviourData::requiresTVectorOrVectorIncludes

  void
  MechanicalBehaviourData::addVariable(VariableDescriptionContainer& c,
				       const VariableDescription& v,
				       const bool b)
  {
    if(b){
      this->registerVariable(v.name);
    }
    c.push_back(v);
  } // end of MechanicalBehaviourData::addVariable

  void
  MechanicalBehaviourData::registerVariable(const std::string& n)
  {
    using namespace std;
    if(!this->vnames.insert(n).second){
      string msg("MechanicalBehaviourData::registerVariable : "
		 "a variable named '"+n+"' has already been registred");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourData::registerVariable

  void
  MechanicalBehaviourData::checkVariableName(const std::string& n) const
  {
    using namespace std;
    if(this->vnames.find(n)==this->vnames.end()){
      string msg("MechanicalBehaviourData::checkVariableName : "
		 "no variable named '"+n+"'");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourData::checkVariableName

  void
  MechanicalBehaviourData::setCode(const std::string& n,
				   const CodeBlock& c,
				   const Mode m,
				   const Position p)
  {
    using namespace std;
    typedef map<string,CodeBlocksAggregator>::value_type MVType;
    map<string,CodeBlocksAggregator>::iterator pc = this->cblocks.find(n);
    if(pc==this->cblocks.end()){
      pc = this->cblocks.insert(MVType(n,CodeBlocksAggregator())).first;
    } else {
      if(m==CREATE){
	string msg("MechanicalBehaviourData::setCode : "
		   "a code block named '"+n+"' already exists.\n"
		   "If you wanted to append this new code to the "
		   "existing one, you shall use the 'Append' option.\n"
		   "You can also replace it with 'Replace' option "
		   "(assuming you know what you are doing).\n");
	throw(runtime_error(msg));
      } else if(m==CREATEORREPLACE){
	this->cblocks.erase(pc);
	pc = this->cblocks.insert(MVType(n,CodeBlocksAggregator())).first;
      } else if(m==CREATEBUTDONTREPLACE){
	return;
      }
    }
    pc->second.set(c,p);
  } // end of MechanicalBehaviourData::setCode


  const CodeBlock&
  MechanicalBehaviourData::getCode(const std::string& n) const
  {
    using namespace std;
    map<string,CodeBlocksAggregator>::const_iterator p = this->cblocks.find(n);
    if(p==this->cblocks.end()){
      string msg("MechanicalBehaviourData::getCode : "
		 "no code block associated with '"+n+"'");
      throw(runtime_error(msg));
    }
    return p->second.get();
  } // end of MechanicalBehaviourData::getCode

  bool
  MechanicalBehaviourData::hasCode(const std::string& n) const
  {
    return this->cblocks.find(n)!=this->cblocks.end();
  }

  void
  MechanicalBehaviourData::setBounds(const BoundsDescription& d)
  {
    this->bounds.push_back(d);
  } // end of MechanicalBehaviourData::setBound

  void
  MechanicalBehaviourData::setParameterDefaultValue(const std::string& n,
						    const double v)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    if(!this->parameters.contains(n)){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    const VariableDescription& p = this->getVariableHandler(this->parameters,n);
    if(p.type!="real"){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "parameter '"+n+"' is not a floatting point");
      throw(runtime_error(msg));
    }
    if(!this->parametersDefaultValues.insert(MVType(n,v)).second){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "default value for parameter '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  }

  void
  MechanicalBehaviourData::setParameterDefaultValue(const std::string& n,
						    const int v)
  {
    using namespace std;
    typedef map<string,int>::value_type MVType;
    if(!this->parameters.contains(n)){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    const VariableDescription& p = this->getVariableHandler(this->parameters,n);
    if(p.type!="int"){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "parameter '"+n+"' is not an integer");
      throw(runtime_error(msg));
    }
    if(!this->iParametersDefaultValues.insert(MVType(n,v)).second){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "default value for parameter '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  }

  void
  MechanicalBehaviourData::setParameterDefaultValue(const std::string& n,
						    const unsigned short v)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    if(!this->parameters.contains(n)){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    const VariableDescription& p = this->getVariableHandler(this->parameters,n);
    if(p.type!="ushort"){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "parameter '"+n+"' is not an unsigned short integer");
      throw(runtime_error(msg));
    }
    if(!this->uParametersDefaultValues.insert(MVType(n,v)).second){
      string msg("MechanicalBehaviourData::setParameterDefaultValue : "
		 "default value for parameter '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  }

  double
  MechanicalBehaviourData::getFloattingPointParameterDefaultValue(const std::string& n) const
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("MechanicalBehaviourData::getFloattingPointParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    map<string,double>::const_iterator p = this->parametersDefaultValues.find(n);
    if(p==this->parametersDefaultValues.end()){
      string msg("MechanicalBehaviourData::getFloattingPointParameterDefaultValue : "
		 "no default value defined for parameter '"+n+"'");
      throw(runtime_error(msg));
    }
    return p->second;
  } // end of MechanicalBehaviourData::getFloattingPointParameterDefaultValue

  int
  MechanicalBehaviourData::getIntegerParameterDefaultValue(const std::string& n) const
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("MechanicalBehaviourData::getIntegerParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    map<string,int>::const_iterator p = this->iParametersDefaultValues.find(n);
    if(p==this->iParametersDefaultValues.end()){
      string msg("MechanicalBehaviourData::getIntegerParameterDefaultValue : "
		 "no default value defined for parameter '"+n+"'");
      throw(runtime_error(msg));
    }
    return p->second;
  } // end of MechanicalBehaviourData::getIntegerParameterDefaultValue

  unsigned short
  MechanicalBehaviourData::getUnsignedShortParameterDefaultValue(const std::string& n) const
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("MechanicalBehaviourData::getUnsignedShortParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    map<string,unsigned short>::const_iterator p = this->uParametersDefaultValues.find(n);
    if(p==this->uParametersDefaultValues.end()){
      string msg("MechanicalBehaviourData::getUnsignedShortParameterDefaultValue : "
		 "no default value defined for parameter '"+n+"'");
      throw(runtime_error(msg));
    }
    return p->second;
  } // end of MechanicalBehaviourData::getUnsignedShortParameterDefaultValue

  void
  MechanicalBehaviourData::setAttribute(const std::string& n,
					const MechanicalBehaviourAttribute& a,
					const bool b)
  {
    using namespace std;
    typedef map<string,MechanicalBehaviourAttribute>::value_type MVType;
    map<string,MechanicalBehaviourAttribute>::const_iterator p;
    if(b){
      p=this->attributes.find(n);
      if(p!=this->attributes.end()){
	if(a.getTypeIndex()!=p->second.getTypeIndex()){
	  string msg("MechanicalBehaviourData::setAttribute : ",
		     "attribute already exists with a different type");
	  throw(runtime_error(msg));
	}
	return;
      }
    }
    if(!this->attributes.insert(MVType(n,a)).second){
      string msg("MechanicalBehaviourData::setAttribute : "
		 "attribute '"+n+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourData::setAttribute

  bool
  MechanicalBehaviourData::hasAttribute(const std::string& n) const
  {
    return this->attributes.count(n)!=0u;
  } // end of MechanicalBehaviourData::hasAttribute
  
  const std::map<std::string,MechanicalBehaviourAttribute>&
  MechanicalBehaviourData::getAttributes() const
  {
    return this->attributes;
  } // end of MechanicalBehaviourData::getAttributes

  std::vector<std::string>
  MechanicalBehaviourData::getCodeBlockNames() const
  {
    using namespace std;
    vector<string> names;
    map<string,CodeBlocksAggregator>::const_iterator pc;
    for(pc=this->cblocks.begin();pc!=this->cblocks.end();++pc){
      names.push_back(pc->first);
    }
    return names;
  } // end of MechanicalBehaviourData::getCodeBlockNames

  std::string
  MechanicalBehaviourData::getGlossaryName(const std::string& n) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    this->checkVariableName(n);
    p=this->glossaryNames.find(n);
    if(p!=this->glossaryNames.end()){
      return p->second;
    }
    p=this->entryNames.find(n);
    if(p!=this->entryNames.end()){
      return p->second;
    }
    return n;
  } // end of MechanicalBehaviourData::getGlossaryNames

  std::vector<std::string>
  MechanicalBehaviourData::getGlossaryNames(const VarContainer& v) const
  {
    using namespace std;
    return v.getGlossaryNames(this->glossaryNames,
			      this->entryNames);
  } // end of MechanicalBehaviourData::getGlossaryNames
  
  void
  MechanicalBehaviourData::getGlossaryNames(std::vector<std::string>& names,
					    const VarContainer& v) const
  {
    v.getGlossaryNames(names,
		       this->glossaryNames,
		       this->entryNames);
  } // end of MechanicalBehaviourData::getGlossaryNames
  
  void
  MechanicalBehaviourData::appendGlossaryNames(std::vector<std::string>& names,
					       const VarContainer& v) const
  {
    v.appendGlossaryNames(names,
			  this->glossaryNames,
			  this->entryNames);
  } // end of MechanicalBehaviourData::getGlossaryNames
  
  void
  MechanicalBehaviourData::setGlossaryName(const std::string& n,
					   const std::string& g)
  {
    this->checkVariableName(n);
    MechanicalBehaviourDataAddToGlossaryOrEntryNames(this->glossaryNames,
						     this->glossaryNames,
						     this->entryNames,n,g);
  } // end of MechanicalBehaviourData::addGlossaryName

  bool
  MechanicalBehaviourData::isGlossaryName(const std::string& n) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=this->glossaryNames.begin();p!=this->glossaryNames.end();++p){
      if(p->second==n){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourData::isGlossaryName
  
  void
  MechanicalBehaviourData::setEntryName(const std::string& n,
					const std::string& e)
  {
    this->checkVariableName(n);
    MechanicalBehaviourDataAddToGlossaryOrEntryNames(this->entryNames,
						     this->glossaryNames,
						     this->entryNames,n,e);
  } // end of MechanicalBehaviourData::addEntryName

  bool
  MechanicalBehaviourData::isEntryName(const std::string& n) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=this->entryNames.begin();p!=this->entryNames.end();++p){
      if(p->second==n){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourData::isEntryName

  std::string
  MechanicalBehaviourData::getVariableNameFromGlossaryNameOrEntryName(const std::string& n) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=this->glossaryNames.begin();p!=this->glossaryNames.end();++p){
      if(p->second==n){
	return p->first;
      }
    }
    for(p=this->entryNames.begin();p!=this->entryNames.end();++p){
      if(p->second==n){
	return p->first;
      }
    }
    string msg("MechanicalBehaviourData::getVariableNameFromGlossaryNameOrEntryName : "
	       "no variable with glossary or entry name '"+n+"'");
    throw(runtime_error(msg));
    return "";
  } // end of MechanicalBehaviourData::getVariableNameFromGlossaryNameOrEntryName

  const VariableDescriptionContainer&
  MechanicalBehaviourData::getParameters(void) const
  {
    return this->parameters;
  } // end of MechanicalBehaviourData::getParameters

} // end of namespace mfront
