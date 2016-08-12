/*!
 * \file   AbaqusInterfaceBase.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 mars 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include"MFront/DSLUtilities.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/AbaqusInterfaceBase.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront{

  std::string
  AbaqusInterfaceBase::getLibraryName(const BehaviourDescription& mb) const
  {
    auto lib = std::string{};
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = this->getInterfaceName()+mb.getMaterialName();
      } else {
	lib = this->getInterfaceName()+"Behaviour";
      }
    } else {
      lib = this->getInterfaceName()+mb.getLibrary();
    }
    return makeUpperCase(lib);
  } // end of AbaqusInterfaceBase::getLibraryName

  std::string AbaqusInterfaceBase::getFunctionName(const std::string& name) const
  {
    return makeUpperCase(name);
  } // end of AbaqusInterfaceBase::getFunctionName

  std::string AbaqusInterfaceBase::getFunctionNameForHypothesis(const std::string& name,
								const Hypothesis h) const
  {
    const auto s = [h]() -> std::string {
      if(h==tfel::material::ModellingHypothesis::AXISYMMETRICAL){
	return "AXIS";
      } else if(h==tfel::material::ModellingHypothesis::PLANESTRAIN){
	return "PSTRAIN";
      } else if(h==tfel::material::ModellingHypothesis::PLANESTRESS){
	return "PSTRESS";
      } else if(h==tfel::material::ModellingHypothesis::TRIDIMENSIONAL){
	return "3D";
      }
      throw(std::runtime_error("AbaqusInterfaceBase::getFunctionNameForHypothesis: "
			       "invalid hypothesis."));
    }();
    return makeUpperCase(name)+"_"+s;
  } // end of AbaqusInterfaceBase::getFunctionNameForHypothesis
  
  std::set<tfel::material::ModellingHypothesis::Hypothesis>
  AbaqusInterfaceBase::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    using tfel::material::ModellingHypothesis;
    auto h = std::set<ModellingHypothesis::Hypothesis>{};
    const auto& bh = mb.getModellingHypotheses();
    if(bh.find(ModellingHypothesis::AXISYMMETRICAL)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICAL);
    }
    if(bh.find(ModellingHypothesis::PLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::PLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::PLANESTRESS)!=bh.end()){
      h.insert(ModellingHypothesis::PLANESTRESS);
    }
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)!=bh.end()){
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if(h.empty()){
      throw(std::runtime_error("AbaqusInterfaceBase::getModellingHypothesesToBeTreated : "
			       "no hypotheses selected. This means that the given beahviour "
			       "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
			       "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
			       "make sense to use the Abaqus interface"));
    }
    return h;
  } // end of AbaqusInterfaceBase::getModellingHypothesesToBeTreated

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  AbaqusInterfaceBase::buildMaterialPropertiesList(const BehaviourDescription& mb,
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
	throw(runtime_error("AbaqusInterfaceBase::buildMaterialPropertiesList: "
			    "internal error : the mechanical behaviour says that not "
			    "all handled mechanical data are specialised, but we found none."));
      }
      // material properties for all the selected hypothesis
      auto mpositions = vector<pair<vector<UMATMaterialProperty>,
				    SupportedTypes::TypeSize>>{};
      for(const auto & elem : uh){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,elem));
      }
      auto ph=uh.begin();
      auto pum = mpositions.begin();
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
	  auto o1 = mp1.offset;
	  o1+=pum->second;
	  auto o2 = mp2.offset;
	  o2+=mfirst.second;
	  if(o1!=o2){
	    throw(runtime_error("AbaqusInterfaceBase::buildMaterialPropertiesList : "
				"incompatible offset for material property '"+mp.name+
				"' (aka '"+mp1.name+"'). This is one pitfall of the umat interface. "
				"To by-pass this limitation, you may want to explicitely "
				"specialise some modelling hypotheses"));
	  }
	}
      }
      return mfirst;
    }
    auto res = pair<vector<UMATMaterialProperty>,
		    SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    if((h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
       (h!=ModellingHypothesis::AXISYMMETRICAL)&&
       (h!=ModellingHypothesis::PLANESTRAIN)&&
       (h!=ModellingHypothesis::PLANESTRESS)&&
       (h!=ModellingHypothesis::TRIDIMENSIONAL)){
      string msg("AbaqusInterfaceBase::buildMaterialPropertiesList : "
		 "unsupported modelling hypothesis");
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg += " (default)";
      } else {
	msg += " '"+ModellingHypothesis::toString(h)+"'";
      }
      throw(runtime_error(msg));
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus","youn",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio","nu",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio12","nu12",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio23","nu23",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio13","nu13",false);
	this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	if (h==ModellingHypothesis::TRIDIMENSIONAL){
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus23","g23",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus13","g13",false);
	} else if((h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)&&
		  (h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
		  (h!=ModellingHypothesis::AXISYMMETRICAL)&&
		  (h!=ModellingHypothesis::PLANESTRAIN)&&
		  (h!=ModellingHypothesis::PLANESTRESS)){
	  throw(std::runtime_error("AbaqusInterfaceBase::buildMaterialPropertiesList : "
				   "unsupported modelling hypothesis"));
	}
      } else {
	throw(std::runtime_error("AbaqusInterfaceBase::buildMaterialPropertiesList : "
				 "unsupported behaviour symmetry type"));
      }
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion",
					     "alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1",
					     "alp1",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2",
					     "alp2",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3",
					     "alp3",false);
      } else {
	throw(std::runtime_error("AbaqusInterfaceBase::buildMaterialPropertiesList : "
				 "unsupported behaviour symmetry type"));
      }
    }
    if(!mprops.empty()){
      const auto& m = mprops.back();
      res.second  = m.offset;
      res.second += this->getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of AbaqusInterfaceBase::buildMaterialPropertiesList

  void
  AbaqusInterfaceBase::writeAbaqusBehaviourTraits(std::ostream& out,
					      const BehaviourDescription& mb,
					      const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb,h);
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    } else {
      out << "template<typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    }
    out << ">\n";
    out << "struct AbaqusTraits<tfel::material::" << mb.getClassName() << "<";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "H";
    } else {
      out << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h);
    }
    out << ",Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{\n";
    out << "//! behaviour type\n";
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " AbaqusBehaviourType btype = abaqus::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " AbaqusBehaviourType btype = abaqus::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else {
      throw(std::runtime_error("AbaqusInterfaceBase::writeAbaqusBehaviourTraits : "
			       "unsupported behaviour type"));
    }
    out << "//! space dimension\n";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "static " << constexpr_c << " unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    } else {
      out << "static " << constexpr_c << " unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<"
	  << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h)
	  << ">::value;\n";
    }
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c << " unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c << " unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array\n";
    out << "static " << constexpr_c << " unsigned short DrivingVariableSize = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c << " unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
    if(mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " AbaqusSymmetryType type = abaqus::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " AbaqusSymmetryType type = abaqus::ORTHOTROPIC;\n";
    } else {
      throw(std::runtime_error("AbaqusInterfaceBase::endTreatment: unsupported behaviour type.\n"
			       "The abaqus interface only support isotropic or orthotropic "
			       "behaviour at this time."));
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if(!mprops.first.empty()){
      const auto& m = mprops.first.back();
      msize  = m.offset;
      msize += this->getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c << " unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " AbaqusSymmetryType etype = abaqus::ISOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 1u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " AbaqusSymmetryType etype = abaqus::ORTHOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
    	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset "
    	    << "= AbaqusOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 3u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else {
      throw(std::runtime_error("AbaqusInterfaceBase::endTreatment: unsupported behaviour type.\n"
			       "The abaqus interface only support isotropic or "
			       "orthotropic behaviour at this time."));
    }
    out << "}; // end of class AbaqusTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis,std::string>
  AbaqusInterfaceBase::gatherModellingHypothesesAndTests(const BehaviourDescription& mb) const
  {
    auto res = std::map<Hypothesis,std::string>{};
    if((mb.getSymmetryType()==mfront::ORTHOTROPIC)&&
       ((mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))||
	(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)))){
      auto h = this->getModellingHypothesesToBeTreated(mb);
      for(const auto & mh : h){
	res.insert({mh,this->getModellingHypothesisTest(mh)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  } // end of AbaqusInterfaceBase::gatherModellingHypothesesAndTests

  std::string
  AbaqusInterfaceBase::getModellingHypothesisTest(const Hypothesis h) const
  {
    if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "*NTENS==4";
    } else if(h==ModellingHypothesis::PLANESTRESS){
      return "*NTENS==3";
    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "*NTENS==6";
    }
    throw(std::runtime_error("AbaqusInterfaceBase::getModellingHypothesisTest : "
			     "unsupported modelling hypothesis"));
  } // end of AbaqusInterfaceBase::gatherModellingHypothesesAndTests

  void
  AbaqusInterfaceBase::writeUMATxxAdditionalSymbols(std::ostream&,
					      const std::string&,
					      const Hypothesis,
					      const BehaviourDescription&,
					      const FileDescription&) const
  {} // end of AbaqusInterfaceBase::writeUMATxxAdditionalSymbols

  void
  AbaqusInterfaceBase::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "ModellingHypothesis::Hypothesis h;\n"
	<< "if( *NTENS == 6 ){\n"
	<< "  h = ModellingHypothesis::TRIDIMENSIONAL;\n"
	<< "} else if(*NTENS==3){\n"
	<< "  h = ModellingHypothesis::PLANESTRESS;\n"
	<< "} else if(*NTENS==4){\n"
	<< "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;\n"
	<< "} else {\n"
	<< "  return;\n"
	<< "}\n"
	<< "mg.setModellingHypothesis(h);\n";
  } // end of AbaqusInterfaceBase::writeMTestFileGeneratorSetModellingHypothesis

  void
  AbaqusInterfaceBase::writeInputFileExample(const BehaviourDescription& mb,
					     const FileDescription& fd,
					     const bool b) const{ 
    const auto name =  mb.getLibrary()+mb.getClassName();
    const auto mn = this->getLibraryName(mb)+"_"+mb.getClassName();
    const auto fn = (b ? "abaqus/" : "abaqus-explicit/") +name+".inp";
    std::ofstream out{fn};
    if(!out){
      throw(std::runtime_error("AbaqusInterfaceBase::writeInputFileExample: "
			       "could not open file '"+fn+"'"));
    }
    // header
    out << "** \n"
	<< "** File generated by MFront from the "  << fd.fileName << " source\n"
	<< "** Example of how to use the " << mb.getClassName() << " behaviour law\n"
	<< "** Author "  << fd.authorName << '\n'
	<< "** Date   "  << fd.date       << '\n'
	<< "**\n\n";
    // loop over hypothesis
    for(const auto & h : this->getModellingHypothesesToBeTreated(mb)){
      const auto& d = mb.getBehaviourData(h);
      const auto mps = this->buildMaterialPropertiesList(mb,h);
      auto msize = SupportedTypes::TypeSize{};
      if(!mps.first.empty()){
	const auto& m = mps.first.back();
	msize  = m.offset;
	msize += this->getTypeSize(m.type,m.arraySize);
	msize -= mps.second;
      }
      const auto& persistentVarsHolder = d.getPersistentVariables();
      auto vs = SupportedTypes::TypeSize{};
      for(const auto& v : persistentVarsHolder){
	vs+=this->getTypeSize(v.type,v.arraySize);
      }
      const auto vsize = vs.getValueForModellingHypothesis(h);
      const auto& externalStateVarsHolder = d.getExternalStateVariables();
      out << "** Example for the '" << ModellingHypothesis::toString(h) << "' modelling hypothesis\n";
      if(!externalStateVarsHolder.empty()){
	out << "** This behaviour requires " << externalStateVarsHolder.size()
	    <<  " field variables to be defined:\n";
	int i=1;
	for(auto pv=externalStateVarsHolder.begin();pv!=externalStateVarsHolder.end();++pv,++i){
	  out << "** " << i << ": " << mb.getExternalName(h,pv->name);
	}
      }
      out << "*Material, name="
	  << this->getFunctionNameForHypothesis(mn,h) << '\n';
      if(!b){
	out << "*DENSITY\n<density>\n";
      }
      if(vsize!=0){
	out << "*Depvar\n" << vsize  << ",\n";
	int i=1;
	for(const auto& v : persistentVarsHolder){
	  const auto vn = mb.getExternalName(h,v.name);
	  if(v.arraySize==1){
	    this->writeDepvar(out,i,h,v,vn);
	  } else {
	    for(unsigned short a=0;a!=v.arraySize;++a){
	      this->writeDepvar(out,i,h,v,vn+'['+std::to_string(a)+']');
	    }
	  }
	}
      }
      if(!mps.first.empty()){
	out << "** The material properties are given as if we used parameters to explicitly\n"
	    << "** display their names. Users shall replace those declaration by\n"
	    << "** theirs values and/or declare those parameters in the appropriate *parameters\n"
	    << "** section of the input file\n";
      }
      out << "*User Material, constants=" << msize.getValueForModellingHypothesis(h);
      if(!mb.getAttribute(h,BehaviourData::isConsistentTangentOperatorSymmetric,false)){
	out << ", unsymm";
      }
      out << '\n';
      if(!mps.first.empty()){      
	int i=1;
	for(auto pm =mps.first.begin();pm!=mps.first.end();){
	  if(i%9==0){
	    out << "\n";
	    i=1;
	  }
	  out << '<' << pm->name << '>';
	  if(++pm!=mps.first.end()){
	    out << ", ";
	  }
	  ++i;
	}
      }
      out << "\n\n";
    }
  } // end of AbaqusInterfaceBase::writeInputFileExample

  void 
  AbaqusInterfaceBase::writeDepvar(std::ostream& out,
				   int& i,
				   const Hypothesis& h,
				   const VariableDescription& v,
				   const std::string& n) const {
    if(this->getTypeFlag(v.type)==SupportedTypes::Scalar){
      out << i++ << ", " << n << '\n';
    } else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	out << i++ << ", " << n << "_13\n";
	out << i++ << ", " << n << "_23\n";
      }
    } else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      out << i++ << ", " << n << "_21\n";
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	out << i++ << ", " << n << "_13\n";
	out << i++ << ", " << n << "_31\n";
	out << i++ << ", " << n << "_23\n";
	out << i++ << ", " << n << "_32\n";
      }
    } else {
      throw(std::runtime_error("AbaqusInterfaceBase::writeDepvar: "
			       "unsupported variable type"));
    }
  }
  
  AbaqusInterfaceBase::~AbaqusInterfaceBase() = default;
  
} // end of namespace mfront
