/*!
 * \file   System.cpp
 * \brief    
 * \author Helfer Thomas
 * \date   06 Nov 2007
 */

#include<sstream>
#include<fstream>
#include<algorithm>
#include<functional>
#include<cstdio>
#include<cstring>
#include<climits>

#include<unistd.h>
#include<dirent.h>

#include"System/SystemError.hxx"
#include"System/System.hxx"

namespace tfel
{

  namespace system
  {

    void
    systemCall::throwSystemError(const std::string& errMsg,
				 const int errNbr)
    {
      using namespace std;
      string err = strerror(errNbr);
      transform(err.begin(),err.end(),err.begin(),ptr_fun(::tolower));
      string msg(errMsg);
      msg += " (system error : ";
      msg += err;
      msg += ")";
      switch(errNbr){
      case EPERM:
	throw(PosixError<EPERM>(msg));
	break;
      case ENOENT:
	throw(PosixError<ENOENT>(msg));
	break;
      case ESRCH:
	throw(PosixError<ESRCH>(msg));
	break;
      case EINTR:
	throw(PosixError<EINTR>(msg));
	break;
      case EIO:
	throw(PosixError<EIO>(msg));
	break;
      case ENXIO:
	throw(PosixError<ENXIO>(msg));
	break;
      case E2BIG:
	throw(PosixError<E2BIG>(msg));
	break;
      case ENOEXEC:
	throw(PosixError<ENOEXEC>(msg));
	break;
      case EBADF:
	throw(PosixError<EBADF>(msg));
	break;
      case ECHILD:
	throw(PosixError<ECHILD>(msg));
	break;
      case EAGAIN:
	throw(PosixError<EAGAIN>(msg));
	break;
      case ENOMEM:
	throw(PosixError<ENOMEM>(msg));
	break;
      case EACCES:
	throw(PosixError<EACCES>(msg));
	break;
      case EFAULT:
	throw(PosixError<EFAULT>(msg));
	break;
      case ENOTBLK:
	throw(PosixError<ENOTBLK>(msg));
	break;
      case EBUSY:
	throw(PosixError<EBUSY>(msg));
	break;
      case EEXIST:
	throw(PosixError<EEXIST>(msg));
	break;
      case EXDEV:
	throw(PosixError<EXDEV>(msg));
	break;
      case ENODEV:
	throw(PosixError<ENODEV>(msg));
	break;
      case ENOTDIR:
	throw(PosixError<ENOTDIR>(msg));
	break;
      case EISDIR:
	throw(PosixError<EISDIR>(msg));
	break;
      case EINVAL:
	throw(PosixError<EINVAL>(msg));
	break;
      case ENFILE:
	throw(PosixError<ENFILE>(msg));
	break;
      case EMFILE:
	throw(PosixError<EMFILE>(msg));
	break;
      case ENOTTY:
	throw(PosixError<ENOTTY>(msg));
	break;
      case ETXTBSY:
	throw(PosixError<ETXTBSY>(msg));
	break;
      case EFBIG:
	throw(PosixError<EFBIG>(msg));
	break;
      case ENOSPC:
	throw(PosixError<ENOSPC>(msg));
	break;
      case ESPIPE:
	throw(PosixError<ESPIPE>(msg));
	break;
      case EROFS:
	throw(PosixError<EROFS>(msg));
	break;
      case EMLINK:
	throw(PosixError<EMLINK>(msg));
	break;
      case EPIPE:
	throw(PosixError<EPIPE>(msg));
	break;
      case EDOM:
	throw(PosixError<EDOM>(msg));
	break;
      case ERANGE:
	throw(PosixError<ERANGE>(msg));
	break;
      default:
	throw(SystemError(msg));
      }
    } // end of throwSystemError

    std::vector<std::string>
    systemCall::tokenize(const std::string& s,
			 const char c)
    {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(c, b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(c, e);
	e = s.find_first_of(c, b);
      }
      return res;
    } // end of systemCall::tokenize

    std::vector<std::string>
    systemCall::tokenize(const std::string& s,
			 const std::string& delim)
    {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(delim, b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(delim, e);
	e = s.find_first_of(delim, b);
      }
      return res;
    } // end of systemCall::tokenize

    void
    systemCall::copy(const std::string& src,
		     const std::string& dest)
    {
      using namespace std;
      struct stat srcInfos;
      struct stat destInfos;
      bool destStatus;
      if(src==dest){
	return;
      }
      if(::stat(src.c_str(),&srcInfos)==-1){
	systemCall::throwSystemError("systemCall::copy : can't stat file "+src,errno);
      }
      destStatus=::stat(dest.c_str(),&destInfos)==0;
      if(!destStatus){
	if(errno!=ENOENT){
	  systemCall::throwSystemError("systemCall::copy : can't stat file "+dest,errno);
	}
      }
      if(S_ISDIR(srcInfos.st_mode)){
	// copying a directory
	if(destStatus){
	  // the destination exist, it shall be a directory
	  if(!S_ISDIR(destInfos.st_mode)){
	    throw(SystemError("systemCall::copy : can't copy directory '"+
			      src+"' on "+systemCall::fileType(destInfos.st_mode)
			      +" '"+dest+"'."));
	  }
	  systemCall::copyDirectory(src,dest,true);
	  return;
	} else {
	  systemCall::copyDirectory(src,dest,false);
	  return;
	}
      } else if(S_ISREG(srcInfos.st_mode)){
	// copying a file, the destination
	// shall be a file or a directory
	if(destStatus){
	  if(S_ISDIR(destInfos.st_mode)){
	    // destination is a directory
	    const vector<string>& tmp = systemCall::tokenize(src,'/');
	    systemCall::copyFile(src,dest+'/'+tmp.back());
	    return;
	  } else if (S_ISREG(destInfos.st_mode)){
	    // destination is a file
	    systemCall::copyFile(src,dest);
	    return;
	  }
	  throw(SystemError("systemCall::copy : can't copy file '"+src+"' on "+
			    systemCall::fileType(destInfos.st_mode)+" '"+dest+"'."));
	}
	// destination don't exist, trying to copy to it
	systemCall::copyFile(src,dest);
	return;
      } else if(S_ISCHR(destInfos.st_mode)||S_ISBLK(destInfos.st_mode)||
		S_ISFIFO(destInfos.st_mode)||S_ISLNK(destInfos.st_mode)||
		S_ISSOCK(destInfos.st_mode)){
	string msg("systemCall::copy : can't copy object of type "+
		   systemCall::fileType(destInfos.st_mode));	
	throw(SystemError(msg));
      }
    } // end of systemCall::copy

    void
    systemCall::mkdir(const std::string& dir,const mode_t mode)
    {
      using namespace std;
      string path;
      const vector<string>& paths = systemCall::tokenize(dir,'/');
      vector<string>::const_iterator p;
      for(p=paths.begin();p!=paths.end();++p){
	if(!path.empty()){
	  path+="/"+*p;	  
	} else {
	  path=*p;
	}
	struct stat infos;
	if(::stat(path.c_str(),&infos)==-1){
	  if(errno==ENOENT){
	    // the file does not exist, create the directory
	    if(::mkdir(path.c_str(),mode)!=0){
	      if(errno==EEXIST){
		// this may happen if many processes are
		// used in the same time
		if(::stat(path.c_str(),&infos)==0){
		  if(S_ISDIR(infos.st_mode)){
		    return;
		  }
		} else {
		  systemCall::throwSystemError("systemCall::mkdir : can't stat file '"+*p+"'",errno);
		}
	      }
	      systemCall::throwSystemError("systemCall::mkdir : can't create directory '"+dir+"'",errno);
	    }
	  } else {
	    systemCall::throwSystemError("systemCall::mkdir : can't stat file '"+*p+"'",errno);
	  }
	} else {
	  // the path is valid, check if it is a directory
	  if(!S_ISDIR(infos.st_mode)){
	    string msg("systemCall::mkdir : '");
	    msg += path+"' exists and is not a directory.";
	    throw(SystemError(msg));
	  }
	}
      }
    } // end of systemCall::mkdir
  
    void
    systemCall::unlink(const std::string& f)
    {
      if(::unlink(f.c_str())!=0){
	systemCall::throwSystemError("systemCall::unlink : can't unlink file"+f,errno);
      }
    } // end of systemCall::unlinkx

    void
    systemCall::write(const int f,
		      const void* const v,
		      size_t s)
    {
      /*
       * code adapated from C. Blaess
       * "Programmation système en C sous linux"
       * 2ème édition, 2005, Eyrolles
       * page 5009
       */ 
      using namespace std;
      const unsigned char * b = static_cast<const unsigned char*>(v);
      size_t  r = s;
      ssize_t w;
      if(s>=SSIZE_MAX){
	string msg("systemCall::write : invalid size (s>=SSIZE_MAX)");
	throw(SystemError(msg));
      }
      while(r>0){
	while((w=::write(f,b,r))==-1){
	  if(errno==EINTR){
	    continue;
	  }
	  if(errno!=EAGAIN){
	    systemCall::throwSystemError("systemCall::write failed",errno);
	  }
	  ::sleep(1);
	}
	r -= w;
	b += w;
      }
    } // end of systemCall::write

    std::string
    systemCall::fileType(const mode_t mode)
    {
      using namespace std;
      if(S_ISDIR(mode)){
	return "directory";
      } else if(S_ISREG(mode)){
	return "regular file";
      } else if(S_ISCHR(mode)){
	return "character device";
      } else if(S_ISBLK(mode)){
	return "block device";
      } else if(S_ISFIFO(mode)){
	return "fifo";
      } else if(S_ISLNK(mode)){
	return "symbolic link";
      } else if(S_ISSOCK(mode)){
	return "socket";
      }
      return "unknown";
    } // end of systemCall::fileType

    void
    systemCall::copyDirectory(const std::string& src,
			      const std::string& dest,
			      const bool exists)
    {
      using namespace std;
      const vector<string>& paths = systemCall::tokenize(src,'/');
      string rdest;
      if(exists){
	rdest=dest+'/'+paths.back();
      } else {
	rdest=dest;
      }
      systemCall::mkdir(rdest);
      DIR *dir = opendir(src.c_str());
      struct dirent *p;
      if(dir==0){
	systemCall::throwSystemError("systemCall::copy, can't open directory "+src,
				     errno);
      }
      while((p=readdir(dir))!=NULL){
	if((strcmp(p->d_name,".")!=0)&&
	   (strcmp(p->d_name,"..")!=0)){
	  systemCall::copy(src+'/'+p->d_name,rdest);
	}
      }
      closedir(dir);
    } // end of systemCall::copyDirectory

    void
    systemCall::copyFile(const std::string& src,const std::string& dest)
    {
      using namespace std;
      ofstream out(dest.c_str(),ios_base::binary);
      ifstream in(src.c_str(),ios_base::binary);
      if(!in){
	string msg("systemCall::copyFile : can't open file "+src+" in read mode.");
	throw(SystemError(msg));
      }
      // local copy of the file
      if(!out){
	string msg("systemCall::copyFile : can't open file "+dest+" in write mode.");
	throw(SystemError(msg));
      }
      out.exceptions(ifstream::failbit | ifstream::badbit);
      out << in.rdbuf();
      in.close();
      out.close();
    } // end of systemCall::copyFile
    
    std::string
    systemCall::getCurrentWorkingDirectory(void)
    {
      using namespace std;
      char *name  = 0;
      size_t size = 16u;
      while(1){
	if((name=static_cast<char *>(realloc(name,size)))==0){
	  throw(SystemError("systemCall::getCurrentWorkingDirectory : out of memory"));
	}
	if(::getcwd(name,size)!=0){
	  break;
	}
	if(errno!=ERANGE){
	  systemCall::throwSystemError("systemCall::getCurrentWorkingDirectory ",errno);
	}
	size*=2u;
      }
      const string res(name);
      ::free(name);
      return res;
    } // end of systemCall::getCurrentWorkingDirectory

    std::string
    systemCall::getHostName(void)
    {
      using namespace std;
      char *name  = 0;
      size_t size = 16u;
      while(1){
	if((name=static_cast<char *>(realloc(name,size)))==0){
	  throw(SystemError("systemCall::getHostName : out of memory"));
	}
	if(::gethostname(name,size)==0){
	  break;
	}
	if(errno!=ENAMETOOLONG){
	  systemCall::throwSystemError("systemCall::getHostName ",errno);
	}
	size*=2u;
      }
      const string res(name);
      ::free(name);
      return res;
    } // end of systemCall::getHostName

    std::string
    systemCall::getUserName(void)
    {
      return ::getlogin();
    } // end of systemCall::getUserName

    void
    systemCall::changeCurrentWorkingDirectory(const std::string& name)
    {
      using namespace std;
      if(::chdir(name.c_str())==-1){
	string msg("systemCall::changeCurrentWorkingDirectory : ");
	msg += "can't change to directory "+name+".";
	systemCall::throwSystemError(msg,errno);
      }
    } // end of systemCall::changeCurrentWorkingDirectory

  } // end of namespace system

} // end of namespace tfel
