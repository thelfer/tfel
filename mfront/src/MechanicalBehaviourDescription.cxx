/*! 
 * \file  mfront/src/MechanicalBehaviourDescription.cxx
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
#include"MFront/MechanicalBehaviourDescription.hxx"

namespace mfront
{

  template<typename Arg1>
  void
  MechanicalBehaviourDescription::callMechanicalBehaviourData(const Hypothesis h,
  							      void (MechanicalBehaviourData:: *m)(const Arg1&),
  							      const Arg1& a)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->md.*m)(a);
      typename map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
  	MechanicalBehaviourData& d = *(p->second);
  	(d.*m)(a);
      }
    } else {
      (this->getMechanicalBehaviourData2(h).*m)(a);
    }
  } // end of MechanicalBehaviourDescription::callMechanicalBehaviourData

  template<typename Arg1>
  void
  MechanicalBehaviourDescription::callMechanicalBehaviourData(const Hypothesis h,
							      void (MechanicalBehaviourData:: *m)(const Arg1),
							      const Arg1 a)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->md.*m)(a);
      typename map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	MechanicalBehaviourData& d = *(p->second);
	(d.*m)(a);
      }
    } else {
      (this->getMechanicalBehaviourData2(h).*m)(a);
    }
  } // end of MechanicalBehaviourDescription::callMechanicalBehaviourData

  template<typename Arg1,
	   typename Arg2>
  void
  MechanicalBehaviourDescription::callMechanicalBehaviourData(const Hypothesis h,
							      void (MechanicalBehaviourData:: *m)(const Arg1&,
												  const Arg2),
							      const Arg1& a,
							      const Arg2  b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->md.*m)(a,b);
      typename map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	MechanicalBehaviourData& d = *(p->second);
	(d.*m)(a,b);
      }
    } else {
      (this->getMechanicalBehaviourData2(h).*m)(a,b);
    }
  } // end of MechanicalBehaviourDescription::callMechanicalBehaviourData

  template<typename Arg1,
	   typename Arg2>
  void
  MechanicalBehaviourDescription::callMechanicalBehaviourData(const Hypothesis h,
							      void (MechanicalBehaviourData:: *m)(const Arg1&,
												  const Arg2&),
							      const Arg1& a,
							      const Arg2& b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->md.*m)(a,b);
      typename map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	MechanicalBehaviourData& d = *(p->second);
	(d.*m)(a,b);
      }
    } else {
      (this->getMechanicalBehaviourData2(h).*m)(a,b);
    }
  } // end of MechanicalBehaviourDescription::callMechanicalBehaviourData

  template<typename Res,
	   typename Arg1>
  Res
  MechanicalBehaviourDescription::getData(const Hypothesis h,
					  Res (MechanicalBehaviourData:: *m)(const Arg1&) const,
					  const Arg1& a) const
  {
    return (this->getMechanicalBehaviourData(h).*m)(a);
  } // end of MechanicalBehaviourDescription::getData

  static void
  MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument(const MaterialPropertyDescription& a)
  {
    using namespace std;
    if(!((a.inputs.size())||(a.inputs.size()!=1u))){
      string msg("MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument : "
		 "thermal expansion shall only depend on temperature or be constant");
      throw(runtime_error(msg));
    }
    if(a.inputs.size()==1u){
      const VariableDescription& v =  a.inputs.front();
      const string& vn = v.getExternalName(a.glossaryNames,a.entryNames);
      if(vn!="Temperature"){
	string msg("MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument : "
		   "thermal expansion shall only depend on temperature");
	throw(runtime_error(msg));
      }
    }
  } // end of MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument

  const std::string
  MechanicalBehaviourDescription::requiresStiffnessTensor("requiresStiffnessTensor");
  
  const std::string
  MechanicalBehaviourDescription::requiresUnAlteredStiffnessTensor("requiresUnAlteredStiffnessTensor");
  
  const std::string
  MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor("requiresThermalExpansionCoefficientTensor");
  
  void
  MechanicalBehaviourDescription::throwUndefinedAttribute(const std::string& n)
  {
    using namespace std;
    string msg("MechanicalBehaviourDescription::getAttribute : "
	       "no attribute named '"+n+"'");
    throw(runtime_error(msg));
  } // end of MechanicalBehaviourDescription::throwUndefinedAttribute

  MechanicalBehaviourDescription::MechanicalBehaviourDescription()
    : use_qt(false),
      type(MechanicalBehaviourDescription::GENERALBEHAVIOUR),
      // By default, a behaviour is isotropic 
      stype(mfront::ISOTROPIC),
      // By default, a behaviour is isotropic 
      estype(mfront::ISOTROPIC)
  {} // end of MechanicalBehaviourDescription::MechanicalBehaviourDescription()

  const MechanicalBehaviourData&
  MechanicalBehaviourDescription::getMechanicalBehaviourData(const Hypothesis& h) const
  {
    using namespace std;
    // check that the given hypothesis is supported
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->md;
    }
    this->checkModellingHypothesis(h);
    this->requestedHypotheses.insert(h);
    // check if a specialised version of the mechanical behaviour
    // description has been defined
    map<Hypothesis,MBDPtr>::const_iterator p =this->smd.find(h);
    if(p!=this->smd.end()){
      return *(p->second);
    }
    // return the default...
    return this->md;
  } // end of MechanicalBehaviourDescription::getMechanicalBehaviourData

  MechanicalBehaviourData&
  MechanicalBehaviourDescription::getMechanicalBehaviourData2(const ModellingHypothesis::Hypothesis& h)
  {
    using namespace std;
    typedef map<Hypothesis,MBDPtr>::value_type MVType;
    // check that the given hypothesis is supported
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->md;
    }
    if(this->areModellingHypothesesDefined()){
      this->checkModellingHypothesis(h);
    }
    this->requestedHypotheses.insert(h);
    map<Hypothesis,MBDPtr>::iterator p = this->smd.find(h);
    if(p==this->smd.end()){
      // copy of the default description
      p=this->smd.insert(MVType(h,MBDPtr(new MechanicalBehaviourData(this->md)))).first;
    }
    return *(p->second);
  } // end of MechanicalBehaviourDescription::getMechanicalBehaviourData2

  void
  MechanicalBehaviourDescription::setBehaviourName(const std::string& m)
  {
    using namespace std;
    if(!this->behaviour.empty()){
      string msg("MechanicalBehaviourDescription::setBehaviourName : ");
      msg += "behaviour name alreay defined";
      throw(runtime_error(msg));
    }
    this->behaviour = m;
    this->updateClassName();
  } // end of MechanicalBehaviourDescription::setBehaviourName

  const std::string&
  MechanicalBehaviourDescription::getBehaviourName(void) const
  {
    return this->behaviour;
  } // end of MechanicalBehaviourDescription::getBehaviourName

  void
  MechanicalBehaviourDescription::setLibrary(const std::string& l)
  {
    using namespace std;
    if(!this->library.empty()){
      string msg("MechanicalBehaviourDescription::setLibrary : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->library = l;
  } // end of MechanicalBehaviourDescription::setLibrary

  const std::string&
  MechanicalBehaviourDescription::getLibrary(void) const
  {
    return this->library;
  } // end of MechanicalBehaviourDescription::getLibrary

  void
  MechanicalBehaviourDescription::setMaterialName(const std::string& m)
  {
    using namespace std;
    if(!this->material.empty()){
      string msg("MechanicalBehaviourDescription::setMaterialName : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->material = m;
    this->updateClassName();
  } // end of MechanicalBehaviourDescription::setMaterialName

  const std::string&
  MechanicalBehaviourDescription::getMaterialName(void) const
  {
    return this->material;
  } // end of MechanicalBehaviourDescription::getMaterialName

  void
  MechanicalBehaviourDescription::setClassName(const std::string& n)
  {
    using namespace std;
    if(!this->className.empty()){
      string msg("MechanicalBehaviourDescription::setClassName : ");
      msg += "class name alreay defined";
      throw(runtime_error(msg));
    }
    this->className = n;
  } // end of MechanicalBehaviourDescription::setClassName

  const std::string&
  MechanicalBehaviourDescription::getClassName(void) const
  {
    using namespace std;
    if(this->className.empty()){
      string msg("MechanicalBehaviourDescription::getClassName : ");
      msg += "class name not defined";
      throw(runtime_error(msg));
    }
    return this->className;
  } // end of MechanicalBehaviourDescription::getClassName

  void
  MechanicalBehaviourDescription::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of MechanicalBehaviourDescription::appendToIncludes

  const std::string&
  MechanicalBehaviourDescription::getIncludes(void) const
  {
    return this->includes;
  } // end of MechanicalBehaviourDescription::getIncludes

  void
  MechanicalBehaviourDescription::appendToMembers(const std::string& c)
  {
    this->members+=c;
    if(!this->members.empty()){
      if(*(this->members.rbegin())!='\n'){
	this->members+='\n';
      }
    }
  } // end of MechanicalBehaviourDescription::appendToMembers

  const std::string&
  MechanicalBehaviourDescription::getMembers(void) const
  {
    return this->members;
  } // end of MechanicalBehaviourDescription::getMembers

  void
  MechanicalBehaviourDescription::appendToPrivateCode(const std::string& c)
  {
    this->privateCode+=c;
    if(!this->privateCode.empty()){
      if(*(this->privateCode.rbegin())!='\n'){
	this->privateCode+='\n';
      }
    }
  } // end of MechanicalBehaviourDescription::appendToPrivateCode

  const std::string&
  MechanicalBehaviourDescription::getPrivateCode(void) const
  {
    return this->privateCode;
  } // end of MechanicalBehaviourDescription::getPrivateCode

  void
  MechanicalBehaviourDescription::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of MechanicalBehaviourDescription::appendToSources

  const std::string&
  MechanicalBehaviourDescription::getSources(void) const
  {
    return this->sources;
  } // end of MechanicalBehaviourDescription::getSources

  MechanicalBehaviourDescription::BehaviourType
  MechanicalBehaviourDescription::getBehaviourType() const
  {
    return this->type;
  } // end of MechanicalBehaviourDescription::getBehaviourType

  std::string
  MechanicalBehaviourDescription::getBehaviourTypeFlag(void) const
  {
    using namespace std;
    std::string btype;
    if(this->getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      btype="MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR";
    } else if (this->getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      btype="MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR";
    } else if (this->getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
      btype="MechanicalBehaviourBase::COHESIVEZONEMODEL";
    } else {
      throw(runtime_error("MechanicalBehaviourDescription::getBehaviourTypeFlag : "
			  "unsupported behaviour type"));
    }
    return btype;
  } // end of MechanicalBehaviourDescription::getBehaviourTypeFlag

  MechanicalBehaviourSymmetryType
  MechanicalBehaviourDescription::getElasticSymmetryType() const
  {
    return this->estype;
  } // end of MechanicalBehaviourDescription::getElasticSymmetryType

  void
  MechanicalBehaviourDescription::setElasticSymmetryType(const MechanicalBehaviourSymmetryType t)
  {
    this->estype = t;
  } // end of MechanicalBehaviourDescription::setElasticSymmetryType

  MechanicalBehaviourSymmetryType
  MechanicalBehaviourDescription::getSymmetryType() const
  {
    return this->stype;
  } // end of MechanicalBehaviourDescription::getSymmetryType

  void
  MechanicalBehaviourDescription::setSymmetryType(const MechanicalBehaviourSymmetryType t)
  {
    using namespace std;
    if(this->stype!=mfront::ISOTROPIC){
      string msg("MechanicalBehaviourDescription::setSymmetryType : "
		 "symmetry type already declared");
    }
    if((this->thermalExpansionCoefficients.size()==1u)&&(t!=mfront::ISOTROPIC)){
      string msg("MechanicalBehaviourDescription::setSymmetryType : "
		 "an isotropic thermal expansion coefficient was previoulsy defined");
    }
    this->stype = t;
  } // end of MechanicalBehaviourDescription::setSymmetryType
  
  void
  MechanicalBehaviourDescription::declareAsASmallStrainStandardBehaviour(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(!this->mvariables.empty()){
      string msg("MechanicalBehaviourDescription::declareAsASmallStrainStandardBehaviour : ");
      msg += "some driving variables are already declared";
      throw(runtime_error(msg));
    }
    DrivingVariable eto;
    eto.name = "eto";
    eto.type = "StrainStensor";
    eto.increment_known = true;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert(MVType(eto,sig));
    this->type = MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR;
  }

  void
  MechanicalBehaviourDescription::declareAsAFiniteStrainStandardBehaviour(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(!this->mvariables.empty()){
      string msg("MechanicalBehaviourDescription::declareAsAFiniteStrainStandardBehaviour : ");
      msg += "some driving variables are already declared";
      throw(runtime_error(msg));
    }
    DrivingVariable F;
    F.name = "F";
    F.type = "DeformationGradientTensor";
    F.increment_known = false;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert(MVType(F,sig));
    this->type = MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR;
  }
    
  void
  MechanicalBehaviourDescription::declareAsACohesiveZoneModel(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(!this->mvariables.empty()){
      string msg("MechanicalBehaviourDescription::declareAsACohesiveZoneModel : ");
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
    this->mvariables.insert(MVType(u,t));
    this->type = MechanicalBehaviourDescription::COHESIVEZONEMODEL;
  }

  void
  MechanicalBehaviourDescription::addMainVariable(const DrivingVariable&    v,
						  const ThermodynamicForce& f)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(this->type!=MechanicalBehaviourDescription::GENERALBEHAVIOUR){
      string msg("MechanicalBehaviourDescription::addMainVariables : "
		 "one can not add a main variable if the behaviour "
		 "don't have a general behaviour type");
      throw(runtime_error(msg));
    }
    if(!this->mvariables.insert(MVType(v,f)).second){
      string msg("MechanicalBehaviourDescription::addMainVariables : "
		 "a driving variable '"+v.name+"' has already been declared");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourDescription::addMainVariables

  const std::map<DrivingVariable,
		 ThermodynamicForce>&
  MechanicalBehaviourDescription::getMainVariables(void) const
  {
    return this->mvariables;
  } // end of MechanicalBehaviourDescription::getMainVariables
  
  bool
  MechanicalBehaviourDescription::isDrivingVariableName(const std::string& n) const
  {
    using namespace std;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator pm;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.name==n){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::isDrivingVariableName

  bool
  MechanicalBehaviourDescription::isDrivingVariableIncrementName(const std::string& n) const
  {
    using namespace std;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator pm;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.increment_known){
	if("d"+dv.name==n){
	  return true;
	}
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::isDrivingVariableIncrementName

  std::pair<SupportedTypes::TypeSize,
	    SupportedTypes::TypeSize>
  MechanicalBehaviourDescription::getMainVariablesSize(void) const
  {
    using namespace std;
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    SupportedTypes::TypeSize of;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      ov += this->getTypeSize(pm->first.type,1u);
      of += this->getTypeSize(pm->second.type,1u);
    }
    return make_pair(ov,of);
  } // end of MechanicalBehaviourDescription::getMainVariablesSize

  void
  MechanicalBehaviourDescription::setThermalExpansionCoefficient(const tfel::utilities::shared_ptr<MaterialPropertyDescription>& a)
  {
    using namespace std;
    if(this->areThermalExpansionCoefficientsDefined()){
      string msg("MechanicalBehaviourDescription::setThermalExpansionCoefficient : "
		 "thermal expansion coefficient already defined");
      throw(runtime_error(msg));
    }
    if(this->stype!=mfront::ISOTROPIC){
      string msg("MechanicalBehaviourDescription::setThermalExpansionCoefficient : "
		 "the mechanical behaviour is not isotropic.");
      throw(runtime_error(msg));
    }
    MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a);
    this->thermalExpansionCoefficients.push_back(a);
  } // end of MechanicalBehaviourDescription::setThermalExpansionCoefficient

  void
  MechanicalBehaviourDescription::setThermalExpansionCoefficients(const tfel::utilities::shared_ptr<MaterialPropertyDescription>& a1,
								  const tfel::utilities::shared_ptr<MaterialPropertyDescription>& a2,
								  const tfel::utilities::shared_ptr<MaterialPropertyDescription>& a3)
  {
    using namespace std;
    if(this->areThermalExpansionCoefficientsDefined()){
      string msg("MechanicalBehaviourDescription::setThermalExpansionCoefficient : "
		 "thermal expansion coefficient already defined");
      throw(runtime_error(msg));
    }
    if(this->stype!=mfront::ORTHOTROPIC){
      string msg("MechanicalBehaviourDescription::setThermalExpansionCoefficient : "
		 "the mechanical behaviour is not orthotropic.");
      throw(runtime_error(msg));
    }
    MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a1);
    MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a2);
    MechanicalBehaviourDescriptionCheckThermalExpansionCoefficientArgument(*a3);
    this->thermalExpansionCoefficients.push_back(a1);
    this->thermalExpansionCoefficients.push_back(a2);
    this->thermalExpansionCoefficients.push_back(a3);
  } // end of MechanicalBehaviourDescription::setThermalExpansionCoefficients

  bool
  MechanicalBehaviourDescription::areThermalExpansionCoefficientsDefined(void) const
  {
    return !this->thermalExpansionCoefficients.empty();
  } // end of MechanicalBehaviourDescription::areThermalExpansionCoefficientsDefined

  const std::vector<tfel::utilities::shared_ptr<MaterialPropertyDescription> >&
  MechanicalBehaviourDescription::getThermalExpansionCoefficients(void) const
  {
    using namespace std;
    if(!this->areThermalExpansionCoefficientsDefined()){
      string msg("MechanicalBehaviourDescription::getThermalExpansionCoefficients : "
		 "no thermal expansion coefficients defined");
      throw(runtime_error(msg));
    }
    return this->thermalExpansionCoefficients;
  }

  void
  MechanicalBehaviourDescription::setUseQt(const bool b)
  {
    this->use_qt = b;
  } // end of MechanicalBehaviourDescription::setUseQt

  bool
  MechanicalBehaviourDescription::useQt(void) const
  {
    return this->use_qt;
  } // end of MechanicalBehaviourDescription::useQt

  std::string
  MechanicalBehaviourDescription::getTangentOperatorType(void) const
  {
    using namespace std;
    if(this->type==GENERALBEHAVIOUR){
      pair<SupportedTypes::TypeSize,
	SupportedTypes::TypeSize> msizes = this->getMainVariablesSize();
      ostringstream t;
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
    string msg("MechanicalBehaviourDescription::getStiffnessOperatorType : "
	       "internal error (unsupported behaviour type)");
    throw(runtime_error(msg));
    return "";
  } // end of MechanicalBehaviourDescription::getStiffnessOperatorType

  std::string
  MechanicalBehaviourDescription::getStressFreeExpansionType(void) const
  {
    using namespace std;
    if((this->type==SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->type==FINITESTRAINSTANDARDBEHAVIOUR)){
      return "StrainStensor";
    }
    string msg("MechanicalBehaviourDescription::getStressFreeExpansionType : "
	       "internal error (unsupported behaviour type)");
    throw(runtime_error(msg));
    return "";
  } // end of MechanicalBehaviourDescription::getStressFreeExpansionType

  void
  MechanicalBehaviourDescription::checkModellingHypothesis(const Hypothesis& h) const
  {
    using namespace std;
    if(this->getModellingHypotheses().find(h)==this->getModellingHypotheses().end()){
      ostringstream msg;
      msg << "MechanicalBehaviourDescription::checkModellingHypothesis : "
	  << "modelling hypothesis '" << ModellingHypothesis::toString(h) 
	  << "' is not supported. Refer to the documentation of "
	  << "the '@ModellingHypothesis' or "
	  << "the '@ModellingHypotheses' keywords for details.\n";
      msg << "Supported modelling hypotheses are :";
      for(set<Hypothesis>::const_iterator ph=this->hypotheses.begin();
	  ph!=this->hypotheses.end();++ph){
	msg << "\n- '" << ModellingHypothesis::toString(*ph) << "'";
      }
      throw(runtime_error(msg.str()));
    }
  } // end of MechanicalBehaviourDescription::checkModellingHypothesis

  bool
  MechanicalBehaviourDescription::areModellingHypothesesDefined(void) const
  {
    return !this->hypotheses.empty();
  } // end of MechanicalBehaviourDescription::areModellingHypothesesDefined

  const std::set<MechanicalBehaviourDescription::Hypothesis>&
  MechanicalBehaviourDescription::getModellingHypotheses(void) const
  {
    using namespace std;
    if(this->hypotheses.empty()){
      string msg("MechanicalBehaviourDescription::getModellingHypotheses : "
		 "hypothesis undefined yet");
      throw(runtime_error(msg));
    }
    return this->hypotheses;
  } // end of MechanicalBehaviourDescription::getModellingHypotheses

  std::set<MechanicalBehaviourDescription::Hypothesis>
  MechanicalBehaviourDescription::getDistinctModellingHypotheses(void) const
  {
    using namespace std;
    set<Hypothesis> h;
    const set<Hypothesis>& mh = this->getModellingHypotheses();
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
    for(set<Hypothesis>::const_iterator p=mh.begin();p!=mh.end();++p){
      if(this->hasSpecialisedMechanicalData(*p)){
	h.insert(*p);
      }
    }
    return h;
  } // end of MechanicalBehaviourDescription::getDistinctModellingHypotheses


  bool
  MechanicalBehaviourDescription::isModellingHypothesisSupported(const Hypothesis h) const
  {
    return this->getModellingHypotheses().count(h)!=0u;
  }

  void
  MechanicalBehaviourDescription::setModellingHypotheses(const std::set<Hypothesis>& h)
  {
    using namespace std;
    // never ever trust a user
    if(h.empty()){
      string msg("MechanicalBehaviourDescription::setHypotheses : "
		 "empty set of modelling hypotheses specificied");
      throw(runtime_error(msg));
    }
    // check that the user did not already set the modelling hypotheses
    if(!this->hypotheses.empty()){
      string msg("MechanicalBehaviourDescription::setHypotheses : "
		 "supported modelling hypotheses have already been declared");
      throw(runtime_error(msg));
    }
    // check that if a specialised version of the mechanical behaviour
    // is defined, it is present in the set of hypotheses defined here
    map<Hypothesis,MBDPtr>::const_iterator p;
    for(p=this->smd.begin();p!=this->smd.end();++p){
      if(h.find(p->first)==h.end()){
	string msg("MechanicalBehaviourDescription::setHypotheses : "
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
	string msg("MechanicalBehaviourDescription::setHypotheses : "
		   "a description of the behaviour for "
		   "the hypothesis '"+ModellingHypothesis::toString(*p2)+"' "
		   "has been requested earlier, but this hypothesis is not "
		   "in the set of hypotheses which will to be "
		   "supported by the behaviour. This may lead to inconsistencies. "
		   "Cowardly aborting.");
	throw(runtime_error(msg));
      }
    }
    this->hypotheses.insert(h.begin(),h.end());
  } // end of MechanicalBehaviourDescription::setModellingHypotheses

  void
  MechanicalBehaviourDescription::addMaterialProperties(const Hypothesis h,
							const VariableDescriptionContainer& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addMaterialProperty;
    this->addVariables(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addMaterialProperty(const Hypothesis h,
						      const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addMaterialProperty;
    this->addVariable(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addIntegrationVariables(const Hypothesis h,
						    const VariableDescriptionContainer& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addIntegrationVariable;
    this->addVariables(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addStateVariables(const Hypothesis h,
						    const VariableDescriptionContainer& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addStateVariable;
    this->addVariables(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addIntegrationVariable(const Hypothesis h,
						      const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addIntegrationVariable;
    this->addVariable(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addStateVariable(const Hypothesis h,
						   const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addStateVariable;
    this->addVariable(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addAuxiliaryStateVariables(const Hypothesis h,
						    const VariableDescriptionContainer& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addAuxiliaryStateVariable;
    this->addVariables(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addAuxiliaryStateVariable(const Hypothesis h,
							    const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addAuxiliaryStateVariable;
    this->addVariable(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addExternalStateVariables(const Hypothesis h,
							    const VariableDescriptionContainer& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addExternalStateVariable;
    this->addVariables(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addExternalStateVariable(const Hypothesis h,
							   const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addExternalStateVariable;
    this->addVariable(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addLocalVariables(const Hypothesis h,
						    const VariableDescriptionContainer& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addLocalVariable;
    this->addVariables(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addLocalVariable(const Hypothesis h,
						   const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addLocalVariable;
    this->addVariable(h,v,f);
  }

  void
  MechanicalBehaviourDescription::addParameter(const Hypothesis h,
						      const VariableDescription& v)
  {
    typedef void (MechanicalBehaviourData::* mptr)(const VariableDescription&);
    mptr f = &MechanicalBehaviourData::addParameter;
    this->addVariable(h,v,f);
  }

  bool
  MechanicalBehaviourDescription::hasParameter(const Hypothesis h,
					       const std::string& v) const
  {
    return this->getData(h,&MechanicalBehaviourData::hasParameter,v);
  } // end of MechanicalBehaviourDescription::hasParameter

  bool
  MechanicalBehaviourDescription::hasParameters(const Hypothesis h) const
  {
    return this->getMechanicalBehaviourData(h).hasParameters();
  } // end of MechanicalBehaviourDescription::hasParameters

  bool
  MechanicalBehaviourDescription::hasParameters(void) const
  {
    using namespace std;
    if(this->md.hasParameters()){
      return true;
    }
    map<Hypothesis,MBDPtr>::const_iterator p;
    for(p=this->smd.begin();p!=this->smd.end();++p){
      if(p->second->hasParameters()){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::hasParameters
    
  void
  MechanicalBehaviourDescription::setParameterDefaultValue(const Hypothesis h,
							   const std::string& n,
							   const double v)
  {
    void (MechanicalBehaviourData:: *mptr)(const std::string&,
					   const double);
    mptr = &MechanicalBehaviourData::setParameterDefaultValue;
    this->callMechanicalBehaviourData(h,mptr,n,v);
  }

  void
  MechanicalBehaviourDescription::setParameterDefaultValue(const Hypothesis h,
							   const std::string& n,
							   const int v)
  {
    void (MechanicalBehaviourData:: *mptr)(const std::string&,
					   const int v);
    mptr = &MechanicalBehaviourData::setParameterDefaultValue;
    this->callMechanicalBehaviourData(h,mptr,n,v);
  }

  void
  MechanicalBehaviourDescription::setParameterDefaultValue(const Hypothesis h,
							   const std::string& n,
							   const unsigned short v)
  {
    void (MechanicalBehaviourData:: *mptr)(const std::string&,
					   const unsigned short v);
    mptr = &MechanicalBehaviourData::setParameterDefaultValue;
    this->callMechanicalBehaviourData(h,mptr,n,v);
  }

  unsigned short
  MechanicalBehaviourDescription::getUnsignedShortParameterDefaultValue(const Hypothesis h,
									const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::getUnsignedShortParameterDefaultValue,n);
  } // end of MechanicalBehaviourDescription::getUnsignedShortParameterDefaultValue

  int
  MechanicalBehaviourDescription::getIntegerParameterDefaultValue(const Hypothesis h,
									const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::getIntegerParameterDefaultValue,n);
  } // end of MechanicalBehaviourDescription::getIntegerParameterDefaultValue

  double
  MechanicalBehaviourDescription::getFloattingPointParameterDefaultValue(const Hypothesis h,
									const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::getFloattingPointParameterDefaultValue,n);
  } // end of MechanicalBehaviourDescription::getFloattingPointParameterDefaultValue

  void
  MechanicalBehaviourDescription::addStaticVariable(const Hypothesis h,
						    const StaticVariableDescription& v)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->md.addStaticVariable(v);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	p->second->addStaticVariable(v);
      }
    } else {
      this->getMechanicalBehaviourData2(h).addStaticVariable(v);
    }
  } // end of MechanicalBehaviourDescription::addStaticVariable

  void
  MechanicalBehaviourDescription::addVariables(const Hypothesis h,
					       const VariableDescriptionContainer& v,
					       void (MechanicalBehaviourData::* m)(const VariableDescription&))
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->addVariables(this->md,v,m);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	this->addVariables(*(p->second),v,m);
      }
    } else {
      this->addVariables(this->getMechanicalBehaviourData2(h),v,m);
    }
  }

  void
  MechanicalBehaviourDescription::addVariable(const Hypothesis h,
					      const VariableDescription& v,
					      void (MechanicalBehaviourData::* m)(const VariableDescription&))
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->addVariable(this->md,v,m);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	this->addVariable(*(p->second),v,m);
      }
    } else {
      this->addVariable(this->getMechanicalBehaviourData2(h),v,m);
    }
  }

  void
  MechanicalBehaviourDescription::addVariables(MechanicalBehaviourData& b,
					       const VariableDescriptionContainer& v,
					       void (MechanicalBehaviourData::* m)(const VariableDescription&))
  {
    for(VariableDescriptionContainer::const_iterator p=v.begin();p!=v.end();++p){
      this->addVariable(b,*p,m);
    }
  }

  void
  MechanicalBehaviourDescription::addVariable(MechanicalBehaviourData& b,
					      const VariableDescription& v,
					      void (MechanicalBehaviourData::* m)(const VariableDescription&))
  {
    (b.*m)(v);
  }

  bool MechanicalBehaviourDescription::areAllMechanicalDataSpecialised(void) const
  {
    return this->getModellingHypotheses().size()==this->smd.size();
  } // end of MechanicalBehaviourDescription::areAllMechanicalDataSpecialised

  bool MechanicalBehaviourDescription::areAllMechanicalDataSpecialised(const std::set<Hypothesis>& h) const
  {
    using namespace std;
    for(set<Hypothesis>::const_iterator p=h.begin();p!=h.end();++p){
      if(!this->hasSpecialisedMechanicalData(*p)){
	return false;
      }
    }
    return true;
  } // end of MechanicalBehaviourDescription::areAllMechanicalDataSpecialised

  bool
  MechanicalBehaviourDescription::hasSpecialisedMechanicalData(const Hypothesis h) const
  {
    using namespace std;
    using namespace std;
    if(this->getModellingHypotheses().find(h)==this->getModellingHypotheses().end()){
      string msg("MechanicalBehaviourDescription::areAllMechanicalDataSpecialised : "
		 "hypothesis '"+ModellingHypothesis::toString(h)+"' is not supported");
      throw(runtime_error(msg));
    }
    return this->smd.find(h)!=this->smd.end();
  }

  void
  MechanicalBehaviourDescription::requiresTVectorOrVectorIncludes(bool& b1,
								  bool& b2)
  {
    using namespace std;
    b1 = b2 = false;
    if(this->hypotheses.empty()){
      string msg("MechanicalBehaviourDescription::areAllMechanicalDataSpecialised : "
		 "no hypothesis defined");
      throw(runtime_error(msg));
    }
    if(!this->areAllMechanicalDataSpecialised()){
      this->md.requiresTVectorOrVectorIncludes(b1,b2);
    }
    map<Hypothesis,MBDPtr>::iterator p;
    for(p=this->smd.begin();(p!=this->smd.end())&&(!(b1&&b2));++p){
      p->second->requiresTVectorOrVectorIncludes(b1,b2);
    }
  } // end of MechanicalBehaviourDescription::requiresTVectorOrVectorIncludes

  void
  MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis h,
													 const std::string& n)
  {
    void (MechanicalBehaviourData::* m)(const std::string&) =
      &MechanicalBehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution;
    this->callMechanicalBehaviourData(h,m,n);
  } // end of MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  void
  MechanicalBehaviourDescription::setUsableInPurelyImplicitResolution(const Hypothesis h,
								      const bool b)
  {
    void (MechanicalBehaviourData::* m)(const bool) =
      &MechanicalBehaviourData::setUsableInPurelyImplicitResolution;
    this->callMechanicalBehaviourData(h,m,b);
  } // end of MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  bool
  MechanicalBehaviourDescription::isVariableUsedInCodeBlocks(const Hypothesis h,
							     const std::string& v) const
  {
    return this->getData(h,&MechanicalBehaviourData::isVariableUsedInCodeBlocks,v);
  } // end of MechanicalBehaviourDescription::isMaterialPropertyName

  bool
  MechanicalBehaviourDescription::isMaterialPropertyName(const Hypothesis h,
							 const std::string& v) const
  {
    return this->getData(h,&MechanicalBehaviourData::isMaterialPropertyName,v);
  } // end of MechanicalBehaviourDescription::isMaterialPropertyName

  bool
  MechanicalBehaviourDescription::isLocalVariableName(const Hypothesis h,
						      const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isLocalVariableName,n);
  }  // end of MechanicalBehaviourDescription::isLocalVariableName

  bool
  MechanicalBehaviourDescription::isPersistentVariableName(const Hypothesis h,
							      const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isPersistentVariableName,n);
  }  // end of MechanicalBehaviourDescription::isPersistentVariableName

  bool
  MechanicalBehaviourDescription::isIntegrationVariableName(const Hypothesis h,
							      const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isIntegrationVariableName,n);
  }  // end of MechanicalBehaviourDescription::isIntegrationVariableName

  bool
  MechanicalBehaviourDescription::isIntegrationVariableIncrementName(const Hypothesis h,
								     const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isIntegrationVariableIncrementName,n);
  }  // end of MechanicalBehaviourDescription::isIntegrationVariableIncrementName

  bool
  MechanicalBehaviourDescription::isStateVariableName(const Hypothesis h,
							      const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isStateVariableName,n);
  }  // end of MechanicalBehaviourDescription::isStateVariableName

  bool
  MechanicalBehaviourDescription::isStateVariableIncrementName(const Hypothesis h,
								       const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isStateVariableIncrementName,n);
  }  // end of MechanicalBehaviourDescription::isStateVariableIncrementName

  bool
  MechanicalBehaviourDescription::isAuxiliaryStateVariableName(const Hypothesis h,
								       const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isAuxiliaryStateVariableName,n);
  }  // end of MechanicalBehaviourDescription::isAuxiliaryStateVariableName

  bool
  MechanicalBehaviourDescription::isExternalStateVariableName(const Hypothesis h,
							      const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isExternalStateVariableName,n);
  }  // end of MechanicalBehaviourDescription::isExternalStateVariableName

  bool
  MechanicalBehaviourDescription::isExternalStateVariableIncrementName(const Hypothesis h,
								       const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isExternalStateVariableIncrementName,n);
  }  // end of MechanicalBehaviourDescription::isExternalStateVariableIncrementName

  bool
  MechanicalBehaviourDescription::isParameterName(const Hypothesis h,
						  const std::string& v) const
  {
    return this->getData(h,&MechanicalBehaviourData::isParameterName,v);
  } // end of MechanicalBehaviourDescription::isParameterName

  bool
  MechanicalBehaviourDescription::isStaticVariableName(const Hypothesis h,
						       const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::isStaticVariableName,n);
  }  // end of MechanicalBehaviourDescription::isStaticVariableName

  void
  MechanicalBehaviourDescription::updateClassName(void)
  {
    if((!this->getBehaviourName().empty())||
       (!this->getMaterialName().empty())){
      this->className = this->getMaterialName()+this->getBehaviourName();
    }
  } // end of MechanicalBehaviourDescription::updateClassName
  
  void
  MechanicalBehaviourDescription::setCode(const Hypothesis h,
					  const std::string& n,
					  const CodeBlock& c,
					  const Mode m,
					  const Position p)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MechanicalBehaviourDescription::setCode : setting '"
	    << n << "' on default hypothesis"  << endl;
      }
      this->md.setCode(n,c,m,p);
      map<Hypothesis,MBDPtr>::iterator pd;
      for(pd=this->smd.begin();pd!=this->smd.end();++pd){
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  ostream& log = getLogStream();
	  log << "MechanicalBehaviourDescription::setCode : setting '"
	      << n << "' on hypothesis '" 
	      << ModellingHypothesis::toString(pd->first) << "'" << endl;
	}
	pd->second->setCode(n,c,m,p);
      }
    } else {
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MechanicalBehaviourDescription::setCode : setting '"
	    << n << "' on hypothesis '" 
	    << ModellingHypothesis::toString(h) << "'" << endl;
      }
      this->getMechanicalBehaviourData2(h).setCode(n,c,m,p);
    }
  } // end of MechanicalBehaviourDescription::setCode

  const CodeBlock&
  MechanicalBehaviourDescription::getCodeBlock(const Hypothesis h,
					       const std::string& n) const
  {
    return this->getMechanicalBehaviourData(h).getCodeBlock(n);
  } // end of MechanicalBehaviourDescription::getCode

  std::string
  MechanicalBehaviourDescription::getCode(const Hypothesis h,
					  const std::string& n) const
  {
    const bool b = this->getAttribute(MechanicalBehaviourData::profiling,false);
    return this->getMechanicalBehaviourData(h).getCode(n,this->getClassName(),b);
  } // end of MechanicalBehaviourDescription::getCode

  bool
  MechanicalBehaviourDescription::hasCode(const Hypothesis h,
					  const std::string& n) const
  {
    return this->getMechanicalBehaviourData(h).hasCode(n);
  } // end of MechanicalBehaviourDescription::getCode

  void
  MechanicalBehaviourDescription::setBounds(const Hypothesis h,
					    const BoundsDescription& b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->md.setBounds(b);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
	p->second->setBounds(b);
      }
    } else {
      this->getMechanicalBehaviourData2(h).setBounds(b);
    }
  } // end of MechanicalBehaviourDescription::setBounds

  void
  MechanicalBehaviourDescription::setAttribute(const Hypothesis h,
					       const std::string& n,
					       const MechanicalBehaviourAttribute& a,
					       const bool b)
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->md.setAttribute(n,a,b);
      map<Hypothesis,MBDPtr>::iterator p;
      for(p=this->smd.begin();p!=this->smd.end();++p){
  	MechanicalBehaviourData& d = *(p->second);
  	d.setAttribute(n,a,b);
      }
    } else {
      this->getMechanicalBehaviourData2(h).setAttribute(n,a,b);
    }
  } // end of MechanicalBehaviourDescription::setAttribute

  bool
  MechanicalBehaviourDescription::hasAttribute(const Hypothesis h,
					       const std::string& n) const
  {
    return this->getMechanicalBehaviourData(h).hasAttribute(n);
  } // end of MechanicalBehaviourDescription::hasAttribute

  std::vector<std::string>
  MechanicalBehaviourDescription::getCodeBlockNames(const Hypothesis h) const
  {
    return this->getMechanicalBehaviourData(h).getCodeBlockNames();
  }

  std::vector<std::string>
  MechanicalBehaviourDescription::getExternalNames(const Hypothesis h,
						   const VarContainer& v) const
  {
    return this->getMechanicalBehaviourData(h).getExternalNames(v);
  }

  void
  MechanicalBehaviourDescription::getExternalNames(std::vector<std::string>& n,
						   const Hypothesis h,
						   const VarContainer& v) const
  {
    return this->getMechanicalBehaviourData(h).getExternalNames(n,v);
  } // end of MechanicalBehaviourDescription::getExternalNames

  void
  MechanicalBehaviourDescription::appendExternalNames(std::vector<std::string>& n,
						      const Hypothesis h,
						      const VarContainer& v) const
  {
    return this->getMechanicalBehaviourData(h).appendExternalNames(n,v);
  } // end of MechanicalBehaviourDescription::appendExternalNames

  void
  MechanicalBehaviourDescription::setGlossaryName(const Hypothesis h,
						  const std::string& n,
						  const std::string& g)
  {
    this->callMechanicalBehaviourData(h,&MechanicalBehaviourData::setGlossaryName,n,g);
  } // end of MechanicalBehaviourDescription::setGlossaryName

  bool
  MechanicalBehaviourDescription::isGlossaryName(const Hypothesis h,
						 const std::string& n) const
  {
    return this->getMechanicalBehaviourData(h).isGlossaryName(n);
  } // end of MechanicalBehaviourDescription::isGlossaryName

  void
  MechanicalBehaviourDescription::setEntryName(const Hypothesis h,
					       const std::string& n,
					       const std::string& g)
  {
    this->callMechanicalBehaviourData(h,&MechanicalBehaviourData::setEntryName,n,g);
  } // end of MechanicalBehaviourDescription::setEntryName

  bool
  MechanicalBehaviourDescription::isEntryName(const Hypothesis h,
					      const std::string& n) const
  {
    return this->getMechanicalBehaviourData(h).isEntryName(n);
  } // end of MechanicalBehaviourDescription::isEntryName

  std::string
  MechanicalBehaviourDescription::getExternalName(const Hypothesis h,
						  const std::string& n) const
  {
    return this->getData(h,&MechanicalBehaviourData::getExternalName,n);
  } // end of MechanicalBehaviourDescription::getGlossaryName

  std::string
  MechanicalBehaviourDescription::getVariableNameFromGlossaryNameOrEntryName(const Hypothesis h,
									     const std::string& n) const
  {
    return this->getMechanicalBehaviourData(h).getVariableNameFromGlossaryNameOrEntryName(n);
  } // end of MechanicalBehaviourDescription::getVariableNameFromGlossaryNameOrEntryName

  void
  MechanicalBehaviourDescription::setAttribute(const std::string& n,
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
	  string msg("MechanicalBehaviourDescription::setAttribute : ",
		     "attribute already exists with a different type");
	  throw(runtime_error(msg));
	}
	return;
      }
    }
    if(!this->attributes.insert(MVType(n,a)).second){
      string msg("MechanicalBehaviourDescription::setAttribute : "
		 "attribute '"+n+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourDescription::setAttribute

  bool
  MechanicalBehaviourDescription::hasAttribute(const std::string& n) const
  {
    return this->attributes.count(n)!=0u;
  } // end of MechanicalBehaviourDescription::hasAttribute
  
  const std::map<std::string,MechanicalBehaviourAttribute>&
  MechanicalBehaviourDescription::getAttributes() const
  {
    return this->attributes;
  } // end of MechanicalBehaviourDescription::getAttributes

  void
  MechanicalBehaviourDescription::registerVariable(const Hypothesis h,
						   const std::string& n)
  {
    this->callMechanicalBehaviourData(h,&MechanicalBehaviourData::registerVariable,n);
  } // end of MechanicalBehaviourDescription::registerVariableName

  void
  MechanicalBehaviourDescription::addMaterialLaw(const std::string& m)
  {
    using namespace std;
    if(find(this->materialLaws.begin(),
	    this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of MechanicalBehaviourDescription::getMaterialLaws

  const std::vector<std::string>&
  MechanicalBehaviourDescription::getMaterialLaws(void) const
  {
    return this->materialLaws;
  }

} // end of namespace mfront
