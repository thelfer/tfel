/*! 
 * \file  mfront/mtest/MTestImposedThermodynamicForce.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#include"MFront/MTestBehaviour.hxx"
#include"MFront/MTestImposedThermodynamicForce.hxx"

namespace mfront
{

  MTestImposedThermodynamicForce::MTestImposedThermodynamicForce(const MTestBehaviour& b,
								 const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& cname,
								 const tfel::utilities::shared_ptr<MTestEvolution> s_)
    : sev(s_)
  {
    this->c = b.getThermodynamicForceComponentPosition(h,cname);
  } // end of MTestImposedThermodynamicForce::MTestImposedThermodynamicForce

  MTestImposedThermodynamicForce::MTestImposedThermodynamicForce(const unsigned short c_,
								 const tfel::utilities::shared_ptr<MTestEvolution> s_)
    : sev(s_),
      c(c_)
  {} // end of MTestImposedThermodynamicForce::MTestImposedThermodynamicForce

  unsigned short
  MTestImposedThermodynamicForce::getNumberOfLagrangeMultipliers(void) const
  {
    return 0u;
  } // end of MTestImposedThermodynamicForce::getNumberOfLagrangeMultipliers

  void
  MTestImposedThermodynamicForce::setValues(tfel::math::matrix<real>&,
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
    const MTestEvolution& s = *(this->sev);
    r(this->c) -= s(t+dt);
  } // end of MTestImposedThermodynamicForce::setValues

  bool
  MTestImposedThermodynamicForce::checkConvergence(const tfel::math::vector<real>&,
						   const tfel::math::vector<real>& s,
						   const real,
						   const real seps,
						   const real t,
						   const real dt) const
  {
    using namespace std;
    const MTestEvolution& sv = *(this->sev);
    const bool cb = abs(s(this->c)-sv(t+dt))<seps;
    return cb;
  }

   std::string
   MTestImposedThermodynamicForce::getFailedCriteriaDiagnostic(const tfel::math::vector<real>&,
							       const tfel::math::vector<real>& s,
							       const real,
							       const real seps,
							       const real t,
							       const real dt) const
   {
     using namespace std;
     const MTestEvolution& sv = *(this->sev);
     ostringstream msg;
     msg << "imposed thermodynmic force not reached for component " << this->c 
	 << "(imposed value : " << sv(t+dt) << ", computed value : " 
	 << s(this->c) << ", criteria : " << seps << ")";
     return msg.str();
   }

  MTestImposedThermodynamicForce::~MTestImposedThermodynamicForce()
  {} // end of MTestImposedThermodynamicForce::~MTestImposedThermodynamicForce

} // end of namespace mfront

