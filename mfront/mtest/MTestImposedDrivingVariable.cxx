/*! 
 * \file  mfront/mtest/MTestImposedDrivingVariable.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>

#include"MFront/MTestBehaviour.hxx"
#include"MFront/MTestImposedDrivingVariable.hxx"

namespace mfront
{

  MTestImposedDrivingVariable::MTestImposedDrivingVariable(const MTestBehaviour& b,
							   const tfel::material::ModellingHypothesis::Hypothesis h,
							   const std::string& cname,
							   const tfel::utilities::shared_ptr<MTestEvolution> e_)
    : eev(e_)
  {
    this->c = b.getDrivingVariableComponentPosition(h,cname);
  } // end of MTestImposedDrivingVariable::MTestImposedDrivingVariable

  MTestImposedDrivingVariable::MTestImposedDrivingVariable(const unsigned short c_,
							   const tfel::utilities::shared_ptr<MTestEvolution> e_)
    : eev(e_),
      c(c_)
  {} // end of MTestImposedDrivingVariable::MTestImposedDrivingVariable

  unsigned short
  MTestImposedDrivingVariable::getNumberOfLagrangeMultipliers(void) const
  {
    return 1u;
  } // end of MTestImposedDrivingVariable::getNumberOfLagrangeMultipliers

  void
  MTestImposedDrivingVariable::setValues(tfel::math::matrix<real>& K,
					 tfel::math::vector<real>& r,
					 const tfel::math::vector<real>&,
					 const tfel::math::vector<real>& u1,
					 const unsigned short pos,
					 const unsigned short,
					 const real t,
					 const real dt,
					 const real a) const
  {
    const MTestEvolution& e = *(this->eev);
    K(pos,this->c)+=a;
    K(this->c,pos)+=a;
    r(this->c)     =a*u1(pos);
    r(pos)        -=a*(e(t+dt)-u1(this->c));
  } // end of MTestImposedDrivingVariable::setValues

  bool
  MTestImposedDrivingVariable::checkConvergence(const tfel::math::vector<real>& u,
						const tfel::math::vector<real>&,
						const real eeps,
						const real,
						const real t,
						const real dt) const
  {
    using namespace std;
    const MTestEvolution& e = *(this->eev);
    return abs(u(this->c)-e(t+dt))<eeps;
  }
  
  std::string
  MTestImposedDrivingVariable::getFailedCriteriaDiagnostic(const tfel::math::vector<real>& e,
							   const tfel::math::vector<real>&,
							   const real eeps,
							   const real ,
							   const real t,
							   const real dt) const
  {
    using namespace std;
    const MTestEvolution& ev = *(this->eev);
    ostringstream msg;
    msg << "imposed driving variable not reached for component " << this->c 
	<< "(imposed value : " << ev(t+dt) << ", computed value : " 
	<< e(this->c) << ", criteria : " << eeps << ")";
    return msg.str();
  }

  MTestImposedDrivingVariable::~MTestImposedDrivingVariable()
  {} // end of MTestImposedDrivingVariable::~MTestImposedDrivingVariable

} // end of namespace mfront

