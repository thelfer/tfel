/*!
 * \file   MTestUmatStandardBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 */

#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/MTestUmatStandardBehaviour.hxx"

namespace mfront
{
  
  MTestUmatStandardBehaviour::MTestUmatStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : MTestUmatBehaviourBase(l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getUMATFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b);
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
	tmp.push_back("PoissonRatio13");
	tmp.push_back("PoissonRatio23");
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

  MTestUmatStandardBehaviour::~MTestUmatStandardBehaviour()
  {} // end of MTestUmatStandardBehaviour::~MTestUmatStandardBehaviour
  
}
