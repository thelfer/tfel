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

  ElasticityBehaviourBrick::ElasticityBehaviourBrick(AbstractBehaviourDSL& dsl_,
						     BehaviourDescription& mb_,
						     const BehaviourBrick::Parameters& p)
    : BehaviourBrickBase(dsl_,mb_)
  {
    using namespace std;
    using namespace tfel::glossary;
    typedef tfel::material::ModellingHypothesis MH;
    typedef MH::Hypothesis Hypothesis;
    const Hypothesis h = MH::UNDEFINEDHYPOTHESIS;
    bool pss = true; //< plane stress support
    bool gto = true; //< generic tangent operator support support
    // checks
    this->dsl.reserveName("Je",false);
    // basic checks
    if(this->bd.getBehaviourType()!=
       BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
		 "this BehaviourBrick is only usable for small strain behaviours");
      throw(runtime_error(msg));
    }
    if(this->bd.getIntegrationScheme()!=
       BehaviourDescription::IMPLICITSCHEME){
      string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
		 "this BehaviourBrick is only usable in implicit schemes");
      throw(runtime_error(msg));
    }
    // parameters
    for(const auto & elem : p){
      if(elem.first=="NoPlaneStressSupport"){
	this->checkThatParameterHasNoValue(elem);
	pss = false;
      } else if(elem.first=="Isotropic"){
	this->checkThatParameterHasNoValue(elem);
	this->bd.setElasticSymmetryType(mfront::ISOTROPIC);
      } else if(elem.first=="Orthotropic"){
	this->checkThatParameterHasNoValue(elem);
	this->bd.setElasticSymmetryType(mfront::ORTHOTROPIC);
      } else if(elem.first=="NoGenericTangentOperator"){
	this->checkThatParameterHasNoValue(elem);
	gto = false;
	throw(runtime_error("ElasticityBehaviourBrick: unimplemented parameter handling"));
      } else {
	string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : "
		   "unsupported parameter '"+elem.first+"'");
	throw(runtime_error(msg));
      }
    }
    // modelling hypothesis
    if(!this->bd.areModellingHypothesesDefined()){
      set<Hypothesis> mh = this->dsl.getDefaultModellingHypotheses();
      if(pss){
	if(mh.count(MH::PLANESTRESS)==0){
	  if(this->dsl.isModellingHypothesisSupported(MH::PLANESTRESS)){
	    mh.insert(MH::PLANESTRESS);
	  }
	}
	if(mh.count(MH::PLANESTRESS)==0){
	  if(this->dsl.isModellingHypothesisSupported(MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	    mh.insert(MH::AXISYMMETRICALGENERALISEDPLANESTRESS);
	  }
	}
      }
      this->bd.setModellingHypotheses(mh);
    }
    const auto& bmh = this->bd.getModellingHypotheses();
    if(((bmh.count(MH::AXISYMMETRICALGENERALISEDPLANESTRESS)!=0)||
	(bmh.count(MH::PLANESTRESS)!=0))&&(!pss)){
      string msg("ElasticityBehaviourBrick::ElasticityBehaviourBrick : ");
      msg += "support for plane stress must be provided by the brick";
      throw(runtime_error(msg));
    }
    // deformation strain
    pair<bool,bool> b = this->bd.checkVariableExistence("eel");
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
      this->dsl.registerVariable("eel",false);
      this->dsl.registerVariable("deel",false);
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
    if((bmh.count(MH::AXISYMMETRICALGENERALISEDPLANESTRESS)!=0)||
       (bmh.count(MH::PLANESTRESS)!=0)){
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
    // tangent operator
    if(gto){
      CodeBlock tangentOperator;
      ostringstream to;
      if(this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	to << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){" << endl
	   << "  computeAlteredStiffnessTensor<hypothesis>::exe(this->Dt,this->D);" << endl
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
   } // end of ElasticityBehaviourBrick::ElasticityBehaviourBrick

  void
  ElasticityBehaviourBrick::treatIsotropicBehaviour()
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
  ElasticityBehaviourBrick::treatOrthotropicBehaviour()
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

  ElasticityBehaviourBrick::~ElasticityBehaviourBrick()
  {} // end of ElasticityBehaviourBrick::~ElasticityBehaviourBrick

} // end of namespace mfront
