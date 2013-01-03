/*! 
 * \file  MFrontLock.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 nov 2009
 */

#include<stdexcept>
#include<sstream>
#include<cerrno>

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#warning "windows port"
#else
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>
#include<unistd.h>
#endif

#include"TFEL/System/System.hxx"
#include"MFront/MFrontLock.hxx"

namespace mfront{

  MFrontLock&
  MFrontLock::getMFrontLock()
  {
    static MFrontLock lock;
    return lock;
  } // end of MFrontLock::getMFrontLock

  MFrontLock::MFrontLock()
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#warning "windows port"
#else
    ostringstream sn;
    sn << "/mfront-" << ::geteuid();
    this->l = ::sem_open(sn.str().c_str(),O_CREAT,S_IRUSR|S_IWUSR,1);
    if(this->l==SEM_FAILED){
      string msg("MFrontLock::MFrontLock : ");
      msg += "semaphore creation failed";
      throw(runtime_error(msg));
    }
#endif
  } // end of MFrontLock::MFrontLock()

  void
  MFrontLock::lock()
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#warning "windows port"
#else
    if(::sem_wait(this->l)==-1){
      string msg("MFrontLock::MFrontLock : ");
      msg += "semaphore can't be aquired";
      throw(runtime_error(msg));
    }
#endif
  } // end of MFrontLock::lock()

  void
  MFrontLock::unlock()
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#warning "windows port"
#else
    ::sem_post(this->l);
#endif
  } // end of MFrontLock::unlock()
    
  MFrontLock::~MFrontLock()
  {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#warning "windows port"
#else
    ::sem_close(this->l);
#endif
  } // end of MFrontLock::~MFrontLock()

} // end of namespace mfront

