/*! 
 * \file  mfront/src/MFrontLock.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 nov 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<sstream>
#include<cerrno>

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
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
    this->ghMutex = CreateMutex(NULL,      // default security attributes
				FALSE,     // initially not owned
				"mfront"); // named mutex
    if (this->ghMutex == NULL){
      string msg("MFrontLock::MFrontLock : ");
      msg += "semaphore creation failed";
      throw(runtime_error(msg));
    }
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
    DWORD dwWaitResult;
    dwWaitResult = ::WaitForSingleObject(this->ghMutex, // handle to mutex
					 INFINITE);     // no time-out interval
    if(dwWaitResult==WAIT_ABANDONED){
      string msg("MFrontLock::MFrontLock : ");
      msg += "semaphore can't be aquired";
      throw(runtime_error(msg));
    }
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
    ::ReleaseMutex(this->ghMutex);
#else
    ::sem_post(this->l);
#endif
  } // end of MFrontLock::unlock()
    
  MFrontLock::~MFrontLock()
  {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    ::CloseHandle(this->ghMutex);
#else
    ::sem_close(this->l);
#endif
  } // end of MFrontLock::~MFrontLock()

} // end of namespace mfront

