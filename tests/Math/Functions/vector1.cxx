/*!
 * \file   vector.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 Jan 2007
 */

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Math/functions.hxx"
#include"Math/vector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  USING_TFEL_FUNCTIONS;

  const double value[]  = {0,1.32,2.12,4.33,12.23};
  const double value2[] = {1.2,3.2,2.122,5.3,6.59};

  vector<double> v(value,value+5);
  vector<double> v2(value2,value2+5);
  vector<double> v3(5);

  v3 = (exp[cos])(v+v2);

  copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
  cout << endl;

  v3 = Cst<2>()(v+v2);
  copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
  cout << endl;

  v3 = Cst<2,3>()(v+v2);
  copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
  cout << endl;

  v3 = power<2>()(v+v2);
  copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
  cout << endl;

  v3 = sin[power<2>()](v+v2);
  copy(v3.begin(),v3.end(),ostream_iterator<double>(cout," "));
  cout << endl;

  return EXIT_SUCCESS;
}
