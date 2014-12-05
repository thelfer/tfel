/*!
 * \file   tests/System/rstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 nov 2007
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
#include<cstring>

#include"TFEL/System/rfstream.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::system;
  
  rfstream r("test.bin");
  char c[5];
  double t;
  
  r.read(c,4u);
  c[4]='\0';
  assert(strcmp(c,"toto")==0);

  r >> t;
  assert(abs(t-12.)<1.e-14);

  r.read(c,4u);
  c[4]='\0';
  assert(strcmp(c,"tutu")==0);

  r.close();
  
  return EXIT_SUCCESS;
} // end of main
