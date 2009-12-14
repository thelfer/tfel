/*!
 * \file   runge_kutta4.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 jui 2006
 */

#include"runge_kutta4-header.hxx"

int main(void)
{
  using namespace std;

  test<float>();
  test<double>();
  test<long double>();

  cout << "success" << endl;

  return EXIT_SUCCESS;
}
