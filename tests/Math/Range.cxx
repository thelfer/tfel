/*!
 * \file   tests/Math/Range.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2006
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

#include<cstdlib>

#include"TFEL/Math/Vector/Range.hxx"
#include"TFEL/Math/vector.hxx"

int main(void){

  using namespace tfel::math;
  range<>::exe(12.,13.,5);
  return EXIT_SUCCESS;
}
