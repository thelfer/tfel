/*! 
 * \file  mfront/mtest/MTestSecantAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 */

#include<limits>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MFront/MTestSecantAccelerationAlgorithm.hxx"

namespace mfront
{

  MTestSecantAccelerationAlgorithm::MTestSecantAccelerationAlgorithm()
    : sa_w(1.),
      sat(-1)      
  {} // end of MTestSecantAccelerationAlgorithm::MTestSecantAccelerationAlgorithm
    
  std::string
  MTestSecantAccelerationAlgorithm::getName() const{
    return "secant";
  }

  void
  MTestSecantAccelerationAlgorithm::setParameter(const std::string& p,
						    const std::string& v)
  {
    using namespace std;
    const string m = "MTestSecantAccelerationAlgorithm::setParameter";
    if(p=="AccelerationTrigger"){
      const unsigned short i =
	MTestAccelerationAlgorithm::convertToUnsignedShort(m,v);
      if(this->sat!=-1){
	string msg("MTestSecantAccelerationAlgorithm::setParameter : "
		   "the castem acceleration trigger has already "
		   "been defined");
	throw(runtime_error(msg));
      }
      if(i<3){
	string msg("MTestSecantAccelerationAlgorithm::setParameter",
		   "invalid acceleration trigger value.");
	throw(runtime_error(msg));
      }
      this->sat = i;
    } else {
      string msg("MTestSecantAccelerationAlgorithm::setParameter : "
		 "invalid parameter '"+p+"'.");
      throw(runtime_error(msg));
    }
  } // end of MTestSecantAccelerationAlgorithm::setParameter

  void
  MTestSecantAccelerationAlgorithm::initialize(const unsigned short psz)
  {
    this->sa_u0.resize(psz,0.);      
    this->sa_u1.resize(psz,0.);
    this->sa_r0.resize(psz,0.);
    this->sa_r1.resize(psz,0.);
    this->sa_r.resize(psz,0.);
    this->sa_dr.resize(psz,0.);
    if(this->sat==-1){
      this->sat=3;
    }
  } // end of MTestSecantAccelerationAlgorithm::initialize

  void
  MTestSecantAccelerationAlgorithm::preExecuteTasks()
  {
    using namespace std;
    this->sa_w = max(min(this->sa_w,1.),-1.);
  } // end of MTestAccelerationAlgorithm::preExecuteTaks

  void
  MTestSecantAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
					    const tfel::math::vector<real>&,
					    const tfel::math::vector<real>& r,
					    const real ,
					    const real seps,
					    const unsigned short iter)
  {
    using namespace std;
    const real sa_eps = 100*(seps*numeric_limits<real>::epsilon());
    this->sa_u0.swap(this->sa_u1);
    this->sa_r0.swap(this->sa_r1);
    this->sa_r1 = r;
    this->sa_u1 = u1;
    this->sa_dr = this->sa_r1-this->sa_r0;
    if(iter>=this->sat){
      if(getVerboseMode()>=VERBOSE_LEVEL1){
	ostream& log = getLogStream();
	log << "Secant acceleration convergence" << endl;
      }
      const real nr2_dr = this->sa_dr|this->sa_dr;
      if(nr2_dr>sa_eps){
	const real sa_a = (this->sa_r1|this->sa_dr)/nr2_dr;
	u1 -= sa_a*(this->sa_u1-this->sa_u0);
	// this->sa_w *= -(this->sa_r0|this->sa_dr)/nr2_dr;
	// u1    = (this->sa_w)*this->sa_u1+(1-this->sa_w)*this->sa_u0;
      }
      //      this->sa_u1 = u1;
    }
// wk.sa_r0.swap(wk.sa_r1);
// wk.sa_r1 = wk.rdv;
// wk.sa_dr = wk.sa_r1 - wk.sa_r0;
// wk.sa_u0.swap(wk.sa_u1);
// wk.sa_u1 = state.u1;
// wk.sa_du = wk.sa_u1 - wk.sa_u0;
// if(iter>=this->sat){
// if(getVerboseMode()>=VERBOSE_LEVEL1){
// ostream& log = getLogStream();
// log << "Secant acceleration convergence" << endl;
// }
// const real nr2_dr = wk.sa_dr|wk.sa_dr;
// if(nr2_dr>sa_eps){
// const real sa_a = (wk.sa_du|wk.sa_dr)/nr2_dr;
// state.u1 -= sa_a*wk.sa_r1;
// }
// }

  } // end of MTestSecantAccelerationAlgorithm::execute

  void
  MTestSecantAccelerationAlgorithm::postExecuteTasks(void)
  {} // end of MTestAccelerationAlgorithm::postExecuteTaks

  MTestSecantAccelerationAlgorithm::~MTestSecantAccelerationAlgorithm()
  {} // end of MTestAccelerationAlgorithm::~MTestAccelerationAlgorithm

} // end of namespace mfront
