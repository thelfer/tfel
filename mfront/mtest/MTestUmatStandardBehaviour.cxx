/*!
 * \file   mfront/mtest/MTestUmatStandardBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 */

#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestUmatStandardBehaviour.hxx"

namespace mfront
{
  
  MTestUmatStandardBehaviour::MTestUmatStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
							 const std::string& l,
							 const std::string& b)
    : MTestUmatBehaviourBase(h,l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getUMATFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,this->hypothesis);
    if(this->stype==0){
      this->mpnames.insert(this->mpnames.begin(),"ThermalExpansion");
      this->mpnames.insert(this->mpnames.begin(),"MassDensity");
      this->mpnames.insert(this->mpnames.begin(),"PoissonRatio");
      this->mpnames.insert(this->mpnames.begin(),"YoungModulus");
    } else {
      vector<string> tmp;
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	tmp.push_back("YoungModulus1");
	tmp.push_back("YoungModulus2");
	tmp.push_back("YoungModulus3");
	tmp.push_back("PoissonRatio12");
	tmp.push_back("PoissonRatio23");
	tmp.push_back("PoissonRatio13");
	tmp.push_back("MassDensity");
	tmp.push_back("ThermalExpansion1");
	tmp.push_back("ThermalExpansion2");
	tmp.push_back("ThermalExpansion3");
      } else if(h==ModellingHypothesis::PLANESTRESS){
	tmp.push_back("YoungModulus1");
	tmp.push_back("YoungModulus2");
	tmp.push_back("PoissonRatio12");
	tmp.push_back("ShearModulus12");
	tmp.push_back("V1X");
	tmp.push_back("V1Y");
	tmp.push_back("YoungModulus3");
	tmp.push_back("PoissonRatio23");
	tmp.push_back("PoissonRatio13");
	tmp.push_back("MassDensity");
	tmp.push_back("ThermalExpansion1");
	tmp.push_back("ThermalExpansion2");
	tmp.push_back("PlateWidth");
      } else if((h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	tmp.push_back("YoungModulus1");
	tmp.push_back("YoungModulus2");
	tmp.push_back("YoungModulus3");
	tmp.push_back("PoissonRatio12");
	tmp.push_back("PoissonRatio23");
	tmp.push_back("PoissonRatio13");
	tmp.push_back("ShearModulus12");
	tmp.push_back("V1X");
	tmp.push_back("V1Y");
	tmp.push_back("MassDensity");
	tmp.push_back("ThermalExpansion1");
	tmp.push_back("ThermalExpansion2");
	tmp.push_back("ThermalExpansion3");
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	tmp.push_back("YoungModulus1");
	tmp.push_back("YoungModulus2");
	tmp.push_back("YoungModulus3");
	tmp.push_back("PoissonRatio12");
	tmp.push_back("PoissonRatio23");
	tmp.push_back("PoissonRatio13");
	tmp.push_back("ShearModulus12");
	tmp.push_back("ShearModulus23");
	tmp.push_back("ShearModulus13");
	tmp.push_back("V1X");
	tmp.push_back("V1Y");
	tmp.push_back("V1Z");
	tmp.push_back("V2X");
	tmp.push_back("V2Y");
	tmp.push_back("V2Z");
	tmp.push_back("MassDensity");
	tmp.push_back("ThermalExpansion1");
	tmp.push_back("ThermalExpansion2");
	tmp.push_back("ThermalExpansion3");
      } else {
	string msg("MTestUmatStandardBehaviour::MTestUmatStandardBehaviour : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
      this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
    }
  }

  tfel::math::tmatrix<3u,3u,real>
  MTestUmatStandardBehaviour::getRotationMatrix(const tfel::math::vector<real>& mp,
						const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    using namespace std;
    using namespace tfel::math;
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    tmatrix<3u,3u,real> nr(0.);
    const Hypothesis& h = ModellingHypothesis::fromString(this->hypothesis);
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      nr(0,0) = nr(1,1) = nr(2,2) = 1.; // identité
    } else if((h==ModellingHypothesis::AXISYMMETRICAL)||
	      (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	      (h==ModellingHypothesis::PLANESTRAIN)||
	      (h==ModellingHypothesis::PLANESTRESS)){
      real V1X = 0;
      real V1Y = 0;
      if(h==ModellingHypothesis::PLANESTRESS){
	V1X = mp[4];
	V1Y = mp[5];
      } else {
	V1X = mp[7];
	V1Y = mp[8];
      }
      nr(0,0) = r(0,0)*V1X+r(0,1)*V1Y;
      nr(1,0) = r(1,0)*V1X+r(1,1)*V1Y;
      nr(0,1) =-nr(1,0);
      nr(1,1) = nr(0,0);
      nr(2,2) = 1;
    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
      const real V1X = mp[9];
      const real V1Y = mp[10];
      const real V1Z = mp[11];
      const real V2X = mp[12];
      const real V2Y = mp[13];
      const real V2Z = mp[14];
      nr(0,0)=r(0,0)*V1X+r(0,1)*V1Y+r(0,2)*V1Z;
      nr(1,0)=r(1,0)*V1X+r(1,1)*V1Y+r(1,2)*V1Z;
      nr(2,0)=r(2,0)*V1X+r(2,1)*V1Y+r(2,2)*V1Z;
      nr(0,1)=r(0,0)*V2X+r(0,1)*V2Y+r(0,2)*V2Z;
      nr(1,1)=r(1,0)*V2X+r(1,1)*V2Y+r(1,2)*V2Z;
      nr(2,1)=r(2,0)*V2X+r(2,1)*V2Y+r(2,2)*V2Z;
      nr(0,2)=nr(1,0)*nr(2,1)-nr(1,1)*nr(2,0);
      nr(1,2)=nr(2,0)*nr(0,1)-nr(2,1)*nr(0,0);
      nr(2,2)=nr(0,0)*nr(1,1)-nr(0,1)*nr(1,0);
    }
    return nr;
  } // end of MTestUmatStandardBehaviour::getRotationMatrix

  void
  MTestUmatStandardBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short ndv     = this->getDrivingVariablesSize(h);
    const unsigned short nth     = this->getThermodynamicForcesSize(h);
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(nth,ndv);
    this->iv.resize(nstatev);
    if(iv.size()==0){
      iv.push_back(0.);
    }
  }

  MTestStiffnessMatrixType::mtype
  MTestUmatStandardBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;    
  } // end of MTestUmatStandardBehaviour::getDefaultStiffnessMatrixType

  void
  MTestUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(MTestEvolutionManager& mp,
									 const MTestEvolutionManager& evm) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    const ModellingHypothesis::Hypothesis h = ModellingHypothesis::fromString(this->hypothesis);      
    MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"MassDensity",0.);
    if(this->stype==1){
      if(h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	// orthotropic behaviour
	const bool bv1x = evm.find("V1X")!=evm.end();
	const bool bv1y = evm.find("V1Y")!=evm.end();
	if((h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	   (h==ModellingHypothesis::AXISYMMETRICAL)||
	   (h==ModellingHypothesis::PLANESTRESS)||
	   (h==ModellingHypothesis::PLANESTRAIN)){
	  const bool b = bv1x&&bv1y;
	  if(((bv1x)&&(!b))||((bv1y)&&(!b))){
	    string msg("MTestBehaviour::setOptionalMaterialPropertiesDefaultValues : "
		       "if on component of the orthotropic basis is defined, all "
		       "the components must be defined.");
	    throw(runtime_error(msg));
	  }
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V1X",1.);
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V1Y",0.);
	} else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  const bool bv1z = evm.find("V1Z")!=evm.end();
	  const bool bv2x = evm.find("V2X")!=evm.end();
	  const bool bv2y = evm.find("V2Y")!=evm.end();
	  const bool bv2z = evm.find("V2Z")!=evm.end();
	  const bool b = bv1x&&bv1y&&bv1z&&bv2x&&bv2y&&bv2z;
	  if(((bv1x)&&(!b))||((bv1y)&&(!b))||((bv1z)&&(!b))||
	     ((bv2x)&&(!b))||((bv2y)&&(!b))||((bv1z)&&(!b))){
	    string msg("MTestBehaviour::setOptionalMaterialPropertiesDefaultValues : "
		       "if on component of the orthotropic basis is defined, all "
		       "the components must be defined.");
	    throw(runtime_error(msg));
	  }
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V1X",1.);
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V1Y",0.);
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V1Z",0.);
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V2X",0.);
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V2Y",1.);
	  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"V2Z",0.);
	} else {
	  string msg("MTestBehaviour::setOptionalMaterialPropertiesDefaultValues : "
		     "unsupported hypothesis");
	  throw(runtime_error(msg));
	}
      }
    }
  } // end of MTestUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues

  MTestUmatStandardBehaviour::~MTestUmatStandardBehaviour()
  {} // end of MTestUmatStandardBehaviour::~MTestUmatStandardBehaviour
  
}
