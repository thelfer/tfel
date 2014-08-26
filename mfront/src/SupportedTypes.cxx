/*!
 * \file   SupportedTypes.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   12 Jan 2007
 */

#include<utility>
#include<stdexcept>

#include"MFront/SupportedTypes.hxx"

namespace mfront{


  const unsigned short SupportedTypes::ArraySizeLimit = 10u;

  SupportedTypes::TypeSize::TypeSize()
    : scalarSize(0u),
      tvectorSize(0u),
      stensorSize(0u),
      tensorSize(0u)
  {}
  
  SupportedTypes::TypeSize::TypeSize(const SupportedTypes::TypeSize& src)
    : scalarSize(src.scalarSize),
      tvectorSize(src.tvectorSize),
      stensorSize(src.stensorSize),
      tensorSize(src.tensorSize)
  {}
  
  SupportedTypes::TypeSize::TypeSize(const ushort a,
				     const ushort b,
				     const ushort c,
				     const ushort d)				     
    : scalarSize(a),
      tvectorSize(b),
      stensorSize(c),
      tensorSize(d)
  {}
  
  SupportedTypes::TypeSize&
  SupportedTypes::TypeSize::operator=(const SupportedTypes::TypeSize& src)
  {
    scalarSize  = src.scalarSize;
    tvectorSize = src.tvectorSize;
    stensorSize = src.stensorSize;
    tensorSize  = src.tensorSize;
    return *this;
  }
  
  SupportedTypes::TypeSize&
  SupportedTypes::TypeSize::operator+=(const SupportedTypes::TypeSize& src)
  {
    scalarSize  = static_cast<unsigned short>(scalarSize+src.scalarSize);
    tvectorSize = static_cast<unsigned short>(tvectorSize+src.tvectorSize);
    stensorSize = static_cast<unsigned short>(stensorSize+src.stensorSize);
    tensorSize = static_cast<unsigned short>(tensorSize+src.tensorSize);
    return *this;
  }

  std::ostream&
  operator << (std::ostream& os,const SupportedTypes::TypeSize& size)
  {
    bool first = true;
    if(size.scalarSize!=0u){
      os << size.scalarSize;
      first = false;
    }
    if(size.tvectorSize!=0u){
      if(!first){
	os << "+";
      }
      if(size.tvectorSize==1u){
	os << "TVectorSize";
      } else {
	os << size.tvectorSize << "*TVectorSize";
      }
      first = false;
    }
    if(size.stensorSize!=0u){
      if(!first){
	os << "+";
      }
      if(size.stensorSize==1u){
	os << "StensorSize";
      } else {
	os << size.stensorSize << "*StensorSize";
      }
      first = false;
    }
    if(size.tensorSize!=0u){
      if(!first){
	os << "+";
      }
      if(size.tensorSize==1u){
	os << "TensorSize";
      } else {
	os << size.tensorSize << "*TensorSize";
      }
      first = false;
    }
    if(first){
      os << "0u";
    }
    return os;
  }
  
  void
  SupportedTypes::registerTypes(void)
  {
    using namespace std;
    typedef map<string,TypeFlag>::value_type MVType;
    this->flags.insert(MVType("real",Scalar));
    this->flags.insert(MVType("frequency",Scalar));
    this->flags.insert(MVType("stress",Scalar));
    //    this->flags.insert(MVType("stressrate",Scalar));
    this->flags.insert(MVType("strain",Scalar));
    this->flags.insert(MVType("strainrate",Scalar));
    this->flags.insert(MVType("temperature",Scalar));
    this->flags.insert(MVType("energy_density",Scalar));
    this->flags.insert(MVType("thermalexpansion",Scalar));
    this->flags.insert(MVType("density",Scalar));
    this->flags.insert(MVType("TVector",TVector));
    this->flags.insert(MVType("Stensor",Stensor));
    this->flags.insert(MVType("Tensor",Tensor));
    this->flags.insert(MVType("StressStensor",Stensor));
    this->flags.insert(MVType("StressRateStensor",Stensor));
    this->flags.insert(MVType("StrainStensor",Stensor));
    this->flags.insert(MVType("StrainRateStensor",Stensor));
    // CZM
    this->flags.insert(MVType("DisplacementTVector",
			      TVector));
    this->flags.insert(MVType("ForceTVector",
			      TVector));
    // Finite Strain
    this->flags.insert(MVType("DeformationGradientTensor",Tensor));
  }

  SupportedTypes::SupportedTypes()
    : areDynamicallyAllocatedVectorsAllowed_(true)
  {
    this->registerTypes();
  }

  void
  SupportedTypes::reset(void)
  {
    this->areDynamicallyAllocatedVectorsAllowed_ = true;
  } // end of SupportedTypes::reset

  SupportedTypes::TypeFlag
  SupportedTypes::getTypeFlag(const std::string& name) const
  {
    using namespace std;
    map<string,TypeFlag>::const_iterator p;
    p = flags.find(name);
    if(p==flags.end()){
      string msg("SupportedTypes::getTypeTag ");
      msg += name;
      msg += " is not a supported type.";
      throw(runtime_error(msg));
    }
    return p->second;
  }

  SupportedTypes::TypeSize
  SupportedTypes::getTypeSize(const std::string& type,
			      const unsigned short a) const
  {
    using namespace std;
    TypeSize res;
    switch(this->getTypeFlag(type)){
    case Scalar : 
      res=TypeSize(a,0u,0u,0u);
      break;
    case TVector : 
      res=TypeSize(0u,a,0u,0u);
      break;
    case Stensor :
      res=TypeSize(0u,0u,a,0u);
      break;
    case Tensor : 
      res=TypeSize(0u,0u,0u,a);
      break;
    default : 
      throw(runtime_error("SupportedTypes::getTypeSize : internal error."));
    }
    return res;
  }

  std::string
  SupportedTypes::getTimeDerivativeType(const std::string& type) const
  {
    using namespace std;
    if (type=="real"){
      return "frequency";
    } else if(type=="strain"){
      return "strainrate";
    } else if (type=="stress"){
      return "stressrate";
    } else if (type=="Stensor"){
      return "FrequencyStensor";
    } else if (type=="StressStensor"){
      return "StressRateStensor";
    } else if (type=="StrainStensor"){
      return "StrainRateStensor";
    } else {
      string msg("SupportedTypes::getTimeDerivativeType : ");
      msg+="internal error, unsupported type ";
      msg+=type;
      throw(runtime_error(msg));
    }
  }

  SupportedTypes::TypeSize::ushort
  SupportedTypes::TypeSize::getScalarSize(void) const
  {
    return this->scalarSize;
  }

  SupportedTypes::TypeSize::ushort
  SupportedTypes::TypeSize::getTVectorSize(void) const
  {
    return this->tvectorSize;
  }
    
  SupportedTypes::TypeSize::ushort
  SupportedTypes::TypeSize::getStensorSize(void) const
  {
    return this->stensorSize;
  }

  SupportedTypes::TypeSize::ushort
  SupportedTypes::TypeSize::getTensorSize(void) const
  {
    return this->tensorSize;
  }

  bool
  SupportedTypes::TypeSize::isNull(void) const
  {
    return ((this->getScalarSize()==0)&&
	    (this->getStensorSize()==0)&&
	    (this->getTVectorSize()==0)&&
	    (this->getTensorSize()==0));
  } // end of SupportedTypes::TypeSize::isNull

  SupportedTypes::TypeSize
  SupportedTypes::writeVariableInitializersInBehaviourDataConstructorI(std::ostream& f,
								       const VarContainer& v,
								       const std::string& src,
								       const std::string& prefix,
								       const std::string& suffix,
								       const SupportedTypes::TypeSize& o) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize currentOffset = o;
    if(!v.empty()){
      for(p=v.begin();p!=v.end();++p){
	if(p->arraySize==1u){
	  const string n = prefix+p->name+suffix;
	  f << ",\n";
	  SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	  if(flag==SupportedTypes::Scalar){
	    f << n << "("+src+"[" 
	      << currentOffset << "])";  
	  } else if((flag==SupportedTypes::TVector)||
		    (flag==SupportedTypes::Stensor)||
		    (flag==SupportedTypes::Tensor)){
	    f << n << "(&"+src+"[" 
	      << currentOffset << "])";  
	  } else {
	    string msg("SupportedTypes::");
	    msg += "writeVariableInitializersInBehaviourDataConstructorI : ";
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	}
	currentOffset+=this->getTypeSize(p->type,p->arraySize);
      }
    }
    return currentOffset;
  } // end of SupportedTypes::writeVariableInitializersInBehaviourDataConstructorI

  bool
  SupportedTypes::useDynamicallyAllocatedVector(const unsigned short s) const
  {
    return (s>=SupportedTypes::ArraySizeLimit)&&(this->areDynamicallyAllocatedVectorsAllowed());
  } // end of SupportedTypes::useDynamicallyAllocatedVector

  bool
  SupportedTypes::areDynamicallyAllocatedVectorsAllowed(void) const
  {
    return this->areDynamicallyAllocatedVectorsAllowed_;
  } // end of SupportedTypes::areDynamicallyAllocatedVectorsAllowed

  SupportedTypes::TypeSize
  SupportedTypes::writeVariableInitializersInBehaviourDataConstructorII(std::ostream& f,
									const VarContainer& v,
									const std::string& src,
									const std::string& prefix,
									const std::string& suffix,
									const SupportedTypes::TypeSize& o) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize currentOffset = o;
    if(!v.empty()){
      for(p=v.begin();p!=v.end();++p){
	if(p->arraySize==1u){
	  currentOffset+=this->getTypeSize(p->type,p->arraySize);
	} else {
	  const SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	  const string n = prefix+p->name+suffix;
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    f << n << ".resize(" << p->arraySize << ");" << endl;
	    f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	    switch(flag){
	    case SupportedTypes::Scalar : 
	      f << n << "[idx] = "+src+"[" 
		<< currentOffset << "+idx];\n";  
	      break;
	    case SupportedTypes::TVector :
	      f << "tfel::fsalgo<TVectorSize>(&"+src+"[" 
		<< currentOffset << "+idx*TVectorSize],"
		<< n << "[idx].begin());\n";  
	      break;
	    case SupportedTypes::Stensor :
	      f << n << "[idx].import(&"+src+"[" 
		  << currentOffset << "+idx*StensorSize]);\n";  
	      break;
	    case SupportedTypes::Tensor :
	      f << "tfel::fsalgo<TensorSize>(&"+src+"[" 
		<< currentOffset << "+idx*TensorSize],"
		<< n << "[idx].begin());\n";  
	      break;
	    default : 
	      string msg("SupportedTypes::");
	      msg += "writeVariableInitializersInBehaviourDataConstructorII : ";
	      msg += "internal error, tag unsupported";
	      throw(runtime_error(msg));
	    }
	    f << "}" << endl;
	    currentOffset+=this->getTypeSize(p->type,p->arraySize);
	  } else {
	    for(int i=0;i!=p->arraySize;++i){
	      switch(flag){
	      case SupportedTypes::Scalar : 
		f << n << "[" << i << "] = "+src+"[" 
		  << currentOffset << "];\n";  
		break;
	      case SupportedTypes::TVector :
		f << "tfel::fsalgo<TVectorSize>(&"+src+"[" 
		  << currentOffset << "]," << n << "[" << i << "].begin());\n";  
		break;
	      case SupportedTypes::Stensor :
		f << n << "["<< i << "].import(&"+src+"[" 
		  << currentOffset << "]);\n";  
		break;
	      case SupportedTypes::Tensor :
		f << "tfel::fsalgo<TensorSize>(&"+src+"[" 
		  << currentOffset << "]," << n << "[" << i << "].begin());\n";  
		break;
	      default : 
		string msg("SupportedTypes::");
		msg += "writeVariableInitializersInBehaviourDataConstructorII : ";
		msg += "internal error, tag unsupported";
		throw(runtime_error(msg));
	      }
	      currentOffset+=this->getTypeSize(p->type,1u);
	    }
	  }
	}
      }
    }
    return currentOffset;
  } // end of SupportedTypes::writeVariableInitializersInBehaviourDataConstructorII

  void
  SupportedTypes::writeStateVariableIncrementsInitializers(std::ostream& f,
							   const VarContainer& v,
							   const bool useStateVarTimeDerivative) const 
  {
    using namespace std;
    VarContainer::const_iterator p;
    if(!v.empty()){
      for(p=v.begin();p!=v.end();++p){
	SupportedTypes::TypeFlag flag = getTypeFlag(p->type);
	const string n = p->name;
	const string t = (!useStateVarTimeDerivative) ? p->type : this->getTimeDerivativeType(p->type);
	f << ",\n";
	if(flag==SupportedTypes::Scalar){
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    f << "d" << n << "(" << p->arraySize << "," << t <<"(0))";
	  } else {
	    f << "d" << n << "(" << t <<"(0))";
	  }
	} else if ((flag==SupportedTypes::TVector)||
		   (flag==SupportedTypes::Stensor)||
		   (flag==SupportedTypes::Tensor)){
	  string traits;
	  if(flag==SupportedTypes::TVector){
	    traits = "VectorTraits";
	  } else if(flag==SupportedTypes::Stensor){
	    traits = "StensorTraits";
	  } else if(flag==SupportedTypes::Tensor){
	    traits = "TensorTraits";
	  } else {
	    string msg("SupportedTypes::writeStateVariableIncrementsInitializers : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	  if(p->arraySize==1u){
	    f << "d" << n 
	      << "(typename tfel::math::"+traits+"<" 
	      << t << ">::NumType(0))";
	  } else {
	    if(this->useDynamicallyAllocatedVector(p->arraySize)){
	      f << "d" << n 
		<< "(" << p->arraySize << "," << t << "(typename tfel::math::"+traits+"<" 
		<< t << ">::NumType(0)))";
	    } else {
	      f << "d" << n 
		<< "(" << t << "(typename tfel::math::"+traits+"<" 
		<< t << ">::NumType(0)))";
	    }
	  }
	} else {
	  string msg("SupportedTypes::writeStateVariableIncrementsInitializers : ");
	  msg += "internal error, tag unsupported";
	  throw(runtime_error(msg));
	}
      }
    }
    
  } // end of SupportedTypes::writeStateVariableIncrementsInitializers

  SupportedTypes::TypeSize
  SupportedTypes::getTotalSize(const VarContainer& v) const
  {
    SupportedTypes::TypeSize s;
    VarContainer::const_iterator p;
    for(p =v.begin();p!=v.end();++p){
      s+=this->getTypeSize(p->type,p->arraySize);
    }
    return s;
  } // end of SupportedTypes::getTotalSize

  unsigned short
  SupportedTypes::getNumberOfVariables(const VarContainer& v) const
  {
    unsigned short n = 0u;
    VarContainer::const_iterator p;
    for(p =v.begin();p!=v.end();++p){
      n = n + p->arraySize;
    }
    return n;
  } // end of SupportedTypes::getNumberOfVariables

  void
  SupportedTypes::writeResultsArrayResize(std::ostream& f,
					  const std::string& dest,
					  const VarContainer& v) const
  {
    this->writeResultsArrayResize(f,dest,this->getTotalSize(v));
  } // end of SupportedTypes::writeResultsArrayResize

  void
  SupportedTypes::writeResultsArrayResize(std::ostream& f,
					  const std::string& dest,
					  const SupportedTypes::TypeSize& s) const
  {
    f << "if(" << dest << "!=" << s << "){\n";
    f << dest+".resize(" << s <<");\n";
    f << "}\n"; 
  }

  SupportedTypes::TypeSize
  SupportedTypes::exportResults(std::ostream& f,
				const VarContainer& v,
				const std::string& dest,
				const bool useQt,
				const SupportedTypes::TypeSize& o) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize currentOffset = o;
    if(!v.empty()){
      for(p=v.begin();p!=v.end();++p){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	if(p->arraySize==1u){
	  if(flag==SupportedTypes::Scalar){
	    if(useQt){
	      f << dest << "[" 
		<< currentOffset << "] = common_cast(this->"
		<< p->name << ");\n"; 
	    } else {
	      f << dest << "[" 
		<< currentOffset << "] = this->"
		<< p->name << ";\n"; 
	    } 
	  } else if((flag==SupportedTypes::TVector)||
		    (flag==SupportedTypes::Stensor)||
		    (flag==SupportedTypes::Tensor)){
	    f << "exportToBaseTypeArray(" << p->name 
	      << ",&" << dest << "[" 
	      << currentOffset << "]);\n";  
	  } else {
	    string msg("SupportedTypes::exportResults : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	  currentOffset+=this->getTypeSize(p->type,p->arraySize);
	} else {
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	    if(flag==SupportedTypes::Scalar){ 
	      if(useQt){
		f << dest << "[" 
		  << currentOffset << "+idx] = common_cast(this->"
		  << p->name << "[idx]);\n"; 
	      } else {
		f << dest << "[" 
		  << currentOffset << "+idx] = this->"
		  << p->name << "[idx];\n"; 
	      }
	    } else if((flag==SupportedTypes::TVector)||
		      (flag==SupportedTypes::Stensor)||
		      (flag==SupportedTypes::Tensor)){
	      f << "exportToBaseTypeArray(this->" << p->name
		<< "[idx],&" << dest << "[" 
		<< currentOffset << "+idx*StensorSize]);\n";  
	    } else {
	      string msg("SupportedTypes::exportResults : ");
	      msg += "internal error, tag unsupported";
	      throw(runtime_error(msg));
	    }
	    f << "}" << endl;
	    currentOffset+=this->getTypeSize(p->type,p->arraySize);
	  } else {
	    for(unsigned short i=0;i!=p->arraySize;++i){
	      if(flag==SupportedTypes::Scalar){
		if(useQt){
		  f << dest << "[" 
		    << currentOffset << "] = common_cast(this->"
		    << p->name << "[" << i << "]);\n"; 
		} else {
		  f << dest << "[" 
		    << currentOffset << "] = this->"
		    << p->name << "[" << i << "];\n"; 
		} 
	      } else if((flag==SupportedTypes::TVector)||
			(flag==SupportedTypes::Stensor)||
			(flag==SupportedTypes::Tensor)){
		f << "exportToBaseTypeArray(this->" << p->name
		  << "[" << i << "],&" << dest << "[" 
		  << currentOffset << "]);\n";  
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
    return currentOffset;
  }

  void
  SupportedTypes::writeVariablesDeclarations(std::ostream& f,
					     const VarContainer& v,
					     const std::string& prefix,
					     const std::string& suffix,
					     const std::string& fileName,
					     const bool useTimeDerivative,
					     const bool b) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      const string n = prefix+p->name+suffix;
      const string t = (!useTimeDerivative) ? p->type :  this->getTimeDerivativeType(p->type);
      if((!b)&&(p->lineNumber!=0u)){
	f << "#line " << p->lineNumber << " \"" 
	  << fileName << "\"\n";
      }
      if(p->arraySize==1u){
	f << t << " "  << n << ";\n";  
      } else {
	if(this->useDynamicallyAllocatedVector(p->arraySize)){
	  f << "tfel::math::vector<" << t << " > "  << n << ";\n";
	} else {
	  f << "tfel::math::tvector<" << p->arraySize 
	    << ", " << t << " > "  << n << ";\n";
	}
      }
    }
  } // end of SupportedTypes::writeVariablesDeclarations

  SupportedTypes::~SupportedTypes()
  {} // end of SupportedTypes::~SupportedTypes

} // end of namespace mfront
