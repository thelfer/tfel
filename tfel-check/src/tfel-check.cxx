/*!
 * \file   tfel-check.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Jan 2008
 */

#include<iostream>
#include<cstdlib>
#include<vector>
#include<map>
#include<string>
#include<stdexcept>
#include<iterator>
#include<algorithm>
#include<cstring>
#include<cerrno>

#include<dirent.h>
#include<sys/types.h>
#include<sys/param.h>
#include<unistd.h>
#include<regex.h>

#include"System/System.hxx"
#include"Utilities/TerminalColors.hxx"

#include"Utilities/TestLauncher.hxx"

static
const std::map<std::string,std::vector<std::string> >
recursiveFind(const regex_t& re,
	      const std::string& name,
	      const unsigned short depth)
{
  using namespace std;
  using namespace tfel::system;
  map<string,vector<string> > res;
  DIR* dir;
  struct dirent* p;
  struct stat buf;
  if(depth>100){
    string msg("recursiveFind : ");
    msg += "maximal directory depth reached";
    throw(runtime_error(msg));
  }
  dir = opendir(name.c_str());
  if(dir==0){
    systemCall::throwSystemError("can't open directory "+name,errno);
  }
  while((p=readdir(dir))!=0){
    string file = name+"/";
    file += p->d_name;
    if(stat(file.c_str(),&buf)!=0){
      string msg("MFront::analyseSourceDirectory : ");
      msg += "can't stat file ";
      msg += file;
      cerr << msg << endl;
      systemCall::throwSystemError(msg,errno);
    }
    if(S_ISREG(buf.st_mode)){
      if(regexec(&re,p->d_name,0,0,0)==0){
	res[name].push_back(p->d_name);
      }
    } else if(S_ISDIR(buf.st_mode)){
      if((strcmp(p->d_name,".")!=0)&&
	 strcmp(p->d_name,"..")!=0){
	const map<string,vector<string> > & r = recursiveFind(re,
							      name+'/'+p->d_name,
							      depth+1);
	res.insert(r.begin(),r.end());
      }
    }
  }
  closedir(dir);
  return res;
} // end of recursiveFind

static void
report(std::ofstream& log,
       const std::string& file)
{
  using namespace std;
  using namespace tfel::utilities;
  string name;
  name = "* beginning of test '" + file+"'";
  log  << name << endl;
  cout << name << endl;
} // end of reportStatus

static void
reportStatus(std::ofstream& log,
	     const std::string& file,
	     const bool success)
{
  using namespace std;
  using namespace tfel::utilities;
  string name;
  name = "* result of test '" + file+"'";
  if(name.size()<=80){
    name.insert(name.size(),80-name.size(),' ');
  }
  log  << name << " : ";
  cout << name << " : ";
  if(success){
    log  << "[SUCCESS]" << endl << endl;
    cout << "[SUCCESS]" << endl << endl;
  } else {
    log  << " [FAILED]" << endl << endl;
    cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
    cout << " [FAILED]" << endl << endl;
    cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
    cout.flush();
  }
} // end of reportStatus

int
main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  regex_t re;
  char path[MAXPATHLEN];
  char cpath[MAXPATHLEN];
  ofstream log("tfel-check.log");
  map<string,vector<string> >::const_iterator p;
  vector<string>::const_iterator p2;
  int status;
  bool success;
  if(regcomp(&re,".*\\.check$",REG_EXTENDED|REG_NOSUB)!=0){
    cerr << "main : can't compile regular expression\n";
    return EXIT_FAILURE;      /* Report error. */
  }
  const map<string,vector<string> >& files = recursiveFind(re,".",0);
  if(realpath(".",cpath)==0){
    cerr << "main : can't get real path of current directory, aborting\n";
    log  << "main : can't get real path of current directory, aborting\n";
    exit(EXIT_FAILURE);
  }
  status = EXIT_SUCCESS;
  for(p=files.begin();p!=files.end();++p){
    success = true;
    if(realpath(p->first.c_str(),path)==0){
      log << "entering directory " << p->first << endl;
    } else {
      log << "entering directory " << path << endl;
    } 
    if(chdir(p->first.c_str())==-1){
      log << "can't move to directory " << p->first << endl;
      for(p2=p->second.begin();p2!=p->second.end();++p2){
	reportStatus(log,p->first+'/'+*p2,false);
	status = EXIT_FAILURE;
      }
    } else {
      for(p2=p->second.begin();p2!=p->second.end();++p2){
	report(log,p->first+'/'+*p2);
	string name = p->first+'/'+*p2;
	success = true;
	try{
	  TestLauncher c(*p2,log);
	  success = c.execute();
	}
	catch(exception& e){
	  log << "test '"+*p2+"' failed : "
	      << e.what() << endl;
	  success = false;
	}
	reportStatus(log,p->first+'/'+*p2,success);
	if(!success){
	  status = EXIT_FAILURE;
	}
      }
      if(chdir(cpath)==-1){
	cerr << "can't move back to top directory " << cpath << endl;
	cerr << ", aborting\n";
	log  << "can't move back to top directory " << cpath << endl;
	log  << ", aborting";
	exit(EXIT_FAILURE);
      }
    }
  }
  regfree(&re);
  return status;
} // end of main
