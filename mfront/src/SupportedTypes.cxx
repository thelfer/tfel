/*!
 * \file   SupportedTypes.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   12 Jan 2007
 */

#include<utility>
#include<stdexcept>
#include"SupportedTypes.hxx"

namespace mfront{

  
  SupportedTypes::TypeSize::TypeSize()
    : scalarSize(0u),
      stensorSize(0u)
  {}
  
  SupportedTypes::TypeSize::TypeSize(const SupportedTypes::TypeSize& src)
    : scalarSize(src.scalarSize),
      stensorSize(src.stensorSize)
  {}
  
  SupportedTypes::TypeSize::TypeSize(const ushort a,const ushort b)
    : scalarSize(a),
      stensorSize(b)
  {}
  
  SupportedTypes::TypeSize&
  SupportedTypes::TypeSize::operator=(const SupportedTypes::TypeSize& src)
  {
    scalarSize  = src.scalarSize;
    stensorSize = src.stensorSize;
    return *this;
  }
  
  SupportedTypes::TypeSize&
  SupportedTypes::TypeSize::operator+=(const SupportedTypes::TypeSize& src)
  {
    scalarSize  = static_cast<unsigned short>(scalarSize+src.scalarSize);
    stensorSize = static_cast<unsigned short>(stensorSize+src.stensorSize);
    return *this;
  }

  std::ostream&
  operator << (std::ostream& os,const SupportedTypes::TypeSize& size)
  {
    if((size.scalarSize==0u)&&(size.stensorSize==0u)){
      os << 0u;
    } else {
      if(size.scalarSize!=0u){
	os << size.scalarSize;
	if(size.stensorSize!=0u){
	  if(size.stensorSize==1u){
	    os << "+StensorSize";
	  } else {
	    os << "+" << size.stensorSize << "*StensorSize";
	  }
	}
      } else{
	if(size.stensorSize==1u){
	  os << "StensorSize";
	} else {
	  os << size.stensorSize << "*StensorSize";
	}
      }
    }
    return os;
  }
  
  void
  SupportedTypes::registerTypes(void)
  {
    using namespace std;
    this->flags.insert(make_pair("real",Scalar));
    this->flags.insert(make_pair("frequency",Scalar));
    this->flags.insert(make_pair("stress",Scalar));
    this->flags.insert(make_pair("stress_rate",Scalar));
    this->flags.insert(make_pair("strain",Scalar));
    this->flags.insert(make_pair("strain_rate",Scalar));
    this->flags.insert(make_pair("temperature",Scalar));
    this->flags.insert(make_pair("energy_density",Scalar));
    this->flags.insert(make_pair("thermalexpansion",Scalar));
    this->flags.insert(make_pair("density",Scalar));
    this->flags.insert(make_pair("Stensor",Stensor));
    this->flags.insert(make_pair("StressStensor",Stensor));
    this->flags.insert(make_pair("StressRateStensor",Stensor));
    this->flags.insert(make_pair("StrainStensor",Stensor));
    this->flags.insert(make_pair("StrainRateStensor",Stensor));
  }

  SupportedTypes::SupportedTypes()
  {
    this->registerTypes();
  }

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
  SupportedTypes::getTypeSize(const std::string& name) const
  {
    using namespace std;
    map<string,TypeFlag>::const_iterator p;
    TypeSize res;

    p = flags.find(name);

    if(p==flags.end()){
      string msg("SupportedTypes::getTypeSize : internal error.\nType '");
      msg += name;
      msg += "' is not supported";
      throw(runtime_error(msg));
    }

    switch(p->second){
    case Scalar : 
      res=TypeSize(1u,0u);
      break;
    case Stensor :
      res=TypeSize(0u,1u);
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
  SupportedTypes::TypeSize::getStensorSize(void) const
  {
    return this->stensorSize;
  }

} // end of namespace mfront
