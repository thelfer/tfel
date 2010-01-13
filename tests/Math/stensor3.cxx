/*!
 * \file   stensor3.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   07 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <cmath>

#include"Math/stensor.hxx"
#include"Math/Stensor/StensorExternalTabStorage.hxx"
#include"Math/Stensor/StensorExternalVoigtStorage.hxx"

int main(void){

  using namespace tfel::math;
  using namespace std;

  double  a[] ={0.,0.,0.,1.};
  double  b[] ={0.,0.,0.,std::sqrt(2.)};

//   cout << "Test ExternalTab" << endl;
  
//   copy(a,a+4,ostream_iterator<double>(cout," "));
//   cout << endl;
  {
    stensor<2,double,StensorExternalTab> sig(a);
//     cout << sig << endl;
    sig*=2u;
//     cout << sig << endl;
  }
//   copy(a,a+4,ostream_iterator<double>(cout," "));
//   cout << endl;

//   cout << "Test ExternalVoigt" << endl;
//   copy(b,b+4,ostream_iterator<double>(cout," "));
//   cout << endl;
  {
    stensor<2,double,StensorExternalVoigt> sig(b);
//     cout << sig << endl;
    sig*=2u;
//     cout << sig << endl;
  }
//   copy(b,b+4,ostream_iterator<double>(cout," "));
//   cout << endl;


  return EXIT_SUCCESS;
}
