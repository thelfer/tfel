/*! 
 * \file  MTestUmatBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/MTestUmatBehaviour.hxx"
#include"MFront/UMAT/UMATComputeStiffnessTensor.hxx"

namespace mfront
{

  MTestUmatBehaviour::MTestUmatBehaviour(const std::string& l,
					 const std::string& b)
  {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getUMATFunction(l,b);
    this->type = elm.getUMATBehaviourType(l,b);
    if(this->type>=2u){
      string msg("MTestUmatBehaviour::MTestUmatBehaviour : "
		 "unsupported behaviour type "
		 "(neither isotropic nor orthotropic)");
      throw(runtime_error(msg));
    }
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b);
    if(this->type==0){
      this->mpnames.insert(this->mpnames.begin(),"ThermalExpansion");
      this->mpnames.insert(this->mpnames.begin(),"MassDensity");
      this->mpnames.insert(this->mpnames.begin(),"PoissonRatio");
      this->mpnames.insert(this->mpnames.begin(),"YoungModulus");
    } else {
      // shall depend on the modelling hypothesis
      string msg("MTestUmatBehaviour::MTestUmatBehaviour : ");
      msg += "orthotropic material are not supported yet";
      throw(runtime_error(msg));
    }
    this->ivnames = elm.getUMATInternalStateVariablesNames(l,b);
    this->ivtypes = elm.getUMATInternalStateVariablesTypes(l,b);
    this->evnames = elm.getUMATExternalStateVariablesNames(l,b);
    this->evnames.insert(this->evnames.begin(),"Temperature");
  }

  std::vector<std::string>
  MTestUmatBehaviour::getMaterialPropertiesNames(void) const
  {
    return this->mpnames;
  }

  std::vector<std::string>
  MTestUmatBehaviour::getInternalStateVariablesNames(void) const
  {
    return this->ivnames;
  }

  std::vector<std::string>
  MTestUmatBehaviour::getExternalStateVariablesNames(void) const
  {
    return this->evnames;
  }

  size_t
  MTestUmatBehaviour::getInternalStateVariablesSize(const unsigned short d) const
  {
    using namespace std;
    vector<int>::const_iterator p;
    size_t s = 0;
    for(p=this->ivtypes.begin();p!=this->ivtypes.end();++p){
      if(*p==0){
	s+=1;
      } else if(*p==1){
	if(d==1){
	  s+=3;
	} else if(d==2){ 
	  s+=4;
	} else if(d==3){ 
	  s+=6;
	} else {
	  string msg("MTestUmatBehaviour::getInternalStateVariablesSize : "
		     "invalid space dimension");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MTestUmatBehaviour::getInternalStateVariablesSize : "
		   "unsupported variable type");
	throw(runtime_error(msg));
      }
    }
    return s;
  } // end of MTestUmatBehaviour::getInternalStateVariablesSize

  std::vector<std::string>
  MTestUmatBehaviour::getInternalStateVariablesDescriptions(const unsigned short d) const
  {
    using namespace std;
    vector<string> desc;
    vector<int>::const_iterator p;
    vector<string>::const_iterator pn;
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("MTestUmatBehaviour::getInternalStateVariablesDescriptions : "
		 "internal error (the number of internal state variables names "
		 "dont match the number of internal state variables types)");
      throw(runtime_error(msg));
    }
    for(p=this->ivtypes.begin(),pn=this->ivnames.begin();
	p!=this->ivtypes.end();++p,++pn){
      if(*p==0){
	desc.push_back(*pn);
      } else if(*p==1){
	desc.push_back("first  component of internal variable '"+*pn+"'");
	desc.push_back("second component of internal variable '"+*pn+"'");
	desc.push_back("third  component of internal variable '"+*pn+"'");
	if(d==2){ 
	  desc.push_back("fourth  component of internal variable '"+*pn+"'");
	} else if(d==3){ 
	  desc.push_back("fourth  component of internal variable '"+*pn+"'");
	  desc.push_back("fifth   component of internal variable '"+*pn+"'");
	  desc.push_back("sixth   component of internal variable '"+*pn+"'");
	} else {
	  string msg("MTestUmatBehaviour::getInternalStateVariablesDescriptions : "
		     "invalid space dimension");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MTestUmatBehaviour::getInternalStateVariablesDescriptions : "
		   "unsupported variable type");
	throw(runtime_error(msg));
      }
    }
    return desc;
  } // end of MTestUmatBehaviour::getInternalStateVariablesDescriptions

  unsigned short
  MTestUmatBehaviour::getInternalStateVariableType(const std::string& n) const
  {
    using namespace std;
    vector<string>::const_iterator p;
    p=find(this->ivnames.begin(),this->ivnames.end(),n);
    if(p==this->ivnames.end()){
      string msg("MTestUmatBehaviour::getInternalStateVariableType : ");
      msg += "no internal variable named '"+n+"' declared";
      throw(runtime_error(msg));
    }
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("MTestUmatBehaviour::getInternalStateVariableType : "
		 "the number of internal variables names and "
		 "the number of internal variables types do not match");
      throw(runtime_error(msg));
    }
    int t = this->ivtypes[p-this->ivnames.begin()];
    if(t==0){
      return 0u;
    } else if(t==1){
      return 1u;
    } else {
      string msg("MTestUmatBehaviour::getInternalStateVariableType : ");
      msg += "unsupported internal variable type";
      throw(runtime_error(msg));
    }
    return 0u;
  }
  
  unsigned short
  MTestUmatBehaviour::getInternalStateVariablePosition(const unsigned short d,
						       const std::string& n) const
  {
    using namespace std;
    vector<string>::const_iterator p;
    p=find(this->ivnames.begin(),this->ivnames.end(),n);
    if(p==this->ivnames.end()){
      string msg("MTestUmatBehaviour::getInternalStateVariablePosition : ");
      msg += "no internal variable named '"+n+"' declared";
      throw(runtime_error(msg));
    }
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("MTestUmatBehaviour::getInternalStateVariablePosition : "
		 "the number of internal variables names and "
		 "the number of internal variables types do not match");
      throw(runtime_error(msg));
    }
    vector<string>::size_type i  = 0;
    vector<string>::size_type ie = p-this->ivnames.begin();
    unsigned short s = 0;
    while(i!=ie){
      int t = this->ivtypes[i];
      if(t==0){
	s += 1;
      } else if(t==1){
	if(d==1u){
	  s += 3u;
	} else if(d==2u){
	  s += 4u;
	} else if(d==3u){
	  s += 6u;
	} else {
	  string msg("MTestUmatBehaviour::getInternalStateVariablePosition : "
		     "invalid dimension");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MTestUmatBehaviour::getInternalStateVariablePosition : "
		   "unsupported internal variable type");
	throw(runtime_error(msg));
      }
    }
    return s;
  }

  bool
  MTestUmatBehaviour::integrate(tfel::math::matrix<real>& Kt,
				tfel::math::tvector<6u,real>& s1,
				tfel::math::vector<real>& iv1,
				const tfel::math::tmatrix<3u,3u,real>& r,
				const tfel::math::tvector<6u,real>& e0,
				const tfel::math::tvector<6u,real>& de,
				const tfel::math::tvector<6u,real>& s0,
				const tfel::math::vector<real>& mp,
				const tfel::math::vector<real>& iv0,
				const tfel::math::vector<real>& ev0,
				const tfel::math::vector<real>& dev,
				const tfel::material::ModellingHypothesis::Hypothesis h,
				const real dt) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace umat;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using umat::UMATComputeStiffnessTensor;
    static const real sqrt2 = sqrt(real(2));
    UMATInt ntens;
    UMATInt ndi;
    UMATInt nprops = static_cast<UMATInt>(mp.size());
    UMATInt nstatv;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
    } else if (h==MH::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
    } else if (h==MH::PLANESTRESS){
      ndi = -2;
      ntens = 4;
    } else if (h==MH::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
    } else if (h==MH::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
    } else {
      string msg("MTestUmatBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    matrix<real> D(Kt.getNbRows(),Kt.getNbCols(),0.);
    vector<real> iv(iv0);
    if(iv.size()==0){
      iv.push_back(0.);
    }
    nstatv = static_cast<UMATInt>(iv.size());
    // rotation matrix
    matrix<real> drot(r.getNbRows(),r.getNbCols(),0.);
    matrix<real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    UMATInt kinc(0);
    tvector<6u,real> ue0(e0);
    tvector<6u,real> ude(de);
    copy(s0.begin(),s0.end(),s1.begin());
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    (this->fct)(&ntens,&dt,&drot(0,0),
		&D(0,0),&ue0(0),&ude(0),
		&ev0(0),&dev(0),
		&mp(0),&nprops,
		&ev0(0)+1,&dev(0)+1,
		&iv(0),&nstatv,&s1(0),
		&ndi,&kinc);
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // tangent operator (...)
//     if(this->hasTangentOpertor){
//     for(i=0;i!=r.getNbRows();++i){
//       for(j=0;j!=r.getNbCols();++j){
//     	Kt(i,j) = D(j,i);
// #warning "something to correct here"
// 	if(i>=3){
// 	  Kt(i,j) *= sqrt2;
// 	}
// 	if(j>=3){
// 	  Kt(i,j) *= sqrt2;
// 	}
//       }
//     }

//     } else {
    if(this->type==0u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	UMATComputeStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	UMATComputeStiffnessTensor<MH::AXISYMMETRICAL,
				   umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRESS){
	st2tost2<2u,real> De;
	UMATComputeStiffnessTensor<MH::PLANESTRESS,
				   umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRAIN){
	st2tost2<2u,real> De;
	UMATComputeStiffnessTensor<MH::PLANESTRAIN,
				   umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	UMATComputeStiffnessTensor<MH::GENERALISEDPLANESTRAIN,
				   umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	UMATComputeStiffnessTensor<MH::TRIDIMENSIONAL,
				   umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=6u;++i){
	  for(j=0;j!=6u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestUmatBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestUmatBehaviour::integrate : ");
      msg += "orthotropic material are not supported yet";
      throw(runtime_error(msg));
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i) /= sqrt2;
    }
    return true;
  } // end of MTestUmatBehaviour::integrate

  MTestUmatBehaviour::~MTestUmatBehaviour()
  {}
  
} // end of namespace mfront

