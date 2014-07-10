/*! 
 * \file  MFrontSearchFile.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mai 2013
 */

#include<unistd.h>
#include<cstdlib>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"
#include"MFront/MFrontSearchFile.hxx"

namespace mfront
{

  std::string
  MFrontSearchFile::search(const std::string& f)
  {
    using namespace std;
    using namespace tfel::system;
    MFrontSearchFile& msf = MFrontSearchFile::getMFrontSearchFile();
    vector<string>::const_iterator p;
    string file(f);
    if(::access(file.c_str(),F_OK)==0){
      if(::access(file.c_str(),R_OK)!=0){
	string msg("MFrontSearchFile::search : ");
	msg+= file + "is not readable";
	throw(runtime_error(msg));
      }
      return file;
    }
    for(p=msf.paths.begin();p!=msf.paths.end();++p){
      file = *p+dirSeparator()+f;
      if(::access(file.c_str(),F_OK)==0){
	if(::access(file.c_str(),R_OK)!=0){
	  string msg("MFrontSearchFile::search : ");
	  msg+= file + "is not readable";
	  throw(runtime_error(msg));
	}
	return file;
      }
    }
    string msg("MFrontSearchFile::search : '");
    msg += f + "' has not been found.";
    throw(runtime_error(msg));
    return "";
  }

  void
  MFrontSearchFile::addSearchPaths(const std::string& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    MFrontSearchFile& msf = MFrontSearchFile::getMFrontSearchFile();
#if defined _WIN32 || defined _WIN64
    vector<string> npaths(tokenize(p,';'));
#else
    vector<string> npaths(tokenize(p,':'));
#endif
    msf.paths.insert(msf.paths.begin(),
		     npaths.begin(),npaths.end());
  } // end of MFrontSearchFile::addSearchPaths

  const std::vector<std::string>&
  MFrontSearchFile::getSearchPaths(void)
  {
    return MFrontSearchFile::getMFrontSearchFile().paths;
  } // end of MFrontSearchFile::getSearchPaths

  MFrontSearchFile::MFrontSearchFile()
  {
    using namespace std;
    using namespace tfel::utilities;
    const char * const p = ::getenv("MFRONT_INCLUDE_PATH");
    if(p!=0){
#if defined _WIN32 || defined _WIN64
    vector<string> npaths(tokenize(p,';'));
#else
    vector<string> npaths(tokenize(p,':'));
#endif
      this->paths.insert(this->paths.begin(),
			 npaths.begin(),npaths.end());
    }
  }

  MFrontSearchFile&
  MFrontSearchFile::getMFrontSearchFile()
  {
    static MFrontSearchFile msf;
    return msf;
  } // end of MFrontSearchFile::getMFrontSearchFile

} // end of namespace mfront
