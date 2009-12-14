/*!
 * \file   tvector.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
 */

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<limits>

#include"Math/tvector.hxx"
#include"Math/qt.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  
  tvector<4,length> v1(length(12.)); 
  tvector<4,length> v2(length(14.)); 
  tvector<4,length> v3;
  
  v3 = v1+v2;

  cout << v3 << endl;

  return EXIT_SUCCESS;
}
