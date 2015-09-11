/*! 
 * \file  mfront/src/BehaviourDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"MFront/MFrontLogStream.hxx"
#include"MFront/BehaviourDescription.hxx"

namespace mfront
{

  template<typename Arg1>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1&),
					  const Arg1& a)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a);
      for(auto md : this->sd){
  	(md.second.get()->*m)(a);
      }
    } else {
      (this->getBehaviourData2(h).*m)(a);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Arg1>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1),
					  const Arg1 a)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a);
      for(const auto& ptr : this->sd){
	auto& bdata = *(ptr.second);
	(bdata.*m)(a);
      }
    } else {
      (this->getBehaviourData2(h).*m)(a);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Arg1,
	   typename Arg2>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1&,
								    const Arg2),
					  const Arg1& a,
					  const Arg2  b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a,b);
      for(const auto& ptr : this->sd){
	auto& bdata = *(ptr.second);
	(bdata.*m)(a,b);
      }
    } else {
      (this->getBehaviourData2(h).*m)(a,b);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Arg1,
	   typename Arg2>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1&,
								    const Arg2&),
					  const Arg1& a,
					  const Arg2& b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a,b);
      for(const auto& ptr : this->sd){
	auto& bdata = *(ptr.second);
	(bdata.*m)(a,b);
      }
    } else {
      (this->getBehaviourData2(h).*m)(a,b);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Res,
	   typename Arg1>
  Res
  BehaviourDescription::getData(const Hypothesis h,
				Res (BehaviourData:: *m)(const Arg1&) const,
				const Arg1& a) const
  {
    return (this->getBehaviourData(h).*m)(a);
  } // end of BehaviourDescription::getData

  static void
  BehaviourDescriptionCheckThermalExpansionCoefficientArgument(const MaterialPropertyDescription& a)
  {
    if(!((a.inputs.size())||(a.inputs.size()!=1u))){
      throw(std::runtime_error("BehaviourDescriptionCheckThermalExpansionCoefficientArgument : "
			       "thermal expansion shall only depend on temperature or be constant"));
    }
    if(a.inputs.size()==1u){
      const auto& v = a.inputs.front();
      const auto& vn = v.getExternalName(a.glossaryNames,a.entryNames);
      if(vn!="Temperature"){
	throw(std::runtime_error("BehaviourDescriptionCheckThermalExpansionCoefficientArgument : "
				 "thermal expansion shall only depend on temperature"));
      }
    }
  } // end of BehaviourDescriptionCheckThermalExpansionCoefficientArgument

  const std::string
  BehaviourDescription::requiresStiffnessTensor("requiresStiffnessTensor");
  
  const std::string
  BehaviourDescription::requiresUnAlteredStiffnessTensor("requiresUnAlteredStiffnessTensor");
  
  const std::string
  BehaviourDescription::requiresThermalExpansionCoefficientTensor("requiresThermalExpansionCoefficientTensor");
  
  void
  BehaviourDescription::throwUndefinedAttribute(const std::string& n)
  {
    throw(std::runtime_error("BehaviourDescription::getAttribute : "
			     "no attribute named '"+n+"'"));
  } // end of BehaviourDescription::throwUndefinedAttribute

  BehaviourDescription::BehaviourDescription()
    : use_qt(false),
      type(BehaviourDescription::GENERALBEHAVIOUR),
      // By default, a behaviour is isotropic 
      stype(mfront::ISOTROPIC),
      stypeIsDefined(false),
      // By default, a behaviour is isotropic 
      estype(mfront::ISOTROPIC),
      estypeIsDefined(false),
      ischeme(UNDEFINEDINTEGRATIONSCHEME)
  {} // end of BehaviourDescription::BehaviourDescription()

  const BehaviourData&
  BehaviourDescription::getBehaviourData(const Hypothesis& h) const
  {
    // check that the given hypothesis is supported
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->d;
    }
    this->checkModellingHypothesis(h);
    this->requestedHypotheses.insert(h);
    // check if a specialised version of the mechanical behaviour
    // description has been defined
    const auto p = this->sd.find(h);
    if(p!=this->sd.end()){
      return *(p->second);
    }
    // return the default...
    return this->d;
  } // end of BehaviourDescription::getBehaviourData

  BehaviourData&
  BehaviourDescription::getBehaviourData2(const ModellingHypothesis::Hypothesis& h)
  {
    // check that the given hypothesis is supported
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->d;
    }
    if(this->areModellingHypothesesDefined()){
      this->checkModellingHypothesis(h);
    }
    this->requestedHypotheses.insert(h);
    auto p = this->sd.find(h);
    if(p==this->sd.end()){
      // copy of the default description
      p=this->sd.insert({h,MBDPtr{new BehaviourData{this->d}}}).first;
    }
    return *(p->second);
  } // end of BehaviourDescription::getBehaviourData2

  void
  BehaviourDescription::setBehaviourName(const std::string& m)
  {
    if(!this->behaviour.empty()){
      throw(std::runtime_error("BehaviourDescription::setBehaviourName : "
			       "behaviour name already defined"));
    }
    this->behaviour = m;
    this->updateClassName();
  } // end of BehaviourDescription::setBehaviourName

  const std::string&
  BehaviourDescription::getBehaviourName(void) const
  {
    if(this->behaviour.empty()){
      throw(std::runtime_error("BehaviourDescription::getBehaviourName : "
			       "behaviour name not defined"));
    }
    return this->behaviour;
  } // end of BehaviourDescription::getBehaviourName

  void
  BehaviourDescription::setParserName(const std::string& m)
  {
    if(!this->parser.empty()){
      throw(std::runtime_error("BehaviourDescription::setParserName : "
			       "parser name already defined"));
    }
    this->parser = m;
    this->updateClassName();
  } // end of BehaviourDescription::setParserName

  const std::string&
  BehaviourDescription::getParserName(void) const
  {
    if(this->parser.empty()){
      throw(std::runtime_error("BehaviourDescription::getParserName : "
			       "parser name not defined"));
    }
    return this->parser;
  } // end of BehaviourDescription::getParserName

  void
  BehaviourDescription::setIntegrationScheme(const BehaviourDescription::IntegrationScheme s)
  {
    if(this->ischeme!=UNDEFINEDINTEGRATIONSCHEME){
      throw(std::runtime_error("BehaviourDescription::setIntegrationScheme : "
			       "integration scheme already defined"));
    }
    this->ischeme = s;
  } // end of BehaviourDescription::setIntegrationScheme

  BehaviourDescription::IntegrationScheme
  BehaviourDescription::getIntegrationScheme(void) const
  {
    if(this->ischeme==UNDEFINEDINTEGRATIONSCHEME){
      throw(std::runtime_error("BehaviourDescription::getIntegrationScheme : "
			       "the integration scheme is undefined"));
    }
    return this->ischeme;
  } // end of BehaviourDescription::getIntegrationScheme

  void
  BehaviourDescription::setLibrary(const std::string& l)
  {
    using namespace std;
    if(!this->library.empty()){
      string msg("BehaviourDescription::setLibrary : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->library = l;
  } // end of BehaviourDescription::setLibrary

  const std::string&
  BehaviourDescription::getLibrary(void) const
  {
    return this->library;
  } // end of BehaviourDescription::getLibrary

  void
  BehaviourDescription::setMaterialName(const std::string& m)
  {
    using namespace std;
    if(!this->material.empty()){
      string msg("BehaviourDescription::setMaterialName : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->material = m;
    this->updateClassName();
  } // end of BehaviourDescription::setMaterialName

  const std::string&
  BehaviourDescription::getMaterialName(void) const
  {
    return this->material;
  } // end of BehaviourDescription::getMaterialName

  void
  BehaviourDescription::setClassName(const std::string& n)
  {
    using namespace std;
    if(!this->className.empty()){
      string msg("BehaviourDescription::setClassName : ");
      msg += "class name alreay defined";
      throw(runtime_error(msg));
    }
    this->className = n;
  } // end of BehaviourDescription::setClassName

  const std::string&
  BehaviourDescription::getClassName(void) const
  {
    using namespace std;
    if(this->className.empty()){
      string msg("BehaviourDescription::getClassName : ");
      msg += "class name not defined";
      throw(runtime_error(msg));
    }
    return this->className;
  } // end of BehaviourDescription::getClassName

  void
  BehaviourDescription::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of BehaviourDescription::appendToIncludes

  const std::string&
  BehaviourDescription::getIncludes(void) const
  {
    return this->includes;
  } // end of BehaviourDescription::getIncludes

  void
  BehaviourDescription::appendToMembers(const std::string& c)
  {
    this->members+=c;
    if(!this->members.empty()){
      if(*(this->members.rbegin())!='\n'){
	this->members+='\n';
      }
    }
  } // end of BehaviourDescription::appendToMembers

  const std::string&
  BehaviourDescription::getMembers(void) const
  {
    return this->members;
  } // end of BehaviourDescription::getMembers

  void
  BehaviourDescription::appendToPrivateCode(const std::string& c)
  {
    this->privateCode+=c;
    if(!this->privateCode.empty()){
      if(*(this->privateCode.rbegin())!='\n'){
	this->privateCode+='\n';
      }
    }
  } // end of BehaviourDescription::appendToPrivateCode

  const std::string&
  BehaviourDescription::getPrivateCode(void) const
  {
    return this->privateCode;
  } // end of BehaviourDescription::getPrivateCode

  void
  BehaviourDescription::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of BehaviourDescription::appendToSources

  const std::string&
  BehaviourDescription::getSources(void) const
  {
    return this->sources;
  } // end of BehaviourDescription::getSources

  BehaviourDescription::BehaviourType
  BehaviourDescription::getBehaviourType() const
  {
    return this->type;
  } // end of BehaviourDescription::getBehaviourType

  std::string
  BehaviourDescription::getBehaviourTypeFlag(void) const
  {
    using namespace std;
    std::string btype;
    if(this->getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      btype="MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR";
    } else if (this->getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      btype="MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR";
    } else if (this->getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL){
      btype="MechanicalBehaviourBase::COHESIVEZONEMODEL";
    } else {
      throw(runtime_error("BehaviourDescription::getBehaviourTypeFlag : "
			  "unsupported behaviour type"));
    }
    return btype;
  } // end of BehaviourDescription::getBehaviourTypeFlag

  BehaviourSymmetryType
  BehaviourDescription::getElasticSymmetryType() const
  {
    if(!this->estypeIsDefined){
      this->estype = this->getSymmetryType();
      this->estypeIsDefined=true;
    }
    return this->estype;
  } // end of BehaviourDescription::getElasticSymmetryType

  void
  BehaviourDescription::setElasticSymmetryType(const BehaviourSymmetryType t)
  {
    using namespace std;
    if(this->estypeIsDefined){
      string msg("BehaviourDescription::setElasticSymmetryType : "
		 "elastic symmetry type already declared");
      throw(runtime_error(msg));
    }
    const BehaviourSymmetryType s = this->getSymmetryType();
    if((s==mfront::ISOTROPIC)&&(t==mfront::ORTHOTROPIC)){
      string msg("BehaviourDescription::setElasticSymmetryType : "
		 "can't define an orthotropic elastic symmetry for "
		 "an isotropic material");
      throw(runtime_error(msg));
    }
    this->estype = t;
    this->estypeIsDefined=true;
  } // end of BehaviourDescription::setElasticSymmetryType

  bool
  BehaviourDescription::isElasticSymmetryTypeDefined() const
  {
    return this->estypeIsDefined;
  } // end of BehaviourDescription::isElasticSymmetryTypeDefined

  BehaviourSymmetryType
  BehaviourDescription::getSymmetryType() const
  {
    if(!this->stypeIsDefined){
      this->stype = mfront::ISOTROPIC;
      this->stypeIsDefined=true;
    }
    return this->stype;
  } // end of BehaviourDescription::getSymmetryType

  void
  BehaviourDescription::setSymmetryType(const BehaviourSymmetryType t)
  {
    using namespace std;
    if(this->stypeIsDefined){
      string msg("BehaviourDescription::setSymmetryType : "
		 "symmetry type already declared");
      throw(runtime_error(msg));
    }
    this->stype = t;
    this->stypeIsDefined=true;
  } // end of BehaviourDescription::setSymmetryType
  
  bool
  BehaviourDescription::isSymmetryTypeDefined() const
  {
    return this->stypeIsDefined;
  } // end of BehaviourDescription::setSymmetryType
  
  void
  BehaviourDescription::declareAsASmallStrainStandardBehaviour(void)
  {
    if(!this->mvariables.empty()){
      throw(std::runtime_error("BehaviourDescription::declareAsASmallStrainStandardBehaviour: "
			       "some driving variables are already declared"));
    }
    DrivingVariable eto;
    eto.name = "eto";
    eto.type = "StrainStensor";
    eto.increment_known = true;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert({eto,sig});
    this->type = BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"eto");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"deto");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"sig");
  }

  void
  BehaviourDescription::declareAsAFiniteStrainStandardBehaviour(void)
  {
    if(!this->mvariables.empty()){
      throw(std::runtime_error("BehaviourDescription::declareAsAFiniteStrainStandardBehaviour: "
			       "some driving variables are already declared"));
    }
    DrivingVariable F;
    F.name = "F";
    F.type = "DeformationGradientTensor";
    F.increment_known = false;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert({F,sig});
    this->type = BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"F0");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"F1");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"sig");
  }

  void
  BehaviourDescription::addLibraryDependency(const std::string& l){
    if(std::find(this->librariesDependencies.begin(),
		 this->librariesDependencies.end(),l)==this->librariesDependencies.end()){
      this->librariesDependencies.push_back(l);
    }
  } // end of BehaviourDescription::addLibraryDependency

  const std::vector<std::string>&
  BehaviourDescription::getLibrariesDependencies(void) const
  {
    return this->librariesDependencies;
  }
  
  void
  BehaviourDescription::declareAsACohesiveZoneModel(void)
  {
    using namespace std;
    if(!this->mvariables.empty()){
      string msg("BehaviourDescription::declareAsACohesiveZoneModel : ");
      msg += "some driving variables are already declared";
      throw(runtime_error(msg));
    }
    DrivingVariable u;
    u.name = "u";
    u.type = "DisplacementTVector";
    u.increment_known = true;
    ThermodynamicForce t;
    t.name = "t";
    t.type = "ForceTVector";
    this->mvariables.insert({u,t});
    this->type = BehaviourDescription::COHESIVEZONEMODEL;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"u");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"du");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"t");
  }

  void
  BehaviourDescription::addMainVariable(const DrivingVariable&    v,
					const ThermodynamicForce& f)
  {
    if(this->type!=BehaviourDescription::GENERALBEHAVIOUR){
      throw(std::runtime_error("BehaviourDescription::addMainVariables : "
			       "one can not add a main variable if the behaviour "
			       "don't have a general behaviour type"));
    }
    if(!this->mvariables.insert({v,f}).second){
      throw(std::runtime_error("BehaviourDescription::addMainVariables : "
			       "a driving variable '"+v.name+"' has "
			       "already been declared"));
    }
  } // end of BehaviourDescription::addMainVariables

  const std::map<DrivingVariable,
		 ThermodynamicForce>&
  BehaviourDescription::getMainVariables(void) const
  {
    return this->mvariables;
  } // end of BehaviourDescription::getMainVariables
  
  bool
  BehaviourDescription::isDrivingVariableName(const std::string& n) const
  {
    for(const auto& v : this->getMainVariables()){
      if(v.first.name==n){
	return true;
      }
    }
    return false;
  } // end of BehaviourDescription::isDrivingVariableName

  bool
  BehaviourDescription::isDrivingVariableIncrementName(const std::string& n) const
  {
    for(const auto& v : this->getMainVariables()){
      const auto& dv = v.first;
      if(dv.increment_known){
	if("d"+dv.name==n){
	  return true;
	}
      }
    }
    return false;
  } // end of BehaviourDescription::isDrivingVariableIncrementName

  std::pair<SupportedTypes::TypeSize,
	    SupportedTypes::TypeSize>
  BehaviourDescription::getMainVariablesSize(void) const
  {
    auto ov = SupportedTypes::TypeSize{};
    auto of = SupportedTypes::TypeSize{};
    for(const auto& v : this->getMainVariables()){
      ov += this->getTypeSize(v.first.type,1u);
      of += this->getTypeSize(v.second.type,1u);
    }
    return {ov,of};
  } // end of BehaviourDescription::getMainVariablesSize

  void
  BehaviourDescription::setThermalExpansionCoefficient(const std::shared_ptr<MaterialPropertyDescription>& a)
  {
    if(this->areThermalExpansionCoefficientsDefined()){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient : "
			       "thermal expansion coefficient already defined"));
    }
    if(this->getSymmetryType()!=mfront::ISOTROPIC){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient : "
			       "the mechanical behaviour is not isotropic"));
    }
    BehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a);
    this->thermalExpansionCoefficients.push_back(a);
  } // end of BehaviourDescription::setThermalExpansionCoefficient

  void
  BehaviourDescription::setThermalExpansionCoefficients(const std::shared_ptr<MaterialPropertyDescription>& a1,
							const std::shared_ptr<MaterialPropertyDescription>& a2,
							const std::shared_ptr<MaterialPropertyDescription>& a3)
  {
    if(this->areThermalExpansionCoefficientsDefined()){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient : "
			       "thermal expansion coefficient already defined"));
    }
    if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient : "
			       "the mechanical behaviour is not orthotropic."));
    }
    BehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a1);
    BehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a2);
    BehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a3);
    this->thermalExpansionCoefficients.push_back(a1);
    this->thermalExpansionCoefficients.push_back(a2);
    this->thermalExpansionCoefficients.push_back(a3);
  } // end of BehaviourDescription::setThermalExpansionCoefficients

  bool
  BehaviourDescription::areThermalExpansionCoefficientsDefined(void) const
  {
    return !this->thermalExpansionCoefficients.empty();
  } // end of BehaviourDescription::areThermalExpansionCoefficientsDefined

  const std::vector<std::shared_ptr<MaterialPropertyDescription> >&
  BehaviourDescription::getThermalExpansionCoefficients(void) const
  {
    if(!this->areThermalExpansionCoefficientsDefined()){
      throw(std::runtime_error("BehaviourDescription::getThermalExpansionCoefficients : "
			       "no thermal expansion coefficients defined"));
    }
    return this->thermalExpansionCoefficients;
  }

  void
  BehaviourDescription::setUseQt(const bool b)
  {
    if (this->use_qt) {
      throw(std::runtime_error("BehaviourDescription::setUseQt : "
			       "setUseQt already called"));
    }
    this->use_qt = b;
  } // end of BehaviourDescription::setUseQt

  bool
  BehaviourDescription::useQt(void) const
  {
    return this->use_qt;
  } // end of BehaviourDescription::useQt

  std::string
  BehaviourDescription::getTangentOperatorType(void) const
  {
    if(this->type==GENERALBEHAVIOUR){
      auto msizes = this->getMainVariablesSize();
      std::ostringstream t;
      t << "tfel::math::tmatrix<"
	<< msizes.first  << "," 
	<< msizes.second << ",real>";
      return t.str();
    } else if(this->type==SMALLSTRAINSTANDARDBEHAVIOUR){
      return "StiffnessTensor";
    } else if(this->type==FINITESTRAINSTANDARDBEHAVIOUR){
      return "FiniteStrainBehaviourTangentOperator<N,stress>";
    } else if(this->type==COHESIVEZONEMODEL){
      return "tfel::math::tmatrix<N,N,stress>";
    }
    throw(std::runtime_error("BehaviourDescription::getStiffnessOperatorType : "
			     "internal error (unsupported behaviour type)"));
  } // end of BehaviourDescription::getStiffnessOperatorType

  std::string
  BehaviourDescription::getStressFreeExpansionType(void) const
  {
    if((this->type==SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->type==FINITESTRAINSTANDARDBEHAVIOUR)){
      return "StrainStensor";
    }
    throw(std::runtime_error("BehaviourDescription::getStressFreeExpansionType : "
			     "internal error (unsupported behaviour type)"));
  } // end of BehaviourDescription::getStressFreeExpansionType

  void
  BehaviourDescription::checkModellingHypothesis(const Hypothesis& h) const
  {
    if(this->getModellingHypotheses().find(h)==this->getModellingHypotheses().end()){
      std::ostringstream msg;
      msg << "BehaviourDescription::checkModellingHypothesis : "
	  << "modelling hypothesis '" << ModellingHypothesis::toString(h) 
	  << "' is not supported. Refer to the documentation of "
	  << "the '@ModellingHypothesis' or "
	  << "the '@ModellingHypotheses' keywords for details.\n";
      msg << "Supported modelling hypotheses are :";
      for(const auto & elem : this->hypotheses){
	msg << "\n- '" << ModellingHypothesis::toString(elem) << "'";
      }
      throw(std::runtime_error(msg.str()));
    }
  } // end of BehaviourDescription::checkModellingHypothesis

  bool
  BehaviourDescription::areModellingHypothesesDefined(void) const
  {
    return !this->hypotheses.empty();
  } // end of BehaviourDescription::areModellingHypothesesDefined

  const std::set<BehaviourDescription::Hypothesis>&
  BehaviourDescription::getModellingHypotheses(void) const
  {
    if(this->hypotheses.empty()){
      throw(std::runtime_error("BehaviourDescription::getModellingHypotheses : "
			       "hypothesis undefined yet"));
    }
    return this->hypotheses;
  } // end of BehaviourDescription::getModellingHypotheses

  std::set<BehaviourDescription::Hypothesis>
  BehaviourDescription::getDistinctModellingHypotheses(void) const
  {
    using namespace std;
    set<Hypothesis> h;
    const auto& mh = this->getModellingHypotheses();
    if(mh.size()==1u){
      // if only one modelling hypothesis is supported, it is not
      // considered as specialised, so we return it.
      return mh;
    }
    if(!this->areAllMechanicalDataSpecialised()){
      // We return UNDEFINEDHYPOTHESIS to take into account all the
      // modelling hypotheses that were not specialised
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & elem : mh){
      if(this->hasSpecialisedMechanicalData(elem)){
	h.insert(elem);
      }
    }
    return h;
  } // end of BehaviourDescription::getDistinctModellingHypotheses


  bool
  BehaviourDescription::isModellingHypothesisSupported(const Hypothesis h) const
  {
    return this->getModellingHypotheses().count(h)!=0u;
  }

  void
  BehaviourDescription::setModellingHypotheses(const std::set<Hypothesis>& h,
					       const bool b)
  {
    using namespace std;
    // never ever trust a user
    if(h.empty()){
      string msg();
      throw(runtime_error("BehaviourDescription::setHypotheses : "
			  "empty set of modelling hypotheses specificied"));
    }
    // check that the user did not already set the modelling hypotheses
    if(!this->hypotheses.empty()){
      string msg("BehaviourDescription::setHypotheses : "
		 "supported modelling hypotheses have already been declared");
      throw(runtime_error(msg));
    }
    // check that if a specialised version of the mechanical behaviour
    // is defined, it is present in the set of hypotheses defined here
    map<Hypothesis,MBDPtr>::const_iterator p;
    for(p=this->sd.begin();p!=this->sd.end();++p){
      if(h.find(p->first)==h.end()){
	string msg("BehaviourDescription::setHypotheses : "
		   "partial specialisation of the behaviour exists for "
		   "the hypothesis '"+ModellingHypothesis::toString(p->first)+"' "
		   "which is not in the set of hypotheses which have to be "
		   "supported by the behaviour.");
	throw(runtime_error(msg));
      }
    }
    set<Hypothesis>::const_iterator p2;
    for(p2=this->requestedHypotheses.begin();
	p2!=this->requestedHypotheses.end();++p2){
      if(h.find(*p2)==h.end()){
	string msg("BehaviourDescription::setHypotheses : "
		   "a description of the behaviour for "
		   "the hypothesis '"+ModellingHypothesis::toString(*p2)+"' "
		   "has been requested earlier, but this hypothesis is not "
		   "in the set of hypotheses which will to be "
		   "supported by the behaviour. This may lead to inconsistencies. "
		   "Cowardly aborting.");
	throw(runtime_error(msg));
      }
    }
    if(this->hypotheses.empty()){
      this->hypotheses.insert(h.begin(),h.end());
    } else {
      if(b){
	// find the intersection of the given hypotheses and the
	// existing one
	set<Hypothesis> nh;
	for(set<Hypothesis>::const_iterator ph=this->hypotheses.begin();
	    ph!=this->hypotheses.end();++ph){
	  if(h.find(*ph)!=h.end()){
	    nh.insert(*ph);
	  }
	}
	if(nh.empty()){
	  string msg("BehaviourDescription::setHypotheses : "
		     "intersection of previously modelling hypotheses "
		     "with the new ones is empty");
	  throw(runtime_error(msg));
	}
	this->hypotheses.swap(nh);
      } else {
	string msg("BehaviourDescription::setHypotheses : "
		   "supported modelling hypotheses have already been declared");
	throw(runtime_error(msg));
      }
    }
  } // end of BehaviourDescription::setModellingHypotheses

  void
  BehaviourDescription::addMaterialProperties(const Hypothesis h,
					      const VariableDescriptionContainer& v,
					      const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addMaterialProperty(const Hypothesis h,
					    const VariableDescription& v,
					    const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addIntegrationVariables(const Hypothesis h,
						const VariableDescriptionContainer& v,
						const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addIntegrationVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addStateVariables(const Hypothesis h,
					  const VariableDescriptionContainer& v,
					  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addStateVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addIntegrationVariable(const Hypothesis h,
					       const VariableDescription& v,
					       const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addIntegrationVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addStateVariable(const Hypothesis h,
					 const VariableDescription& v,
					 const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addStateVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addAuxiliaryStateVariables(const Hypothesis h,
						   const VariableDescriptionContainer& v,
						   const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addAuxiliaryStateVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addAuxiliaryStateVariable(const Hypothesis h,
						  const VariableDescription& v,
						  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addAuxiliaryStateVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addExternalStateVariables(const Hypothesis h,
						  const VariableDescriptionContainer& v,
						  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addExternalStateVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addExternalStateVariable(const Hypothesis h,
						 const VariableDescription& v,
						 const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addExternalStateVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addLocalVariables(const Hypothesis h,
					  const VariableDescriptionContainer& v,
					  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addLocalVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addLocalVariable(const Hypothesis h,
					 const VariableDescription& v,
					 const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addLocalVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addParameter(const Hypothesis h,
				     const VariableDescription& v,
				     const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addParameter;
    this->addVariable(h,v,s,f);
  }

  bool
  BehaviourDescription::hasGlossaryName(const Hypothesis h,
					const std::string& v) const
  {
    return this->getData(h,&BehaviourData::hasGlossaryName,v);
  } // end of BehaviourDescription::hasGlossaryName
  
  bool
  BehaviourDescription::hasEntryName(const Hypothesis h,
				     const std::string& v) const
  {
    return this->getData(h,&BehaviourData::hasEntryName,v);
  } // end of BehaviourDescription::hasEntryName
  
  bool
  BehaviourDescription::hasParameter(const Hypothesis h,
				     const std::string& v) const
  {
    return this->getData(h,&BehaviourData::hasParameter,v);
  } // end of BehaviourDescription::hasParameter

  bool
  BehaviourDescription::hasParameters(const Hypothesis h) const
  {
    return this->getBehaviourData(h).hasParameters();
  } // end of BehaviourDescription::hasParameters

  bool
  BehaviourDescription::hasParameters(void) const
  {
    using namespace std;
    if(this->d.hasParameters()){
      return true;
    }
    map<Hypothesis,MBDPtr>::const_iterator p;
    for(p=this->sd.begin();p!=this->sd.end();++p){
      if(p->second->hasParameters()){
	return true;
      }
    }
    return false;
  } // end of BehaviourDescription::hasParameters
    
  void
  BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
						 const std::string& n,
						 const double v)
  {
    void (BehaviourData:: *mptr)(const std::string&,
				 const double);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h,mptr,n,v);
  }

  void
  BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
						 const std::string& n,
						 const int v)
  {
    void (BehaviourData:: *mptr)(const std::string&,
				 const int v);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h,mptr,n,v);
  }

  void
  BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
						 const std::string& n,
						 const unsigned short v)
  {
    void (BehaviourData:: *mptr)(const std::string&,
				 const unsigned short v);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h,mptr,n,v);
  }

  unsigned short
  BehaviourDescription::getUnsignedShortParameterDefaultValue(const Hypothesis h,
							      const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getUnsignedShortParameterDefaultValue,n);
  } // end of BehaviourDescription::getUnsignedShortParameterDefaultValue

  int
  BehaviourDescription::getIntegerParameterDefaultValue(const Hypothesis h,
							const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getIntegerParameterDefaultValue,n);
  } // end of BehaviourDescription::getIntegerParameterDefaultValue

  double
  BehaviourDescription::getFloattingPointParameterDefaultValue(const Hypothesis h,
							       const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getFloattingPointParameterDefaultValue,n);
  } // end of BehaviourDescription::getFloattingPointParameterDefaultValue

  void
  BehaviourDescription::addStaticVariable(const Hypothesis h,
					  const StaticVariableDescription& v,
					  const BehaviourData::RegistrationStatus s)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->d.addStaticVariable(v,s);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->sd.begin();p!=this->sd.end();++p){
	p->second->addStaticVariable(v,s);
      }
    } else {
      this->getBehaviourData2(h).addStaticVariable(v,s);
    }
  } // end of BehaviourDescription::addStaticVariable

  void
  BehaviourDescription::addVariables(const Hypothesis h,
				     const VariableDescriptionContainer& v,
				     const BehaviourData::RegistrationStatus s,
				     void (BehaviourData::* m)(const VariableDescription&,
							       const BehaviourData::RegistrationStatus))
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->addVariables(this->d,v,s,m);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->sd.begin();p!=this->sd.end();++p){
	this->addVariables(*(p->second),v,s,m);
      }
    } else {
      this->addVariables(this->getBehaviourData2(h),v,s,m);
    }
  }

  void
  BehaviourDescription::addVariable(const Hypothesis h,
				    const VariableDescription& v,
				    const BehaviourData::RegistrationStatus s,
				    void (BehaviourData::* m)(const VariableDescription&,
							      const BehaviourData::RegistrationStatus))
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->addVariable(this->d,v,s,m);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->sd.begin();p!=this->sd.end();++p){
	this->addVariable(*(p->second),v,s,m);
      }
    } else {
      this->addVariable(this->getBehaviourData2(h),v,s,m);
    }
  }

  void
  BehaviourDescription::addVariables(BehaviourData& b,
				     const VariableDescriptionContainer& v,
				     const BehaviourData::RegistrationStatus s,
				     void (BehaviourData::* m)(const VariableDescription&,
							       const BehaviourData::RegistrationStatus))
  {
    for(const auto & elem : v){
      this->addVariable(b,elem,s,m);
    }
  }

  void
  BehaviourDescription::addVariable(BehaviourData& b,
				    const VariableDescription& v,
				    const BehaviourData::RegistrationStatus s,
				    void (BehaviourData::* m)(const VariableDescription&,
							      const BehaviourData::RegistrationStatus))
  {
    (b.*m)(v,s);
  }

  bool BehaviourDescription::areAllMechanicalDataSpecialised(void) const
  {
    return this->getModellingHypotheses().size()==this->sd.size();
  } // end of BehaviourDescription::areAllMechanicalDataSpecialised

  bool BehaviourDescription::areAllMechanicalDataSpecialised(const std::set<Hypothesis>& h) const
  {
    using namespace std;
    for(const auto & elem : h){
      if(!this->hasSpecialisedMechanicalData(elem)){
	return false;
      }
    }
    return true;
  } // end of BehaviourDescription::areAllMechanicalDataSpecialised

  bool
  BehaviourDescription::hasSpecialisedMechanicalData(const Hypothesis h) const
  {
    using namespace std;
    using namespace std;
    if(this->getModellingHypotheses().find(h)==this->getModellingHypotheses().end()){
      string msg("BehaviourDescription::areAllMechanicalDataSpecialised : "
		 "hypothesis '"+ModellingHypothesis::toString(h)+"' is not supported");
      throw(runtime_error(msg));
    }
    return this->sd.find(h)!=this->sd.end();
  }

  void
  BehaviourDescription::requiresTVectorOrVectorIncludes(bool& b1,
							bool& b2)
  {
    using namespace std;
    b1 = b2 = false;
    if(this->hypotheses.empty()){
      string msg("BehaviourDescription::areAllMechanicalDataSpecialised : "
		 "no hypothesis defined");
      throw(runtime_error(msg));
    }
    if(!this->areAllMechanicalDataSpecialised()){
      this->d.requiresTVectorOrVectorIncludes(b1,b2);
    }
    map<Hypothesis,MBDPtr>::iterator p;
    for(p=this->sd.begin();(p!=this->sd.end())&&(!(b1&&b2));++p){
      p->second->requiresTVectorOrVectorIncludes(b1,b2);
    }
  } // end of BehaviourDescription::requiresTVectorOrVectorIncludes

  void
  BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis h,
											       const std::string& n)
  {
    void (BehaviourData::* m)(const std::string&) =
      &BehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution;
    this->callBehaviourData(h,m,n);
  } // end of BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  void
  BehaviourDescription::setUsableInPurelyImplicitResolution(const Hypothesis h,
							    const bool b)
  {
    void (BehaviourData::* m)(const bool) =
      &BehaviourData::setUsableInPurelyImplicitResolution;
    this->callBehaviourData(h,m,b);
  } // end of BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  bool
  BehaviourDescription::isMemberUsedInCodeBlocks(const Hypothesis h,
						   const std::string& v) const
  {
    return this->getData(h,&BehaviourData::isMemberUsedInCodeBlocks,v);
  } // end of BehaviourDescription::isMaterialPropertyName

  bool
  BehaviourDescription::isMaterialPropertyName(const Hypothesis h,
					       const std::string& v) const
  {
    return this->getData(h,&BehaviourData::isMaterialPropertyName,v);
  } // end of BehaviourDescription::isMaterialPropertyName

  bool
  BehaviourDescription::isLocalVariableName(const Hypothesis h,
					    const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isLocalVariableName,n);
  }  // end of BehaviourDescription::isLocalVariableName

  bool
  BehaviourDescription::isPersistentVariableName(const Hypothesis h,
						 const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isPersistentVariableName,n);
  }  // end of BehaviourDescription::isPersistentVariableName

  bool
  BehaviourDescription::isIntegrationVariableName(const Hypothesis h,
						  const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isIntegrationVariableName,n);
  }  // end of BehaviourDescription::isIntegrationVariableName

  bool
  BehaviourDescription::isIntegrationVariableIncrementName(const Hypothesis h,
							   const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isIntegrationVariableIncrementName,n);
  }  // end of BehaviourDescription::isIntegrationVariableIncrementName

  bool
  BehaviourDescription::isStateVariableName(const Hypothesis h,
					    const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isStateVariableName,n);
  }  // end of BehaviourDescription::isStateVariableName

  bool
  BehaviourDescription::isStateVariableIncrementName(const Hypothesis h,
						     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isStateVariableIncrementName,n);
  }  // end of BehaviourDescription::isStateVariableIncrementName

  bool
  BehaviourDescription::isAuxiliaryStateVariableName(const Hypothesis h,
						     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isAuxiliaryStateVariableName,n);
  }  // end of BehaviourDescription::isAuxiliaryStateVariableName

  bool
  BehaviourDescription::isExternalStateVariableName(const Hypothesis h,
						    const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isExternalStateVariableName,n);
  }  // end of BehaviourDescription::isExternalStateVariableName

  bool
  BehaviourDescription::isExternalStateVariableIncrementName(const Hypothesis h,
							     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isExternalStateVariableIncrementName,n);
  }  // end of BehaviourDescription::isExternalStateVariableIncrementName

  bool
  BehaviourDescription::isParameterName(const Hypothesis h,
					const std::string& v) const
  {
    return this->getData(h,&BehaviourData::isParameterName,v);
  } // end of BehaviourDescription::isParameterName

  bool
  BehaviourDescription::isStaticVariableName(const Hypothesis h,
					     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isStaticVariableName,n);
  }  // end of BehaviourDescription::isStaticVariableName

  void
  BehaviourDescription::updateClassName(void)
  {
    if((!this->behaviour.empty())||
       (!this->material.empty())){
      this->className = this->material+this->behaviour;
    }
  } // end of BehaviourDescription::updateClassName
  
  void
  BehaviourDescription::setCode(const Hypothesis h,
				const std::string& n,
				const CodeBlock& c,
				const Mode m,
				const Position p,
				const bool b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDescription::setCode : setting '"
	    << n << "' on default hypothesis"  << endl;
      }
      this->d.setCode(n,c,m,p,b);
      for(const auto& pd : sd){
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  auto& log = getLogStream();
	  log << "BehaviourDescription::setCode : setting '"
	      << n << "' on hypothesis '" 
	      << ModellingHypothesis::toString(pd.first) << "'" << endl;
	}
	pd.second->setCode(n,c,m,p,b);
      }
    } else {
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDescription::setCode : setting '"
	    << n << "' on hypothesis '" 
	    << ModellingHypothesis::toString(h) << "'" << endl;
      }
      this->getBehaviourData2(h).setCode(n,c,m,p,b);
    }
  } // end of BehaviourDescription::setCode

  const CodeBlock&
  BehaviourDescription::getCodeBlock(const Hypothesis h,
				     const std::string& n) const
  {
    return this->getBehaviourData(h).getCodeBlock(n);
  } // end of BehaviourDescription::getCode

  std::string
  BehaviourDescription::getCode(const Hypothesis h,
				const std::string& n) const
  {
    const bool b = this->getAttribute(BehaviourData::profiling,false);
    return this->getBehaviourData(h).getCode(n,this->getClassName(),b);
  } // end of BehaviourDescription::getCode

  bool
  BehaviourDescription::hasCode(const Hypothesis h,
				const std::string& n) const
  {
    return this->getBehaviourData(h).hasCode(n);
  } // end of BehaviourDescription::getCode

  void
  BehaviourDescription::setBounds(const Hypothesis h,
				  const BoundsDescription& b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->d.setBounds(b);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->sd.begin();p!=this->sd.end();++p){
	p->second->setBounds(b);
      }
    } else {
      this->getBehaviourData2(h).setBounds(b);
    }
  } // end of BehaviourDescription::setBounds

  void
  BehaviourDescription::setAttribute(const Hypothesis h,
				     const std::string& n,
				     const BehaviourAttribute& a,
				     const bool b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->d.setAttribute(n,a,b);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->sd.begin();p!=this->sd.end();++p){
  	BehaviourData& bdata = *(p->second);
  	bdata.setAttribute(n,a,b);
      }
    } else {
      this->getBehaviourData2(h).setAttribute(n,a,b);
    }
  } // end of BehaviourDescription::setAttribute

  bool
  BehaviourDescription::hasAttribute(const Hypothesis h,
				     const std::string& n) const
  {
    return this->getBehaviourData(h).hasAttribute(n);
  } // end of BehaviourDescription::hasAttribute

  std::vector<std::string>
  BehaviourDescription::getCodeBlockNames(const Hypothesis h) const
  {
    return this->getBehaviourData(h).getCodeBlockNames();
  }

  std::vector<std::string>
  BehaviourDescription::getExternalNames(const Hypothesis h,
					 const VarContainer& v) const
  {
    return this->getBehaviourData(h).getExternalNames(v);
  }

  void
  BehaviourDescription::getExternalNames(std::vector<std::string>& n,
					 const Hypothesis h,
					 const VarContainer& v) const
  {
    return this->getBehaviourData(h).getExternalNames(n,v);
  } // end of BehaviourDescription::getExternalNames

  void
  BehaviourDescription::appendExternalNames(std::vector<std::string>& n,
					    const Hypothesis h,
					    const VarContainer& v) const
  {
    return this->getBehaviourData(h).appendExternalNames(n,v);
  } // end of BehaviourDescription::appendExternalNames

  void
  BehaviourDescription::setGlossaryName(const Hypothesis h,
					const std::string& n,
					const std::string& g)
  {
    this->callBehaviourData(h,&BehaviourData::setGlossaryName,n,g);
  } // end of BehaviourDescription::setGlossaryName

  bool
  BehaviourDescription::isGlossaryNameUsed(const Hypothesis h,
					   const std::string& n) const
  {
    return this->getBehaviourData(h).isGlossaryNameUsed(n);
  } // end of BehaviourDescription::isGlossaryName

  void
  BehaviourDescription::setEntryName(const Hypothesis h,
				     const std::string& n,
				     const std::string& g)
  {
    this->callBehaviourData(h,&BehaviourData::setEntryName,n,g);
  } // end of BehaviourDescription::setEntryName

  bool
  BehaviourDescription::isUsedAsEntryName(const Hypothesis h,
					const std::string& n) const
  {
    return this->getBehaviourData(h).isUsedAsEntryName(n);
  } // end of BehaviourDescription::isEntryName

  std::string
  BehaviourDescription::getExternalName(const Hypothesis h,
					const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getExternalName,n);
  } // end of BehaviourDescription::getGlossaryName

  std::string
  BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName(const Hypothesis h,
								   const std::string& n) const
  {
    return this->getBehaviourData(h).getVariableNameFromGlossaryNameOrEntryName(n);
  } // end of BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName

  void
  BehaviourDescription::setAttribute(const std::string& n,
				     const BehaviourAttribute& a,
				     const bool b)
  {
    using namespace std;
    if(b){
      auto p=this->attributes.find(n);
      if(p!=this->attributes.end()){
	if(a.getTypeIndex()!=p->second.getTypeIndex()){
	  string msg("BehaviourDescription::setAttribute : ",
		     "attribute already exists with a different type");
	  throw(runtime_error(msg));
	}
	return;
      }
    }
    if(!this->attributes.insert({n,a}).second){
      string msg("BehaviourDescription::setAttribute : "
		 "attribute '"+n+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of BehaviourDescription::setAttribute

  bool
  BehaviourDescription::hasAttribute(const std::string& n) const
  {
    return this->attributes.count(n)!=0u;
  } // end of BehaviourDescription::hasAttribute
  
  const std::map<std::string,BehaviourAttribute>&
  BehaviourDescription::getAttributes() const
  {
    return this->attributes;
  } // end of BehaviourDescription::getAttributes

  void
  BehaviourDescription::reserveName(const Hypothesis h,
				    const std::string& n)
  {
    this->callBehaviourData(h,&BehaviourData::reserveName,n);
  }
  
  void
  BehaviourDescription::registerMemberName(const Hypothesis h,
					   const std::string& n)
  {
    this->callBehaviourData(h,&BehaviourData::registerMemberName,n);
  } // end of BehaviourDescription::registerMemberName

  void
  BehaviourDescription::registerStaticMemberName(const Hypothesis h,
						 const std::string& n)
  {
    this->callBehaviourData(h,&BehaviourData::registerStaticMemberName,n);
  } // end of BehaviourDescription::registerMemberName
  
  void
  BehaviourDescription::addMaterialLaw(const std::string& m)
  {
    if(find(this->materialLaws.begin(),
	    this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of BehaviourDescription::getMaterialLaws

  const std::vector<std::string>&
  BehaviourDescription::getMaterialLaws(void) const
  {
    return this->materialLaws;
  }

  std::pair<bool,bool>
  BehaviourDescription::checkVariableExistence(const std::string& v) const
  {
    const auto& h = this->getDistinctModellingHypotheses();
    std::pair<bool,bool> r{true,false};
    for(const auto & elem : h){
      const auto& bdata = this->getBehaviourData(elem);
      const auto& vn = bdata.getVariablesNames();
      const bool b = vn.find(v)!=vn.end();
      r.first  = r.first  && b;
      r.second = r.second || b;
    }
    if(!r.second){
      r.first = false;
    }
    return r;
  } // end of checkVariableExistence

  std::pair<bool,bool>
  BehaviourDescription::checkVariableExistence(const std::string& n,
					       const std::string& c,
					       const bool b) const
  {
    using namespace std;
    const auto& h = this->getDistinctModellingHypotheses();
    pair<bool,bool> r = {true,false};
    for(const auto & elem : h){
      const auto& bdata = this->getBehaviourData(elem);
      const bool f = bdata.getVariables(c).contains(n);
      if(!f&&b){
	string msg("BehaviourDescription::checkVariableExistence : "
		   "no variable named '"+n+"' found for at least one modelling hypothesis");
	throw(runtime_error(msg));
      }
      r.first  = r.first  && f;
      r.second = r.second || f;
    }
    if(!r.second){
      r.first = false;
    }
    return r;
  }

  void
  BehaviourDescription::checkVariableGlossaryName(const std::string& n,
						  const std::string& g) const
  {
    using namespace std;
    const auto& h = this->getDistinctModellingHypotheses();
    for(const auto & elem : h){
      const auto& bdata = this->getBehaviourData(elem);
      if(!bdata.hasGlossaryName(n)){
	string msg("BehaviourDescription::VariableGlossaryName : "
		   "no glossary name associated with variable '"+n+"'");
	throw(runtime_error(msg));
      }
      const auto& en = bdata.getExternalName(n);
      if(en!=g){
	string msg("BehaviourDescription::VariableGlossaryName : "
		   "the glossary name associated with "
		   "variable '"+n+"' is not '"+g+"', but '"+en+"'");
	throw(runtime_error(msg));
      }
    }
  } // end of BehaviourDescription::checkVariableGlossaryName

  void
  BehaviourDescription::checkVariablePosition(const std::string& n,
					      const std::string& c,
					      const size_t p)
  {
    using namespace std;
    const auto& h = this->getDistinctModellingHypotheses();
    for(const auto & elem : h){
      const auto& bdata = this->getBehaviourData(elem);
      const auto& vc = bdata.getVariables(c);
      if(p>=vc.size()){
	string msg("BehaviourDescription::checkVariablePosition : "
		   "position given is greater than the number "
		   "of variables of category '"+c+"'");
	throw(runtime_error(msg));
      }
      const auto& v = vc[p];
      if(v.name!=n){
	string msg("BehaviourDescription::checkVariablePosition : "
		   "variable at the given position is not named '"+n+
		   "' but '"+v.name+"'");
	throw(runtime_error(msg));
      }
    }
  } // end of BehaviourDescription::checkVariablePosition

  void
  BehaviourDescription::setOrthotropicAxesConvention(const tfel::material::OrthotropicAxesConvention c)
  {
    if(this->oacIsDefined){
      throw(std::runtime_error("BehaviourDescription::setOrthotropicAxesConvention : "
			       "orthotropic axes convention already defined"));
    }
    if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
      throw(std::runtime_error("BehaviourDescription::setOrthotropicAxesConvention : "
			       "the mechanical behaviour is not orthotropic."));
    }
    this->oacIsDefined = true;
    this->oac = c;
  }

  tfel::material::OrthotropicAxesConvention
  BehaviourDescription::getOrthotropicAxesConvention(void) const
  {
    if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
      throw(std::runtime_error("BehaviourDescription::getOrthotropicAxesConvention : "
			       "the mechanical behaviour is not orthotropic."));
    }
    if(!this->oacIsDefined){
      this->oacIsDefined = true;
    }
    return this->oac;
  }
  
  BehaviourDescription::~BehaviourDescription()
  {}

} // end of namespace mfront
