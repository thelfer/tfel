/*! 
 * \file  StandardElasticityBrick.cxx
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
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/StandardElasticityBrick.hxx"

namespace mfront{

  StandardElasticityBrick::StandardElasticityBrick(AbstractBehaviourDSL& dsl_,
						   BehaviourDescription& mb_,
						   const AbstractBehaviourBrick::Parameters& p)
    : BehaviourBrickBase(dsl_,mb_)
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("StandardElasticityBrick::StandardElasticityBrick: "+m));}
    };
    // reserve some specific variables
    this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"sebdata");
    // basic checks
    throw_if(this->bd.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
	     "this BehaviourBrick is only usable for small strain behaviours");
    throw_if(this->bd.getIntegrationScheme()!=BehaviourDescription::IMPLICITSCHEME,
	     "this BehaviourBrick is only usable in implicit schemes");
    // parameters
    for(auto pp=p.cbegin();pp!=p.cend();++pp){
      if(pp->first=="Isotropic"){
	this->checkThatParameterHasNoValue(*pp);
	setElasticSymmetryType(this->bd,mfront::ISOTROPIC);
      } else if(pp->first=="Orthotropic"){
	// this is also checked later during the call of
	// setElasticSymmetryType call
	throw_if(this->bd.getSymmetryType()!=mfront::ORTHOTROPIC,
		 "using the orthotropic behaviour requires that the "
		 "behaviour has not been declared orthotropic");
	this->checkThatParameterHasNoValue(*pp);
	setElasticSymmetryType(this->bd,mfront::ORTHOTROPIC);
      } else if(pp->first=="NoPlaneStressSupport"){
	this->checkThatParameterHasNoValue(*pp);
	this->pss = false;
      } else if(pp->first=="NoGenericTangentOperator"){
	this->checkThatParameterHasNoValue(*pp);
	this->gto = false;
      } else {
	throw_if(true,"unsupported parameter '"+pp->first+"'");
      }
    }
    if(this->pss){
      this->bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"etozz");
      this->bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"detozz");
      this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"prediction_stress");
      this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"prediction_strain");
      this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"prediction_theta");
    }
  } // end of StandardElasticityBrick::StandardElasticityBrick

  void 
  StandardElasticityBrick::endTreatment(void) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("StandardElasticityBrick::endTreatment: "+m));}
    };
    using tfel::glossary::Glossary; 
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      getLogStream() << "StandardElasticityBrick::endTreatment: begin\n";
    }
    LocalDataStructure d;
    d.name = "sebdata";
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    // deformation strain
    const auto b = this->bd.checkVariableExistence("eel");
    if(b.first){
      throw_if(!b.second,"'eel' is not declared for all specialisation of the behaviour");
      this->bd.checkVariableExistence("eel","IntegrationVariable");
      this->bd.checkVariablePosition("eel","IntegrationVariable",0u);
      this->bd.checkVariableGlossaryName("eel",Glossary::ElasticStrain);
    } else {
      VariableDescription eel("StrainStensor","eel",1u,0u);
      eel.description = "elastic strain";
      this->bd.addStateVariable(uh,eel);
      this->bd.setGlossaryName(uh,"eel",Glossary::ElasticStrain);
    }
    // treating material properties and stress computation
    if((this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))||
       (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,false))){
      this->declareComputeStressWhenStiffnessTensorIsDefined();
    } else {
      if(this->bd.getElasticSymmetryType()==mfront::ISOTROPIC){
    	this->treatIsotropicBehaviour(d);	
      } else if(this->bd.getElasticSymmetryType()==mfront::ORTHOTROPIC){
    	this->treatOrthotropicBehaviour();
      } else {
	throw_if(true,"unsupported elastic symmetry type");
      }
    }
    // declaring the computeElasticPrediction member
    for(const auto h:this->bd.getDistinctModellingHypotheses()){
      std::string m =
    	"//! \brief return an elastic prediction of the stresses\n"
    	"StressStensor computeElasticPrediction(void) const{\n"
	"return this->computeElasticPrediction(this->theta);\n"
	"}\n"
    	"//! \brief return an elastic prediction of the stresses\n"
    	"StressStensor computeElasticPrediction(const real prediction_theta) const{\n";
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS){
	m += "throw(std::runtime_error(\"computeElasticPrediction: unsupported case\");\n";
	m += "return {};\n";
      } else if (h==ModellingHypothesis::PLANESTRESS){
    	if((this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))||
    	   (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,false))){
	  if(this->bd.hasAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor)){
	    m += "throw(std::runtime_error(\"computeElasticPrediction: unsupported case\");\n";
	    m += "return {};\n";
	  } else {
	    m += "return (this->D)*(this->eel+prediction_theta*this->deto);";
	  }
    	} else {
    	  if(this->bd.getElasticSymmetryType()==mfront::ISOTROPIC){
	    const auto& lvs = this->bd.getBehaviourData(uh).getLocalVariables();
	    const std::string lambda = lvs.contains("lambda") ? "this->lambda" : "this->sebdata.lambda";
	    const std::string mu     = lvs.contains("mu")     ? "this->mu"     : "this->sebdata.mu";
	    m += "StressStensor prediction_strain = this->eel+prediction_theta*this->deto;\n";
	    m += "prediction_stress(0) = (2*("+lambda+"+"+mu+"))/("+lambda+"+2*("+mu+"))*(prediction_strain(0)+prediction_strain(1))+2*("+mu+")*prediction_strain(0);\n";
	    m += "prediction_stress(1) = (2*("+lambda+"+"+mu+"))/("+lambda+"+2*("+mu+"))*(prediction_strain(0)+prediction_strain(1))+2*("+mu+")*prediction_strain(1);\n";
	    m += "prediction_stress(2) = stress(0);\n";
	    m += "prediction_stress(3) = 2*("+mu+")*prediction_strain(3);\n";
    	  } else {
    	    if(!this->bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor)){
    	      throw(std::runtime_error("StandardElasticityBrick::treatOrthotropicBehaviour: "
    				       "the stiffness tensor must be defined for "
    				       "orthotropic behaviours"));
    	    }
    	  }
    	}
      } else {
    	if((this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))||
    	   (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,false))){
	    m += "return (this->D)*(this->eel+prediction_theta*this->deto);";
    	} else {
    	  if(this->bd.getElasticSymmetryType()==mfront::ISOTROPIC){
	    const auto& lvs = this->bd.getBehaviourData(uh).getLocalVariables();
	    const std::string lambda = lvs.contains("lambda") ? "this->lambda" : "this->sebdata.lambda";
	    const std::string mu     = lvs.contains("mu")     ? "this->mu"     : "this->sebdata.mu";
	    m+= "return "+lambda+"*trace(this->eel+prediction_theta*(this->deto))*Stensor::Id()+"
	      "2*("+mu+")*(this->eel+prediction_theta*(this->deto));\n";
    	  } else {
    	    if(!this->bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor)){
    	      throw(std::runtime_error("StandardElasticityBrick::treatOrthotropicBehaviour: "
    				       "the stiffness tensor must be defined for "
    				       "orthotropic behaviours"));
    	    }
    	  }
    	}
      }
      m+= "}\n";
      this->bd.appendToMembers(h,m,false);
    }    
    // plane stress support
    if(this->pss){
      const bool agps = bmh.count(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)!=0;
      const bool ps   = bmh.count(ModellingHypothesis::PLANESTRESS)!=0;
      if(agps){
	this->addPlaneStressSupport(d);
      }
      if(ps){
	this->addAxisymmetricalGeneralisedPlaneStressSupport(d);
      }
    }
    // tangent operator
    if(gto){
      this->addGenericTangentOperatorSupport();
    }
    // implicit equation associated with the elastic strain
    CodeBlock integrator;
    integrator.code = "feel -= this->deto;\n";
    this->bd.setCode(uh,BehaviourData::Integrator,
    		     integrator,BehaviourData::CREATEORAPPEND,
    		     BehaviourData::AT_END);
    this->bd.addLocalDataStructure(d,BehaviourData::ALREADYREGISTRED);
  } // end of StandardElasticityBrick::endTreatment

  void
  StandardElasticityBrick::declareComputeStressWhenStiffnessTensorIsDefined(void) const{
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock smts,sets;
    smts.code = "this->sig=(this->D)*(this->eel+theta*(this->deel));\n";
    sets.code = "this->sig=(this->D)*(this->eel);\n";
    this->bd.setCode(h,BehaviourData::ComputeStress,
		     smts,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
    this->bd.setCode(h,BehaviourData::ComputeFinalStress,
		     sets,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
  } // end of StandardElasticityBrick::declareComputeStressWhenStiffnessTensorIsDefined
  
  void
  StandardElasticityBrick::treatIsotropicBehaviour(LocalDataStructure& d) const
  {
    using tfel::glossary::Glossary; 
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      getLogStream() << "StandardElasticityBrick::treatIsotropicBehaviour: begin\n";
    }
    this->bd.appendToIncludes("#include\"TFEL/Material/Lame.hxx\"");
    CodeBlock smts;
    CodeBlock sets;
    if(this->bd.areElasticMaterialPropertiesDefined()){
      smts.code =
	"this->sig=this->lambda*trace(this->eel+theta*(this->deel))*Stensor::Id()+"
	"2*(this->mu)*(this->eel+theta*(this->deel));\n";
      sets.code =
	"this->sig=this->lambda_tdt*trace(this->eel)*Stensor::Id()+2*(this->mu_tdt)*this->eel;";
    } else {
      this->addMaterialPropertyIfNotDefined("stress","young",Glossary::YoungModulus);
      this->addMaterialPropertyIfNotDefined("real","nu",Glossary::PoissonRatio);
      d.addVariable(h,{"stress","lambda"});
      d.addVariable(h,{"stress","mu"});
      // local variable initialisation
      CodeBlock init;
      init.code = "// initialisation Lame's coefficient\n"
	"this->sebdata.lambda=tfel::material::computeLambda(this->young,this->nu);\n"
	"this->sebdata.mu=tfel::material::computeMu(this->young,this->nu);\n";
      smts.code =
	"this->sig=(this->sebdata.lambda)*trace(this->eel+theta*(this->deel))*Stensor::Id()+"
	"2*(this->sebdata.mu)*(this->eel+theta*(this->deel));\n";
      sets.code =
	"this->sig=(this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+2*(this->sebdata.mu)*this->eel;";
      this->bd.setCode(h,BehaviourData::BeforeInitializeLocalVariables,
		       init,BehaviourData::CREATE,
		       BehaviourData::AT_BEGINNING,false);
    }
    this->bd.setCode(h,BehaviourData::ComputeStress,
		     smts,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
    this->bd.setCode(h,BehaviourData::ComputeFinalStress,
		     sets,BehaviourData::CREATE,
		     BehaviourData::AT_BEGINNING,false);
  } // end of StandardElasticityBrick::treatIsotropicBehaviour

  void
  StandardElasticityBrick::treatOrthotropicBehaviour(void) const
  {
    if(getVerboseMode()>=VERBOSE_DEBUG){
      getLogStream() << "StandardElasticityBrick::treatOrthotropic: begin\n";
    }
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!this->bd.hasAttribute(BehaviourDescription::requiresStiffnessTensor)){
      this->bd.setAttribute(h,BehaviourDescription::requiresStiffnessTensor,true);
    }
    if(!this->bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor)){
      throw(std::runtime_error("StandardElasticityBrick::treatOrthotropicBehaviour: "
			       "the stiffness tensor must be defined for "
			       "orthotropic behaviours"));
    }
  } // end of StandardElasticityBrick::treatOrthotropicBehaviour


  void
  StandardElasticityBrick::addAxisymmetricalGeneralisedPlaneStressSupport(LocalDataStructure& d) const{
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto agps = ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
    VariableDescription etozz("strain","etozz",1u,0u);
    etozz.description = "axial strain";
    d.addVariable(agps,{"stress","szz"});
    this->bd.addStateVariable(agps,etozz,BehaviourData::ALREADYREGISTRED);
    this->bd.setGlossaryName(agps,"etozz",tfel::glossary::Glossary::AxialStrain);
    CodeBlock integrator;
    // The brick contains a reference to an abstract behaviour dsl.
    // We need to know if we have to define the jacobian terms. So we
    // downcast it to the ImplicitDSLBase class have access to the
    // solver. See the getSolver call below.
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    if((this->bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,false))||
       (this->bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,false))){
      const std::string D =
	this->bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,false) ? "D_tdt" : "D"; 
      integrator.code +=
	"// the generalised plane stress equation is satisfied at the end of the time step\n"
	"this->sebdata.szz = (this->"+D+"(1,1))*(this->eel(1)+this->deel(1))+(this->"+D+"(1,0))*(this->eel(0)+this->deel(0))+(this->"+D+"(2,0))*(this->eel(2)+this->deel(2));\n"
	"fetozz   = (this->sebdata.szz-this->sigzz-this->dsigzz)/(this->"+D+"(1,1));\n"
	"// modification of the partition of strain\n"
	"feel(1) -= this->detozz;\n";
      if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	integrator.code +=
	  "// jacobian\n"
	  "dfeel_ddetozz(1) = -1;\n"
	  "dfetozz_ddetozz  = real(0);\n"
	  "dfetozz_ddeel(1) = 1;\n"
	  "dfetozz_ddeel(0) = (this->"+D+"(1,0))/(this->"+D+"(1,1));\n"
	  "dfetozz_ddeel(2) = (this->"+D+"(2,0))/(this->"+D+"(1,1));\n";
      }
    } else {
      if(this->bd.areElasticMaterialPropertiesDefined()){
	integrator.code +=
	  "// the generalised plane stress equation is satisfied at the end of the time step\n"
	  "this->sebdata.szz =   (this->lambda_tdt+2*(this->mu_tdt))*(this->eel(1)+this->deel(1))"
	  "                   + (this->lambda_tdt)*(this->eel(0)+this->deel(0)+this->eel(2)+this->deel(2));\n"
	  "fetozz   = (this->sebdata.szz-this->sigzz-this->dsigzz)/this->young_tdt;\n"
	  "// modification of the partition of strain\n"
	  "feel(1) -= this->detozz;\n";
	if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	  integrator.code +=
	    "// jacobian\n"
	    "dfeel_ddetozz(1) = -1;\n"
	    "dfetozz_ddetozz  = real(0);\n"
	    "dfetozz_ddeel(1) = (this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
	    "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
	    "dfetozz_ddeel(2) = this->lambda_tdt/this->young_tdt;\n";
	}
      } else {
	const auto& lvs = this->bd.getBehaviourData(uh).getLocalVariables();
	const std::string lambda = lvs.contains("lambda") ? "this->lambda" : "this->sebdata.lambda";
	const std::string mu     = lvs.contains("mu")     ? "this->mu"     : "this->sebdata.mu";
	integrator.code +=
	  "// the generalised plane stress equation is satisfied at the end of the time step\n"
	  "this->sebdata.szz =   ("+lambda+"+2*("+mu+"))*(this->eel(1)+this->deel(1))"
	  "                   + ("+lambda+")*(this->eel(0)+this->deel(0)+this->eel(2)+this->deel(2));\n"
	  "fetozz   = (this->sebdata.szz-this->sigzz-this->dsigzz)/this->young;\n"
	  "// modification of the partition of strain\n"
	  "feel(1) -= this->detozz;\n";
	if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	  integrator.code +=
	    "// jacobian\n"
	    "dfeel_ddetozz(1) = -1;\n"
	    "dfetozz_ddetozz  = real(0);\n"
	    "dfetozz_ddeel(1) = ("+lambda+"+2*("+mu+"))/this->young;\n"
	    "dfetozz_ddeel(0) = "+lambda+"/this->young;\n"
	    "dfetozz_ddeel(2) = "+lambda+"/this->young;\n";
	}
      }
    }
    this->bd.setCode(agps,BehaviourData::Integrator,
		     integrator,BehaviourData::CREATEORAPPEND,
		     BehaviourData::AT_END);
  }

  void
  StandardElasticityBrick::addPlaneStressSupport(LocalDataStructure& d) const{
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto ps = ModellingHypothesis::PLANESTRESS;
    d.addVariable(ps,{"stress","szz"});
    VariableDescription etozz("strain","etozz",1u,0u);
    etozz.description = "axial strain";
    this->bd.addStateVariable(ps,etozz,BehaviourData::ALREADYREGISTRED);
    this->bd.setGlossaryName(ps,"etozz",tfel::glossary::Glossary::AxialStrain);
    CodeBlock integrator;
    const auto& idsl = dynamic_cast<ImplicitDSLBase&>(this->dsl);
    if((this->bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,false))||
       (this->bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,false))){
      const std::string D =
	this->bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,false) ? "D_tdt" : "D"; 
      integrator.code +=
	"// the plane stress equation is satisfied at the end of the time step\n"
	"fetozz   = this->eel(2)+this->deel(2)+"
	"           ((this->"+D+"(2,0))/(this->"+D+"(2,2)))*(this->eel(0)+this->deel(0))+"
	"           ((this->"+D+"(2,1))/(this->"+D+"(2,2)))*(this->eel(1)+this->deel(1));\n"
	"// modification of the partition of strain\n"
	"feel(2)          -= this->detozz;\n";
      if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	integrator.code +=
	  "// jacobian\n"
	  "dfeel_ddetozz(2)  = -1;\n"
	  "dfetozz_ddetozz   = real(0);\n"
	  "dfetozz_ddeel(2)  = 1;\n"
	  "dfetozz_ddeel(0)  = (this->"+D+"(1,0))/(this->"+D+"(1,1));\n"
	  "dfetozz_ddeel(1)  = (this->"+D+"(2,0))/(this->"+D+"(1,1));\n";
      }
    } else {
      if(this->bd.areElasticMaterialPropertiesDefined()){
	integrator.code +=
	  "// the plane stress equation is satisfied at the end of the time step\n"
	  "this->sebdata.szz = (this->lambda_tdt+2*(this->mu_tdt))*(this->eel(2)+this->deel(2))+"
	  "                   (this->lambda_tdt)*(this->eel(0)+this->deel(0)+this->eel(1)+this->deel(1));\n"
	  "fetozz   = this->sebdata.szz/(this->young_tdt);\n"
	  "// modification of the partition of strain\n"
	  "feel(2) -= detozz;\n";
	if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	  integrator.code +=
	    "// jacobian\n"
	    "dfeel_ddetozz(2) = -1;\n"
	    "dfetozz_ddetozz  = real(0);\n"
	    "dfetozz_ddeel(2) = (this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
	    "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
	    "dfetozz_ddeel(1) = this->lambda_tdt/this->young_tdt;\n";
	}
      } else {
	const auto& lvs = this->bd.getBehaviourData(uh).getLocalVariables();
	const std::string lambda = lvs.contains("lambda") ? "this->lambda" : "this->sebdata.lambda";
	const std::string mu     = lvs.contains("mu")     ? "this->mu"     : "this->sebdata.mu";
	integrator.code +=
	  "// the plane stress equation is satisfied at the end of the time step\n"
	  "this->sebdata.szz = ("+lambda+"+2*("+mu+"))*(this->eel(2)+this->deel(2))+"
	  "                   ("+lambda+")*(this->eel(0)+this->deel(0)+this->eel(1)+this->deel(1));\n"
	  "fetozz   = this->sebdata.szz/(this->young);\n"
	  "// modification of the partition of strain\n"
	  "feel(2) -= detozz;\n";
	if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	  integrator.code +=
	    "// jacobian\n"
	    "dfeel_ddetozz(2) = -1;\n"
	    "dfetozz_ddetozz  = real(0);\n"
	    "dfetozz_ddeel(2) = ("+lambda+"+2*("+mu+"))/this->young;\n"
	    "dfetozz_ddeel(0) = "+lambda+"/this->young;\n"
	    "dfetozz_ddeel(1) = "+lambda+"/this->young;\n";
	}
      }
    }
    this->bd.setCode(ps,BehaviourData::Integrator,
		     integrator,BehaviourData::CREATEORAPPEND,
		     BehaviourData::AT_END);
  }

  void
  StandardElasticityBrick::addGenericTangentOperatorSupport(void) const{
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("StandardElasticityBrick::"
				     "addGenericTangentOperatorSupport: "+m));}
    };
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    CodeBlock tangentOperator;
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    if(this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      const bool agps = bmh.count(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)!=0;
      const bool ps   = bmh.count(ModellingHypothesis::PLANESTRESS)!=0;
      if(agps || ps){
	if(this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	  if(!this->bd.hasAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor)){
	    this->bd.setAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,true,false);
	  }
	  throw_if(!this->bd.getAttribute<bool>(BehaviourDescription::requiresUnAlteredStiffnessTensor),
		   "genertic tangent operator support for "
		   "plane stress hypotheses requires the use of an unaltered stiffness tensor");
	}
      }
      tangentOperator.code =
	"if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
	"  this->Dt = this->D;\n";
      if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	tangentOperator.code +=
	  "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
	  "  Stensor4 Je;\n"
	  "  getPartialJacobianInvert(Je);\n"
	  "  this->Dt = (this->D)*Je;\n";
      }
      tangentOperator.code +=
	"} else {\n"
	"  return false;\n"
	"}";
    } else {
      if(this->bd.getElasticSymmetryType()==mfront::ISOTROPIC){
	const auto& lvs = this->bd.getBehaviourData(uh).getLocalVariables();
	const std::string lambda = lvs.contains("lambda") ? "this->lambda" : "this->sebdata.lambda";
	const std::string mu     = lvs.contains("mu")     ? "this->mu"     : "this->sebdata.mu";
	tangentOperator.code =
	  "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
	  "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,"+lambda+","+mu+");\n"
	  "} else if (smt==CONSISTENTTANGENTOPERATOR){\n";
	if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	  tangentOperator.code +=
	    "  StiffnessTensor Hooke;\n"
	    "  Stensor4 Je;\n"
	    "  computeElasticStiffness<N,Type>::exe(Hooke,"+lambda+","+mu+");\n"
	    "  getPartialJacobianInvert(Je);\n"
	    "  Dt = Hooke*Je;\n";
	}
	tangentOperator.code +=
	  "} else {\n"
	  "  return false;\n"
	  "}";
      } else if(this->bd.getElasticSymmetryType()==mfront::ORTHOTROPIC){
	throw_if(!this->bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,false),
		 "orthotropic behaviour shall require the stiffness tensor");
	tangentOperator.code =
	  "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
	  "  this->Dt = this->D_tdt;\n";
	if((idsl.getSolver().usesJacobian())&&(!idsl.getSolver().requiresNumericalJacobian())){
	  tangentOperator.code +=
	    "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
	    "  Stensor4 Je;\n"
	    "  getPartialJacobianInvert(Je);\n"
	    "  this->Dt = (this->D_tdt)*Je;\n";
	}
	tangentOperator.code +=
	  "} else {\n"
	  "  return false;\n"
	  "}";
      } else {
	throw_if(true,"unsupported elastic symmetry type");
      }
    }
    this->bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
		     BehaviourData::ComputeTangentOperator,
		     tangentOperator,BehaviourData::CREATEORAPPEND,
		     BehaviourData::AT_BEGINNING);
  }
  
  std::string
  StandardElasticityBrick::getName() const{
    return "Elasticity";
  }
  
  std::vector<tfel::material::ModellingHypothesis::Hypothesis> 
  StandardElasticityBrick::getSupportedModellingHypotheses(void) const
  {
    auto dmh = this->dsl.getDefaultModellingHypotheses();
    std::vector<ModellingHypothesis::Hypothesis> mh(dmh.begin(),dmh.end());
    if(this->pss){
      if(this->dsl.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)){
	mh.push_back(ModellingHypothesis::PLANESTRESS);
      }
      if(this->dsl.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	mh.push_back(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
      }
    }
    return mh;
  } // end of StandardElasticityBrick::getSupportedModellingHypothesis

  StandardElasticityBrick::~StandardElasticityBrick()
  {} // end of StandardElasticityBrick::~StandardElasticityBrick

} // end of namespace mfront
