/*!
 * \file   tests/Math/RungeKutta/runge_kutta2.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   20 jui 2006
 */

#include"runge_kutta2-header.hxx"

int main(void)
{
  using namespace std;

  test<float>();
  test<double>();
  test<long double>();

  return EXIT_SUCCESS;
}
