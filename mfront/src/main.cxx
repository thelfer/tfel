/*!
 * \file   mfront/src/main.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#include<windows.h>
#endif /* __CYGWIN__ */

#include<cstdlib>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/MFront.hxx"
#include"MFront/MFrontInitParsers.hxx"
#include"MFront/MFrontInitInterfaces.hxx"
#include"MFront/MFrontInitAnalysers.hxx"

int main(const int argc, const char *const *const argv)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::system;
  using namespace mfront;
  
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  try{
#endif /* __CYGWIN__ */

  initParsers();
  initInterfaces();
  initAnalysers();

  // calling mfront plugins
  const char * libs = ::getenv("MFRONT_ADDITIONAL_LIBRARIES");
  if(libs!=static_cast<const char*>(0)){
    const vector<string>& l = tokenize(libs,':');
    vector<string>::const_iterator p;
    for(p=l.begin();p!=l.end();++p){
      ExternalLibraryManager& lm = ExternalLibraryManager::getExternalLibraryManager();
      lm.loadLibrary(*p);
    }
  }

  MFront mfront(argc,argv);
  mfront.exe();

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
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
