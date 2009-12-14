/*!
 * \file   test.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   30 Jun 2006
 */

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<iterator>

#include"FSAlgorithm/FSAlgorithm.hxx"

double add(double a,double b)
{
  return a+b;
}


double mult(double a,double b)
{
  return a*b;
}

double times2(double a)
{
  return 2*a;
}

double null_mult(double,double)
{
  return 0.;
}

struct abs_compare{
  template<typename T>
  bool operator()(const T&a,const T&b)
  {
    if(std::abs(a)>std::abs(b)){
      return true;
    } else {
      return false;
    };
  }
};

int main(void)
{

  using namespace tfel::fsalgo;

  double tab1[]={5.,-16.,9.};
  double tab2[3];
  double tab3[3];
  unsigned int tab4[]={15,1,32109};
  unsigned int tab5[3];


  std::cout << "Minimum     :" << *min_element<3>::exe(tab1) << std::endl;
  std::cout << "Maximum     :" << *max_element<3>::exe(tab1) << std::endl;
  std::cout << "Maximum abs :" << *max_element<3>::exe(tab1,abs_compare()) << std::endl;


  copy<3>::exe(tab1,tab2);
  copy<3>::exe(tab2,std::ostream_iterator<double>(std::cout," "));
  std::cout << std::endl;
  fill<3>::exe(tab1,3.);
  fill<3>::exe(tab2,6.);
  copy<3>::exe(tab1,std::ostream_iterator<double>(std::cout," "));
  std::cout << std::endl;

  transform<3>::exe(tab1,tab2,tab3,mult);
  copy<3>::exe(tab3,std::ostream_iterator<double>(std::cout," "));
  std::cout << std::endl;

  fill<3>::exe(tab1,3.);
  transform<3>::exe(tab1,tab2,times2);
  fill<3>::exe(tab1,6.);

  copy<3>::exe(tab4,tab5);
  std::cout << "Equal ? " << (equal<3>::exe(tab4,tab5) ? "Yes" : "no") << std::endl;

  copy<3>::exe(tab3,std::ostream_iterator<double>(std::cout," "));
  std::cout << std::endl;
  swap_ranges<3>::exe(tab1,tab3);
  copy<3>::exe(tab1,std::ostream_iterator<double>(std::cout," "));
  std::cout << std::endl;
  copy<3>::exe(tab3,std::ostream_iterator<double>(std::cout," "));
  std::cout << std::endl;

  tab1[0]=3.;
  tab1[1]=12.1;
  tab1[2]=2.786;
  
  std::cout << "Accumulate : " << accumulate<3>::exe(tab1,1.) << std::endl;
  
  tab1[0]=3.5;
  tab1[1]=8.;
  tab1[2]=3.14159;
  std::cout << "Accumulate2          : " << accumulate<3>::exe(tab1,2.,mult) << std::endl;
  std::cout << "Accumulate2 (result) : " << 56.*3.14159 << std::endl;

  tab1[0]=2.;
  tab1[1]=3.;
  tab1[2]=1.5;
  tab2[0]=2.2;
  tab2[1]=3.6;
  tab2[2]=2.;

  std::cout << "Inner_product  : " << inner_product<3>::exe(tab1,tab2,-16.) << std::endl;
  std::cout << "Inner_product2 : " << inner_product<3>::exe(tab1,tab2,-16.,add,mult) << std::endl;
  std::cout << "Inner_product3 : " << inner_product<3>::exe(tab1,tab2,-16.,add,null_mult) << std::endl;
  std::cout << "Inner_product4 : " << inner_product<3>::exe(tab1,tab2,-16.,add,add) << std::endl;

  return EXIT_SUCCESS;
}
