/*!
 * \file   VectorExprIterator.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   23 oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Math/tvector.hxx"
#include"Math/qt.hxx"

// template<typename T>
// void f(const T p){
//   using namespace std;
//   copy(p.begin(),p.end(),ostream_iterator<typename T::value_type>(cout," "));
//   cout << endl;
// }

int main(void){
  
  using namespace tfel::math;

  tvector<3> v; 
  tvector<3,mass > v2(mass(3.)); 
  
  v(0)=1.5;
  v(1)=4.865;
  v(2)=0.5;

//   f(2.*v);
//   f(3.*v2);
  
  return EXIT_SUCCESS;
}
