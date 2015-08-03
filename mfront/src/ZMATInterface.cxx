/*! 
 * \file  mfront/src/ZMATInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>

#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/System/System.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/ZMATInterface.hxx"

namespace mfront
{

  static void
  writeZMATUndefs(std::ostream& out)
  {
    using std::endl;
    out << "#include\"MFront/ZMAT/ZMATUndefs.hxx\"" << endl;
  } // end of writeZMATUndefs

  template<typename ArrayType>
  static void
  writeArray(std::ostream& out,
	     const ArrayType& a)
  {
    using namespace std;
    std::string buffer;
    out << "{";
    for(typename ArrayType::const_iterator pn=a.begin();
	pn!=a.end();){
      if(buffer.size()+pn->size()+3>15){
	out << buffer << endl;
	buffer.clear();
      }
      buffer += "\"" + *pn + "\"";
      if(++pn!=a.end()){
	buffer += ",";
      }
    }
    if(!buffer.empty()){
      out << buffer;
    }
    out << "};" << endl;
  }

  static std::set<std::string>
  getAllMaterialPropertiesNames(const BehaviourDescription& mb)
  {
    using namespace std;
    typedef ZMATInterface::ModellingHypothesis ModellingHypothesis;
    typedef ZMATInterface::Hypothesis Hypothesis;
    set<string> mp_names;
    const Hypothesis hypotheses[3u] = {ModellingHypothesis::TRIDIMENSIONAL,
				       ModellingHypothesis::GENERALISEDPLANESTRAIN,
				       ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	const auto& d = mb.getBehaviourData(h);
	const auto& mps = d.getMaterialProperties();
	for(const auto & v : mps){
	  
	  const auto& name = d.getExternalName(v.name);
	  if(v.arraySize==1u){
	    mp_names.insert(name);
	  } else {
	    for(unsigned short n=0;n!=v.arraySize;++n){
	      ostringstream str;
	      str << name << '[' << n << ']';
	      mp_names.insert(str.str());
	    }
	  }
	}
      }
    }
    return mp_names;
  }

  static VariableDescriptionContainer
  getAllStateVariables(const BehaviourDescription& mb)
  {
    using namespace std;
    typedef ZMATInterface::ModellingHypothesis ModellingHypothesis;
    typedef ZMATInterface::Hypothesis Hypothesis;
    VariableDescriptionContainer s;
    const Hypothesis hypotheses[3u] = {ModellingHypothesis::TRIDIMENSIONAL,
				       ModellingHypothesis::GENERALISEDPLANESTRAIN,
				       ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	const auto& d = mb.getBehaviourData(h);
	const auto& svs = d.getPersistentVariables();
	for(const auto & sv : svs){
	  if(s.contains(sv.name)){
	    const auto& v = s.getVariable(sv.name);
	    if((sv.type!=v.type)||(sv.arraySize!=v.arraySize)){
	      string msg("getAllStateVariables : "
			 "inconsistent type for variable '"+sv.name+"'");
	      throw(runtime_error(msg));
	    }
	  } else {
	    s.push_back(sv);
	  }
	}
      }
    }
    return s;
  }

  static unsigned short
  getSpaceDimension(const ZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef ZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return 3u;
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return 2u;
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 1u;
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return 0;
  }

  static unsigned short
  getStensorSize(const ZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef ZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return 6u;
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return 4u;
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 3u;
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return 0;
  }

  static unsigned short
  getTensorSize(const ZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef ZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return 9u;
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return 5u;
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 3u;
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return 0;
  }

  static std::string
  getSpaceDimensionSuffix(const ZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef ZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "3D";
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "2D";
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return "1D";
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return "";
  }

  static std::string
  getLibraryName(const BehaviourDescription& mb)
  {
    using namespace std;
    string lib;
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "libZMAT"+mb.getMaterialName();
      } else {
	lib = "libZMATBehaviour";
      }
    } else {
      lib = "libZMAT"+mb.getLibrary();
    }
    return lib;
  } // end of UMATInterface::getLibraryName

  std::string
  ZMATInterface::getName(void)
  {
    return "zmat";
  }

  ZMATInterface::ZMATInterface()
  {} // end of ZMATInterface::ZMATInterface

  void
  ZMATInterface::allowDynamicallyAllocatedArrays(const bool b)
  {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  } // end of ZMATInterface::allowDynamicallyAllocatedArrays


  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  ZMATInterface::treatKeyword(const std::string&,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  }
  
  bool
  ZMATInterface::isModellingHypothesisHandled(const Hypothesis h,
						    const BehaviourDescription& mb) const
  {
    using namespace std;
    set<Hypothesis> ih(this->getModellingHypothesesToBeTreated(mb));
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return !mb.areAllMechanicalDataSpecialised(ih);
    }
    return ih.find(h)!=ih.end();
  }
  
  std::set<ZMATInterface::Hypothesis>
  ZMATInterface::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    // treatment 
    set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    if(bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::GENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)!=bh.end()){
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if(h.empty()){
      string msg("ZMATInterfaceModellingHypothesesToBeTreated : "
		 "no hypotheses selected. This means that the given beahviour "
		 "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
		 "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
		 "make sense to use the ZMAT interface");
      throw(runtime_error(msg));
    }
    return h;
  } // edn of ZMATInterface::getModellingHypothesesToBeTreated

  void 
  ZMATInterface::exportMechanicalData(std::ofstream& behaviourDataFile,
					    const Hypothesis h,
					    const BehaviourDescription& mb) const
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    if(!persistentVarsHolder.empty()){
      behaviourDataFile << "void" << endl
    			<< "ZMATexportStateData("
    			<< "ZSET::TENSOR2_FLUX& ZMATsig,ZSET::MAT_DATA& ZMATdata) const" << endl;
    } else {
      behaviourDataFile << "void" << endl
    			<< "ZMATexportStateData("
    			<< "ZSET::TENSOR2_FLUX& ZMATsig,ZSET::MAT_DATA&) const" << endl;
    }
    behaviourDataFile << "{" << endl;
    behaviourDataFile << "using namespace tfel::math;" << endl;
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      string msg("ZMATInterface::exportMechanicalData : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    behaviourDataFile << "zmat::ZMATInterface::convert(&ZMATsig[0],this->sig);" << endl;
    if(!persistentVarsHolder.empty()){
      behaviourDataFile << "auto& ZMATstatev = ZMATdata.var_int()[0];" << endl;
      VariableDescriptionContainer::const_iterator p;
      SupportedTypes::TypeSize currentOffset;
      for(p=persistentVarsHolder.begin();p!=persistentVarsHolder.end();++p){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	if(p->arraySize==1u){
	  if(flag==SupportedTypes::Scalar){
	    if(mb.useQt()){
	       behaviourDataFile << "ZMATstatev" << "[" 
				 << currentOffset << "] = base_cast(this->"
				 << p->name << ");\n"; 
	    } else {
	      behaviourDataFile << "ZMATstatev" << "[" 
				<< currentOffset << "] = this->"
				<< p->name << ";\n"; 
	    } 
	  } else if((flag==SupportedTypes::TVector)||
		    (flag==SupportedTypes::Stensor)||
		    (flag==SupportedTypes::Tensor)){
	    behaviourDataFile << "zmat::ZMATInterface::convert(&" << "ZMATstatev" << "[" 
			      << currentOffset << "],this->" << p->name 
			      << ");\n";  
	  } else {
	    string msg("SupportedTypes::exportResults : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	  currentOffset+=this->getTypeSize(p->type,p->arraySize);
	} else {
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    behaviourDataFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	    if(flag==SupportedTypes::Scalar){ 
	      if(mb.useQt()){
		behaviourDataFile << "ZMATstatev" << "[" 
				  << currentOffset << "+idx] = common_cast(this->"
				  << p->name << "[idx]);\n"; 
	      } else {
		behaviourDataFile << "ZMATstatev" << "[" 
				  << currentOffset << "+idx] = this->"
				  << p->name << "[idx];\n"; 
	      }
	    } else if((flag==SupportedTypes::TVector)||
		      (flag==SupportedTypes::Stensor)||
		      (flag==SupportedTypes::Tensor)){
	      behaviourDataFile << "zmat::ZMATInterface::convert(&ZMATstatev" << "[" 
				<< currentOffset << "+idx*StensorSize],this->" << p->name
				<< "[idx]);\n";  
	    } else {
	      string msg("SupportedTypes::exportResults : ");
	      msg += "internal error, tag unsupported";
	      throw(runtime_error(msg));
	    }
	    behaviourDataFile << "}" << endl;
	      currentOffset+=this->getTypeSize(p->type,p->arraySize);
	  } else {
	    for(unsigned short i=0;i!=p->arraySize;++i){
	      if(flag==SupportedTypes::Scalar){
		if(mb.useQt()){
		  behaviourDataFile << "ZMATstatev" << "[" 
				    << currentOffset << "] = common_cast(this->"
				    << p->name << "[" << i << "]);\n"; 
		} else {
		  behaviourDataFile << "ZMATstatev" << "[" 
				    << currentOffset << "] = this->"
				    << p->name << "[" << i << "];\n"; 
		} 
	      } else if((flag==SupportedTypes::TVector)||
			(flag==SupportedTypes::Stensor)||
			(flag==SupportedTypes::Tensor)){
		behaviourDataFile << "zmat::ZMATInterface::convert(&" << "ZMATstatev" << "[" 
				  << currentOffset << "],this->" << p->name
				  << "[" << i << "]);\n";  
	      } else {
		string msg("SupportedTypes::exportResults : ");
		msg += "internal error, tag unsupported";
		throw(runtime_error(msg));
	      }
	      currentOffset+=this->getTypeSize(p->type,1u);
	    }
	  }
	}
      }
    }
    behaviourDataFile << "} // end of ZMATexportStateData" << endl;
    behaviourDataFile << endl;
  }
  
  void 
  ZMATInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
						       const BehaviourDescription& mb) const
  {
    using namespace std;
    out << "#include\"MFront/ZMAT/ZMATInterface.hxx\"" << endl;
    out << "#include\"Coefficient.h\"" << endl;
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "#include\"Auto_ptr.h\"" << endl;
      out << "#include\"Elasticity.h\"" << endl;
    }
    out << "#include\"Coefficient.h\"" << endl;
    out << "#include\"Behavior.h\"" << endl;
    writeZMATUndefs(out);
    out << endl;
  } // end of ZMATInterface::writeInterfaceSpecificIncludes

  void
  ZMATInterface::writeBehaviourDataConstructor(std::ofstream& out,
						     const Hypothesis h,
						     const BehaviourDescription& mb) const
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& ivs = d.getPersistentVariables();
    const auto& evs = d.getExternalStateVariables();
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      string msg("ZMATInterface::writeBehaviourDataConstructor : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << " * \\brief constructor for the ZMAT interface" << endl;
    out << " *" << endl;
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << " * \\param ZMATsig     : stress at the beginning of the time step" << endl;
    }
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << " \\param ZMATeto     : strain at the beginning of the time step" << endl;
    }
    if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << " \\param ZMATF0      : deformation gradient at the begining of the time step" << endl;
    }
    out << " * \\param ZMATmprops  : material properties" << endl;
    out << " * \\param ZMATdata    : material data handler" << endl;
    out << " * \\param ZMATT_pos   : position of the temperature" << endl;
    out << " * \\param ZMATevs_pos : position of the external state varaibles" << endl;
    out << " */" << endl;
    out << mb.getClassName() << "BehaviourData(";
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "const ZSET::TENSOR2_FLUX& ZMATsig," << endl;
    }
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << "const real * const ZMATeto," << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << "const real * const ZMATF0," << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    } 
    if(!mps.empty()){
      out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops," << endl;
    } else {
      out << "const ZSET::ARRAY<ZSET::COEFF>&," << endl;
    }
    out << "const ZSET::MAT_DATA& ZMATdata," << endl;
    out << "const int ZMATT_pos," << endl;
    if(!evs.empty()){
      out << "const ZSET::ARRAY<int>& ZMATevs_pos){" << endl;
    } else {
      out << "const ZSET::ARRAY<int>&){" << endl;
    }
    if(!ivs.empty()){
      out << "const auto& ZMATstatev = ZMATdata.var_int_ini()[0];"  << endl;
    }
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t = *(ZMATdata.param_set_ini());"  << endl;
    out << "zmat::ZMATInterface::convert(this->sig,&ZMATsig[0]);" << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << "zmat::ZMATInterface::convert(this->eto,&ZMATeto[0]);" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << "zmat::ZMATInterface::convert(this->F0,&ZMATF0[0]);" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    if(!mps.empty()){
      SupportedTypes::TypeSize o;
      for(const auto & mp : mps){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(mp.type);
	if(flag==SupportedTypes::Scalar){
	  if(mp.arraySize==1u){
	    out << "this->" << mp.name << " = ZMATmprops[" << o << "]();" << endl;
	    o+=this->getTypeSize(mp.type,1u);
	  } else {
	    if(this->useDynamicallyAllocatedVector(mp.arraySize)){
	      out << "this->" << mp.name << ".resize(" << mp.arraySize << ");" << endl;
	      out << "for(unsigned short idx=0;idx!=" << mp.arraySize << ";++idx){" << endl;
	      out << "this->" << mp.name << "[idx] = ZMATmprops[" << o << "+idx]();" << endl;
	      out << "}" << endl;
	      o+=this->getTypeSize(mp.type,mp.arraySize);
	    } else {
	      for(unsigned short i=0;i!=mp.arraySize;++i){
		out << "this->" << mp.name << "[" << i << "] = ZMATmprops[" << o << "]();" << endl;
		o+=this->getTypeSize(mp.type,1u);
	      }
	    }
	  }
	} else {
	  string msg("ZMATInterface::writeBehaviourDataConstructor : "
		     "unsupported material properties type (only scalar supported yet)");
	  throw(runtime_error(msg));
	}
      }
    }
    if(!ivs.empty()){
      SupportedTypes::TypeSize o;
      for(const auto & iv : ivs){
	const string n = "this->"+iv.name;
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(iv.type);
	if(iv.arraySize==1u){
	  switch(flag){
	  case SupportedTypes::Scalar : 	  
	    out << n << " = ZMATstatev[" << o << "];" << endl;
	    break;
	  case SupportedTypes::Stensor :
	  case SupportedTypes::Tensor  :
	    out << "zmat::ZMATInterface::convert(" << n << "," << "&ZMATstatev[" << o << "]);" << endl;
	    break;
	  default:
	    string msg("ZMATInterface::writeBehaviourDataConstructor : ");
	    msg += "unsupported variable type ('"+iv.type+"')";
	    throw(runtime_error(msg));
	  }
	  o+=this->getTypeSize(iv.type,1u);
	} else {
	  if(this->useDynamicallyAllocatedVector(iv.arraySize)){
	    out << n << ".resize(" << iv.arraySize << ");" << endl;
	    out << "for(unsigned short idx=0;idx!=" << iv.arraySize << ";++idx){" << endl;
	    switch(flag){
	    case SupportedTypes::Scalar : 
	      out << n << "[idx] = ZMATstatev[" 
		<< o << "+idx];\n";  
	      break;
	    // case SupportedTypes::TVector :
	    //   out << "tfel::fsalgo::copy<TVectorSize>::exe(&"+src+"[" 
	    // 	<< o << "+idx*TVectorSize],"
	    // 	<< n << "[idx].begin());\n";  
	    //   break;
	    case SupportedTypes::Stensor :
	    case SupportedTypes::Tensor :
	      out << "zmat::ZMATInterface::convert(" << n << "[idx],&ZMATstatev[" 
		  << o << "+idx*StensorSize]);\n";  
	      break;
	    default : 
	      string msg("ZMATInterface::writeBehaviourDataConstructor : "
			 "unsupported variable type ('"+iv.type+"')");
	      throw(runtime_error(msg));
	    }
	    out << "}" << endl;
	    o+=this->getTypeSize(iv.type,iv.arraySize);
	  } else {
	    for(int i=0;i!=iv.arraySize;++i){
	      switch(flag){
	      case SupportedTypes::Scalar : 
		out << n << "[" << i << "] = ZMATstatev[" 
		  << o << "];\n";  
		break;
	      case SupportedTypes::Stensor :
	      case SupportedTypes::Tensor :
		out << "zmat::ZMATInterface::convert(" <<  n << "["<< i << "],&ZMATstatev[" 
		    << o << "]);\n";  
		break;
	      default : 
		string msg("ZMATInterface::writeBehaviourDataConstructor : "
			   "unsupported variable type ('"+iv.type+"')");
		throw(runtime_error(msg));
	      }
	      o+=this->getTypeSize(iv.type,1u);
	    }
	  }
	}
      }
    }
    if(!evs.empty()){
      SupportedTypes::TypeSize o;
      for(const auto & ev : evs){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(ev.type);
	if(flag==SupportedTypes::Scalar){
	  if(ev.arraySize==1u){
	    out << "this->" << ev.name << " = ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
	    o+=this->getTypeSize(ev.type,1u);
	  } else {
	    if(this->useDynamicallyAllocatedVector(ev.arraySize)){
	      out << "this->" << ev.name << ".resize(" << ev.arraySize << ");" << endl;
	      out << "for(unsigned short idx=0;idx!=" << ev.arraySize << ";++idx){" << endl;
	      out << "this->" << ev.name << "[idx] = ZMATextvars_t[ZMATevs_pos[" << o << "+idx]];" << endl;
	      out << "}" << endl;
	      o+=this->getTypeSize(ev.type,ev.arraySize);
	    } else {
	      for(unsigned short i=0;i!=ev.arraySize;++i){
		out << "this->" << ev.name << "[" << i << "] = ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
		o+=this->getTypeSize(ev.type,1u);
	      }
	    }
	  }
	} else {
	  string msg("ZMATInterface::writeBehaviourDataConstructor : "
		     "unsupported material properties type (only scalar supported yet)");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "this->T = ZMATextvars_t[ZMATT_pos];" << endl;
    out << "}" << endl << endl;
  } // end of ZMATInterface::writeBehaviourDataConstructor

  void
  ZMATInterface::writeBehaviourConstructor(std::ofstream& out,
						 const BehaviourDescription& mb,
						 const std::string& initStateVarsIncrements) const
  {
    using namespace std;
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      string msg("ZMATInterface::writeBehaviourDataConstructor : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << " * \\brief constructor for the ZMAT interface" << endl;
    out << " *" << endl;
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << " * \\param ZMATsig     : strains" << endl;
    }
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << " \\param ZMATeto     : strain at the beginning of the time step" << endl;
      out << " \\param ZMATdeto    : strain increment" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << " \\param ZMATF0 : deformation gradient at the beginning of the time step" << endl;
      out << " \\param ZMATF1 : deformation gradient at the end of the time step" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    } 
    out << " * \\param ZMATmprops  : material properties" << endl;
    out << " * \\param ZMATdata    : material data handler" << endl;
    out << " * \\param ZMATT_pos   : position of the temperature" << endl;
    out << " * \\param ZMATevs_pos : position of the external state varaibles" << endl;
    out << " * \\param ZMATdt      : time step" << endl;
    out << " */" << endl;
    out << mb.getClassName() << "(";
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "const ZSET::TENSOR2_FLUX& ZMATsig," << endl;
    }
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << " const real* const ZMATeto," << endl;
      out << " const real* const ZMATdeto," << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << " const real* const ZMATF0," << endl;
      out << " const real* const ZMATF1," << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops," << endl
	<< "const ZSET::MAT_DATA& ZMATdata," << endl
	<< "const int ZMATT_pos," << endl
	<< "const ZSET::ARRAY<int>& ZMATevs_pos," << endl
	<< "const double ZMATdt)" << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      if(mb.useQt()){
	out << ": " << mb.getClassName() 
	    << "BehaviourData<hypothesis,Type,use_qt>(ZMATsig,ZMATeto,ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos)," << endl;
	out << mb.getClassName() 
	    << "IntegrationData<hypothesis,Type,use_qt>(ZMATdeto,ZMATdata,ZMATT_pos,ZMATevs_pos,ZMATdt)";
      } else {
	out << ": " << mb.getClassName() 
	    << "BehaviourData<hypothesis,Type,false>(ZMATsig,ZMATeto,ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos)," << endl;
	out << mb.getClassName() 
	    << "IntegrationData<hypothesis,Type,false>(ZMATdeto,ZMATdata,ZMATT_pos,ZMATevs_pos,ZMATdt)";
      }
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      if(mb.useQt()){
	out << ": " << mb.getClassName() 
	    << "BehaviourData<hypothesis,Type,use_qt>(ZMATsig,ZMATF0,ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos)," << endl;
	out << mb.getClassName() 
	    << "IntegrationData<hypothesis,Type,use_qt>(ZMATF1,ZMATdata,ZMATT_pos,ZMATevs_pos,ZMATdt)";
      } else {
	out << ": " << mb.getClassName() 
	    << "BehaviourData<hypothesis,Type,false>(ZMATsig,ZMATF0,ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos)," << endl;
	out << mb.getClassName() 
	    << "IntegrationData<hypothesis,Type,false>(ZMATF1,ZMATdata,ZMATT_pos,ZMATevs_pos,ZMATdt)";
      }
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    if(!initStateVarsIncrements.empty()){
      out << "," << endl << initStateVarsIncrements;
    }
  }
  
  void 
  ZMATInterface::writeIntegrationDataConstructor(std::ofstream& out,
						       const Hypothesis h,
						       const BehaviourDescription& mb) const
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& evs = d.getExternalStateVariables();
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      string msg("ZMATInterface::writeBehaviourDataConstructor : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << " * \\brief constructor for the ZMAT interface" << endl;
    out << " *" << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << " \\param ZMATdeto  : strain increment " << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << " \\param ZMATF1    : deformation gradient at the end of the time step" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << " * \\param ZMATdata    : material data handler" << endl;
    out << " * \\param ZMATT_pos   : position of the temperature" << endl;
    out << " * \\param ZMATevs_pos : position of the external state varaibles" << endl;
    out << " * \\param ZMATdt      : time step" << endl;
    out << " */" << endl;
    out << mb.getClassName() << "IntegrationData(";
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << "const real * const ZMATdeto," << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << "const real * const ZMATF1," << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "const ZSET::MAT_DATA& ZMATdata," << endl
	<< "const int ZMATT_pos," << endl;
    if(!evs.empty()){
      out << "const ZSET::ARRAY<int>& ZMATevs_pos," << endl;
    } else {
      out << "const ZSET::ARRAY<int>&," << endl;
    }
    out << "const double ZMATdt)" << endl;
    out << ": dt(ZMATdt)" << endl; 
    out << "{" << endl;
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t   = *(ZMATdata.param_set_ini());" << endl;
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_tdt = *(ZMATdata.param_set());"     << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << "zmat::ZMATInterface::convert(this->deto,ZMATdeto);" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << "zmat::ZMATInterface::convert(this->F1,ZMATF1);" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    if(!evs.empty()){
      SupportedTypes::TypeSize o;
      for(const auto & ev : evs){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(ev.type);
	if(flag==SupportedTypes::Scalar){
	  if(ev.arraySize==1u){
	    out << "this->d" << ev.name << " = ZMATextvars_tdt[ZMATevs_pos[" << o << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
	    o+=this->getTypeSize(ev.type,1u);
	  } else {
	    if(this->useDynamicallyAllocatedVector(ev.arraySize)){
	      out << "this->d" << ev.name << ".resize(" << ev.arraySize << ");" << endl;
	      out << "for(unsigned short idx=0;idx!=" << ev.arraySize << ";++idx){" << endl;
	      out << "this->d" << ev.name << "[idx] = ZMATextvars_td-[ZMATevs_pos[" << o << "+idx]]-ZMATextvars_t[ZMATevs_pos[" << o << "+idx]];" << endl;
	      out << "}" << endl;
	      o+=this->getTypeSize(ev.type,ev.arraySize);
	    } else {
	      for(unsigned short i=0;i!=ev.arraySize;++i){
		out << "this->d" << ev.name << "[" << i << "] = ZMATextvars_tdt[ZMATevs_pos[" << o << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
		o+=this->getTypeSize(ev.type,1u);
	      }
	    }
	  }
	} else {
	  string msg("ZMATInterface::writeBehaviourDataConstructor : "
		     "unsupported material properties type (only scalar supported yet)");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "this->dT = ZMATextvars_tdt[ZMATT_pos]-ZMATextvars_t[ZMATT_pos];" << endl;
    out << "}" << endl << endl;
  }

  void 
  ZMATInterface::writeBehaviourDataMainVariablesSetters(std::ofstream&,
							      const BehaviourDescription&) const
  {
    //handled by the constructors
  }

  void 
  ZMATInterface::writeIntegrationDataMainVariablesSetters(std::ofstream&,
								const BehaviourDescription&) const
  {
    //handled by the constructors
  }

  void
  ZMATInterface::endTreatment(const BehaviourDescription& mb,
				     const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/ZMAT");
    const Hypothesis hypotheses[3u] = {ModellingHypothesis::TRIDIMENSIONAL,
				       ModellingHypothesis::GENERALISEDPLANESTRAIN,
				       ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    const string name = mb.getLibrary()+mb.getClassName();
    const string headerFileName("ZMAT"+name+".hxx");
    const string srcFileName("ZMAT"+name+".cxx");
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    const unsigned short nbh =
      (mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ? 1 : 0);
    if(nbh==0){
      string msg("ZMATInterface::endTreatment : "
		 "no suitable modelling hypothesis supported");
      throw(runtime_error(msg));
    }
    ofstream out("include/MFront/ZMAT/"+headerFileName);
    if(!out){
      string msg("UMATInterface::endTreatment : "
		 "could not open header file '"+
		 headerFileName+"'");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << "* \\file   "  << headerFileName << endl;
    out << "* \\brief  This file declares the ZMAT interface for the " 
	<< mb.getClassName() << " behaviour law" << endl;
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/" << endl << endl;
    out << "#ifndef MFRONT_ZMAT_" << makeUpperCase(mb.getClassName()) << "_HXX" << endl;
    out << "#define MFRONT_ZMAT_" << makeUpperCase(mb.getClassName()) << "_HXX" << endl << endl;
    out << "#include\"Behavior.h\"" << endl;
    writeZMATUndefs(out);
    out << endl;
    out << "Z_START_NAMESPACE;" << endl;
    out << endl;
    out << "/*!" << endl
	<< "* ZMAT Interface to the " << mb.getClassName() << " behaviour"
	<< "*/" << endl;
    out << "struct ZMAT" << mb.getClassName() << endl;
    out << ": public BEHAVIOR" << endl;
    out << "{" << endl;
    out << "//! constructor;" << endl;
    out << "ZMAT" << mb.getClassName() << "();" << endl;
    out << "/*!" << endl;
    out << " * \\brief integrate the beahviour over a time step" << endl;
    out << " * \\param[in,out] file  : input file " << endl;
    out << " * \\param[in]     dim   : dimension (used by the BEHAVIOUR class)" << endl;
    out << " * \\param[in]     integ : pointer to the local integration" << endl;
    out << " */" << endl;
    out << "virtual void initialize(ASCII_FILE&,int,LOCAL_INTEGRATION*);" << endl;
    out << "/*!" << endl;
    out << " * \\brief integrate the beahviour over a time step" << endl;
    out << " * \\param[in,out] mat        : material properties" << endl;
    out << " * \\param[out]    delta_grad : increment of driving variables" << endl;
    out << " * \\param[out]    tg_matrix  : tangent operator" << endl;
    out << " * \\param[int]    flags      : integration option" << endl;
    out << " * \\return NULL on success" << endl;
    out << " */" << endl;
    out << "INTEGRATION_RESULT*" << endl;
    out << "integrate(MAT_DATA&,const VECTOR&,MATRIX*&,int);" << endl;
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "/*!" << endl;
      out << " * \\brief return the elasticity matrix" << endl;
      out << " * \\param[in] mdat : material data" << endl;
      out << " */" << endl;
      out << "SMATRIX" << endl
	  << "get_elasticity_matrix(MAT_DATA&,double);" << endl;
    }
    out << "//! destructor" << endl;
    out << "virtual ~ZMAT" << mb.getClassName() << "();" << endl;
    out << endl;
    out << "protected:" << endl;
    out << endl;
    if(!all_mp_names.empty()){
      for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
	const Hypothesis h = *ph;
	if(mb.isModellingHypothesisSupported(h)){
	  out << "/*!" << endl;
	  out << " * \\brief initialize material properties" << endl;
	  out << " * \\param[in,out] file    : input file " << endl;
	  out << " */" << endl;
	  out << "virtual void initializeMaterialProperties" << getSpaceDimensionSuffix(*ph)
	      << "(ASCII_FILE&);" << endl;
	}
      }
    }
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	out << "/*!" << endl;
	out << " * \\brief initialize parameters" << endl;
	out << " * \\param[in,out] file    : input file " << endl;
	out << " */" << endl;
	out << "virtual void initializeParameters" << getSpaceDimensionSuffix(*ph)
	    << "(ASCII_FILE&);" << endl;
      }
    }
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	out << "/*!" << endl;
	out << " * \\brief call the mfront beahviour in " << ModellingHypothesis::toString(*ph) << endl;
	out << " * \\param[in,out] mat     : material properties" << endl;
	out << " * \\param[out] delta_grad : increment of driving variables" << endl;
	out << " * \\param[out] tg_matrix  : tangent operator" << endl;
	out << " * \\param[int] flags      : integration option" << endl;
	out << " * \\return NULL on success" << endl;
	out << " */" << endl;
	out << "INTEGRATION_RESULT*" << endl;
	out << "callMFrontBehaviour" << getSpaceDimensionSuffix(*ph) 
	    <<"(MAT_DATA&,const VECTOR&,MATRIX*&,int);" << endl;
      }
    }
    out << "//! stresses" << endl;
    out << "TENSOR2_FLUX sig;" << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << "//! strains" << endl;
      out << "TENSOR2_GRAD eto;" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << "//! deformation gradient" << endl;
      out << "TENSOR2_GRAD F;" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "//! tangent operator" << endl;
      out << "MATRIX tg_mat; " << endl;
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "//! elasticity matrix" << endl;
      out << "AUTO_PTR<ELASTICITY> elasticity; " << endl;
    }
    out << "//! material properties" << endl;
    out << "ARRAY<COEFF> mprops; " << endl;
    const auto& svs = getAllStateVariables(mb);
    if(!svs.empty()){
      for(const auto & sv : svs){
	out << "//! '" << sv.name << "' state variable" << endl;
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(sv.type);
	if(flag==SupportedTypes::Scalar){
	  if(sv.arraySize==1u){
	    out << "SCALAR_VINT " << sv.name << ";" << endl; 
	  } else {
	    out << "VECTOR_VINT " << sv.name << ";" << endl; 
	  }
	} else if ((flag==SupportedTypes::Stensor)||
		   (flag==SupportedTypes::Tensor)){
	  if(sv.arraySize==1u){
	    out << "TENSOR2_VINT " << sv.name << ";" << endl; 
	  } else {
	    out << "ARRAY<TENSOR2_VINT> " << sv.name << ";" << endl; 
	  }
	} else {
	  string msg("ZMATInterface::endTreatment : "
		     "unsupported state variable type");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "//! external state variables" << endl;
    out << "LIST<EXTERNAL_PARAM*> local_ep_list;" << endl;
    out << "LIST<EXTERNAL_PARAM*>* zbb_keep_ep;" << endl;
    out << "//! external state variables positions" << endl;
    out << "ARRAY<int> evs_positions;" << endl;
    out << "//! position de la température" << endl;
    out << "int temperature_position;" << endl;
    out << "//! local clock" << endl;
    out << "CLOCK  local_clock; " << endl;
    out << "//! out of bounds policy" << endl;
    out << "tfel::material::OutOfBoundsPolicy obp;" << endl;
    out << "};" << endl << endl;
    out << "Z_END_NAMESPACE;" << endl << endl;
    out << "#endif /* MFRONT_ZMAT_" << makeUpperCase(mb.getClassName()) << "_HXX*/" << endl;
    out.close();
    out.open("src/"+srcFileName);
    if(!out){
      string msg("UMATInterface::endTreatment : "
		 "could not open src file '"+
		 srcFileName+"'");
      throw(runtime_error(msg));
    }
    // writing source file
    out << "/*!" << endl;
    out << "* \\file   "  << srcFileName << endl;
    out << "* \\brief  This file declares the ZMAT interface for the " 
	<< mb.getClassName() << " behaviour law" << endl;
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/" << endl << endl;
    out << endl;
    out << "#include<string>" << endl;
    out << "#include<vector>" << endl;
    out << "#include<algorithm>" << endl;
    out << endl;
    out << "#include\"External_parameter.h\"" << endl;
    out << "#include\"Verbose.h\"" << endl;
    writeZMATUndefs(out);
    out << endl;
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"" << endl;
    out << "#include\"MFront/ZMAT/" << headerFileName << "\"" << endl;
    out << endl;
    out << "Z_START_NAMESPACE;" << endl;
    out << endl;
    out << "ZMAT" << mb.getClassName() << "::ZMAT" << mb.getClassName() << "()" << endl
	<< ": obp(tfel::material::None)" << endl
	<< "{" << endl
	<< "this->zbb_keep_ep = &this->local_ep_list;" << endl 
	<< "#ifdef _WIN64" << endl
	<< "ZMAT_GLOBAL_STORAGE::zmat_once();" << endl
	<< "ZMAT_GLOBAL_STORAGE& zmat_globals = *thread_zmat_globals;" << endl
	<< "#endif" << endl
	<< "} // end of ZMAT" << mb.getClassName() << "::ZMAT" << mb.getClassName() << "()" << endl;     
    out << endl;
    out << "void" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::initialize(ASCII_FILE& file,int dim,LOCAL_INTEGRATION* integ){" << endl;
    out << "BEHAVIOR::initialize(file, dim,integ);" << endl;
    out << "using namespace std;" << endl;
    out << "int keep_verbose = ZSET::Verbose;" << endl;
    out << "this->coord.resize(dim);" << endl;
    out << "this->sig.initialize(this,\"sig\",this->tsz(),1);" << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << "this->eto.initialize(this,\"eto\",this->tsz(),1);" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << "this->F.initialize(this,\"F\",this->utsz(),1);" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "this->tg_mat.resize(this->tsz(), this->tsz());" << endl;
    }
    out << "// initialisation dispatch" << endl;
    out << "if(this->tsz()==6){" << endl;
    this->writeBehaviourInitialisation(out,mb,ModellingHypothesis::TRIDIMENSIONAL);
    out << "} else if(this->tsz()==4){" << endl;
    this->writeBehaviourInitialisation(out,mb,ModellingHypothesis::GENERALISEDPLANESTRAIN);
    out << "} else if(this->tsz()==3){" << endl;
    this->writeBehaviourInitialisation(out,mb,ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    out << "} else {" << endl;
    out << "ERROR(\"Invalid tensor size\");" << endl;
    out << "}" << endl;
    out << "this->temperature_position = EXTERNAL_PARAM::rank_of_nodal_ip(\"temperature\");" << endl;
    out << "if(this->temperature_position==-1){;" << endl;
    out << "INPUT_ERROR(\"temperature is not defined\");" << endl;
    out << "}" << endl;
    if(!all_mp_names.empty()){
      out << "// check that all material properties were initialised" << endl;
      out << "for(int pc=0;pc!=this->mprops.size();++pc){" << endl
	  << "if(!this->mprops[pc].ok()){" << endl
	  << "INPUT_ERROR(\"Some material properties were not initialised\");" << endl
	  << "}" << endl
	  << "}" << endl;
    }
    out << "ZSET::Verbose = keep_verbose;" << endl;
    out << "} // end of ZMAT" << mb.getClassName() << "::initialize" << endl << endl;
    if(!all_mp_names.empty()){
      for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
	const Hypothesis h = *ph;
	if(mb.isModellingHypothesisSupported(h)){
	  this->writeMaterialPropertiesInitialisation(out,mb,*ph);
	}
      }
    }
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	this->writeParametersInitialisation(out,mb,*ph);
      }
    }
    out << "INTEGRATION_RESULT*" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::integrate(MAT_DATA& mdat," << endl
	<< "const VECTOR& delta_grad," << endl
	<< "MATRIX*& tg_matrix," << endl
	<< "int flags){" << endl
	<< "int keep_verbose  = ZSET::Verbose; " << endl
	<< "CLOCK* keep_clock = ZSET::stored_thread_zbase_globals->ptr()->active_clock; " << endl
	<< "tg_matrix = &(this->tg_mat);" << endl
	<< "this->set_var_aux_to_var_aux_ini();" << endl
	<< "this->set_var_int_to_var_int_ini();" << endl
	<< "LIST<EXTERNAL_PARAM*>* ep_save = &EXTERNAL_PARAM::Get_EP_list();" << endl
	<< "EXTERNAL_PARAM::set_EP_list(this->zbb_keep_ep);" << endl
	<< "if(!this->curr_ext_param){" << endl
	<< "this->curr_ext_param = *mdat.param_set();" << endl
	<< "}" << endl
	<< "this->calc_local_coefs();" << endl
	<< "INTEGRATION_RESULT* r = NULL;" << endl
	<< "try{" << endl;
    out << "if(this->tsz()==6){" << endl;
    if(!mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL)){
      out << "ERROR(\"Computations using the 'Tridimensional'\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
    } else {
      out << "r=this->callMFrontBehaviour3D"
	  << "(mdat,delta_grad,tg_matrix,flags);" << endl;
    }
    out << "} else if(this->tsz()==4){" << endl;
    if(!mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      out << "ERROR(\"Computations using the 'GeneralisedPlaneStrain'\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
    } else {
      out << "r=this->callMFrontBehaviour2D"
	  << "(mdat,delta_grad,tg_matrix,flags);" << endl;
    }
    out << "} else if(this->tsz()==3){" << endl;
    if(!mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)){
      out << "ERROR(\"Computations using the 'AxisymmetricalGeneralisedPlaneStrain'\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
    } else {
      out << "r=this->callMFrontBehaviour1D"
	  << "(mdat,delta_grad,tg_matrix,flags);" << endl;
    }
    out << "} else {" << endl;
    out << "ERROR(\"Invalid tensor size\");" << endl;
    out << "}" << endl;
    out << "}" << endl
	<< "catch(std::exception& e){" << endl
	<< "static INTEGRATION_RESULT bad_result;" << endl
	<< "*(*ZSET::stored_thread_zbase_globals).ptr()->out_msg << e.what() << endl;" << endl
	<< "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);" << endl
	<< "ZSET::Verbose = keep_verbose; " << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock; " << endl
	<< "return &bad_result;" << endl
	<< "}" << endl
	<< "catch(...){" << endl
	<< "static INTEGRATION_RESULT bad_result;" << endl
	<< "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);" << endl
	<< "ZSET::Verbose = keep_verbose; " << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock; " << endl
	<< "return &bad_result;" << endl
	<< "}" << endl
	<< "if(r!=NULL){" << endl
	<< "ZSET::Verbose = keep_verbose; " << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock; " << endl
	<< "return r;" << endl
    	<< "}" << endl
	<< "this->update_var_aux();" << endl
	<< "this->zbb_keep_ep = &EXTERNAL_PARAM::Get_EP_list();" << endl
	<< "EXTERNAL_PARAM::set_EP_list(ep_save);" << endl
	<< "ZSET::Verbose = keep_verbose;" << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock;" << endl
	<< "return NULL;" << endl;
    out << "} // end of ZMAT" << mb.getClassName() << "::integrate" << endl << endl;
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "SMATRIX" << endl
	  << "ZMAT" << mb.getClassName() << "::get_elasticity_matrix(MAT_DATA& mdat,double){" << endl
	  << "elasticity->attach_all(mdat);" << endl
	  << "elasticity->calc_coef();" << endl
	  << "return SMATRIX(*elasticity());" << endl
	  << "} // end of ZMAT" << mb.getClassName() << "::get_elasticity_matrix" << endl << endl;
    }
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	this->writeCallMFrontBehaviour(out,mb,*ph);
      }
    }
    out << endl;
    out << "ZMAT" << mb.getClassName() << "::~ZMAT" << mb.getClassName() << "(){" << endl;
    out << "} // end of ZMAT" << mb.getClassName() << "::~ZMAT" << mb.getClassName() << endl << endl;
    out << "BEHAVIOR_READER(ZMAT" << mb.getClassName() << "," << mb.getClassName() << ")" << endl << endl;
    out << "Z_END_NAMESPACE;" << endl;
  } // end of ZMATInterface::endTreatment

  void
  ZMATInterface::writeBehaviourInitialisation(std::ostream& out,
						    const BehaviourDescription& mb,
						    const ZMATInterface::Hypothesis h) const
  {
    using namespace std;
    if(!mb.isModellingHypothesisSupported(h)){
      out << "ERROR(\"Computations using the " << ModellingHypothesis::toString(h) << "\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
      return;
    }
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& isvs = d.getPersistentVariables();
    const auto& esvs = d.getExternalStateVariables();
    const unsigned short dime = getSpaceDimension(h);
    const int mps_size  = this->getTotalSize(mps).getValueForDimension(dime);
    if(!mps.empty()){
      out << "this->mprops.resize(" << mps_size << ");" << endl; 
    }
    if(!isvs.empty()){
      for(const auto & isv : isvs){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(isv.type);
	if(flag==SupportedTypes::Scalar){
	  out << "this->" << isv.name << ".initialize(this,\"" << isv.name 
	      << "\"," << isv.arraySize << ",1);" << endl; 
	} else if (flag==SupportedTypes::Stensor){
	  if(isv.arraySize==1u){
	    out << "this->" << isv.name << ".initialize(this,\"" << isv.name 
		<< "\",this->tsz(),1);" << endl; 
	  } else {
	    out << "this->" << isv.name << ".initialize(this,\"" << isv.name 
		<< "\"," << isv.arraySize << "*(this->tsz()),1);" << endl; 
	  }
	} else if (flag==SupportedTypes::Tensor){
	  if(isv.arraySize==1u){
	    out << "this->" << isv.name << ".initialize(this,\"" << isv.name 
		<< "\",this->utsz(),1);" << endl; 
	  } else {
	    out << "this->" << isv.name << ".initialize(this,\"" << isv.name 
		<< "\"," << isv.arraySize << "*(this->uts()),1);" << endl; 
	  }
	} else {
	  string msg("ZMATInterface::endTreatment : "
		     "unsupported state variable type");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "for(;;){" << endl;
    out << "STRING str=file.getSTRING();" << endl;
    out << "if(this->base_read(str,file)){" << endl;
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    if(!all_mp_names.empty()){
      out << "} else if((str==\"**model_coef\")||(str==\"**material_properties\")){" << endl;
      out << "this->initializeMaterialProperties" << getSpaceDimensionSuffix(h) << "(file);" << endl;
    }
    out << "} else if(str==\"**parameters\"){" << endl;
    out << "this->initializeParameters" << getSpaceDimensionSuffix(h) << "(file);" << endl;
    out << "} else if(str==\"**out_of_bounds_policy\"){" << endl;
    out << "STRING policy=file.getSTRING();" << endl;
    out << "if(policy==\"None\"){" << endl;
    out << "this->obp=tfel::material::None;" << endl;
    out << "} else if(policy==\"Strict\"){" << endl;
    out << "this->obp=tfel::material::Strict;" << endl;
    out << "} else if(policy==\"Warning\"){" << endl;
    out << "this->obp=tfel::material::Warning;" << endl;
    out << "} else {" << endl;
    out << "INPUT_ERROR(\"unknown policy '\"+policy+\"'\");" << endl
	<< "}" << endl;
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "} else if((str==\"**elasticity\")||(str==\"**ELASTICITY\")){" << endl;
      out << "if(!this->elasticity.Null()){" << endl
	  << "INPUT_ERROR(\"elasticity already defined\");" << endl
	  << "}" << endl
	  << "this->elasticity=ELASTICITY::read(file,this);" << endl;
    }
    out << "} else if (str.start_with(\"***\")){" << endl
	<< "file.back();" << endl
	<< "break;" << endl
	<< "} else {" << endl
	<< "INPUT_ERROR(\"Invalid keyword '\"+str+\"'\");" << endl
	<< "}" << endl << "}" <<endl;
    if(!esvs.empty()){
      const int ext_size = this->getTotalSize(esvs).getValueForDimension(dime);
      out << "evs_positions.resize(" << ext_size << ");"<< endl;
      VariableDescriptionContainer::const_iterator pev;
      int i=0;
      for(pev=esvs.begin();pev!=esvs.end();++pev){
	const VariableDescription& v = *pev;
	const auto& name = d.getExternalName(v.name);
	if(v.arraySize==1u){
	  out << "this->evs_positions[" << i << "] = " 
	      << "EXTERNAL_PARAM::rank_of_nodal_ip(\"" << name << "\");" << endl;
	  out << "if(this->evs_positions[" << i << "]=-1){;" << endl;
	  out << "INPUT_ERROR(\"'" << name << "' is not defined\");" << endl;
	  out << "}" << endl;
	  ++i;
	} else {
	  for(unsigned short n=0;n!=v.arraySize;++n,++i){
	    out << "this->evs_positions[" << i << "] = " 
		<< "EXTERNAL_PARAM::rank_of_nodal_ip(\"" << name
		<< "[" << n << "]\");" << endl;
	    out << "if(this->evs_positions[" << i << "]=-1){;" << endl;
	    out << "INPUT_ERROR(\"'" << name << "[" << n << "]' is not defined\");" << endl;
	    out << "}" << endl;
	  }
	}
      }
    }
  }

  void
  ZMATInterface::writeParametersInitialisation(std::ostream& out,
						     const BehaviourDescription& mb,
						     const ZMATInterface::Hypothesis h) const
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& params = d.getParameters();
    const vector<string> pnames = d.getExternalNames(params);
    VariableDescriptionContainer::const_iterator p;
    vector<string>::const_iterator pn;
    out << "void" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::initializeParameters" << getSpaceDimensionSuffix(h) << "(ASCII_FILE& file){" << endl;
    out << "for(;;){" << endl
	<< "STRING str=file.getSTRING();" << endl
	<< "if(str[0]=='*'){" << endl
	<< "file.back();" << endl
	<< "break;" << endl;
    for(p=params.begin(),pn=pnames.begin();p!=params.end();++p,++pn){
      out << "} else if(str==\"" << *pn << "\"){" << endl;
      if(p->type=="real"){
	out << "const double value=file.getdouble();" << endl;
      } else if(p->type=="int"){
	out << "const int value=file.getint();" << endl;
      } else if(p->type=="ushort"){
	out << "const unsigned short value=static_cast<unsigned short>(file.getint());" << endl;
      }
      if(mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)){
	out << "tfel::material::" << mb.getClassName() 
	    << "ParametersInitializer::get()." << p->name << " = value;" << endl;  
      } else {
	out << "tfel::material::" << mb.getClassName() 
	    << ModellingHypothesis::toString(h) 
	    << "ParametersInitializer::get()." << p->name << " = value;" << endl;
      }
    }
    out << "} else {" << endl
	<< "INPUT_ERROR(\"invalid parameter name '\"+str+\"'\");" << endl
	<< "}" << endl
	<< "}" << endl
	<< "}" << endl << endl;
  } // end of ZMATInterface::writeParametersInitialisation

  void
  ZMATInterface::writeMaterialPropertiesInitialisation(std::ostream& out,
							     const BehaviourDescription& mb,
							     const ZMATInterface::Hypothesis h) const
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    const vector<string> mpnames = d.getExternalNames(mps);
    const unsigned short nbh =
      (mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ? 1 : 0);
    const unsigned short dime = getSpaceDimension(h);
    const int mps_size  = this->getTotalSize(mps).getValueForDimension(dime);
    out << "void" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::initializeMaterialProperties" << getSpaceDimensionSuffix(h) << "(ASCII_FILE& file){" << endl
	<< "using std::find;" << endl;
    if(nbh!=1){
      if(!all_mp_names.empty()){
	out << "const STRING all_mp_names[" << all_mp_names.size() << "] = ";
	writeArray(out,all_mp_names);
      }
    }
    if(!mpnames.empty()){
      out << "const STRING mp_names" << getSpaceDimensionSuffix(h)
	  << "[" << mpnames.size() << "] = ";
      writeArray(out,mpnames);
    }
    out << "for(;;){" << endl
	<< "STRING str=file.getSTRING();" << endl
	<< "if(str[0]=='*'){" << endl
	<< "file.back();" << endl
	<< "break;" << endl
	<< "}" << endl;
    if(nbh!=1u){
      out << "if(find(all_mp_names,all_mp_names+" << all_mp_names.size() << ",str)==all_mp_names+"
	  << all_mp_names.size() << "){" << endl;
      out << "INPUT_ERROR(\"No material property named '\"+str+\"'\");" << endl;
      out << "}" << endl;
    }
    out << "const STRING * const pmat = find(mp_names"<<getSpaceDimensionSuffix(h)
	<<",mp_names" << getSpaceDimensionSuffix(h) << "+" << mps_size << ",str);" << endl;
    if(nbh==1u){
      out << "if(pmat==mp_names" << getSpaceDimensionSuffix(h) << "+" <<
	mps_size << "){" << endl;
      out << "INPUT_ERROR(\"No material property named '\"+str+\"'\");" << endl;
      out << "}" << endl;
    }
    out << "if(this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h) << "].ok()){" << endl;
    out << "INPUT_ERROR(\"material property '\"+str+\"' already defined\");" << endl;
    out << "}" << endl;
    out << "this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h) << "].read(str,file,this);" << endl;
    out << "}" << endl;
    out << "} // end of ZMAT" << mb.getClassName() 
	<< "::initializeMaterialProperties" << getSpaceDimensionSuffix(h) << endl << endl;
  }

  void
  ZMATInterface::writeCallMFrontBehaviour(std::ostream& out,
						const BehaviourDescription& mb,
						const ZMATInterface::Hypothesis h) const
  {
    using namespace std;
    out << "INTEGRATION_RESULT*" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::callMFrontBehaviour" << getSpaceDimensionSuffix(h) << "(MAT_DATA& mdat," << endl
	<< "const VECTOR& delta_grad," << endl
	<< "MATRIX*& tg_matrix," << endl
	<< "int flags){" << endl
	<< "typedef tfel::material::ModellingHypothesis ModellingHypothesis;" << endl
	<< "typedef tfel::material::" << mb.getClassName()
	<< "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",double,false> "
	<< mb.getClassName() << ";" << endl
	<< "using tfel::math::st2tost2;" << endl;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "// strain and strain increment" << endl;
      out << "double stran[" << getStensorSize(h) << "];" << endl;
      out << "double dstran[" << getStensorSize(h) << "];" << endl;
      out << "stran[0] = this->eto[0]-delta_grad[0];" << endl;
      out << "stran[1] = this->eto[1]-delta_grad[1];" << endl;
      out << "stran[2] = this->eto[2]-delta_grad[2];" << endl;
      if((h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "stran[3] = this->eto[3]-delta_grad[3];" << endl;
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "stran[4] = this->eto[4]-delta_grad[4];" << endl;
	  out << "stran[5] = this->eto[5]-delta_grad[5];" << endl;
	}
      }
      out << "dstran[0] = delta_grad[0];" << endl;
      out << "dstran[1] = delta_grad[1];" << endl;
      out << "dstran[2] = delta_grad[2];" << endl;
      if((h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "dstran[3] = delta_grad[3];" << endl;
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "dstran[4] = delta_grad[4];" << endl;
	  out << "dstran[5] = delta_grad[5];" << endl;
	}
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "// deformation gradients" << endl;
      out << "double F0[" << getTensorSize(h) << "];" << endl;
      out << "F0[0] = this->F[0]-delta_grad[0];" << endl;
      out << "F0[1] = this->F[1]-delta_grad[1];" << endl;
      out << "F0[2] = this->F[2]-delta_grad[2];" << endl;
      if((h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "F0[3] = this->F[3]-delta_grad[3];" << endl;
	out << "F0[4] = this->F[4]-delta_grad[4];" << endl;
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "F0[5] = this->F[5]-delta_grad[5];" << endl;
	  out << "F0[6] = this->F[6]-delta_grad[6];" << endl;
	  out << "F0[7] = this->F[7]-delta_grad[7];" << endl;
	  out << "F0[8] = this->F[8]-delta_grad[8];" << endl;
	}
      }
    } else {
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << mb.getClassName() << "::SMFlag smflag = " << mb.getClassName() << "::STANDARDTANGENTOPERATOR;" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << mb.getClassName() << "::SMFlag smflag = " << mb.getClassName() << "::DSIG_DD;" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "// tangent operator type" << endl
	<< mb.getClassName() << "::SMType smtype = " << mb.getClassName() << "::NOSTIFFNESSREQUESTED;" << endl
	<< "if(flags&CALC_TG_MATRIX){" << endl
	<< "smtype = " << mb.getClassName() << "::CONSISTENTTANGENTOPERATOR;" << endl
	<< "}" << endl;
    switch(mb.getBehaviourType()){
    case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
      out << mb.getClassName() << " b(this->sig,stran,dstran,this->mprops,mdat,this->temperature_position," << endl
	<< "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->active_clock->get_dtime());" << endl;
      break;
    case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
      out << mb.getClassName() << " b(this->sig,F0,&(this->F[0]),this->mprops,mdat,this->temperature_position," << endl
	<< "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->active_clock->get_dtime());" << endl;
      break;
    default:
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "zmat::ZMATInterface::convert(b.getStiffnessTensor(),this->get_elasticity_matrix(mdat,1.));"<< endl;
    }
    out << "b.initialize();" << endl;
    out << "if(b.integrate(smflag,smtype)!=" << mb.getClassName() << "::SUCCESS){" << endl
	<< "static INTEGRATION_RESULT bad_result;" << endl
	<< "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);" << endl
	<< "return &bad_result;" << endl
	<< "}" << endl
	<< "b.ZMATexportStateData(this->sig,mdat);" << endl
	<< "if(smtype!=" << mb.getClassName() << "::NOSTIFFNESSREQUESTED){" << endl;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator());"<< endl;
    } else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator().get<st2tost2<"
	  << getSpaceDimension(h) << ",double> >());"<< endl;
    } else {
      string msg("ZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "}" << endl
	<< "return NULL;" << endl
	<< "} // end of ZMAT" << mb.getClassName()
	<< "::callMFrontBehaviour" << getSpaceDimensionSuffix(h) << endl << endl;
  }

  void
  ZMATInterface::getTargetsDescription(TargetsDescription& d,
				       const BehaviourDescription& bd){
    const auto lib  = getLibraryName(bd);
    const auto name = bd.getLibrary()+bd.getClassName();
    d.cppflags[lib].push_back("`tfel-config --includes --zmat`");
#pragma message("Linux specific")
    d.cppflags[lib].push_back("-DLinux");
    d.headers.push_back("MFront/ZMAT/ZMAT"+name+".hxx");
    d.sources[lib].push_back("ZMAT"+name+".cxx");
    d.epts[lib].push_back(bd.getClassName());
    d.dependencies[lib].push_back("`tfel-config --libs --material --mfront-profiling`");
  } // end of ZMATInterface::getTargetsDescription

  ZMATInterface::~ZMATInterface()
  {}

} // end of namespace mfront
