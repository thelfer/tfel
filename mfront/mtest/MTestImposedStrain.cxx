/*! 
 * \file  MTestImposedStrain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#include<cmath>

#include"MFront/MTestImposedStrain.hxx"

namespace mfront
{

  MTestImposedStrain::MTestImposedStrain(const tfel::material::ModellingHypothesis::Hypothesis h,
					 const std::string& cname,
					 const tfel::utilities::shared_ptr<MTestEvolution> e_)
    : eev(e_)
  {
    this->c = this->getComponentPosition(h,cname);
  } // end of MTestImposedStrain::MTestImposedStrain

  MTestImposedStrain::MTestImposedStrain(const unsigned short c_,
					 const tfel::utilities::shared_ptr<MTestEvolution> e_)
    : eev(e_),
      c(c_)
  {} // end of MTestImposedStrain::MTestImposedStrain

  unsigned short
  MTestImposedStrain::getComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis h,
					   const std::string& cname) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if((h==MH::TRIDIMENSIONAL)||
       (h==MH::PLANESTRAIN)||
       (h==MH::PLANESTRESS)||
       (h==MH::GENERALISEDPLANESTRAIN)){
      if(cname=="EXX"){
	return 0u;
      } else if(cname=="EYY"){
	return 1u;
      } else if((cname=="EZZ")&&(h!=MH::PLANESTRAIN)){
	return 2u;
      } else if(cname=="EXY"){
	return 3u;
      } else if((cname=="EXZ")&&(h==MH::TRIDIMENSIONAL)){
	return 4u;
      } else if((cname=="EYZ")&&(h==MH::TRIDIMENSIONAL)){
	return 5u;
      } else {
	string msg("MTestImposedStrain::getComponentPosition : "
		   "invalid component '"+cname+"'");
	throw(runtime_error(msg));
      }
    } else if ((h==MH::AXISYMMETRICAL)||
	       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)){
      if(cname=="ERR"){
	return 0u;
      } else if(cname=="EZZ"){
	return 1u;
      } else if(cname=="ETT"){
	return 2u;
      } else if((cname=="ERZ")&&(h==MH::AXISYMMETRICAL)){
	return 3u;
      } else {
	string msg("MTestImposedStrain::getComponentPosition : "
		   "invalid component '"+cname+"' in "
		   "axisymmetrical modelling");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestImposedStrain::getComponentPosition : "
		 "unsupported modelling hypothesis");
      throw(runtime_error(msg));
    }
    return 0u;
  } // end of MTestImposedStrain::getComponentPosition					 

  unsigned short
  MTestImposedStrain::getNumberOfLagrangeMultipliers(void) const
  {
    return 1u;
  } // end of MTestImposedStrain::getNumberOfLagrangeMultipliers

  void
  MTestImposedStrain::setValues(tfel::math::matrix<real>& K,
				      tfel::math::vector<real>& r,
				      const tfel::math::vector<real>&,
				      const tfel::math::vector<real>& u1,
				      const unsigned short pos,
				      const unsigned short,
				      const real t,
				      const real dt,
				      const real a) const
  {
    using namespace std;
    const MTestEvolution& e = *(this->eev);
    K(pos,this->c)+=a;
    K(this->c,pos)+=a;
    r(this->c)     =a*u1(pos);
    r(pos)        -=a*(e(t+dt)-u1(this->c));
  } // end of MTestImposedStrain::setValues

  bool
  MTestImposedStrain::checkConvergence(const unsigned short,
				       const tfel::math::vector<real>& u,
				       const tfel::math::stensor<3u,real>&,
				       const real eeps,
				       const real,
				       const real t,
				       const real dt) const
  {
    using namespace std;
    const MTestEvolution& e = *(this->eev);
    return abs(u(this->c)-e(t+dt))<eeps;
  }

  MTestImposedStrain::~MTestImposedStrain()
  {} // end of MTestImposedStrain::~MTestImposedStrain

} // end of namespace mfront

