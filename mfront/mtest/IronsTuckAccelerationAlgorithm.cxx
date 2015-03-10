/*! 
 * \file  mfront/mtest/IronsTuckAccelerationAlgorithm.cxx
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
#include<ostream>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/IronsTuckAccelerationAlgorithm.hxx"

namespace mfront
{

  IronsTuckAccelerationAlgorithm::IronsTuckAccelerationAlgorithm()
    : itat(-1)
  {} // end of IronsTuckAccelerationAlgorithm::IronsTuckAccelerationAlgorithm

  std::string
  IronsTuckAccelerationAlgorithm::getName(void) const{
    return "Irons-Tuck";
  }
    
  void
  IronsTuckAccelerationAlgorithm::setParameter(const std::string& p,
						    const std::string& v)
  {
    using namespace std;
    const string m = "IronsTuckAccelerationAlgorithm::setParameter";
    if(p=="AccelerationTrigger"){
      const unsigned short i =
	AccelerationAlgorithm::convertToUnsignedShort(m,v);
      if(this->itat!=-1){
	string msg("IronsTuckAccelerationAlgorithm::setParameter : "
		   "the castem acceleration trigger has already "
		   "been defined");
	throw(runtime_error(msg));
      }
      if(i<3){
	string msg("IronsTuckAccelerationAlgorithm::setParameter",
		   "invalid acceleration trigger value.");
	throw(runtime_error(msg));
      }
      this->itat = i;
    } else {
      string msg("IronsTuckAccelerationAlgorithm::setParameter : "
		 "invalid parameter '"+p+"'.");
      throw(runtime_error(msg));
    }
  } // end of IronsTuckAccelerationAlgorithm::setParameter

  void
  IronsTuckAccelerationAlgorithm::initialize(const unsigned short psz)
  {
    this->ita_u0.resize(psz,0.);      
    this->ita_u1.resize(psz,0.);
    this->ita_u2.resize(psz,0.);
    this->ita_du.resize(psz,0.);
    this->ita_ddu.resize(psz,0.);
    if(this->itat==-1){
      this->itat=3;
    }
  } // end of IronsTuckAccelerationAlgorithm::initialize

  void
  IronsTuckAccelerationAlgorithm::preExecuteTasks(void)
  {} // end of IronsTuckAccelerationAlgorithm::preExecuteTasks

  void
  IronsTuckAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
					       const tfel::math::vector<real>&,
					       const tfel::math::vector<real>&,
					       const real eeps,
					       const real,
					       const unsigned short iter)
  {
    using namespace std;
    const real it_eps = 100*(eeps*numeric_limits<real>::epsilon());
    this->ita_u0.swap(this->ita_u1);
    this->ita_u1.swap(this->ita_u2);
    this->ita_u2 = u1;
    if((iter>=this->itat)&&((iter-this->itat)%2==0)){
      if(getVerboseMode()>=VERBOSE_LEVEL1){
	auto& log = getLogStream();
	log << "Irons Tuck acceleration convergence" << endl;
      }
      this->ita_du  = this->ita_u2-this->ita_u1;
      this->ita_ddu = this->ita_u2-2*this->ita_u1+this->ita_u0;
      const real nr2_ddu = this->ita_ddu|this->ita_ddu;
      if(nr2_ddu>it_eps){
	const real ita_a = (this->ita_du|this->ita_ddu)/nr2_ddu;
	u1 -= ita_a*this->ita_du;
	this->ita_u2 = u1;
      }
    }
  } // end of IronsTuckAccelerationAlgorithm::execute

  void
  IronsTuckAccelerationAlgorithm::postExecuteTasks(void)
  {} // end of IronsTuckAccelerationAlgorithm::postExecuteTasks

  IronsTuckAccelerationAlgorithm::~IronsTuckAccelerationAlgorithm()
  {} // end of AccelerationAlgorithm::~AccelerationAlgorithm

} // end of namespace mfront
