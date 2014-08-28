/*! 
 * \file   mfront/src/CyranoOutOfBoundsPolicy.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<cstdlib>
#include<cstring>
#include"MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx"

namespace cyrano
{

  const CyranoOutOfBoundsPolicy&
  CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy()
  {
    static CyranoOutOfBoundsPolicy u;
    return u;
  } // end of getCyranoOutOfBoundsPolicy

  CyranoOutOfBoundsPolicy::CyranoOutOfBoundsPolicy()
    : policy(tfel::material::None)
  {
    using namespace tfel::material;
    const char * const p = ::getenv("CYRANO_OUT_OF_BOUND_POLICY");
    if(p!=0){
      if(strcmp(p,"STRICT")==0){
	this->policy = Strict;
      } else if (strcmp(p,"WARNING")==0){
	this->policy = Warning;
      }
    }
  } // end of CyranoOutOfBoundsPolicy::CyranoOutOfBoundsPolicy

  tfel::material::OutOfBoundsPolicy
  CyranoOutOfBoundsPolicy::getOutOfBoundsPolicy() const
  {
    return this->policy;
  } // end of CyranoOutOfBoundsPolicy::getOutOfBoundsPolicy

} // end of namespace cyrano
