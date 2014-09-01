/*!
 * \file   tests/Math/RungeKutta/runge_kutta4.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 jui 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"runge_kutta4-header.hxx"

int main(void)
{
  using namespace std;

  test<float>();
  test<double>();
  test<long double>();

  return EXIT_SUCCESS;
}
