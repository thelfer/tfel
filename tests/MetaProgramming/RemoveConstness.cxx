/*!
 * \file   RemoveConstness.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 Oct 2006
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
