/*!
 * \file   tests/Math/FracSimplify.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>

#include"TFEL/Math/General/FracSimplify.hxx"

int main(void)
{
  using namespace tfel::math;

  assert((FracSimplify<2,1u>::N==2));
  assert((FracSimplify<2,1u>::D==1u));
  assert((FracSimplify<8,6u>::N==4));
  assert((FracSimplify<8,6u>::D==3u)); 
  assert((FracSimplify<-9,9u>::N==-1));
  assert((FracSimplify<-9,9u>::D==1u)); 
  assert((FracSimplify<-10,5u>::N==-2));
  assert((FracSimplify<-10,5u>::D==1u)); 
  assert((FracSimplify<-30,15u>::N==-2));
  assert((FracSimplify<-30,15u>::D==1u)); 
  assert((FracSimplify<11,3u>::N==11));
  assert((FracSimplify<11,3u>::D==3u));   

  return EXIT_SUCCESS;
}
