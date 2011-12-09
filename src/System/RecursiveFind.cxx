/*! 
 * \file  RecursiveFind.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
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
		  const unsigned short depth,
		  const unsigned short mdepth)
    {
      using namespace std;
      map<string,vector<string> > r;
      recursiveFind(r,re,name,depth,mdepth);
      return r;
    } // end of recursiveFind

    void
    recursiveFind(std::map<std::string,std::vector<std::string> >& r,
		  const std::string& re,
		  const std::string& name,
		  const unsigned short depth,
		  const unsigned short mdepth)
    {
      using namespace std;
      regex_t regex;
      try{
	if(regcomp(&regex,re.c_str(),REG_EXTENDED|REG_NOSUB)!=0){
	  string msg("recursiveFind : ");
	  msg += "can't compile regular expression '"+re+"'";
	  throw(runtime_error(msg));
	}
      }	catch(...){
	regfree(&regex);
	throw;
      }
      recursiveFind(r,regex,name,depth,mdepth);
      regfree(&regex);
    } // end of recursiveFind

    std::map<std::string,std::vector<std::string> >
    recursiveFind(const regex_t& re,
		  const std::string& name,
		  const unsigned short depth,
		  const unsigned short mdepth)
    {
      using namespace std;
      map<string,vector<string> > r;
      recursiveFind(r,re,name,depth,mdepth);
      return r;
    } // end of recursiveFind

    void
    recursiveFind(std::map<std::string,std::vector<std::string> >& r,
		  const regex_t& re,
		  const std::string& name,
		  const unsigned short depth,
		  const unsigned short mdepth)
    {
      using namespace std;
      using namespace tfel::system;
      DIR* dir;
      struct dirent* p;
      struct stat buf;
      if(depth>mdepth){
	string msg("recursiveFind : ");
	msg += "maximal directory depth reached";
	throw(runtime_error(msg));
      }
      dir = opendir(name.c_str());
      if(dir==0){
	systemCall::throwSystemError("can't open directory '"+name+"'",errno);
      }
      while((p=readdir(dir))!=0){
	string file = name+"/";
	file += p->d_name;
	if(stat(file.c_str(),&buf)!=0){
	  string msg("tfel::system::recursiveFind : ");
	  msg += "can't stat file '"+file+"'";
	  systemCall::throwSystemError(msg,errno);
	}
	if(S_ISREG(buf.st_mode)){
	  if(regexec(&re,p->d_name,0,0,0)==0){
	    r[name].push_back(p->d_name);
	  }
	} else if(S_ISDIR(buf.st_mode)){
	  if((strcmp(p->d_name,".") !=0)&&
	     (strcmp(p->d_name,"..")!=0)){
	    map<string,vector<string> > r2;
	    recursiveFind(r2,re,name+'/'+p->d_name,
			  depth+1,mdepth);
	    r.insert(r2.begin(),r2.end());
	  }
	}
      }
      closedir(dir);
    } // end of recursiveFind
 
  } // end of namespace system

} // end of namespace tfel
