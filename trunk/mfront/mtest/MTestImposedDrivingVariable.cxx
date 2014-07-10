/*! 
 * \file  MTestImposedDrivingVariable.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
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
    using namespace std;
    const MTestEvolution& e = *(this->eev);
    K(pos,this->c)+=a;
    K(this->c,pos)+=a;
    r(this->c)     =a*u1(pos);
    r(pos)        -=a*(e(t+dt)-u1(this->c));
  } // end of MTestImposedDrivingVariable::setValues

  bool
  MTestImposedDrivingVariable::checkConvergence(const unsigned short,
						const tfel::math::vector<real>& u,
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

  MTestImposedDrivingVariable::~MTestImposedDrivingVariable()
  {} // end of MTestImposedDrivingVariable::~MTestImposedDrivingVariable

} // end of namespace mfront

