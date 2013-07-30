/*! 
 * \file  MTestFileGeneratorBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 juil. 2013
 */

#include<fstream>
#include<stdexcept>

#include"TFEL/Utilities/ToString.hxx"
#include"MFront/MTestFileGeneratorBase.hxx"

namespace mfront{

  unsigned int
  MTestFileGeneratorBase::getIdentifier(void)
  {
    static unsigned int i = 0;
    return ++i;
  }

  MTestFileGeneratorBase::MTestFileGeneratorBase()
    : hypothesis(tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS)
  {} // end of MTestFileGeneratorBase::MTestFileGeneratorBase

  void
  MTestFileGeneratorBase::setModellingHypothesis(tfel::material::ModellingHypothesis::Hypothesis h)
  {
    using namespace std;
    using namespace tfel::material;
    if(this->hypothesis!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::setModellingHypothesis : "
		 "modelling hypothesis already set");
      throw(runtime_error(msg));
    }
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::setModellingHypothesis : "
		 "invalid modelling hypothesis");
      throw(runtime_error(msg));
    }
    this->hypothesis = h;
  } // end of MTestFileGeneratorBase::setModellingHypothesis

  void
  MTestFileGeneratorBase::addTime(const MTestFileGeneratorBase::real t)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!this->times.insert(t).second){
      string msg("MTestFileGeneratorBase::addTime : "
		 "time '"+ToString(t)+"' already defined");
      throw(runtime_error(msg));
    }
  } // end of MTestFileGeneratorBase::addTime

  void
  MTestFileGeneratorBase::addMaterialProperty(const std::string& n,
					      const MTestFileGeneratorBase::real v)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,real>::value_type MVType;
    if(!(this->mps.insert(MVType(n,v)).second)){
      string msg("MTestFileGeneratorBase::addMaterialProperty : "
		 "material property '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  } // end of MTestFileGeneratorBase::addMaterialProperty

  void
  MTestFileGeneratorBase::addInternalStateVariable(const std::string& n,
						   const SupportedTypes::TypeFlag f,
						   const MTestFileGeneratorBase::real* const v)
  {
    using namespace std;
    InternalStateVariable iv;
    vector<InternalStateVariable>::const_iterator p;
    for(p=this->ivs.begin();p!=this->ivs.end();++p){
      if(p->name==n){
	string msg("MTestFileGeneratorBase::addInternalStateVariable : "
		   "variable already declared '"+n+"'");
	throw(runtime_error(msg));
      }
    }
    iv.name = n;
    iv.type = f;
    if(iv.type==SupportedTypes::Scalar){
      iv.values[0] = v[0];
    } else if(iv.type==SupportedTypes::Stensor){
      copy(v,v+this->getStensorSize(),iv.values);
    } else {
      string msg("MTestFileGeneratorBase::addInternalStateVariable : "
		 "unsupported type for variable '"+n+"'");
      throw(runtime_error(msg));
    }
    this->ivs.push_back(iv);
  } // end of MTestFileGeneratorBase::addInternalStateVariable

  void
  MTestFileGeneratorBase::addExternalStateVariableValue(const std::string& n,
							const MTestFileGeneratorBase::real t,
							const MTestFileGeneratorBase::real v)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<real,real>::value_type MVType;
    if(!(this->evs[n].insert(MVType(t,v)).second)){
      string msg("MTestFileGeneratorBase::addExternalStateVariableValue : "
		 "time '"+ToString(t)+"' already defined "
		 "for variable '"+n+"'");
      throw(runtime_error(msg));
    }
  } // end of MTestFileGeneratorBase::addValue

  void
  MTestFileGeneratorBase::generate(const std::string& n) const
  {
    using namespace std;
    using namespace tfel::utilities;
    ofstream file((n+"-"+ToString(getIdentifier())+".mtest").c_str());
    if(!file){
      string msg("MTestFileGeneratorBase::generate : "
		 "can't open file '"+n+".mtest'");
      throw(runtime_error(msg));
    }
    this->writeBehaviourDeclaration(file);
    this->writeModellingHypothesis(file);
    this->writeMaterialProperties(file);
    this->writeInternalStateVariables(file);
    this->writeExternalStateVariables(file);
    this->writeDrivingVariables(file);
    this->writeTimes(file);
  } // end of MTestFileGeneratorBase::generate

  void
  MTestFileGeneratorBase::writeModellingHypothesis(std::ostream& os) const
  {
    using namespace std;
    using namespace tfel::material;
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::writeModellingHypothesis : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    os << "@ModellingHypothesis '";
    switch(this->hypothesis){
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      os << "AxisymmetricalGeneralisedPlaneStrain";
      break;
    case ModellingHypothesis::AXISYMMETRICAL:
      os << "Axisymmetrical";
      break;
    case ModellingHypothesis::PLANESTRESS:
      os << "PlaneStress";
      break;
    case ModellingHypothesis::PLANESTRAIN:
      os << "PlaneStrain";
      break;
    case ModellingHypothesis::GENERALISEDPLANESTRAIN:
      os << "GeneralisedPlaneStrain";
      break;
    case ModellingHypothesis::TRIDIMENSIONAL:
      os << "Tridimensional";
      break;
    default:
      string msg("MTestFileGeneratorBase::writeModellingHypothesis : "
		 "unsupported hypothesis'");
      throw(runtime_error(msg));
    }
    os << "';" << endl;
  }

  void
  MTestFileGeneratorBase::writeMaterialProperties(std::ostream& os) const
  {
    using namespace std;
    map<string,real>::const_iterator p;
    if(!this->mps.empty()){
      os << "// Material properties" << endl;
      for(p=this->mps.begin();p!=this->mps.end();++p){
	os.precision(14);
	os << "@MaterialProperty<constant> '" << p->first << "' " << p->second << ";" << endl;
      }
      os << endl;
    }
  } // end of MTestFileGeneratorBase::writeMaterialProperties

  void
  MTestFileGeneratorBase::writeTimes(std::ostream& os) const
  {
    using namespace std;
    set<real>::const_iterator p;
    if(this->times.empty()){
      string msg("MTestFileGeneratorBase::writeTimes : "
		 "no times defined");
      throw(runtime_error(msg));
    }
    if(this->times.size()<2){
      string msg("MTestFileGeneratorBase::writeTimes : "
		 "only one time given");
      throw(runtime_error(msg));
    }
    os << "// Times" << endl;
    os << "@Times {";
    os.precision(14);
    for(p=this->times.begin();p!=times.end();){
      os << *p;
      if(++p!=times.end()){
	os << ", ";
      }
    }
    os << "};" << endl << endl;
  } // end of MTestFileGeneratorBase::writeTimes

  void
  MTestFileGeneratorBase::writeInternalStateVariables(std::ostream& os) const
  {
    using namespace std;
    vector<InternalStateVariable>::const_iterator p;
    if(!this->ivs.empty()){
      os << "// Internal state variables" << endl;
      for(p=this->ivs.begin();p!=this->ivs.end();++p){
	const InternalStateVariable& iv = *p;
	os << "@InternalStateVariable '" << iv.name << "' ";
	os.precision(14);
	if(iv.type==SupportedTypes::Scalar){
	  os << iv.values[0] << ";" << endl;
	} else if(iv.type==SupportedTypes::Stensor){
	  os << "{";
	  for(unsigned short i=0;i!=this->getStensorSize();){
	    os << iv.values[i];
	    if(++i!=this->getStensorSize()){
	      os << ",";
	    }
	  }
	  os << "};" << endl;
	} else {
	  string msg("MTestFileGeneratorBase::writeInternalStateVariables : "
		     "unsupported internal state variable type");
	  throw(runtime_error(msg));
	}
      }
      os << endl;
    }
  } // end of MTestFileGeneratorBase::writeMaterialProperties

  void
  MTestFileGeneratorBase::writeExternalStateVariables(std::ostream& os) const
  {
    using namespace std;
    map<string,map<real,real> >::const_iterator p;
    if(!this->evs.empty()){
      os << "// External state variables" << endl;
      for(p=this->evs.begin();p!=this->evs.end();++p){
	const string& n                = p->first;
	const map<real,real>& values = p->second;
	if(values.size()==1){
	  os.precision(14);
	  os << "@ExternalStateVariable '" << n<< "' " << values.begin()->second << endl;
	} else {
	  map<real,real>::const_iterator pv;
	  os.precision(14);
	  os << "@ExternalStateVariable<evolution> '" << n<< "' {" ;
	  for(pv=values.begin();pv!=values.end();){
	    os << pv->first << " : " << pv->second;
	    if(++pv!=values.end()){
	      os << ",\n";	    
	    }
	  }
	  os << "};" << endl;
	}
      }
      os << endl;
    }
  } // end of MTestFileGeneratorBase::writeExternalStateVariables

  std::vector<std::string>
  MTestFileGeneratorBase::getStrainComponentsNames() const
  {
    using namespace std;
    using namespace tfel::material;
    static const string exts[6u]  = {"EXX","EYY","EZZ",
				     "EXY","EXZ","EYZ"};
    static const string aexts[4u] = {"ERR","EZZ","ETT","ERZ"};  
    vector<string> n;
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::getStrainComponentsNames : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if(this->hypothesis==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      copy(aexts,aexts+3u,
	   back_inserter(n));
    } else if(this->hypothesis==ModellingHypothesis::AXISYMMETRICAL){
      copy(aexts,aexts+4u,
	   back_inserter(n));
    } else if((this->hypothesis==ModellingHypothesis::PLANESTRESS)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRAIN)||
	      (this->hypothesis==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      copy(exts,exts+4u,
	   back_inserter(n));
    } else if(this->hypothesis==ModellingHypothesis::TRIDIMENSIONAL){
      copy(exts,exts+6u,
	   back_inserter(n));
    } else {
      string msg("MTestFileGeneratorBase::getStrainComponentsNames : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    return n;
  }

  std::vector<std::string>
  MTestFileGeneratorBase::getStressComponentsNames() const
  {
    using namespace std;
    using namespace tfel::material;
    static const string exts[6u]  = {"SXX","SYY","SZZ",
				     "SXY","SXZ","SYZ"};
    static const string aexts[4u] = {"SRR","SZZ","STT","SRZ"};  
    vector<string> n;
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::getStressComponentsNames : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if(this->hypothesis==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      copy(aexts,aexts+3u,
	   back_inserter(n));
    } else if(this->hypothesis==ModellingHypothesis::AXISYMMETRICAL){
      copy(aexts,aexts+4u,
	   back_inserter(n));
    } else if((this->hypothesis==ModellingHypothesis::PLANESTRESS)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRAIN)||
	      (this->hypothesis==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      copy(exts,exts+4u,
	   back_inserter(n));
    } else if(this->hypothesis==ModellingHypothesis::TRIDIMENSIONAL){
      copy(exts,exts+6u,
	   back_inserter(n));
    } else {
      string msg("MTestFileGeneratorBase::getStressComponentsNames : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    return n;
  }

  unsigned short
  MTestFileGeneratorBase::getTVectorSize() const
  {
    using namespace std;
    using namespace tfel::material;
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::getTVectorSize : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if(this->hypothesis==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 1u;
    } else if((this->hypothesis==ModellingHypothesis::AXISYMMETRICAL)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRESS)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRAIN)||
	      (this->hypothesis==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      return 2u;
    } else if(this->hypothesis==ModellingHypothesis::TRIDIMENSIONAL){
      return 3u;
    }
    string msg("MTestFileGeneratorBase::getTVectorSize : ");
    msg += "unsupported modelling hypothesis";
    throw(runtime_error(msg));
    return 0u;
  } // end of MTestFileGeneratorBase::getTVectorSize

  unsigned short
  MTestFileGeneratorBase::getStensorSize() const
  {
    using namespace std;
    using namespace tfel::material;
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::getStensorSize : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if(this->hypothesis==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 3u;
    } else if((this->hypothesis==ModellingHypothesis::AXISYMMETRICAL)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRESS)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRAIN)||
	      (this->hypothesis==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      return 4u;
    } else if(this->hypothesis==ModellingHypothesis::TRIDIMENSIONAL){
      return 6u;
    }
    string msg("MTestFileGeneratorBase::getStensorSize : ");
    msg += "unsupported modelling hypothesis";
    throw(runtime_error(msg));
    return 0u;
  } // end of MTestFileGeneratorBase::getStensorSize

  unsigned short
  MTestFileGeneratorBase::getTensorSize() const
  {
    using namespace std;
    using namespace tfel::material;
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      string msg("MTestFileGeneratorBase::getTensorSize : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if(this->hypothesis==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 3u;
    } else if((this->hypothesis==ModellingHypothesis::AXISYMMETRICAL)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRESS)||
	      (this->hypothesis==ModellingHypothesis::PLANESTRAIN)||
	      (this->hypothesis==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      return 5u;
    } else if(this->hypothesis==ModellingHypothesis::TRIDIMENSIONAL){
      return 9u;
    }
    string msg("MTestFileGeneratorBase::getTensorSize : ");
    msg += "unsupported modelling hypothesis";
    throw(runtime_error(msg));
    return 0u;
  } // end of MTestFileGeneratorBase::getTensorSize

  MTestFileGeneratorBase::~MTestFileGeneratorBase()
  {} // end of MTestFileGeneratorBase::~MTestFileGeneratorBase

} // end of namespace mfront
