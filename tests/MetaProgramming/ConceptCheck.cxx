/*! 
 * \file   tests/MetaProgramming/ConceptCheck.cxx
 * \brief
 * \author Helfer Thomas
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
