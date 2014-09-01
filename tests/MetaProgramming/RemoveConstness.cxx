/*!
 * \file   tests/MetaProgramming/RemoveConstness.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 Oct 2006
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

#include<cassert>
#include<cstdlib>

#include"TFEL/Metaprogramming/RemoveConstness.hxx"
#include"TFEL/Utilities/Name.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::meta;
  assert(Name<double>::getName()=="double");
  assert(Name<const double>::getName()=="const double");
  assert(Name<RemoveConstness<const double>::type>::getName()=="double");
  return EXIT_SUCCESS;
}
