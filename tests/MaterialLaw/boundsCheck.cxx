/*!
 * \file   boundsCheck.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   08 Mar 2007
 */

#include<iostream>
#include<cstdlib>

#include"MaterialLaw/BoundsCheck.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::materiallaw;
  
  stensor<3,mass> s(mass(12.));
  stensor<3,float> s2(12.f);
  long double d = 12.L;

  try{
    BoundsCheck<3>::upperBoundCheck("s",s,10.);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::lowerBoundCheck("s",s,24.);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::lowerAndUpperBoundCheck("s",s,24.,13.);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::upperBoundCheck("s2",s2,10.f);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::lowerBoundCheck("s2",s2,24.f);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::lowerAndUpperBoundCheck("s2",s2,24.f,13.f);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::upperBoundCheck("d",d,10.L);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::lowerBoundCheck("d",d,24.L);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  try{
    BoundsCheck<3>::lowerAndUpperBoundCheck("d",d,24.L,13.L);
  } 
  catch(const OutOfBoundsException& e){
    cout << e.what() << endl;
  }

  return EXIT_SUCCESS;
}
