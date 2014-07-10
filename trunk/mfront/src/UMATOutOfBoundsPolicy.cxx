/*! 
 * \file  UMATOutOfBoundsPolicy.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jan 2010
 */

#include<cstdlib>
#include<cstring>
#include"MFront/UMAT/UMATOutOfBoundsPolicy.hxx"

namespace umat
{

  const UMATOutOfBoundsPolicy&
  UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy()
  {
    static UMATOutOfBoundsPolicy u;
    return u;
  } // end of getUMATOutOfBoundsPolicy

  UMATOutOfBoundsPolicy::UMATOutOfBoundsPolicy()
    : policy(tfel::material::None)
  {
    using namespace tfel::material;
    const char * const p = ::getenv("CASTEM_OUT_OF_BOUND_POLICY");
    if(p!=0){
      if(strcmp(p,"STRICT")==0){
	this->policy = Strict;
      } else if (strcmp(p,"WARNING")==0){
	this->policy = Warning;
      }
    }
  } // end of UMATOutOfBoundsPolicy::UMATOutOfBoundsPolicy

  tfel::material::OutOfBoundsPolicy
  UMATOutOfBoundsPolicy::getOutOfBoundsPolicy() const
  {
    return this->policy;
  } // end of UMATOutOfBoundsPolicy::getOutOfBoundsPolicy


} // end of namespace umat
