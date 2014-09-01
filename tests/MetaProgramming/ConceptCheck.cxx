/*! 
 * \file   tests/MetaProgramming/ConceptCheck.cxx
 * \brief
 * \author Helfer Thomas
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

#include"TFEL/Metaprogramming/IsSuperClassOf.hxx"
#include"TFEL/Metaprogramming/IsSubClassOf.hxx"

struct A{};
struct B
  : public A
{};

int main(void){
  
  assert((tfel::meta::IsSuperClassOf<A,B>::cond==1));
  assert((tfel::meta::IsSuperClassOf<B,A>::cond==0));
  
  assert((tfel::meta::IsSubClassOf<A,B>::cond==0));
  assert((tfel::meta::IsSubClassOf<B,A>::cond==1));

  return EXIT_SUCCESS;
  
}
