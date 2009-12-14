/*!
 * \file   FracSimplify.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jul 2006
 */

#include<iostream>
#include<cstdlib>
#include<cassert>

#include"Math/Quantity/FracSimplify.hxx"

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

  std::cout << "success" << std::endl;

  return EXIT_SUCCESS;
}
