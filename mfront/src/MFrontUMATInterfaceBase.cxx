/*! 
 * \file  MFrontUMATInterfaceBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 */

#include<iostream>

#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/System/System.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MFrontUMATInterfaceBase.hxx"

namespace mfront
{

  MFrontUMATInterfaceBase::UMATMaterialProperty::UMATMaterialProperty(const std::string& t,
								      const std::string& n,
								      const std::string& v,
								      const unsigned short a,
								      const SupportedTypes::TypeSize o,
								      const bool d)
    : type(t),
      name(n),
      var_name(v),
      arraySize(a),
      offset(o),
      dummy(d)
  {} // end olf UMATMaterialProperty::UMATMaterialProperty

  const MFrontUMATInterfaceBase::UMATMaterialProperty&
  MFrontUMATInterfaceBase::findUMATMaterialProperty(const std::vector<UMATMaterialProperty>& mprops,
						    const std::string& n)
  {
    using namespace std;
    vector<UMATMaterialProperty>::const_iterator pm;
    for(pm=mprops.begin();pm!=mprops.end();++pm){
      if((pm->name==n)&&(!pm->dummy)){
	return *pm;
      }
    }
    string msg("MFrontUMATInterfaceBase::findUMATMaterialProperty : "
	       "no material property associated with the glossary name '"+n+"'");
    throw(runtime_error(msg));
    return *(static_cast<const UMATMaterialProperty*>(0));
  } // end of MFrontUMATInterfaceBase::findUMATMaterialProperty


  MFrontUMATInterfaceBase::MFrontUMATInterfaceBase()
  {} // end of MFrontUMATInterfaceBase::MFrontUMATInterfaceBase

  bool
  MFrontUMATInterfaceBase::isModellingHypothesisHandled(const Hypothesis h,
							const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    set<Hypothesis> ih(this->getModellingHypothesesToBeTreated(mb));
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return !mb.areAllMechanicalDataSpecialised(ih);
    }
    return ih.find(h)!=ih.end();
  }

  std::string
  MFrontUMATInterfaceBase::getSymbolName(const std::string& n,
					 const Hypothesis h) const
  {
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->getFunctionName(n)+"_"+ModellingHypothesis::toString(h);
    }
    return this->getFunctionName(n);
  } // end of MFrontUMATInterfaceBase::getLibraryName

  void
  MFrontUMATInterfaceBase::allowDynamicallyAllocatedArrays(const bool b)
  {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  } // end of MFrontUMATInterfaceBase::allowDynamicallyAllocatedArrays

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterfaceBase::getGlobalDependencies(const MechanicalBehaviourDescription&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontUMATInterfaceBase::getGlobalDependencies

  void
  MFrontUMATInterfaceBase::reset(void)
  {
    SupportedTypes::reset();
    this->generateMTestFile = false;
  } // end of MFrontUMATInterfaceBase::reset

  void
  MFrontUMATInterfaceBase::appendToMaterialPropertiesList(std::vector<UMATMaterialProperty>& l,
							  const std::string& t,
							  const std::string& n,
							  const std::string& v,
							  const bool b) const
  {
    using namespace std;
    const SupportedTypes::TypeFlag flag = this->getTypeFlag(t);
    if(flag!=SupportedTypes::Scalar){
      string msg("UMATMaterialProperty::UMATMaterialProperty : "
		 "material properties shall be scalars");
      throw(runtime_error(msg));
    }
    SupportedTypes::TypeSize o;
    if(!l.empty()){
      const UMATMaterialProperty& m = l.back();
      o  = m.offset;
      o += this->getTypeSize(t,1u);
    }
    l.push_back(UMATMaterialProperty(t,n,v,1u,o,b));
  } // end of MFrontUMATInterfaceBase::appendToMaterialPropertiesList

  void
  MFrontUMATInterfaceBase::completeMaterialPropertiesList(std::vector<UMATMaterialProperty>& mprops,
							  const MechanicalBehaviourDescription& mb,
							  const Hypothesis h) const
  {
    using namespace std;
    const MechanicalBehaviourData& d   = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& mp = d.getMaterialProperties();
    VariableDescriptionContainer::const_iterator p;
    for(p=mp.begin();p!=mp.end();++p){
      const string& n =  mb.getGlossaryName(h,p->name);
      vector<UMATMaterialProperty>::const_iterator pum;
      bool found = false;
      const SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      if(flag!=SupportedTypes::Scalar){
	string msg("UMATMaterialProperty::UMATMaterialProperty : "
		   "Invalid type for material property '"+p->name+"' ("+p->type+").\n"
		   "Material properties shall be scalars");
	throw(runtime_error(msg));
      }
      for(pum=mprops.begin();(pum!=mprops.end())&&(!found);++pum){
	if(!pum->dummy){
	  if(pum->name==n){
	    // type check
	    if(mb.useQt()){
	      if(p->type!=pum->type){
		string msg("MFrontUMATInterfaceBase::completeMaterialPropertiesList : "
			   "incompatible type for variable '"+n+
			   "' ('"+p->type+"' vs '"+pum->type+"')");
		throw(runtime_error(msg));
	      }
	    } else {
	      // don't use quantity
	      if(this->getTypeFlag(p->type)!=this->getTypeFlag(pum->type)){;
		string msg("MFrontUMATInterfaceBase::completeMaterialPropertiesList : "
			   "incompatible type for variable '"+n+
			   "' ('"+p->type+"' vs '"+pum->type+"')");
		throw(runtime_error(msg));
	      }
	      if(p->type!=pum->type){
		ostream& log = getLogStream();
		log << "MFrontUMATInterfaceBase::completeMaterialPropertiesList : "
		    << "inconsistent type for variable '" << n
		    << "' ('" << p->type << "' vs '" << pum->type << "')" << endl;
	      }
	    }
	    if(p->arraySize!=pum->arraySize){
	      string msg("MFrontUMATInterfaceBase::completeMaterialPropertiesList : "
			 "incompatible array size for variable '"+n+
			 "' ('"+p->type+"' vs '"+pum->type+"')");
	      throw(runtime_error(msg));
	    }
	    found = true;
	  }
	}
      }
      if(!found){
	SupportedTypes::TypeSize o;
	if(!mprops.empty()){
	  const UMATMaterialProperty& m = mprops.back();
	  o  = m.offset;
	  o += this->getTypeSize(m.type,m.arraySize);
	}
	mprops.push_back(UMATMaterialProperty(p->type,n,p->name,
					      p->arraySize,o,false));
      }
    }
  } // end of MFrontUMATInterfaceBase::completeMaterialPropertiesList

  void 
  MFrontUMATInterfaceBase::exportMechanicalData(std::ofstream& behaviourDataFile,
						const Hypothesis h,
						const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& persistentVarsHolder = d.getPersistentVariables();
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    if(!persistentVarsHolder.empty()){
      behaviourDataFile << "void\n"
			<< iprefix+"exportStateData("
			<< "Type * const "+iprefix+"stress_,Type * const "+iprefix+"statev) const\n";
    } else {
      behaviourDataFile << "void\n"
			<< iprefix+"exportStateData("
			<< "Type * const "+iprefix+"stress_,Type * const) const\n";
    }
    behaviourDataFile << "{\n";
    behaviourDataFile << "using namespace tfel::math;\n";
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize of;
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const ThermodynamicForce& f = pm->second;
      const SupportedTypes::TypeFlag flag = this->getTypeFlag(f.type);
      if(flag==SupportedTypes::Scalar){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "*("+iprefix+"stress_+" << of << ") = this->" << f.name << ";\n";
	} else {
	  behaviourDataFile << "*("+iprefix+"stress_) = this->" << f.name << ";\n";
	}
      } else if(flag==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "this->sig.exportTab("+iprefix+"stress_+" << of << ");\n";
	} else {
	  behaviourDataFile << "this->sig.exportTab("+iprefix+"stress_"");\n";
	}
      } else if((flag==SupportedTypes::TVector)||
		(flag==SupportedTypes::Tensor)){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "exportToBaseTypeArray(this->" << f.name << ","+iprefix+"stress_+"
			    << of << ");\n";	
	} else {
	  behaviourDataFile << "exportToBaseTypeArray(this->" << f.name << ","+iprefix+"stress_);\n";	
	}
      } else {
	string msg("MFrontUMATInterfaceBase::exportMechanicalData : ");
	msg += "unsupported forces type";
	throw(runtime_error(msg));
      }
      of += this->getTypeSize(f.type,1u);
    }
    if(!persistentVarsHolder.empty()){
      this->exportResults(behaviourDataFile,
			  persistentVarsHolder,
			  iprefix+"statev",
			  mb.useQt());
    }
    behaviourDataFile << "} // end of "+iprefix+"ExportStateData\n";
    behaviourDataFile << endl;
  }
  
  void
  MFrontUMATInterfaceBase::writeBehaviourConstructor(std::ofstream& behaviourFile,
						     const MechanicalBehaviourDescription& mb,
						     const std::string& initStateVarsIncrements)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    behaviourFile << "/*\n";
    behaviourFile << " * \\brief constructor for the umat interface\n";
    behaviourFile << " *\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dt_, time increment\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"T_, temperature\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dT_, temperature increment\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"mat, material properties\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"int_vars, state variables\n"; 
    behaviourFile << " * \\param const Type *const "+iprefix+"ext_vars, external state variables\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dext_vars,";
    behaviourFile << " external state variables increments\n";
    behaviourFile << " */\n";
    behaviourFile << mb.getClassName() 
		  << "(const Type* const "+iprefix+"dt_,\n" 
		  <<  "const Type* const "+iprefix+"T_,const Type* const "+iprefix+"dT_,\n"
		  <<  "const Type* const "+iprefix+"mat,const Type* const "+iprefix+"int_vars,\n"
		  <<  "const Type* const "+iprefix+"ext_vars,const Type* const "+iprefix+"dext_vars)\n";
    if(mb.useQt()){
      behaviourFile << ": " << mb.getClassName() 
		    << "BehaviourData<hypothesis,Type,use_qt>("+iprefix+"T_,"+iprefix+"mat,\n"
		    << iprefix+"int_vars,"+iprefix+"ext_vars),\n";
      behaviourFile << mb.getClassName() 
		    << "IntegrationData<hypothesis,Type,use_qt>("+iprefix+"dt_,"+iprefix+"dT_,"+iprefix+"dext_vars)";
    } else {
      behaviourFile << ": " << mb.getClassName() 
		    << "BehaviourData<hypothesis,Type,false>("+iprefix+"T_,"+iprefix+"mat,\n"
		    << iprefix+"int_vars,"+iprefix+"ext_vars),\n";
      behaviourFile << mb.getClassName() 
		    << "IntegrationData<hypothesis,Type,false>("+iprefix+"dt_,"+iprefix+"dT_,"+iprefix+"dext_vars)";
    }
    if(!initStateVarsIncrements.empty()){
      behaviourFile << ",\n" << initStateVarsIncrements;
    }
  }

  void
  MFrontUMATInterfaceBase::writeMaterialPropertiesInitializersInBehaviourDataConstructorI(std::ostream& f,
											  const Hypothesis h,
											  const MechanicalBehaviourDescription& mb,
											  const std::vector<UMATMaterialProperty>& i,
											  const SupportedTypes::TypeSize ioffset,
											  const std::string& src,
											  const std::string& prefix,
											  const std::string& suffix) const
  {
    using namespace std;
    const MechanicalBehaviourData&      d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& v = d.getMaterialProperties();
    VariableDescriptionContainer::const_iterator p;
    if(!v.empty()){
      for(p=v.begin();p!=v.end();++p){
	if(p->arraySize==1u){
	  const string n = prefix+p->name+suffix;
	  const UMATMaterialProperty& m = 
	    MFrontUMATInterfaceBase::findUMATMaterialProperty(i,mb.getGlossaryName(h,p->name));
	  SupportedTypes::TypeSize offset = m.offset;
	  offset -= ioffset;
	  f << ",\n";
	  SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	  if(flag==SupportedTypes::Scalar){
	    f << n << "("+src+"[" 
	      << offset << "])";  
	  } else if((flag==SupportedTypes::TVector)||
		    (flag==SupportedTypes::Stensor)||
		    (flag==SupportedTypes::Tensor)){
	    f << n << "(&"+src+"[" 
	      << offset << "])";  
	  } else {
	    string msg("SupportedTypes::");
	    msg += "writeVariableInitializersInBehaviourDataConstructorI : ";
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	}
      }
    }
    
  } // end of MFrontUMATInterfaceBase::writeVariableInitializersInBehaviourDataConstructorI

  void
  MFrontUMATInterfaceBase::writeMaterialPropertiesInitializersInBehaviourDataConstructorII(std::ostream& f,
											   const Hypothesis h,
											   const MechanicalBehaviourDescription& mb,
											   const std::vector<UMATMaterialProperty>& i,
											   const SupportedTypes::TypeSize ioffset,
											   const std::string& src,
											   const std::string& prefix,
											   const std::string& suffix) const
  {
    using namespace std;
    const MechanicalBehaviourData&      d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& v = d.getMaterialProperties();
    VariableDescriptionContainer::const_iterator p;
    if(!v.empty()){
      for(p=v.begin();p!=v.end();++p){
	if(p->arraySize!=1u){
	  const UMATMaterialProperty& m =
	    MFrontUMATInterfaceBase::findUMATMaterialProperty(i,mb.getGlossaryName(h,p->name));	  
	  const SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	  SupportedTypes::TypeSize offset = m.offset;
	  offset -= ioffset;
	  const string n = prefix+p->name+suffix;
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    f << n << ".resize(" << p->arraySize << ");" << endl;
	    f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	    switch(flag){
	    case SupportedTypes::Scalar : 
	      f << n << "[idx] = "+src+"[" 
		<< offset << "+idx];\n";  
	      break;
	    case SupportedTypes::TVector :
	      f << "tfel::fsalgo<TVectorSize>(&"+src+"[" 
		<< offset << "+idx*TVectorSize],"
		<< n << "[idx].begin());\n";  
	      break;
	    case SupportedTypes::Stensor :
	      f << n << "[idx].import(&"+src+"[" 
		  << offset << "+idx*StensorSize]);\n";  
	      break;
	    case SupportedTypes::Tensor :
	      f << "tfel::fsalgo<TensorSize>(&"+src+"[" 
		<< offset << "+idx*TensorSize],"
		<< n << "[idx].begin());\n";  
	      break;
	    default : 
	      string msg("SupportedTypes::");
	      msg += "writeVariableInitializersInBehaviourDataConstructorII : ";
	      msg += "internal error, tag unsupported";
	      throw(runtime_error(msg));
	    }
	    f << "}" << endl;
	  } else {
	    for(int index=0;index!=p->arraySize;++index){
	      switch(flag){
	      case SupportedTypes::Scalar : 
		f << n << "[" << index << "] = "+src+"[" 
		  << offset << "];\n";  
		break;
	      case SupportedTypes::TVector :
		f << "tfel::fsalgo<TVectorSize>(&"+src+"[" 
		  << offset << "]," << n << "[" << index << "].begin());\n";  
		break;
	      case SupportedTypes::Stensor :
		f << n << "["<< index << "].import(&"+src+"[" 
		  << offset << "]);\n";  
		break;
	      case SupportedTypes::Tensor :
		f << "tfel::fsalgo<TensorSize>(&"+src+"[" 
		  << offset << "]," << n << "[" << index << "].begin());\n";  
		break;
	      default : 
		string msg("SupportedTypes::");
		msg += "writeVariableInitializersInBehaviourDataConstructorII : ";
		msg += "internal error, tag unsupported";
		throw(runtime_error(msg));
	      }
	      offset+=this->getTypeSize(p->type,1u);
	    }
	  }
	}
      }
    }
  } // end of MFrontUMATInterfaceBase::writeVariableInitializersInBehaviourDataConstructorII
  
  void 
  MFrontUMATInterfaceBase::writeBehaviourDataConstructor(std::ofstream& behaviourDataFile,
							 const Hypothesis h,
							 const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,h);
    const VariableDescriptionContainer& mp                       = d.getMaterialProperties();
    const VariableDescriptionContainer& persistentVarsHolder     = d.getPersistentVariables();
    const VariableDescriptionContainer& externalStateVarsHolder  = d.getExternalStateVariables();
    behaviourDataFile << "/*\n";
    behaviourDataFile << " * \\brief constructor for the umat interface\n";
    behaviourDataFile << " *\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"T_, temperature\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"mat, material properties\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"int_vars, state variables\n"; 
    behaviourDataFile << " * \\param const Type *const "+iprefix+"ext_vars, external state variables\n";
    behaviourDataFile << " */\n";
    behaviourDataFile << mb.getClassName() << "BehaviourData"
		      << "(const Type* const "+iprefix+"T_,const Type* const";
    if(!mp.empty()){
      behaviourDataFile << " "+iprefix+"mat,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile <<  "const Type* const";
    if(!persistentVarsHolder.empty()){
      behaviourDataFile << " "+iprefix+"int_vars,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile << "const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourDataFile << " "+iprefix+"ext_vars";
    }
    behaviourDataFile << ")\n";
    behaviourDataFile << ": T(*"+iprefix+"T_)";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorI(behaviourDataFile,
									 h,mb,mprops.first,
									 mprops.second,
									 iprefix+"mat","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       persistentVarsHolder,
							       iprefix+"int_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       externalStateVarsHolder,
							       iprefix+"ext_vars","","");
    behaviourDataFile << "\n{\n";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorII(behaviourDataFile,
									  h,mb,mprops.first,
									  mprops.second,
									  iprefix+"mat","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								persistentVarsHolder,
								iprefix+"int_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								externalStateVarsHolder,
								iprefix+"ext_vars","","");
    behaviourDataFile << "}\n\n";
  }
  
  void 
  MFrontUMATInterfaceBase::writeIntegrationDataConstructor(std::ofstream& behaviourIntegrationFile,
							   const Hypothesis h,
							   const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    const VariableDescriptionContainer& externalStateVarsHolder  = d.getExternalStateVariables();
    behaviourIntegrationFile << "/*\n";
    behaviourIntegrationFile << " * \\brief constructor for the umat interface\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dt_, time increment\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dT_, temperature increment\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dext_vars,";
    behaviourIntegrationFile << " external state variables increments\n";
    behaviourIntegrationFile << " *\n";
    behaviourIntegrationFile << " */\n";
    behaviourIntegrationFile << mb.getClassName() << "IntegrationData"
			     << "(const Type* const "+iprefix+"dt_,\n" 
			     <<  "const Type* const "+iprefix+"dT_,const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourIntegrationFile << " "+iprefix+"dext_vars)\n";
    } else {
      behaviourIntegrationFile << ")\n";
    }
    behaviourIntegrationFile << ": dt(*"+iprefix+"dt_),dT(*"+iprefix+"dT_)";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(behaviourIntegrationFile,
								 externalStateVarsHolder,
								 iprefix+"dext_vars","d","");
    }
    behaviourIntegrationFile << "\n{\n";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorII(behaviourIntegrationFile,
								  externalStateVarsHolder,
								  iprefix+"dext_vars","d","");
    }
    behaviourIntegrationFile << "}\n\n";
  }

  void 
  MFrontUMATInterfaceBase::writeBehaviourDataMainVariablesSetters(std::ofstream& os,
								  const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    os << "void set"+iprefix+"BehaviourDataDrivingVariables(const Type* const "+iprefix+"stran)\n"
       << "{\n";
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const DrivingVariable& v = pm->first;
      if(!v.increment_known){
	if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"stran+"  << ov <<",this->" << v.name << "0.begin());\n";
	  } else {
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"stran,this->" << v.name << "0.begin());\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "this->" << v.name << "0.importVoigt("+iprefix+"stran+" << ov <<");\n";
	  } else {
	    os << "this->" << v.name << "0.importVoigt("+iprefix+"stran);\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << v.type << "::buildFromFortranMatrix(this->" << v.name << "0,"
			      << iprefix << "stran+" << ov <<");\n";
	  } else {
	    os << v.type << "::buildFromFortranMatrix(this->" << v.name << "0,"
			      << iprefix << "stran);\n";
	  }
	} else {
	  string msg("MFrontUMATInterfaceBase::writeBehaviourDataMainVariablesSetters : ");
	  msg += "unsupported driving variable type";
	  throw(runtime_error(msg));
	}
      } else {
	if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"stran+"  << ov << ",this->"  << v.name << ".begin());\n";
	  } else {
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"stran,this->"   << v.name << ".begin());\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "this->" << v.name << ".importVoigt("+iprefix+"stran+" << ov <<");\n";
	  } else {
	    os << "this->"  << v.name << ".importVoigt("+iprefix+"stran);\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << v.type << "::buildFromFortranMatrix(this->" << v.name << ","
			      << iprefix << "stran+" << ov <<");\n";
	  } else {
	    os << v.type << "::buildFromFortranMatrix(this->" << v.name << ","
			      << iprefix+"stran);\n";
	  }
	} else {
	  string msg("MFrontUMATInterfaceBase::writeBehaviourDataMainVariablesSetters : ");
	  msg += "unsupported driving variable type";
	  throw(runtime_error(msg));
	}
      }
      ov += this->getTypeSize(v.type,1u);
    }
    os << "}\n\n";
    os << "void set"+iprefix+"BehaviourDataThermodynamicForces(const Type* const "+iprefix+"stress_)\n"
      << "{\n";
    SupportedTypes::TypeSize of;
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const ThermodynamicForce& f = pm->second;
      if(this->getTypeFlag(f.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  os << "tfel::fsalgo::copy<N>::exe("+iprefix+"stress_+" << of <<",this->" << f.name << ".begin());\n";
	} else {
	  os << "tfel::fsalgo::copy<N>::exe("+iprefix+"stress_,this->" << f.name << ".begin());\n";
	}
      } else if(this->getTypeFlag(f.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  os << "this->" << f.name << ".importTab("+iprefix+"stress_+" << of <<");\n";
	} else {
	  os << "this->" << f.name << ".importTab("+iprefix+"stress_);\n";
	}
      } else if(this->getTypeFlag(f.type)==SupportedTypes::Tensor){
	if(pm!=mb.getMainVariables().begin()){
	  os << f.type << "::buildFromFortranMatrix(this->" << f.name << ","
			    << iprefix << "stress_+" << of <<");\n";
	} else {
	  os << f.type << "::buildFromFortranMatrix(this->" << f.name << ","
			    << iprefix << "stress_);\n";
	}
      } else {
	string msg("MFrontUMATInterfaceBase::writeBehaviourDataMainVariablesSetters : ");
	msg += "unsupported forces type";
	throw(runtime_error(msg));
      }
      of += this->getTypeSize(f.type,1u);
    }
    os << "}\n\n";
  }

  void 
  MFrontUMATInterfaceBase::writeIntegrationDataMainVariablesSetters(std::ofstream& os,
								    const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    os << "void set"+iprefix+"IntegrationDataDrivingVariables(const Type* const "+iprefix+"dstran)\n"
       << "{\n";
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const DrivingVariable& v = pm->first;
      if(!v.increment_known){
	if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"dstran+" << ov <<",this->" << v.name << "1.begin());\n";
	  } else {
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"dstran,this->" << v.name << "1.begin());\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "this->" << v.name << "1.importVoigt("+iprefix+"dstran+" << ov <<");\n";
	  } else {
	    os << "this->" << v.name << "1.importVoigt("+iprefix+"dstran);\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << v.type << "::buildFromFortranMatrix(this->" << v.name << "1,"
			      << iprefix << "dstran+" << ov <<");\n";
	  } else {
	    os << v.type << "::buildFromFortranMatrix(this->" << v.name << "1,"
			      << iprefix << "dstran);\n";
	  }
	} else {
	  string msg("MFrontUMATInterfaceBase::writeIntegrationDataMainVariablesSetters : ");
	  msg += "unsupported driving variable type";
	  throw(runtime_error(msg));
	}
      } else {
	if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"dstran+" << ov << ",this->d" << v.name << ".begin());\n";
	  } else {
	    os << "tfel::fsalgo::copy<N>::exe("+iprefix+"dstran,this->d" << v.name << ".begin());\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << "this->d" << v.name << ".importVoigt("+iprefix+"dstran+" << ov <<");\n";
	  } else {
	    os << "this->d" << v.name << ".importVoigt("+iprefix+"dstran);\n";
	  }
	} else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
	  if(pm!=mb.getMainVariables().begin()){
	    os << v.type << "::buildFromFortranMatrix(this->d" << v.name << ","
			      << iprefix << "dstran+" << ov <<");\n";
	  } else {
	    os << v.type << "::buildFromFortranMatrix(this->d" << v.name << ","
			      << iprefix+"dstran);\n";
	  }
	} else {
	  string msg("MFrontUMATInterfaceBase::writeIntegrationDataMainVariablesSetters : ");
	  msg += "unsupported driving variable type";
	  throw(runtime_error(msg));
	}
      }
      ov += this->getTypeSize(v.type,1u);
    }
    os << "}\n\n";
  }

  void
  MFrontUMATInterfaceBase::writeGlossaryNames(std::ostream& f,
					      const std::string& name,
					      const Hypothesis& h,
					      const std::vector<std::string>& n,
					      const std::string& array,
					      const unsigned short o) const
  {
    using namespace std;
    if(o>n.size()){
      string msg("MFrontUMATInterfaceBase::writeGlossaryNames : ");
      msg += "number of names given is lesser than the offset";
      throw(runtime_error(msg));
    }
    if(n.size()!=o){
      vector<string>::size_type s = 0u;
      vector<string>::const_iterator p = n.begin()+o;      
      f << "MFRONT_SHAREDOBJ const char * " << this->getSymbolName(name,h)
	<< "_" << array << "[" << n.size()-o <<  "] = {";
      while(p!=n.end()){
	f << '"' << *p << '"';
	if(++p!=n.end()){
	  if(s%5==0){
	    f << ",\n";
	  } else {
	    f << ",";
	  }
	}
	++s;
      }
      f << "};\n";
    } else {
      f << "MFRONT_SHAREDOBJ const char * const * "  << this->getSymbolName(name,h)
	<< "_" << array << " = 0;\n\n";
    }      
  } // end of MFrontUMATInterfaceBase::writeGlossaryNames

  bool
  MFrontUMATInterfaceBase::readBooleanValue(const std::string& key,
					    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator& current,
					    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end) const
  {
    using namespace std;
    bool b = true;
    if(current==end){
      string msg("MFrontUmatInterfaceBase::readBooleanValue ("+key+") : ");
      msg += "unexpected end of file";
      throw(runtime_error(msg));
    }
    if(current->value=="true"){
      b = true;
    } else if(current->value=="false"){
      b = false;
    } else {
      string msg("MFrontUmatInterfaceBase::readBooleanValue ("+key+") :");
      msg += "expected 'true' or 'false'";
      throw(runtime_error(msg));
    }
    ++(current); 
    if(current==end){
      string msg("MFrontUmatInterfaceBase::readBooleanValue ("+key+") : ");
      msg += "unexpected end of file";
      throw(runtime_error(msg));
    }    
    if(current->value!=";"){
      string msg("MFrontUmatInterfaceBase::readBooleanValue : expected ';', read ");
      msg += current->value;
      throw(runtime_error(msg));
    }
    ++(current);
    return b;
  }

  void
  MFrontUMATInterfaceBase::writeVisibilityDefines(std::ostream& out) const
  {
    out << "#ifdef WIN32\n";
    out << "#include <windows.h>\n";
    out << "#ifndef MFRONT_STDCALL\n";
    out << "#define MFRONT_STDCALL __stdcall\n"; 
    out << "#endif /* MFRONT_STDCALL */\n"; 
    out << "#else\n";
    out << "#ifndef MFRONT_STDCALL\n";
    out << "#define MFRONT_STDCALL\n"; 
    out << "#endif /* MFRONT_STDCALL */\n"; 
    out << "#endif /* WIN32 */\n\n";
    out << "#ifndef MFRONT_SHAREDOBJ\n";
    out << "#define MFRONT_SHAREDOBJ TFEL_VISIBILITY_EXPORT\n"; 
    out << "#endif /* MFRONT_SHAREDOBJ */\n\n"; 
  } // end of MFrontUMATInterfaceBase::writeVisibilityDefines

  void
  MFrontUMATInterfaceBase::checkParametersType(bool& rp,
					       bool& ip,
					       bool& up,
					       const VariableDescriptionContainer& pc) const
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator pp;
    rp = false;
    ip = false;
    up = false;
    for(pp=pc.begin();pp!=pc.end();++pp){
      if(pp->type=="real"){
	rp = true;
      } else if(pp->type=="int"){
	ip = true;
      } else if(pp->type=="ushort"){
	up = true;
      } else {
	string msg("MFrontUMATInterfaceBase::checkParametersType : ");
	msg += "unsupport parameter type '"+pp->type+"'.\n";
	throw(runtime_error(msg));
      } 
    }
  }
  
  void
  MFrontUMATInterfaceBase::writeSetParametersFunctionsDeclarations(std::ostream& out,
								   const std::string& name,
								   const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    set<Hypothesis> h  = mb.getDistinctModellingHypotheses();
    set<Hypothesis> h2 = this->getModellingHypothesesToBeTreated(mb);
    set<Hypothesis>::const_iterator p;
    for(p=h.begin();p!=h.end();++p){
      if((*p==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	 (h2.find(*p)!=h2.end())){
	bool rp,ip,up;
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(*p);
	const VariableDescriptionContainer& pc = d.getParameters();
	this->checkParametersType(rp,ip,up,pc);
	string suffix;
	if(*p!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	  suffix += ModellingHypothesis::toString(*p);
	}
	if(rp){
	  out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	      << this->getFunctionName(name) << suffix
	      << "_setParameter(const char *const,const double);\n\n";
	}
	if(ip){
	  out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	      << this->getFunctionName(name) << suffix
	      << "_setIntegerParameter(const char *const,const int);\n\n";
	}
	if(up){
	  out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	      << this->getFunctionName(name) << suffix
	      << "_setUnsignedShortParameter(const char *const,const unsigned short);\n\n";
	}
      }
    }
  } // end of MFrontUMATInterfaceBase::writeSetParametersFunctionsDeclarations

  void
  MFrontUMATInterfaceBase::writeSetParametersFunctionsImplementations(std::ostream& out,
								      const std::string& name,
								      const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    set<Hypothesis> h  = mb.getDistinctModellingHypotheses();
    set<Hypothesis> h2 = this->getModellingHypothesesToBeTreated(mb);
    set<Hypothesis>::const_iterator p;
    for(p=h.begin();p!=h.end();++p){
      if((*p==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	 (h2.find(*p)!=h2.end())){
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(*p);
	const VariableDescriptionContainer& pc = d.getParameters();
	bool rp,ip,up;
	this->checkParametersType(rp,ip,up,pc);
	string suffix;
	if(*p!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	  suffix += ModellingHypothesis::toString(*p);
	}
	string cname = mb.getClassName() + suffix + "ParametersInitializer";
	if(rp){
	  out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	      << this->getFunctionName(name) << suffix
	      << "_setParameter(const char *const key,const double value){\n"
	      << "using namespace std;\n"
	      << "using namespace tfel::material;\n"
	      << cname << "& i = " << cname << "::get();\n"
	      << "try{\n"
	      << "i.set(key,value);\n"
	      << "} catch(runtime_error& e){\n"
	      << "cerr << e.what() << endl;\n"
	      << "return 0;\n"
	      << "}\n"
	      << "return 1;\n"
	      << "}\n\n";
	}
	if(ip){
	  out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	      << this->getFunctionName(name) << suffix
	      << "_setIntegerParameter(const char *const key,const int value){\n"
	      << "using namespace std;\n"
	      << "using namespace tfel::material;\n"
	      << cname << "& i = " << cname << "::get();\n"
	      << "try{\n"
	      << "i.set(key,value);\n"
	      << "} catch(runtime_error& e){\n"
	      << "cerr << e.what() << endl;\n"
	      << "return 0;\n"
	      << "}\n"
	      << "return 1;\n"
	      << "}\n\n";
	}
	if(up){
	  out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	      << this->getFunctionName(name) << suffix
	      << "_setUnsignedShortParameter(const char *const key,const unsigned short value){\n"
	      << "using namespace std;\n"
	      << "using namespace tfel::material;\n"
	      << cname << "& i = " << cname << "::get();\n"
	      << "try{\n"
	      << "i.set(key,value);\n"
	      << "} catch(runtime_error& e){\n"
	      << "cerr << e.what() << endl;\n"
	      << "return 0;\n"
	      << "}\n"
	      << "return 1;\n"
	      << "}\n\n";
	}
      }
    }
  }

  std::string
  MFrontUMATInterfaceBase::getHeaderDefine(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const string & m = mb.getMaterialName();
    string header = "_LIB_"+makeUpperCase(this->getInterfaceName());
    if(!mb.getLibrary().empty()){
      header += "_";
      header += makeUpperCase(mb.getLibrary());
    }
    if(!m.empty()){
      header += "_";
      header += makeUpperCase(m);
    }
    header += "_";
    header += makeUpperCase(mb.getClassName());
    header += "_HXX";
    return header;
  }

  void
  MFrontUMATInterfaceBase::getExtraSrcIncludes(std::ostream& out,
					       const MechanicalBehaviourDescription& mb) const
  {
    if(mb.hasParameters()){
      out << "#include<iostream>\n";
      out << "#include<stdexcept>\n";
    }
    if(this->generateMTestFile){
      out << "#include<vector>\n";
    }
    if(this->generateMTestFile){
      out << "#include<sstream>\n";
    }
    if(this->generateMTestFile){
      out << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n";
      out << "#include\"MFront/SupportedTypes.hxx\"\n";
      out << "#include\"MFront/UMATSmallStrainMTestFileGenerator.hxx\"\n";
      out << "#include\"MFront/UMATFiniteStrainMTestFileGenerator.hxx\"\n";
    }
  } // end of MFrontUMATInterfaceBase::getExtraSrcIncludes
  
  void
  MFrontUMATInterfaceBase::generateMTestFile1(std::ostream& out) const
  {
    if(this->generateMTestFile){
      out << "using namespace std;\n";
      out << "using tfel::material::ModellingHypothesis;\n";
      out << "using mfront::SupportedTypes;\n";
      out << "// double is used by MTestFileGeneratorBase\n";
      out << "vector<double> mg_STRESS(*NTENS);\n";
      out << "vector<double> mg_NSTATV(*NSTATV);\n";
      out << "copy(STRESS,STRESS+*NTENS,mg_STRESS.begin());\n";
      out << "copy(STATEV,STATEV+*NSTATV,mg_NSTATV.begin());\n";
    }
  } // end of MFrontUMATInterfaceBase::generateMTestFile1

  void
  MFrontUMATInterfaceBase::generateMTestFile2(std::ostream& out,
					      const MechanicalBehaviourDescription::BehaviourType type,
					      const std::string& name,
					      const std::string& suffix,
					      const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(this->generateMTestFile){
      set<Hypothesis> h = mb.getDistinctModellingHypotheses();
      string fname = name;
      if(!suffix.empty()){
    	fname += "_"+suffix;
      }
      if(type==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
    	out << "mfront::UMATSmallStrainMTestFileGenerator mg(\""
    	    << makeLowerCase(this->getInterfaceName()) << "\",\""
    	    << this->getLibraryName(mb) <<"\",\"" << this->getFunctionName(fname)
    	    <<  "\");\n";
      } else if(type==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
    	out << "mfront::UMATFiniteStrainMTestFileGenerator mg(\""
    	    << makeLowerCase(this->getInterfaceName()) << "\",\""
    	    << this->getLibraryName(mb) <<"\",\"" << this->getFunctionName(fname)
    	    <<  "\");\n";
      } else {
    	string msg("MFrontUMATInterfaceBase::generateMTestFile2 : ");
    	msg += "only small strain or finite strain behaviours are supported";
    	throw(runtime_error(msg));
      }
      this->writeMTestFileGeneratorSetModellingHypothesis(out);
      this->writeMTestFileGeneratorSetRotationMatrix(out,mb);
      out << "const unsigned short TVectorSize = mg.getTVectorSize();\n";
      out << "const unsigned short StensorSize = mg.getStensorSize();\n";
      out << "const unsigned short TensorSize  = mg.getTensorSize();\n";
      out << "mg.setHandleThermalExpansion(false);\n";
      out << "mg.addTime(0.);\n";
      out << "mg.addTime(*DTIME);\n";
      if(type==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
    	out << "mg.setStrainTensor(STRAN);\n";
    	out << "mg.setStrainTensorIncrement(DSTRAN);\n";
    	out << "mg.setStressTensor(&mg_STRESS[0]);\n";
      } else if(type==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
    	out << "mg.setDeformationGradientAtTheBeginningOfTheStimeStep(F0);\n";
    	out << "mg.setDeformationGradientAtTheEndOfTheStimeStep(F1);\n";
    	out << "mg.setStressTensor(&mg_STRESS[0]);\n";
      } else {
    	string msg("MFrontUMATInterfaceBase::generateMTestFile2 : ");
    	msg += "only small strain or finite strain behaviours are supported";
    	throw(runtime_error(msg));
      }
      const map<Hypothesis,string>& gh = this->gatherModellingHypothesesAndTests(mb);
      for(map<Hypothesis,string>::const_iterator ph=gh.begin();ph!=gh.end();++ph){
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(ph->first);
	const VariableDescriptionContainer& persistentVarsHolder    = d.getPersistentVariables();
	const VariableDescriptionContainer& externalStateVarsHolder = d.getExternalStateVariables();
	pair<vector<UMATMaterialProperty>,
	     SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,ph->first);
	VariableDescriptionContainer::const_iterator p;
	unsigned short i;
	unsigned int offset;
	out << "if(" << ph->second << "){" << endl;
	offset=0;
	for(vector<UMATMaterialProperty>::const_iterator pm=mprops.first.begin();
	    pm!=mprops.first.end();++pm){
	  SupportedTypes::TypeFlag flag = this->getTypeFlag(pm->type);
	  if(flag!=SupportedTypes::Scalar){
	    string msg("MFrontUMATInterfaceBase::generateFile2 : "
		       "unsupported external state variable type "
		       "in mtest file generation");
	    throw(runtime_error(msg));
	  }
	  if(pm->arraySize==1u){
	    if(offset==0){
	      out << "mg.addMaterialProperty(\"" << pm->name << "\",*(PROPS));\n";	    
	    } else {
	      out << "mg.addMaterialProperty(\"" << pm->name << "\",*(PROPS+" << offset << "));\n";	    
	    }
	    ++offset;
	  } else {
	    for(unsigned short s=0;s!=pm->arraySize;++s,++offset){
	      if(offset==0){
		out << "mg.addMaterialProperty(\"" << pm->name << "[" << s << "]\",*(PROPS));\n";	    
	      } else {
		out << "mg.addMaterialProperty(\"" << pm->name << "[" << s << "]\","
		    << "*(PROPS+" << offset << "));\n";	    
	      }
	    }
	  }
	}
	SupportedTypes::TypeSize ivoffset;
	for(p=persistentVarsHolder.begin();p!=persistentVarsHolder.end();++p){
	  SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	  const string& ivname = d.getGlossaryName(p->name);
	  if(p->arraySize==1u){
	    if(ivoffset.isNull()){
	      if(flag==SupportedTypes::Scalar){
		out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Scalar,STATEV);\n";
		ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
	      } else {
		ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Stensor,STATEV);\n";
	      }
	    } else {
	      if(flag==SupportedTypes::Scalar){
		out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Scalar,STATEV+" << ivoffset<< ");\n";
		ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
	      } else {
		out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Stensor,STATEV+" << ivoffset<< ");\n";
		ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
	      }
	    }
	  } else {
	    if(p->arraySize>=SupportedTypes::ArraySizeLimit){
	      out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
	      out << "ostringstream name;\n";
	      out << "name << \"" << ivname << "[\" << i << \"]\";\n";
	      if(ivoffset.isNull()){
		if(flag==SupportedTypes::Scalar){
		  out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Scalar,STATEV+i);\n";
		} else {
		  out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Stensor,STATEV+i);\n";
		}
	      } else {
		if(flag==SupportedTypes::Scalar){
		  out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Scalar,STATEV+" << ivoffset<< "+i);\n";
		} else {
		  out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Stensor,STATEV+" << ivoffset<< "+i);\n";
		}
	      }
	      out << "}\n";
	      if(flag==SupportedTypes::Scalar){
		ivoffset += SupportedTypes::TypeSize(p->arraySize,0u,0u,0u);
	      } else {
		ivoffset += SupportedTypes::TypeSize(0u,0u,p->arraySize,0u);
	      }
	    } else {
	      for(i=0;i!=p->arraySize;++i){
		if(ivoffset.isNull()){
		  if(flag==SupportedTypes::Scalar){
		    out << "mg.addInternalStateVariable(\"" << ivname
			<< "[" << i << "]\",SupportedTypes::Scalar,STATEV);\n";
		    ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
		  } else {
		    out << "mg.addInternalStateVariable(\"" << ivname
			<< "[" << i << "]\",SupportedTypes::Stensor,STATEV);\n";
		    ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		  }
		} else {
		  if(flag==SupportedTypes::Scalar){
		    out << "mg.addInternalStateVariable(\""
			<< ivname << "[" << i << "]\",SupportedTypes::Scalar,STATEV+" << ivoffset<< ");\n";
		    ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
		  } else {
		    out << "mg.addInternalStateVariable(\""
			<< ivname << "[" << i << "]\",SupportedTypes::Stensor,STATEV+" << ivoffset<< ");\n";
		    ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		  }
		}
	      }
	    }
	  }
	}
	out << "mg.addExternalStateVariableValue(\"Temperature\",0.,*TEMP);\n";
	out << "mg.addExternalStateVariableValue(\"Temperature\",*DTIME,*TEMP+*DTEMP);\n";
	for(p=externalStateVarsHolder.begin(),offset=0;p!=externalStateVarsHolder.end();++p){
	  SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	  if(flag!=SupportedTypes::Scalar){
	    string msg("MFrontUMATInterfaceBase::generateFile2 : "
		       "unsupported external state variable type "
		       "in mtest file generation");
	    throw(runtime_error(msg));
	  }
	  const string& evname = d.getGlossaryName(p->name);
	  if(p->arraySize==1u){
	    if(offset==0){
	      out << "mg.addExternalStateVariableValue(\"" << evname 
		  << "\",0,*PREDEF);\n";
	      out << "mg.addExternalStateVariableValue(\"" << evname 
		  << "\",*DTIME,*PREDEF+*DPRED);\n";
	    } else {
	      out << "mg.addExternalStateVariableValue(\"" << evname 
		  << "\",0,*(PREDEF+" << offset<< "));\n";
	      out << "mg.addExternalStateVariableValue(\"" << evname 
		  << "\",*DTIME,*(PREDEF+" << offset<< ")+*(DPRED+" << offset<< "));\n";
	    }
	    ++offset;
	  } else {
	    if(p->arraySize>=SupportedTypes::ArraySizeLimit){
	      out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
	      out << "ostringstream name;\n";
	      out << "name << \"" << evname << "[\" << i << \"]\";\n";
	      if(offset==0){
		out << "mg.addExternalStateVariableValue(name.str(),0,*(PREDEF+i));\n";
		out << "mg.addExternalStateVariableValue(name.str(),"
		  "*DTIME,*(PREDEF+i)+*(DPRED+i));\n";
	      } else {
		out << "mg.addExternalStateVariableValue(name.str(),"
		  "0,*(PREDEF+" << offset<< "+i));\n";
		out << "mg.addExternalStateVariableValue(name.str(),"
		  "*DTIME,*(PREDEF+" << offset << "+i)+*(DPRED+" << offset<< "+i));\n";
	      }
	      out << "}\n";
	      offset += p->arraySize;
	    } else {
	      for(i=0;i!=p->arraySize;++i,++offset){
		if(offset==0){
		  out << "mg.addExternalStateVariableValue(\"" << evname
		      << "[" << i << "]\",0,*PREDEF);\n";
		  out << "mg.addExternalStateVariableValue(\"" << evname
		      << "[" << i << "]\",*DTIME,*PREDEF+*DPRED);\n";
		} else {
		  out << "mg.addExternalStateVariableValue(\""
		      << evname << "[" << i << "]\","
		    "0,*(PREDEF+" << offset<< "));\n";
		  out << "mg.addExternalStateVariableValue(\""
		      << evname << "[" << i << "]\","
		    "*DTIME,*(PREDEF+" << offset<< ")+*(DPRED+" << offset<< "));\n";
		}
	      }
	    }
	  }
	}
	out << "}" << endl;
      }
      out << "mg.generate(\""+name+"\");\n";
      out << "static_cast<void>(TVectorSize); // remove gcc warning\n";
      out << "static_cast<void>(StensorSize); // remove gcc warning\n";
      out << "static_cast<void>(TensorSize);  // remove gcc warning\n";
    }
  }

  void
  MFrontUMATInterfaceBase::writeMTestFileGeneratorSetRotationMatrix(std::ostream& out,
								    const MechanicalBehaviourDescription& mb) const
  {
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "mg.setRotationMatrix("
	  << "DROT[0],DROT[3],DROT[6],"
	  << "DROT[1],DROT[4],DROT[7],"
	  << "DROT[2],DROT[5],DROT[8]);\n";
    }
  } // end of MFrontUMATInterfaceBase::writeMTestFileGeneratorSetRotationMatrix

  void
  MFrontUMATInterfaceBase::generateUMATxxGeneralSymbols(std::ostream& out,
							const std::string& name,
							const MechanicalBehaviourDescription& mb,
							const MFrontFileDescription & fd) const
  {
    this->writeUMATxxSourceFileSymbols(out,name,mb,fd);
    this->writeUMATxxBehaviourTypeSymbols(out,name,mb);
    this->writeUMATxxSymmetryTypeSymbols(out,name,mb);
    this->writeUMATxxElasticSymmetryTypeSymbols(out,name,mb);
    this->writeUMATxxSpecificSymbols(out,name,mb,fd);
  }

  void
  MFrontUMATInterfaceBase::generateUMATxxSymbols(std::ostream& out,
						 const std::string& name,
						 const Hypothesis h,
						 const MechanicalBehaviourDescription& mb,
						 const MFrontFileDescription & fd) const
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
    }
    this->writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(out,name,h,mb);
    this->writeUMATxxMaterialPropertiesSymbols(out,name,h,mb);
    this->writeUMATxxStateVariablesSymbols(out,name,h,mb);
    this->writeUMATxxExternalStateVariablesSymbols(out,name,h,mb);
    this->writeUMATxxRequirementsSymbols(out,name,h,mb);
    this->writeUMATxxAdditionalSymbols(out,name,h,mb,fd);
  }

  void
  MFrontUMATInterfaceBase::writeUMATxxSpecificSymbols(std::ostream&,
						      const std::string&,
						      const MechanicalBehaviourDescription&,
						      const MFrontFileDescription&) const
  {} // end of MFrontUMATInterfaceBase::writeUMATxxSpecificSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxBehaviourTypeSymbols(std::ostream& out,
							   const std::string& name,
							   const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourType = " ;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::GENERALBEHAVIOUR){
      out << "0u;" << endl << endl;
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "1u;" << endl << endl;
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "2u;" << endl << endl;
    } else if(mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL){
      out << "3u;" << endl << endl;
    } else {
      string msg("MFrontUMATInterfaceBase::writeUMATxxBehaviourTypeSymbols : ");
      msg += "unsupported behaviour type.\n";
      throw(runtime_error(msg));
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxBehaviourTypeSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxMaterialPropertiesSymbols(std::ostream& out,
								const std::string& name,
								const Hypothesis h,
								const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,h);
    if(mprops.first.empty()){
      out << "MFRONT_SHAREDOBJ unsigned short "  << this->getSymbolName(name,h)
	  << "_nMaterialProperties = 0u;\n\n";
      out << "MFRONT_SHAREDOBJ const char * const *"  << this->getSymbolName(name,h)
	  << "_MaterialProperties = 0;\n\n";
    } else {
      const UMATMaterialProperty& last = mprops.first.back();
      SupportedTypes::TypeSize s;
      if((mprops.second.getTensorSize()!=0)||(mprops.second.getStensorSize()!=0)||
	 (mprops.second.getTVectorSize()!=0)){
	string msg("MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbols : "
		   "internal error : the material properties shall all be scalars");
	throw(runtime_error(msg));
      }
      s  = last.offset;
      s += this->getTypeSize(last.type,last.arraySize);
      s -= mprops.second;
      if((s.getTensorSize()!=0)||(s.getStensorSize()!=0)||(s.getTVectorSize()!=0)){
	string msg("MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbols : "
		   "internal error : the material properties shall all be scalars");
	throw(runtime_error(msg));
      }
      if(s.getScalarSize()<0){
	string msg("MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbols : "
		   "internal error : negative number of the material properties");
	throw(runtime_error(msg));
      }
      vector<UMATMaterialProperty>::size_type ib=0; /* index of the first element which
						     * is not imposed by the material properties */
      bool found = false;
      for(vector<UMATMaterialProperty>::size_type i=0;(i!=mprops.first.size())&&(!found);++i){
	if(mprops.first[i].offset==mprops.second){
	  ib = i;
	  found = true;
	}
      }
      if(!found){
	if(s.getScalarSize()!=0){
	  string msg("MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbols : "
		     "internal error : inconsistent offset declaration");
	  throw(runtime_error(msg));
	}
	out << "MFRONT_SHAREDOBJ unsigned short "  << this->getSymbolName(name,h)
	    << "_nMaterialProperties = 0u;\n\n";
	out << "MFRONT_SHAREDOBJ const char * const *"  << this->getSymbolName(name,h)
	    << "_MaterialProperties = 0;\n\n";
      } else {
	out << "MFRONT_SHAREDOBJ unsigned short "  << this->getSymbolName(name,h)
	    << "_nMaterialProperties = " << s.getScalarSize() << "u;\n\n";
	out << "MFRONT_SHAREDOBJ const char *"  << this->getSymbolName(name,h)
	    << "_MaterialProperties[" <<  s.getScalarSize() << "u] = {";
        for(vector<UMATMaterialProperty>::size_type i=ib;i!=mprops.first.size();){
	  const UMATMaterialProperty& m = mprops.first[i];
	  if(m.arraySize==1u){
	    out << "\"" << m.name << "\"";
	  } else {
	    for(unsigned short j=0;j!=m.arraySize;){
	      out << "\"" << m.name << "[" << j << "]\"";
	      if(++j!=m.arraySize){
		out << ",\n";
	      }
	    }
	  }
	  if(++i!=mprops.first.size()){
	    out << ",\n";
	  }
	}
	out << "};" << endl << endl;
      }
    }
  } // end of MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbol


  void
  MFrontUMATInterfaceBase::writeUMATxxStateVariablesSymbols(std::ostream& out,
  							    const std::string& name,
  							    const Hypothesis h,
  							    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& persistentVarsHolder = d.getPersistentVariables();
    const unsigned short nStateVariables = static_cast<unsigned short>(this->getNumberOfVariables(persistentVarsHolder));
    VariableDescriptionContainer::const_iterator p;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h)
  	<< "_nInternalStateVariables = " << nStateVariables
  	<< ";\n";
    vector<string> stateVariablesNames;
    mb.getGlossaryNames(stateVariablesNames,h,persistentVarsHolder);
    this->writeGlossaryNames(out,name,h,stateVariablesNames,"InternalStateVariables");
    if(!persistentVarsHolder.empty()){
      out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(name,h)
  	  << "_InternalStateVariablesTypes [] = {";
      for(p=persistentVarsHolder.begin();p!=persistentVarsHolder.end();){
  	const SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
  	for(unsigned short is=0;is!=p->arraySize;){
  	  switch(flag){
  	  case SupportedTypes::Scalar : 
  	    out << 0;
  	    break;
  	  case SupportedTypes::Stensor :
  	    out << 1;
  	    break;
  	  case SupportedTypes::TVector :
  	    out << 2;
  	    break;
  	  case SupportedTypes::Tensor :
  	    out << 3;
  	    break;
  	  default :
  	    string msg("MFrontUMATInterfaceBase::writeUMATxxStateVariablesSymbols : ");
  	    msg += "internal error, tag unsupported for variable '"+p->name+"'";
  	    throw(runtime_error(msg));
  	  }
  	  if(++is!=p->arraySize){
  	    out << ",";
  	  }
  	}
  	if(++p!=persistentVarsHolder.end()){
  	  out << ",";
  	}
      }
      out << "};\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ const int * " << this->getSymbolName(name,h)
  	  << "_InternalStateVariablesTypes = 0;\n\n";
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxStateVariablesSymbols
  
  void
  MFrontUMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols(std::ostream& out,
  								    const std::string& name,
  								    const Hypothesis h,
  								    const MechanicalBehaviourDescription& mb) const
  {
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& externalStateVarsHolder  = d.getExternalStateVariables();
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h)
  	<< "_nExternalStateVariables = " << this->getNumberOfVariables(externalStateVarsHolder) << ";\n";
    this->writeGlossaryNames(out,name,h,mb.getGlossaryNames(h,externalStateVarsHolder),
  			     "ExternalStateVariables",0u);
  } // end of MFrontUMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxRequirementsSymbols(std::ostream& out,
							  const std::string& name,
							  const Hypothesis h,
							  const MechanicalBehaviourDescription& mb) const
  {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h);
    out << "_requiresStiffnessTensor = ";
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h);
    out << "_requiresThermalExpansionCoefficientTensor = ";
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";

  } // end of MFrontUMATInterfaceBase::writeUMATxxRequirementsSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(std::ostream& out,
  										const std::string& name,
										const Hypothesis h,
  										const MechanicalBehaviourDescription& mb) const
  {
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name,h)
  	<< "_UsableInPurelyImplicitResolution = ";
    if(d.isUsableInPurelyImplicitResolution()){
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxIsUsableInPurelyImplicitResolution

  void
  MFrontUMATInterfaceBase::writeUMATxxSymmetryTypeSymbols(std::ostream& out,
  						    const std::string& name,
  						    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
  	<< "_SymmetryType = " ;
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "1u;" << endl << endl;
    } else {
      string msg("MFrontUMATInterfaceBase::writeUMATxxSymmetryTypeSymbols : ");
      msg += "unsupported behaviour type.\n";
      msg += "only isotropic or orthotropic behaviours are supported at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxSymmetryTypeSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols(std::ostream& out,
  								  const std::string& name,
  								  const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
  	<< "_ElasticSymmetryType = " ;
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else if(mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "1u;" << endl << endl;
    } else {
      string msg("MFrontUMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols : ");
      msg += "unsupported behaviour type.\n";
      msg += "only isotropic or orthotropic behaviours are supported at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxSourceFileSymbols(std::ostream& out,
  							const std::string& name,
  							const MechanicalBehaviourDescription&,
							const mfront::MFrontFileDescription& fd) const
  {
    using namespace tfel::system;
    using namespace tfel::utilities;
    out << "MFRONT_SHAREDOBJ const char *\n" 
  	<< this->getFunctionName(name) << "_src = \""
  	<< tokenize(fd.fileName,dirSeparator()).back()
  	<< "\";\n\n";
  }

  std::map<MFrontUMATInterfaceBase::Hypothesis,std::string>
  MFrontUMATInterfaceBase::gatherModellingHypothesesAndTests(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    typedef map<Hypothesis,string>::value_type MVType;
    map<Hypothesis,string> res;
    set<Hypothesis> h = this->getModellingHypothesesToBeTreated(mb);
    set<Hypothesis> h1;
    set<Hypothesis> h2;
    for(set<Hypothesis>::const_iterator p=h.begin();p!=h.end();++p){
      if(!mb.hasSpecialisedMechanicalData(*p)){
	h1.insert(*p);
      } else {
	h2.insert(*p);
      }
    }
    if(!h1.empty()){
      if(h1.size()==1u){
	res.insert(MVType(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			  this->getModellingHypothesisTest(*(h1.begin()))));
      } else {
	set<Hypothesis>::const_iterator p = h1.begin();
	string r = "("+this->getModellingHypothesisTest(*(h1.begin()))+")";
	++p;
	for(;p!=h1.end();++p){
	  r += "||("+this->getModellingHypothesisTest(*p)+")";
	}
	res.insert(MVType(ModellingHypothesis::UNDEFINEDHYPOTHESIS,r));
      }
    }
    for(set<Hypothesis>::const_iterator p=h2.begin();p!=h2.end();++p){
      res.insert(MVType(*p,this->getModellingHypothesisTest(*p)));
    }
    return res;
  } // end of MFrontUMATInterface::gatherModellingHypothesesAndTests

  MFrontUMATInterfaceBase::~MFrontUMATInterfaceBase()
  {}

} // end of namespace mfront
