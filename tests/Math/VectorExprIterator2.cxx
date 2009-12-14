/*!
 * \file   VectorExprIterator2.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   23 oct 2006
 */

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Math/tvector.hxx"
#include"Math/functions.hxx"

template<typename T>
void f(const T o){
  using namespace std;
  typename T::const_iterator p;
  for(p=o.begin();p!=o.end();++p){
    cout << *p << endl;
  }
}

int main(void){
  
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  tvector<3> v; 
  
  v(0)=1.5;
  v(1)=4.865;
  v(2)=0.5;

  f(v);
  f(cos(v));
  f(exp(cos(v)));
  f(exp(v+2*cos(v)));

  return EXIT_SUCCESS;
}
