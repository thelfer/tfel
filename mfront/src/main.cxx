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
  
#ifdef __CYGWIN
  try{
#endif /* __CYGWIN */

  initParsers();
  initInterfaces();
  MFront mfront(argc,argv);
  mfront.exe();

#ifdef __CYGWIN
  }
  catch(exception& e){
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  catch(...){
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN */

  return EXIT_SUCCESS;
}
