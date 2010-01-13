/*!
 * \file   vector.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<iterator>
#include<algorithm>

#include"Math/vector.hxx"
#include"Math/functions.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;
  using tfel::math::vector;

  double tab[] = {0.,1.,2.,3.,4.};

  vector<double> v(tab,tab+5);
  vector<double> v2(5,3.);
  vector<double> v3(5);

  v3 = v+2.5*v2;
  
//   copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
//   cout << endl;

//   cout << "Size : " << v3.size() << endl; 
  v3.resize(8);
//   cout << "Size : " << v3.size() << endl; 

//   copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
//   cout << endl;

//   cout << "sin : " << sin(v3)(3) << endl;

  return EXIT_SUCCESS;
}
