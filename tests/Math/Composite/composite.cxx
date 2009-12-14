/*!
 * \file   composite.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<iostream>
#include<cstdlib>
#include"Utilities/Name.hxx"
#include"Math/functions.hxx"
#include"Math/tvector.hxx"
#include"Math/composite.hxx"

template<typename T>
void printItems(const T&)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::meta;
  cout << TypeListGetNames<typename CompositeTraits<T>::Items>::exe() << endl;
} // end of printItems

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  composite<double,tvector<3,double> > c(12.,tvector<3,double>(13));
  composite<double,tvector<3,double> > c2(9.,tvector<3,double>(1.4));

  composite<double,tvector<3,double> > c3;

  cout << Name<ResultType<composite<double,float>,
                          composite<float,float>,OpPlus>::type>::getName() << endl; 
  cout << Name<ResultType<composite<double,tvector<3,int> >,
                          composite<float,tvector<3,float> >,OpPlus>::type>::getName() << endl; 
  cout << Name<ResultType<composite<double,tvector<3,unsigned int> >,
                          composite<float,tvector<3,short> >,OpMinus>::type>::getName() << endl; 

  typedef CompositeTraits<composite<float,tvector<3,short> > >::Items Items;
  typedef tfel::math::internals::ScalarCompositeComputeResultList<double,Items,OpMinus>::type Res;

  if(tfel::meta::TypeListContainsInvalidType<Res>::cond==true){
    cout << Name<Res>::getName() << "is invalid" << endl;
  } else {
    cout << Name<Res>::getName() << "is not invalid" << endl;
  }

  cout << Name<ResultType<double,composite<float,tvector<3,short> >,OpMult>::type>::getName() << endl;

  cout << Name<ResultType<composite<float,tvector<3,short> >,double,OpMult>::type>::getName() << endl;

  cout << Name<UnaryResultType<composite<float,tvector<3,short> >,OpNeg>::type>::getName() << endl;

  printItems(c);
  printItems(c2);
  printItems(c+c2);
  c3 = c2+c*2;

  cout << c3.getComponent<0>() << "," 
       << c3.getComponent<1>() << "\n";

  c3 = -(c2+c*2);

  cout << c3.getComponent<0>() << "," 
       << c3.getComponent<1>() << "\n";

  c3 = sin(c2+c*2);

  cout << c3.getComponent<0>() << "," 
       << c3.getComponent<1>() << "\n";
  
  return EXIT_SUCCESS;
}
