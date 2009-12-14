/*! 
 * \file  test.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avr 2009
 */

#include<iostream>
#include<cstdlib>

#include"Math/vector.hxx"
#include"Math/matrix.hxx"
#include"Math/LUSolve.hxx"

void
print(const tfel::math::matrix<double>& m)
{
  using namespace std;
  using namespace tfel::math;
  matrix<double>::size_type i,j;
  cout << "m :"  << endl;
  for(i=0;i!=m.getNbRows();++i){
    for(j=0;j!=m.getNbRows();++j){
      cout << m(i,j) << " ";
    }
    cout << endl;
  }
}

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  matrix<double> m(3,3);
  vector<double> b(3);

  m(0,0)=0.;
  m(0,1)=2.;
  m(0,2)=1.;
  m(1,0)=1.;
  m(1,1)=0.;
  m(1,2)=0.;
  m(2,0)=3.;
  m(2,1)=0.;
  m(2,2)=1.;

  b(0) = 5;
  b(1) = -1;
  b(2) = -2;

  cout << "********" << endl;
  print(m);
  LUSolve::exe(m,b);
  print(m);
  cout << "b : " << endl;
  copy(b.begin(),b.end(),ostream_iterator<double>(cout," "));
  cout << endl;

} // end of main
