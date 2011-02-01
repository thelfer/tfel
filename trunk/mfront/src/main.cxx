/*!
 * \file   MFront.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifdef __CYGWIN__
#include<windows.h>
#endif /* __CYGWIN__ */

#include"MFront/MFront.hxx"
#include"MFront/MFrontInitParsers.hxx"
#include"MFront/MFrontInitInterfaces.hxx"

int main(const int argc, const char *const *const argv)
{
  using namespace std;
  using namespace mfront;
  
#ifdef __CYGWIN__
  try{
#endif /* __CYGWIN__ */

  initParsers();
  initInterfaces();
  MFront mfront(argc,argv);
  mfront.exe();

#ifdef __CYGWIN__
  }
  catch(exception& e){
    MessageBox(0,e.what(),
	       "mfront",0);
    return EXIT_FAILURE;
  }
  catch(...){
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */

  return EXIT_SUCCESS;
}
