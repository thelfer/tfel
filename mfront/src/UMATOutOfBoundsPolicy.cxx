/*! 
 * \file  mfront/src/UMATOutOfBoundsPolicy.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jan 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
    const char * const p = ::getenv("CASTEM_OUT_OF_BOUNDS_POLICY");
    if(p!=nullptr){
      if(strcmp(p,"STRICT")==0){
	this->policy = tfel::material::Strict;
      } else if (strcmp(p,"WARNING")==0){
	this->policy = tfel::material::Warning;
      }
    }
  } // end of UMATOutOfBoundsPolicy::UMATOutOfBoundsPolicy

  tfel::material::OutOfBoundsPolicy
  UMATOutOfBoundsPolicy::getOutOfBoundsPolicy() const
  {
    return this->policy;
  } // end of UMATOutOfBoundsPolicy::getOutOfBoundsPolicy


} // end of namespace umat
