/*!
 * \file   tests/Math/Functions/function.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   04 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/functions.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;
  // Comparison with Octave results
  assert(abs((sin+cos)(3.4312)+ 1.2439)<1.e-4);
  assert(abs((-(sin+cos))(3.4312)-1.2439)<1.e-4);
  assert(abs((exp[sin])(3.4312)-0.75158)<1.e-4);
  assert(abs((exp[sin+cos])(3.4312)-0.28825)<1.e-4);
  return EXIT_SUCCESS;

}
