/*! 
 * \file  mfront/src/SearchFile.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mai 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<unistd.h>
#include<cstdlib>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"
#include"MFront/SearchFile.hxx"

namespace mfront
{

  std::string
  SearchFile::search(const std::string& f)
  {
    using namespace std;
    using namespace tfel::system;
    auto& msf = SearchFile::getSearchFile();
    string file(f);
    if(::access(file.c_str(),F_OK)==0){
      if(::access(file.c_str(),R_OK)!=0){
	string msg("SearchFile::search : '"+
		   file + "' is not readable");
	throw(runtime_error(msg));
      }
      return file;
    }
    for(auto p=msf.paths.begin();p!=msf.paths.end();++p){
      file = *p+dirSeparator()+f;
      if(::access(file.c_str(),F_OK)==0){
	if(::access(file.c_str(),R_OK)!=0){
	  string msg("SearchFile::search : '"+
		     file + "' is not readable");
	  throw(runtime_error(msg));
	}
	return file;
      }
    }
    throw(runtime_error("SearchFile::search : '"+f+
			"' has not been found."));
    return "";
  }

  void
  SearchFile::addSearchPaths(const std::string& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    auto& msf = SearchFile::getSearchFile();
#if defined _WIN32 || defined _WIN64
    vector<string> npaths(tokenize(p,';'));
#else
    vector<string> npaths(tokenize(p,':'));
#endif
    msf.paths.insert(msf.paths.begin(),
		     npaths.begin(),npaths.end());
  } // end of SearchFile::addSearchPaths

  const std::vector<std::string>&
  SearchFile::getSearchPaths(void)
  {
    return SearchFile::getSearchFile().paths;
  } // end of SearchFile::getSearchPaths

  SearchFile::SearchFile()
  {
    using namespace std;
    using namespace tfel::utilities;
    const char * const p = ::getenv("MFRONT_INCLUDE_PATH");
    if(p!=nullptr){
#if defined _WIN32 || defined _WIN64
    vector<string> npaths(tokenize(p,';'));
#else
    vector<string> npaths(tokenize(p,':'));
#endif
      this->paths.insert(this->paths.begin(),
			 npaths.begin(),npaths.end());
    }
  }

  SearchFile&
  SearchFile::getSearchFile()
  {
    static SearchFile msf;
    return msf;
  } // end of SearchFile::getSearchFile

} // end of namespace mfront
