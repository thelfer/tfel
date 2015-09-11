/*! 
 * \file  ElasticityBehaviourBrick.cxx
 * \brief
 * \author Helfer Thomas
 * \date   20 oct. 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ElasticityBehaviourBrick.hxx"

namespace mfront{

  static void
  setElasticSymmetryType(BehaviourDescription& bd,
			 const BehaviourSymmetryType s)
  {
    using namespace std;
    if(bd.isElasticSymmetryTypeDefined()){
      if(bd.getElasticSymmetryType()!=s){
	string msg("setElasticSymmetryType : "
		   "the elastic symmetry type defined for "
		   "the behaviour is inconsistent with the option "
		   "passed to the 'Elasticity' brick.");
	throw(runtime_error(msg));
      }
    } else {
      bd.setElasticSymmetryType(s);
    }
  } // end of setElasticSymmetryType

  ElasticityBehaviourBrick::ElasticityBehaviourBrick(AbstractBehaviourDSL& dsl_,
						     BehaviourDescription& mb_,
						     const AbstractBehaviourBrick::Parameters& p)
    : BehaviourBrickBase(dsl_,mb_),
      pss(true),
      gto(true)
  {
    typedef tfel::material::ModellingHypothesis MH; 
    // reserve some specific variables
    this->bd.reserveName(MH::UNDEFINEDHYPOTHESIS,"Je");
    // basic checks
    if(this->bd.getBehaviourType()!=
       BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
			       "this BehaviourBrick is only usable for "
			       "small strain behaviours"));
    }
    if(this->bd.getIntegrationScheme()!=
       BehaviourDescription::IMPLICITSCHEME){
      throw(std::runtime_error("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
			       "this BehaviourBrick is only usable in "
			       "implicit schemes"));
    }
    // parameters
    for(AbstractBehaviourBrick::Parameters::const_iterator pp=p.begin();pp!=p.end();++pp){
      if(pp->first=="Isotropic"){
	this->checkThatParameterHasNoValue(*pp);
	setElasticSymmetryType(this->bd,mfront::ISOTROPIC);
      } else if(pp->first=="Orthotropic"){
	this->checkThatParameterHasNoValue(*pp);
	setElasticSymmetryType(this->bd,mfront::ORTHOTROPIC);
      } else if(pp->first=="NoPlaneStressSupport"){
	this->checkThatParameterHasNoValue(*pp);
	this->pss = false;
      } else if(pp->first=="NoGenericTangentOperator"){
	this->checkThatParameterHasNoValue(*pp);
	this->gto = false;
      } else {
	throw(std::runtime_error("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
				 "unsupported parameter '"+pp->first+"'"));
      }
    }
    if(this->pss){
      this->bd.registerMemberName(MH::UNDEFINEDHYPOTHESIS,"etozz");
      this->bd.registerMemberName(MH::UNDEFINEDHYPOTHESIS,"detozz");
    }
   } // end of ElasticityBehaviourBrick::ElasticityBehaviourBrick

  
  void 
  ElasticityBehaviourBrick::endTreatment(void) const
  {
    using namespace std;
    using tfel::glossary::Glossary; 
    typedef tfel::material::ModellingHypothesis MH; 
    typedef MH::Hypothesis Hypothesis; 
    const Hypothesis h = MH::UNDEFINEDHYPOTHESIS;
    // modelling hypotheses supported by the behaviour
    const set<Hypothesis> bmh = bd.getModellingHypotheses();
    // deformation strain
    const pair<bool,bool> b = this->bd.checkVariableExistence("eel");
    if(b.first){
      if(!b.second){
    	string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
    		   "'eel' is not declared for all specialisation of the behaviour");
    	throw(runtime_error(msg));
      }
      this->bd.checkVariableExistence("eel","StateVariable");
      this->bd.checkVariablePosition("eel","StateVariable",0u);
      this->bd.checkVariableGlossaryName("eel",Glossary::ElasticStrain);
    } else {
      VariableDescription eel("StrainStensor","eel",1u,0u);
      eel.description = "elastic strain";
      this->bd.addStateVariable(h,eel);
      this->bd.setGlossaryName(h,"eel",Glossary::ElasticStrain);
    }
    // treating material properties and stress computation
    if(this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      this->bd.appendToIncludes("#include\"TFEL/Material/StiffnessTensor.hxx\"");
      CodeBlock smts;
      CodeBlock sets;
      smts.code = "this->sig=(this->D)*(this->eel+theta*(this->deel));\n";
      sets.code = "this->sig=(this->D)*(this->eel);\n";
      this->bd.setCode(h,BehaviourData::ComputeStress,
    		       smts,BehaviourData::CREATE,
    		       BehaviourData::AT_BEGINNING,false);
      this->bd.setCode(h,BehaviourData::ComputeFinalStress,
    		       sets,BehaviourData::CREATE,
    		       BehaviourData::AT_BEGINNING,false);
    } else {
      if(this->bd.getElasticSymmetryType()==mfront::ISOTROPIC){
    	this->treatIsotropicBehaviour();	
      } else if(this->bd.getElasticSymmetryType()==mfront::ORTHOTROPIC){
    	this->treatOrthotropicBehaviour();
      } else {
    	string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
    		   "unsupported elastic symmetry type");
    	throw(runtime_error(msg));
      }
    }
    // consistency checks
    // plane stress support
    if(this->pss){
      const bool agps = bmh.count(MH::AXISYMMETRICALGENERALISEDPLANESTRESS)!=0;
      const bool ps   = bmh.count(MH::PLANESTRESS)!=0;
      VariableDescription etozz("strain","etozz",1u,0u);
      etozz.description = "axial strain";
      if(agps){
	this->bd.addStateVariable(MH::AXISYMMETRICALGENERALISEDPLANESTRESS,etozz);
	this->bd.setGlossaryName(MH::AXISYMMETRICALGENERALISEDPLANESTRESS,
				 "etozz",Glossary::AxialStrain);
	CodeBlock integrator;
	integrator.code += "// the plane stress equation is satisfied at the end of the time\n";
	integrator.code += "// step\n";
	integrator.code += "const stress szz = (lambda+2*mu)*(this->eel(1)+this->deel(1))+lambda*(this->eel(0)+this->deel(0)+this->eel(2)+this->deel(2));\n";
	integrator.code += "fetozz   = (szz-this->sigzz-this->dsigzz)/this->young;\n";
	integrator.code += "// modification of the partition of strain\n";
	integrator.code += "feel(1) -= this->detozz;\n";
	integrator.code += "// jacobian\n";
	integrator.code += "dfeel_ddetozz(1)=-1;\n";
	integrator.code += "dfetozz_ddetozz  = real(0);\n";
	integrator.code += "dfetozz_ddeel(1) = (this->lambda+2*(this->mu))/this->young;\n";
	integrator.code += "dfetozz_ddeel(0) = this->lambda/this->young;\n";
	integrator.code += "dfetozz_ddeel(2) = this->lambda/this->young;\n";
	this->bd.setCode(MH::AXISYMMETRICALGENERALISEDPLANESTRESS,
			 BehaviourData::Integrator,
			 integrator,BehaviourData::CREATEORAPPEND,
			 BehaviourData::AT_END);
      }
      if(ps){
	this->bd.addStateVariable(MH::PLANESTRESS,etozz);
	this->bd.setGlossaryName(MH::PLANESTRESS,
				 "etozz",Glossary::AxialStrain);
	CodeBlock integrator;
	integrator.code += "// the plane stress equation is satisfied at the end of the time\n";
	integrator.code += "// step\n";
	integrator.code += "const stress szz = (this->lambda+2*(this->mu))*(this->eel(2)+this->deel(2))+this->lambda*(this->eel(0)+this->deel(0)+this->eel(1)+this->deel(1));\n";
	integrator.code += "fetozz   = szz/(this->young);\n";
	integrator.code += "// modification of the partition of strain\n";
	integrator.code += "feel(2) -= detozz;\n";
	integrator.code += "// jacobian\n";
	integrator.code += "dfeel_ddetozz(2)=-1;\n";
	integrator.code += "dfetozz_ddetozz  = real(0);\n";
	integrator.code += "dfetozz_ddeel(2) = (this->lambda+2*(this->mu))/this->young;\n";
	integrator.code += "dfetozz_ddeel(0) = this->lambda/this->young;\n";
	integrator.code += "dfetozz_ddeel(1) = this->lambda/this->young;\n";
	this->bd.setCode(MH::PLANESTRESS,BehaviourData::Integrator,
			 integrator,BehaviourData::CREATEORAPPEND,
			 BehaviourData::AT_END);
      }
    }
    // tangent operator
    if(gto){
      CodeBlock tangentOperator;
      ostringstream to;
      if(this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	const bool agps = bmh.count(MH::AXISYMMETRICALGENERALISEDPLANESTRESS)!=0;
	const bool ps   = bmh.count(MH::PLANESTRESS)!=0;
	if(agps || ps){
	  if(this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	    if(!this->bd.hasAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor)){
	      this->bd.setAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,true,false);
	    }
	    if(!this->bd.getAttribute<bool>(BehaviourDescription::requiresUnAlteredStiffnessTensor)){
	      string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
			 "support for plane stress requires the use of an unaltered stiffness tensor");
	      throw(runtime_error(msg));
	    }
	  }
	}
    	to << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){" << endl
    	   << "  this->Dt = this->D;" << endl
    	   << "} else if (smt==CONSISTENTTANGENTOPERATOR){" << endl
    	   << "  Stensor4 Je;" << endl
    	   << "  getPartialJacobianInvert(Je);" << endl
    	   << "  this->Dt = (this->D)*Je;" << endl
    	   << "} else {" << endl
    	   << "  return false;" << endl
    	   << "}";
    	tangentOperator.code = to.str();
    	this->bd.setCode(h,BehaviourData::ComputeTangentOperator,
    			 tangentOperator,BehaviourData::CREATEORAPPEND,
    			 BehaviourData::AT_BEGINNING);
      } else {
    	if(this->bd.getElasticSymmetryType()==mfront::ISOTROPIC){
    	  to << "using namespace tfel::material::lame;" << endl
    	     << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){" << endl
    	     << "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,lambda,mu);" << endl
    	     << "} else if (smt==CONSISTENTTANGENTOPERATOR){" << endl
    	     << "  StiffnessTensor Hooke;" << endl
    	     << "  Stensor4 Je;" << endl
    	     << "  computeElasticStiffness<N,Type>::exe(Hooke,lambda,mu);" << endl
    	     << "  getPartialJacobianInvert(Je);" << endl
    	     << "  Dt = Hooke*Je;" << endl
    	     << "} else {" << endl
    	     << "  return false;" << endl
    	     << "}";
    	} else if(this->bd.getElasticSymmetryType()==mfront::ORTHOTROPIC){
    	  string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
    		     "orthotropic behaviour shall require the stiffness tensor");
    	  throw(runtime_error(msg));
    	} else {
    	  string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
    		     "unsupported elastic symmetry type");
    	  throw(runtime_error(msg));
    	}
      }
    }
    // implicit equation associated with the elastic strain
    CodeBlock integrator;
    integrator.code = "feel -= this->deto;\n";
    this->bd.setCode(h,BehaviourData::Integrator,
    		     integrator,BehaviourData::CREATEORAPPEND,
    		     BehaviourData::AT_BEGINNING);
} // end of ElasticityBehaviourBrick::endTreatment

  void
  ElasticityBehaviourBrick::treatIsotropicBehaviour(void) const
  {
    using tfel::glossary::Glossary; 
    typedef tfel::material::ModellingHypothesis MH; 
    typedef MH::Hypothesis Hypothesis; 
    const Hypothesis h = MH::UNDEFINEDHYPOTHESIS;
    this->bd.appendToIncludes("#include\"TFEL/Material/Lame.hxx\"");
    this->addMaterialPropertyIfNotDefined("stress","young",Glossary::YoungModulus);
    this->addMaterialPropertyIfNotDefined("real","nu",Glossary::PoissonRatio);
    this->addLocalVariable("stress","lambda");
    this->addLocalVariable("stress","mu");
    // local variable initialisation
    CodeBlock init;
    init.code = "// initialisation Lame's coefficient\n"
      "this->lambda=tfel::material::lame::computeLambda(this->young,this->nu);\n"
      "this->mu=tfel::material::lame::computeMu(this->young,this->nu);\n";
    CodeBlock smts;
    smts.code =
      "this->sig=this->lambda*trace(this->eel+theta*(this->deel))*Stensor::Id()+2*(this->mu)*(this->eel+theta*(this->deel));\n";
    CodeBlock sets;
    sets.code =
      "this->sig=this->lambda*trace(this->eel)*Stensor::Id()+2*(this->mu)*this->eel;";
    this->bd.setCode(h,BehaviourData::BeforeInitializeLocalVariables,
		     init,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
    this->bd.setCode(h,BehaviourData::ComputeStress,
		     smts,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
    this->bd.setCode(h,BehaviourData::ComputeFinalStress,
		     sets,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
  } // end of ElasticityBehaviourBrick::treatIsotropicBehaviour

  void
  ElasticityBehaviourBrick::treatOrthotropicBehaviour(void) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH; 
    typedef MH::Hypothesis Hypothesis; 
    const Hypothesis h = MH::UNDEFINEDHYPOTHESIS;
    if(!this->bd.hasAttribute(BehaviourDescription::requiresStiffnessTensor)){
      this->bd.setAttribute(h,BehaviourDescription::requiresStiffnessTensor,true);
    }
    if(!this->bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor)){
      string msg("ElasticityBehaviourBrick::treatOrthotropicBehaviour : "
		 "the stiffness tensor must be defined for orthotropic behaviours");
      throw(runtime_error(msg));
    }
  } // end of ElasticityBehaviourBrick::treatOrthotropicBehaviour

  std::string
  ElasticityBehaviourBrick::getName() const{
    return "Elasticity";
  }
  
  std::vector<tfel::material::ModellingHypothesis::Hypothesis> 
  ElasticityBehaviourBrick::getSupportedModellingHypotheses(void) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    set<MH::Hypothesis> dmh = this->dsl.getDefaultModellingHypotheses();
    vector<MH::Hypothesis> mh(dmh.begin(),dmh.end());
    if(this->pss){
      if(this->dsl.isModellingHypothesisSupported(MH::PLANESTRESS)){
	mh.push_back(MH::PLANESTRESS);
      }
      if(this->dsl.isModellingHypothesisSupported(MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	mh.push_back(MH::AXISYMMETRICALGENERALISEDPLANESTRESS);
      }
    }
    return mh;
  } // end of ElasticityBehaviourBrick::getSupportedModellingHypothesis

  ElasticityBehaviourBrick::~ElasticityBehaviourBrick()
  {} // end of ElasticityBehaviourBrick::~ElasticityBehaviourBrick

} // end of namespace mfront
