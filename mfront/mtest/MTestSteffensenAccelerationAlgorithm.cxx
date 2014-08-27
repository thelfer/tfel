/*! 
 * \file  MTestSteffensenAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 */

#include<limits>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MFront/MTestSteffensenAccelerationAlgorithm.hxx"

namespace mfront
{

  MTestSteffensenAccelerationAlgorithm::MTestSteffensenAccelerationAlgorithm()
    : stat(-1)
  {} // end of MTestSteffensenAccelerationAlgorithm::MTestSteffensenAccelerationAlgorithm
    
  std::string
  MTestSteffensenAccelerationAlgorithm::getName(void) const
  {
    return "Steffensen";
  } // end of MTestSteffensenAccelerationAlgorithm::getName

  void
  MTestSteffensenAccelerationAlgorithm::setParameter(const std::string& p,
						     const std::string& v)
  {
    using namespace std;
    const string m = "MTestSteffensenAccelerationAlgorithm::setParameter";
    if(p=="AccelerationTrigger"){
      const unsigned short i =
	MTestAccelerationAlgorithm::convertToUnsignedShort(m,v);
      if(this->stat!=-1){
	string msg("MTestSteffensenAccelerationAlgorithm::setParameter : "
		   "the castem acceleration trigger has already "
		   "been defined");
	throw(runtime_error(msg));
      }
      if(i<3){
	string msg("MTestSteffensenAccelerationAlgorithm::setParameter",
		   "invalid acceleration trigger value.");
	throw(runtime_error(msg));
      }
      this->stat = i;
    } else {
      string msg("MTestSteffensenAccelerationAlgorithm::setParameter : "
		 "invalid parameter '"+p+"'.");
      throw(runtime_error(msg));
    }
  } // end of MTestSteffensenAccelerationAlgorithm::setParameter

  void
  MTestSteffensenAccelerationAlgorithm::initialize(const unsigned short psz)
  {
    this->sta_u0.resize(psz,0.);      
    this->sta_u1.resize(psz,0.);
    this->sta_u2.resize(psz,0.);
    this->sta_du1.resize(psz,0.);
    this->sta_du2.resize(psz,0.);
    if(this->stat==-1){
      this->stat=3;
    }
  } // end of MTestSteffensenAccelerationAlgorithm::initialize

  void
  MTestSteffensenAccelerationAlgorithm::preExecuteTasks(void)
  {} // end of MTestSteffensenAccelerationAlgorithm::preExecuteTasks

  void
  MTestSteffensenAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
						const tfel::math::vector<real>&,
						const tfel::math::vector<real>&,
						const real eeps,
						const real,
						const unsigned short iter)
  {
    using namespace std;
    const real it_eps = 100*(eeps*numeric_limits<real>::epsilon());
    this->sta_u0.swap(this->sta_u1);
    this->sta_u1.swap(this->sta_u2);
    this->sta_u2 = u1;
    if((iter>=this->stat)&&((iter-this->stat)%2==0)){
      if(getVerboseMode()>=VERBOSE_LEVEL1){
	ostream& log = getLogStream();
	log << "Steffensen acceleration convergence" << endl;
      }
      this->sta_du2  = this->sta_u2-this->sta_u1;
      this->sta_du1  = this->sta_u1-this->sta_u0;
      for(vector<real>::size_type id=0;id!=this->sta_du1.size();++id){
	if((abs(this->sta_du2[id])>it_eps)&&
	   (abs(this->sta_du1[id])>it_eps)){
	  u1[id] =  this->sta_u1[id]+1/(1/(this->sta_du2[id])-1/(this->sta_du1[id]));
	}
      }
    }
  } // end of MTestSteffensenAccelerationAlgorithm::execute

  void
  MTestSteffensenAccelerationAlgorithm::postExecuteTasks(void)
  {} // end of MTestSteffensenAccelerationAlgorithm::postExecuteTasks

  MTestSteffensenAccelerationAlgorithm::~MTestSteffensenAccelerationAlgorithm()
  {} // end of MTestAccelerationAlgorithm::~MTestAccelerationAlgorithm

} // end of namespace mfront
