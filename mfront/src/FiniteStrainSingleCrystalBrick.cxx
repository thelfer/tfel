/*! 
 * \file   FiniteStrainSingleCrystalBrick.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04/10/2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include<sstream>
#include<stdexcept>

#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/FiniteStrainSingleCrystalBrick.hxx"

namespace mfront{

  FiniteStrainSingleCrystalBrick::FiniteStrainSingleCrystalBrick(AbstractBehaviourDSL& dsl_,
								 BehaviourDescription& mb_,
								 const Parameters& p,
								 const DataMap&)
    : BehaviourBrickBase(dsl_,mb_)
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("FiniteStrainSingleCrystalBrick::FiniteStrainSingleCrystalBrick: "+m));}
    };
    const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    // basic checks
    if(this->bd.areModellingHypothesesDefined()){
      const auto bmh = this->bd.getModellingHypotheses();
      throw_if(bmh.size()==1u,"the only supported hypothesis is 'Tridimensional'");
      throw_if(*(bmh.begin())!=ModellingHypothesis::TRIDIMENSIONAL,
	       "the only supported hypothesis is 'Tridimensional'");
    } else {
      this->bd.setModellingHypotheses({ModellingHypothesis::TRIDIMENSIONAL});
    }
    throw_if(this->bd.getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
	     "this BehaviourBrick is only usable for small strain behaviours");
    throw_if(this->bd.getIntegrationScheme()!=BehaviourDescription::IMPLICITSCHEME,
	     "this BehaviourBrick is only usable in implicit schemes");
    throw_if(!p.empty(),"no parameters allowed");
    // material symmetry
    if(this->bd.isSymmetryTypeDefined()){
      throw_if(this->bd.getSymmetryType()!=mfront::ORTHOTROPIC,
	       "material must be declared orthotropic");
    } else {
      this->bd.setSymmetryType(mfront::ORTHOTROPIC);
    }
    throw_if(this->bd.getElasticSymmetryType()!=mfront::ORTHOTROPIC,
	     "elastic symmetry type must be orthortropic");
    // declaring the elastic strain as the first integration variable
    throw_if(!this->bd.getBehaviourData(h).getIntegrationVariables().empty(),
	     "no integration variable shall be declared before declaring the "
	     "'FiniteStrainSingleCrystal' brick");
    VariableDescription eel("StrainStensor","eel",1u,0u);
    eel.description = "elastic strain";
    this->bd.addIntegrationVariable(h,eel,BehaviourData::UNREGISTRED);
    this->bd.setGlossaryName(h,"eel",tfel::glossary::Glossary::ElasticStrain);
    // declaring the elastic part of the deformation gradient
    VariableDescription Fe("DeformationGradientTensor","Fe",1u,0u);
    Fe.description = "elastic part of the deformation gradient";
    this->bd.addAuxiliaryStateVariable(h,Fe,BehaviourData::UNREGISTRED);
    this->bd.setEntryName(h,"Fe","ElasticPartOfTheDeformationGradient");
    // additional includes
    this->bd.appendToIncludes("#include\"TFEL/Math/General/CubicRoots.hxx\"");
    // reserve some specific variables
    this->bd.reserveName(h,"ss");
    this->bd.reserveName(h,"fsscb_data");
    this->bd.reserveName(h,"fsscb_tprd");
    this->bd.reserveName(h,"fsscb_dfeel_dinv_dFp");
    this->bd.reserveName(h,"fsscb_dC_dFe");
    this->bd.reserveName(h,"fsscb_dS_dFe");
    this->bd.reserveName(h,"fsscb_dtau_dFe");
    this->bd.reserveName(h,"fsscb_dFe_dDF_tot");
    this->bd.reserveName(h,"fsscb_dfeel_dDF");
    this->bd.reserveName(h,"fsscb_Je");
    this->bd.reserveName(h,"fsscb_Jg");
    this->bd.reserveName(h,"fsscb_dinv_Fp_dDF");
    this->bd.reserveName(h,"fsscb_dFe_dDF");
  } // end of FiniteStrainSingleCrystalBrick::FiniteStrainSingleCrystalBrick

  std::pair<bool,FiniteStrainSingleCrystalBrick::tokens_iterator>
  FiniteStrainSingleCrystalBrick::treatKeyword(const std::string& key,
					       tokens_iterator& p,
					       const tokens_iterator pe)
  {
    using CrystalStructure       = BehaviourDescription::CrystalStructure;
    using SlipSystemsDescription = BehaviourDescription::SlipSystemsDescription;
    const auto m = std::string("FiniteStrainSingleCrystalBrick::treatKeyword");
    auto throw_if = [](const bool c,const std::string& msg){
      if(c){throw(std::runtime_error("FiniteStrainSingleCrystalBrick::"
				     "treatKeyword: "+msg));}
    };
    auto gs = [&m,&p,&throw_if,pe](){
      const auto direction = CxxTokenizer::readList(m,"<",">",p,pe);
      const auto plane     = CxxTokenizer::readList(m,"{","}",p,pe);
      throw_if(plane.size()!=direction.size(),
	       "plane and direction don't match in size");
      throw_if((plane.size()!=3u)&&(plane.size()!=4u),
	       "invalid definition of a plane "
	       "(must be an array of 3 or 4 integers, read '"+
	       std::to_string(plane.size())+"' values)");
      BehaviourDescription::SlipSystem s;
      if(plane.size()==3u){
	SlipSystemsDescription::system3d s3d;
	for(tfel::math::tvector<3u,int>::size_type i=0;i!=3;++i){
	  s3d.plane[i]  = std::stoi(plane[i].value);
	  s3d.burgers[i] = std::stoi(direction[i].value);
	}
	s = s3d;
      } else {
	SlipSystemsDescription::system4d s4d;
	for(tfel::math::tvector<4u,int>::size_type i=0;i!=4;++i){
	  s4d.plane[i]  = std::stoi(plane[i].value);
	  s4d.burgers[i] = std::stoi(direction[i].value);
	}
	s = s4d;
      }
      return s;
    };
    if((key=="@SlidingSystem")||(key=="@GlidingSystem")||(key=="@SlipSystem")){
      const auto s = gs();
      this->bd.setSlipSystems({1u,s});
      CxxTokenizer::readSpecifiedToken(m,";",p,pe);
      return {true,p};
    } else if ((key=="@SlidingSystems")||(key=="@GlidingSystems")||(key=="@SlipSystems")){
      std::vector<BehaviourDescription::SlipSystem> ss;
      CxxTokenizer::readSpecifiedToken(m,"{",p,pe);
      CxxTokenizer::checkNotEndOfLine(m,p,pe);
      while(p->value!="}"){
	CxxTokenizer::checkNotEndOfLine(m,p,pe);
	ss.push_back(gs());
	CxxTokenizer::checkNotEndOfLine(m,p,pe);
	if(p->value!="}"){
	  CxxTokenizer::readSpecifiedToken(m,",",p,pe);
	  CxxTokenizer::checkNotEndOfLine(m,p,pe);
	  throw_if(p->value=="}","unexpected token '}'");
	}
      }
      CxxTokenizer::readSpecifiedToken(m,"}",p,pe);
      CxxTokenizer::readSpecifiedToken(m,";",p,pe);
      this->bd.setSlipSystems(ss);
      return {true,p};
    } else if (key=="@CrystalStructure"){
      CxxTokenizer::checkNotEndOfLine(m,p,pe);
      const auto cs = p->value;
      ++p;
      if(cs=="Cubic"){
	this->bd.setCrystalStructure(CrystalStructure::Cubic);
      } else if(cs=="FCC"){
	this->bd.setCrystalStructure(CrystalStructure::FCC);
      } else if(cs=="BCC"){
	this->bd.setCrystalStructure(CrystalStructure::BCC);
      } else if(cs=="HCP"){
	this->bd.setCrystalStructure(CrystalStructure::HCP);
      } else {
	throw_if(true,"unsupported crystal structure '"+cs+"'");
      }
      CxxTokenizer::readSpecifiedToken(m,";",p,pe);
      return {true,p};
    } else if (key=="@InteractionMatrix"){
      throw_if(!this->bd.areSlipSystemsDefined(),
	       "slip systems have not been defined");
      const auto& im = this->bd.getInteractionMatrixStructure();
      const auto  r  = im.rank();
      const auto  mv = CxxTokenizer::readArray(m,p,pe);
      CxxTokenizer::readSpecifiedToken(m,";",p,pe);
      throw_if(mv.size()!=r,"the number of values does "
	       "not match the number of independent coefficients "
	       "in the interaction matrix");
      auto imv = std::vector<long double>{};
      imv.reserve((mv.size()));
      for(const auto& v:mv){
	imv.push_back(tfel::utilities::convert<long double>(v));
      }
      this->bd.setInteractionMatrix(imv);
      return {true,p};
    }
    return BehaviourBrickBase::treatKeyword(key,p,pe);
  } // end of FiniteStrainSingleCrystalBrick::treatKeyword
  
  void FiniteStrainSingleCrystalBrick::completeVariableDeclaration() const
  {
    using tfel::glossary::Glossary; 
    const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      getLogStream() << "FiniteStrainSingleCrystalBrick::completeVariableDeclaration: begin\n";
    }
    // defining the stiffness tensor, if necessary
    if((!this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))&&
       (!this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,false))){
      this->bd.setAttribute(BehaviourDescription::requiresStiffnessTensor,true,false);
    }
    LocalDataStructure d;
    d.name = "fsscb_data";
    // local data
    d.addVariable(h,{"DeformationGradientTensor","dF"});
    d.addVariable(h,{"DeformationGradientTensor","Fe_tr"});
    d.addVariable(h,{"DeformationGradientTensor","Fe0"});
    d.addVariable(h,{"StressStensor","S"});
    d.addVariable(h,{"Tensor","inv_dFp"});
    d.addVariable(h,{"real","J_inv_dFp"});
    d.addVariable(h,{"StrainStensor","tmp"});
    this->bd.addLocalDataStructure(d,BehaviourData::ALREADYREGISTRED);
    if(getVerboseMode()>=VERBOSE_DEBUG){
      getLogStream() << "FiniteStrainSingleCrystalBrick::completeVariableDeclaration: end\n";
    }
  }
  
  void FiniteStrainSingleCrystalBrick::endTreatment() const
  {
    const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto cn   = this->bd.getClassName()+"SlipSystems<real>";
    // local data values initialisation
    CodeBlock init;
    init.code =
      "this->fsscb_data.dF    = (this->F1)*invert(this->F0);\n"
      "this->fsscb_data.Fe0   = this->Fe;\n"
      "this->fsscb_data.Fe_tr = (this->fsscb_data.dF)*(this->fsscb_data.Fe0);\n"
      "this->eel = computeGreenLagrangeTensor(this->Fe);\n";
    init.members  = {"F0","F1","Fe","eel"};
    this->bd.setCode(h,BehaviourData::BeforeInitializeLocalVariables,
    		     init,BehaviourData::CREATEORAPPEND,
    		     BehaviourData::AT_END);
    CodeBlock integrator;
    integrator.code =
      "const auto& ss = "+cn+"::getSlidingSystems();\n"
      "this->fsscb_data.S = (this->D)*(this->eel+this->deel);\n"
      "this->fsscb_data.tmp = StrainStensor::Id() + 2*(this->eel+this->deel);\n"
      "// Mandel stress tensor\n"
      "const auto M = eval((StrainStensor::Id() + 2*(this->eel+this->deel))*(this->fsscb_data.S));\n"
      "// Mandel stress tensor derivative\n"
      "const auto dM_ddeel = eval(2*st2tot2<N,real>::tpld(this->fsscb_data.S)+\n"
      "				    st2tot2<N,real>::tprd(this->fsscb_data.tmp,this->D));\n"
      "this->fsscb_data.inv_dFp = Tensor::Id();\n"
      "for(unsigned short i=0;i!="+cn+"::Nss;++i){\n"
      "  this->fsscb_data.inv_dFp -= (this->dg[i])*ss.mu[i];\n"
      "}\n"
      "this->fsscb_data.J_inv_dFp = det(this->fsscb_data.inv_dFp);\n"
      "this->fsscb_data.inv_dFp /= CubicRoots::cbrt(this->fsscb_data.J_inv_dFp);\n"
      "this->Fe = (this->fsscb_data.Fe_tr)*(this->fsscb_data.inv_dFp);\n"
      "feel = this->eel+this->deel-computeGreenLagrangeTensor(this->Fe);\n"
      "const auto fsscb_tprd = t2tot2<N,real>::tprd(this->fsscb_data.Fe_tr);\n"
      "const auto fsscb_dfeel_dinv_dFp = t2tost2<N,real>::dCdF(this->Fe)*fsscb_tprd;\n"
      "for(unsigned short i=0;i!="+cn+"::Nss;++i){\n"
      "  dfeel_ddg(i) = (fsscb_dfeel_dinv_dFp)*ss.mu[i]/2;\n"
      "}\n";
    integrator.members  = {"eel","Fe","D"};
    this->bd.setCode(h,BehaviourData::Integrator,
    		     integrator,BehaviourData::CREATEORAPPEND,
    		     BehaviourData::AT_BEGINNING);
    CodeBlock fs;
    fs.code = 
      "const auto& ss = "+cn+"::getSlidingSystems();\n"
      "this->fsscb_data.inv_dFp = Tensor::Id();\n"
      "for(unsigned short i=0;i!="+cn+"::Nss;++i){\n"
      "  this->fsscb_data.inv_dFp -= dg[i]*ss.mu[i];\n"
      "}\n"
      "this->fsscb_data.J_inv_dFp = det(this->fsscb_data.inv_dFp);\n"
      "this->fsscb_data.inv_dFp /= CubicRoots::cbrt(this->fsscb_data.J_inv_dFp);\n"
      "this->Fe = (this->fsscb_data.Fe_tr)*(this->fsscb_data.inv_dFp);\n"
      "this->fsscb_data.S = (this->D)*(this->eel);\n"
      "this->sig = convertSecondPiolaKirchhoffStressToCauchyStress(this->fsscb_data.S,this->Fe);\n";
    fs.members  = {"sig","Fe","D"};
    this->bd.setCode(h,BehaviourData::ComputeFinalStress,
    		     fs,BehaviourData::CREATE,
    		     BehaviourData::AT_BEGINNING);
    // tangent operator
    CodeBlock to;
    to.code = 
      "static_cast<void>(smt);\n"
      "const auto& ss = "+cn+"::getSlidingSystems();\n"
      "const t2tost2<N,stress> fsscb_dC_dFe = t2tost2<N,real>::dCdF(this->Fe);\n"
      "const t2tost2<N,stress> fsscb_dS_dFe = 0.5*(this->D)*fsscb_dC_dFe;\n"
      "const auto fsscb_dtau_dFe = computePushForwardDerivative(fsscb_dS_dFe,this->fsscb_data.fsscb_S,this->Fe); \n"
      "const t2tot2<N,real> fsscb_dFe_dDF_tot = t2tot2<N,real>::tpld(this->fsscb_data.inv_dFp,t2tot2<N,real>::tpld(this->fsscb_data.Fe0));\n"
      "const t2tost2<N,real> fsscb_dfeel_dDF  = -0.5*(fsscb_dC_dFe)*(fsscb_dFe_dDF_tot);\n"
      "st2tost2<N,real> fsscb_Je;\n"
      "tvector<"+cn+"::Nss,Stensor> fsscb_Jg;\n"
      "getPartialJacobianInvert(fsscb_Je,fsscb_Jg);\n"
      "t2tot2<N,real> fsscb_dinv_Fp_dDF = (ss.mu[0])^(fsscb_Jg[0]|fsscb_dfeel_dDF);\n"
      "for(unsigned short i=1;i!="+cn+"::Nss;++i){\n"
      "  fsscb_dinv_Fp_dDF += (ss.mu[i])^(fsscb_Jg[i]|fsscb_dfeel_dDF);\n"
      "}\n"
      "const t2tot2<N,real> fsscb_dFe_dDF=\n"
      "  fsscb_dFe_dDF_tot+t2tot2<N,real>::tprd(this->fsscb_data.Fe_tr,fsscb_dinv_Fp_dDF);\n"
      "Dt = fsscb_dtau_dFe*fsscb_dFe_dDF;\n";
    to.members  = {"Fe","D"};
    this->bd.setCode(h,BehaviourData::BehaviourData::ComputeTangentOperator+"-DTAU_DDF",
    		     to,BehaviourData::CREATE,
    		     BehaviourData::AT_BEGINNING);
  } // end of FiniteStrainSingleCrystalBrick::endTreatment
  
  std::string FiniteStrainSingleCrystalBrick::getName() const{
    return "FiniteStrainSingleCrystal";
  }
  
  std::vector<tfel::material::ModellingHypothesis::Hypothesis> 
  FiniteStrainSingleCrystalBrick::getSupportedModellingHypotheses() const
  {
    return {ModellingHypothesis::TRIDIMENSIONAL};
  } // end of FiniteStrainSingleCrystalBrick::getSupportedModellingHypothesis

  FiniteStrainSingleCrystalBrick::~FiniteStrainSingleCrystalBrick() = default;

} // end of namespace mfront
