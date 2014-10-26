/*! 
 * \file  mfront/mtest/CastemAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<limits>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/CastemAccelerationAlgorithm.hxx"

namespace mfront
{

  CastemAccelerationAlgorithm::CastemAccelerationAlgorithm()
    : cat(-1),
      cap(-1)
  {} // end of CastemAccelerationAlgorithm::CastemAccelerationAlgorithm

  std::string
  CastemAccelerationAlgorithm::getName(void) const
  {
    return "Cast3M";
  } // end of CastemAccelerationAlgorithm::getName

  void
  CastemAccelerationAlgorithm::setParameter(const std::string& p,
						 const std::string& v)
  {
    using namespace std;
    const string m = "CastemAccelerationAlgorithm::setParameter";
    if(p=="AccelerationTrigger"){
      const unsigned short i = AccelerationAlgorithm::convertToUnsignedShort(m,v);
      if(this->cat!=-1){
	string msg("CastemAccelerationAlgorithm::setParameter : "
		   "the castem acceleration trigger has already "
		   "been defined");
	throw(runtime_error(msg));
      }
      if(i<3){
	string msg("CastemAccelerationAlgorithm::setParameter : ",
		   "invalid acceleration trigger value.");
	throw(runtime_error(msg));
      }
      this->cat = i;
    } else if(p=="AccelerationPeriod"){
      const unsigned short i = AccelerationAlgorithm::convertToUnsignedShort(m,v);
      if(this->cap!=-1){
	string msg("MTest::setCastemAccelerationPeriod : "
		   "the castem acceleration period has already "
		   "been defined");
	throw(runtime_error(msg));
      }
      if(i==0){
	string msg("MTest::setCastemAccelerationPeriod : ",
		   "invalid acceleration period value.");
	throw(runtime_error(msg));
      }
      this->cap = i;
    } else {
      string msg("CastemAccelerationAlgorithm::setParameter : "
		 "invalid parameter '"+p+"'.");
      throw(runtime_error(msg));
    }
  } // end of CastemAccelerationAlgorithm::setParameter

  void
  CastemAccelerationAlgorithm::initialize(const unsigned short psz)
  {
    //! allocating memory
    this->ca_u0.resize(psz,0.);      
    this->ca_u1.resize(psz,0.);
    this->ca_u2.resize(psz,0.);
    this->ca_r0.resize(psz,0.);
    this->ca_r1.resize(psz,0.);
    this->ca_r2.resize(psz,0.);
    this->ca_n0.resize(psz,0.);
    this->ca_n1.resize(psz,0.);
    this->ca_tmp0.resize(psz,0.);
    this->ca_tmp1.resize(psz,0.);
    // parameters
    if(this->cap==-1){
      this->cap=2;
    }
    if(this->cat==-1){
      this->cat=4;
    }
  } // end of CastemAccelerationAlgorithm::initialize

  void
  CastemAccelerationAlgorithm::preExecuteTasks()
  {} // end of CastemAccelerationAlgorithm::preExecuteTasks

  void
  CastemAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
					    const tfel::math::vector<real>&,
					    const tfel::math::vector<real>& r,
					    const real,
					    const real seps,
					    const unsigned short iter)
  {
    using namespace std;
    const real ca_eps = 100*(seps*numeric_limits<real>::epsilon());
    this->ca_u0.swap(this->ca_u1);
    this->ca_u1.swap(this->ca_u2);
    this->ca_r0.swap(this->ca_r1);
    this->ca_r1.swap(this->ca_r2);
    this->ca_u2 = u1;
    this->ca_r2  = r;
    if((iter>=this->cat)&&((iter-this->cat)%this->cap==0)){
      if(getVerboseMode()>=VERBOSE_LEVEL1){
	ostream& log = getLogStream();
	log << "Cast3M acceleration convergence" << endl;
      }
      bool c  = true;
      this->ca_tmp0 = this->ca_r1-this->ca_r0;
      this->ca_tmp1 = this->ca_r2-this->ca_r0;
      const real nr0 = norm(this->ca_tmp0);
      c = nr0>ca_eps;
      if(c){
	this->ca_n0   = this->ca_tmp0/nr0;
	const real ntmp1  = this->ca_tmp1|this->ca_n0;
	this->ca_tmp1       -= ntmp1*this->ca_n0;
	const real nr1    = norm(this->ca_tmp1);
	c = nr1>0.1*abs(ntmp1);
	if(c){
	  this->ca_n1      =   this->ca_tmp1/nr1;
	  const real ca_p0 = -(this->ca_r0|this->ca_n0);
	  const real ca_p1 = -(this->ca_r0|this->ca_n1);
	  // La projection du vecteur nul est
	  // donnée par
	  // (1-ca_c2-ca_c1)*r0+ca_c0*ca_r1+ca_c2*r2;
	  // avec - ca_c1 = (ca_p0-ca_p1*ntmp1/nr1)/nr0
	  //      - ca_c2 = ca_p1/nr1
	  // maintenant on applique la même relation
	  // linéaire
	  // aux inconnues..
	  const real ca_c2 = ca_p1/nr1;
	  const real ca_c1 = (ca_p0-ntmp1*ca_c2)/nr0;
	  u1 = (1-ca_c2-ca_c1)*this->ca_u0+ca_c1*this->ca_u1+ca_c2*this->ca_u2;
	} else {
	  // the previous iterations were (almost) colinear
	  const real ca_c0 = -(this->ca_r0|this->ca_n0)/nr0;
	  u1 = (1-ca_c0)*this->ca_u0+ca_c0*this->ca_u1;
	}
      }
    }
  } // end of CastemAccelerationAlgorithm::execute

  void
  CastemAccelerationAlgorithm::postExecuteTasks()
  {} // end of CastemAccelerationAlgorithm::postExecuteTasks

  CastemAccelerationAlgorithm::~CastemAccelerationAlgorithm()
  {} // end of AccelerationAlgorithm::~AccelerationAlgorithm

} // end of namespace mfront
