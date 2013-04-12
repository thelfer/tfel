/*! 
 * \file  MTestImposedStress.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#include<iostream>
#include"MFront/MTestImposedStress.hxx"

namespace mfront
{

  MTestImposedStress::MTestImposedStress(const tfel::material::ModellingHypothesis::Hypothesis h,
					 const std::string& cname,
					 const tfel::utilities::shared_ptr<MTestEvolution> s_)
    : sev(s_)
  {
    this->c = this->getComponentPosition(h,cname);
  } // end of MTestImposedStress::MTestImposedStress

  MTestImposedStress::MTestImposedStress(const unsigned short c_,
					 const tfel::utilities::shared_ptr<MTestEvolution> s_)
    : sev(s_),
      c(c_)
  {} // end of MTestImposedStress::MTestImposedStress

  unsigned short
  MTestImposedStress::getComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis h,
					   const std::string& cname) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(h==MH::TRIDIMENSIONAL){
      if(cname=="SXX"){
	return 0u;
      } else if(cname=="SYY"){
	return 1u;
      } else if(cname=="SZZ"){
	return 2u;
      } else if(cname=="SXY"){
	return 3u;
      } else if(cname=="SXZ"){
	return 4u;
      } else if(cname=="SYZ"){
	return 5u;
      } else {
	string msg("MTestImposedStress::getComponentPosition : "
		   "invalid component '"+cname+"' in 3D");
	throw(runtime_error(msg));
      }
    } else if ((h==MH::PLANESTRAIN)||
	       (h==MH::GENERALISEDPLANESTRAIN)){
      if(cname=="SXX"){
	return 0u;
      } else if(cname=="SYY"){
	return 1u;
      } else if(cname=="SZZ"){
	return 2u;
      } else if(cname=="SXY"){
	return 3u;
      } else {
	string msg("MTestImposedStress::getComponentPosition : "
		   "invalid component '"+cname+"' in plane "
		   "strain or generalised plane strain");
	throw(runtime_error(msg));
      }
    } else if (h==MH::PLANESTRESS){
      if(cname=="SXX"){
	return 0u;
      } else if(cname=="SZZ"){
	return 2u;
      } else if(cname=="SXY"){
	return 3u;
      } else {
	string msg("MTestImposedStress::getComponentPosition : "
		   "invalid component '"+cname+"' in plane stress");
	throw(runtime_error(msg));
      }
    } else if (h==MH::AXISYMMETRICAL){
      if(cname=="SRR"){
	return 0u;
      } else if(cname=="SZZ"){
	return 1u;
      } else if(cname=="STT"){
	return 2u;
      } else if(cname=="SRZ"){
	return 3u;
      } else {
	string msg("MTestImposedStress::getComponentPosition : "
		   "invalid component '"+cname+"' in "
		   "axisymmetrical modelling");
	throw(runtime_error(msg));
      }
    } else if (h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      if(cname=="SRR"){
	return 0u;
      } else if(cname=="SZZ"){
	return 1u;
      } else if(cname=="STT"){
	return 2u;
      } else {
	string msg("MTestImposedStress::getComponentPosition : "
		   "invalid component '"+cname+"' in "
		   "axisymmetrical generalised plane strain modelling");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestImposedStress::getComponentPosition : "
		 "unsupported modelling hypothesis");
      throw(runtime_error(msg));
    }
    return 0u;
  } // end of MTestImposedStress::getComponentPosition					 

  unsigned short
  MTestImposedStress::getNumberOfLagrangeMultipliers(void) const
  {
    return 0u;
  } // end of MTestImposedStress::getNumberOfLagrangeMultipliers

  void
  MTestImposedStress::setValues(tfel::math::matrix<real>&,
				tfel::math::vector<real>& r,
				const tfel::math::vector<real>&,
				const tfel::math::vector<real>&,
				const unsigned short,
				const unsigned short,
				const real t,
				const real dt,
				const real) const
  {
    using namespace std;
    static const real sqrt2 = sqrt(real(2));
    const MTestEvolution& s = *(this->sev);
    if(this->c<3){
      r(this->c) -= s(t+dt);
    } else {
      r(this->c) -= sqrt2*s(t+dt);
    }
  } // end of MTestImposedStress::setValues

  bool
  MTestImposedStress::checkConvergence(const unsigned short,
				       const tfel::math::vector<real>&,
				       const tfel::math::tvector<6u,real>& s,
				       const real,
				       const real seps,
				       const real t,
				       const real dt) const
  {
    using namespace std;
    static const real sqrt2 = sqrt(real(2));
    const MTestEvolution& sv = *(this->sev);
    if(this->c<3){
      return abs(s(this->c)-sv(t+dt))<seps;
    }
    return abs(s(this->c)-sqrt2*sv(t+dt))<seps;
  }

  MTestImposedStress::~MTestImposedStress()
  {} // end of MTestImposedStress::~MTestImposedStress

} // end of namespace mfront

