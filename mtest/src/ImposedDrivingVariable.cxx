/*! 
 * \file  mfront/mtest/ImposedDrivingVariable.cxx
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
#include<sstream>

#include"MTest/Behaviour.hxx"
#include"MTest/ImposedDrivingVariable.hxx"

namespace mtest
{

  ImposedDrivingVariable::ImposedDrivingVariable(const Behaviour& b,
						 const tfel::material::ModellingHypothesis::Hypothesis h,
						 const std::string& cname,
						 const std::shared_ptr<Evolution> e_)
    : eev(e_)
  {
    this->c = b.getDrivingVariableComponentPosition(h,cname);
  } // end of ImposedDrivingVariable::ImposedDrivingVariable

  ImposedDrivingVariable::ImposedDrivingVariable(const unsigned short c_,
						 const std::shared_ptr<Evolution> e_)
    : eev(e_),
      c(c_)
  {} // end of ImposedDrivingVariable::ImposedDrivingVariable

  unsigned short
  ImposedDrivingVariable::getNumberOfLagrangeMultipliers(void) const
  {
    return 1u;
  } // end of ImposedDrivingVariable::getNumberOfLagrangeMultipliers

  void
  ImposedDrivingVariable::setValues(tfel::math::matrix<real>& K,
				    tfel::math::vector<real>& r,
				    const tfel::math::vector<real>&,
				    const tfel::math::vector<real>& u1,
				    const unsigned short pos,
				    const unsigned short,
				    const real t,
				    const real dt,
				    const real a) const
  {
    const Evolution& e = *(this->eev);
    K(pos,this->c)+=a;
    K(this->c,pos)+=a;
    r(this->c)     =a*u1(pos);
    r(pos)        -=a*(e(t+dt)-u1(this->c));
  } // end of ImposedDrivingVariable::setValues

  bool
  ImposedDrivingVariable::checkConvergence(const tfel::math::vector<real>& u,
						const tfel::math::vector<real>&,
						const real eeps,
						const real,
						const real t,
						const real dt) const
  {
    using namespace std;
    const Evolution& e = *(this->eev);
    return abs(u(this->c)-e(t+dt))<eeps;
  }
  
  std::string
  ImposedDrivingVariable::getFailedCriteriaDiagnostic(const tfel::math::vector<real>& e,
							   const tfel::math::vector<real>&,
							   const real eeps,
							   const real ,
							   const real t,
							   const real dt) const
  {
    using namespace std;
    const Evolution& ev = *(this->eev);
    ostringstream msg;
    msg << "imposed driving variable not reached for component " << this->c 
	<< "(imposed value : " << ev(t+dt) << ", computed value : " 
	<< e(this->c) << ", criteria : " << eeps << ")";
    return msg.str();
  }

  ImposedDrivingVariable::~ImposedDrivingVariable()
  {} // end of ImposedDrivingVariable::~ImposedDrivingVariable

} // end of namespace mtest

