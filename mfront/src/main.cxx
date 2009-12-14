/*!
 * \file   MFront.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#include"MFront.hxx"
#include"MFrontInitParsers.hxx"
#include"MFrontInitInterfaces.hxx"

int main(const int argc, const char *const *const argv)
{
  using namespace std;
  using namespace mfront;
  
  initParsers();
  initInterfaces();
  MFront mfront(argc,argv);
  mfront.exe();

  return EXIT_SUCCESS;
}
