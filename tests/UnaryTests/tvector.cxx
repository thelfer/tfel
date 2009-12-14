#include<iostream>
#include<cstdlib>

#include "Math/tvector.hxx"

template<class T>
void function(const tfel::math::VectorConcept<T>& x){
  std::cout << x(0) << std::endl;
  std::cout << x(1) << std::endl;
  std::cout << "------" << std::endl;
}

int main(void){
  
  typedef tfel::math::tvector<5> vector1;
  typedef tfel::math::tvector<5,unsigned short> vector2;

  vector1 v1;
  vector2 v2;

  v1(0)=12.2;
  v1(1)=431.432;
  v2(0)=7;
  v2(1)=32;
  
  function(v1);
  function(2*v1);
  function(v1*2);
  function(v2);
  function(v1+v2);
  function(v1-v2+v1);
  function(2.*v1-v2);
  //  function(2.*(v1-v2));
    
  return EXIT_SUCCESS;

}
