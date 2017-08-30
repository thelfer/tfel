/*! 
 * \file  src/System/RecursiveFind.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<cstring>
#include<cerrno>

#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/param.h>
#include<regex.h>

#include"TFEL/System/System.hxx"
#include"TFEL/System/RecursiveFind.hxx"

namespace tfel
{

  namespace system
  {

    std::map<std::string,std::vector<std::string> >
    recursiveFind(const std::string& re,
		  const std::string& name,
		  const bool b,
		  const unsigned short depth,
		  const unsigned short mdepth)
    {
      std::map<std::string,std::vector<std::string> > r;
      recursiveFind(r,re,name,b,depth,mdepth);
      return r;
    } // end of recursiveFind

    void recursiveFind(std::map<std::string,std::vector<std::string> >& r,
		       const std::string& re,
		       const std::string& name,
		       const bool b,
		       const unsigned short depth,
		       const unsigned short mdepth)
    {
      recursiveFind(r,std::regex(re),name,b,depth,mdepth);
    } // end of recursiveFind

    std::map<std::string,std::vector<std::string> >
    recursiveFind(const std::regex& re,
		  const std::string& name,
		  const bool b,
		  const unsigned short depth,
		  const unsigned short mdepth)
    {
      std::map<std::string,std::vector<std::string> > r;
      recursiveFind(r,re,name,b,depth,mdepth);
      return r;
    } // end of recursiveFind

    void recursiveFind(std::map<std::string,std::vector<std::string> >& r,
		       const std::regex& re,
		       const std::string& name,
		       const bool b,
		       const unsigned short depth,
		       const unsigned short mdepth)
    {
      using namespace tfel::system;
      auto throw_if = [](const bool c,const std::string& m){
	if(c){throw(std::runtime_error("recursiveFind: "+m));}
      };
      DIR* dir;
      struct dirent* p;
      struct stat buf;
      if(depth>mdepth){
	throw_if(b,"maximal directory depth reached");
	return;
      }
      dir = opendir(name.c_str());
      if(dir==nullptr){
	if(b){
	  systemCall::throwSystemError("can't open directory '"+name+"'",errno);
	}
	return;
      }
      try{
	while((p=readdir(dir))!=nullptr){
	  const auto file = name+dirSeparator()+p->d_name;
	  if(stat(file.c_str(),&buf)==0){
	    if(S_ISREG(buf.st_mode)){
	      if(std::regex_match(p->d_name,re)){
		r[name].emplace_back(p->d_name);
	      }
	    } else if(S_ISDIR(buf.st_mode)){
	      if((strcmp(p->d_name,".") !=0)&&
	       (strcmp(p->d_name,"..")!=0)){
		std::map<std::string,std::vector<std::string> > r2;
		recursiveFind(r2,re,name+'/'+p->d_name,
			      depth+1,mdepth);
		r.insert(r2.begin(),r2.end());
	      }
	    }
	  } else {
	    throw_if(b,"can't stat file '"+file+"'");
	  }
	}
      } catch(...){
	closedir(dir);
	throw;
      }
      closedir(dir);
    } // end of recursiveFind
 
  } // end of namespace system

} // end of namespace tfel
