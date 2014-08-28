/*!
 * \file   tests/System/rstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 nov 2007
 */


#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
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
