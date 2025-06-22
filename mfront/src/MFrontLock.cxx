/*!
 * \file  mfront/src/MFrontLock.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 nov 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <sstream>
#include <cerrno>

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontLock.hxx"

namespace mfront {

  MFrontLock& MFrontLock::getMFrontLock() {
    static MFrontLock lock;
    return lock;
  }  // end of MFrontLock::getMFrontLock

  MFrontLock::MFrontLock() {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    this->ghMutex = CreateMutex(nullptr,    // default security attributes
                                FALSE,      // initially not owned
                                "mfront");  // named mutex
    tfel::raise_if(this->ghMutex == nullptr,
                   "MFrontLock::MFrontLock: "
                   "semaphore creation failed");
#else
    std::ostringstream sn;
    sn << "/mfront-" << ::geteuid();
    this->l = ::sem_open(sn.str().c_str(), O_CREAT, S_IRUSR | S_IWUSR, 1);
    tfel::raise_if(this->l == SEM_FAILED,
                   "MFrontLock::MFrontLock: "
                   "semaphore creation failed");
#endif
  }  // end of MFrontLock::MFrontLock()

  void MFrontLock::lock() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "MFrontLock::lock: "
                     << "trying to lock semaphore\n";
    }
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    DWORD dwWaitResult;
    dwWaitResult = ::WaitForSingleObject(this->ghMutex,  // handle to mutex
                                         INFINITE);      // no time-out interval
    tfel::raise_if(dwWaitResult == WAIT_ABANDONED,
                   "MFrontLock::MFrontLock: "
                   "semaphore can't be aquired");
#else
    tfel::raise_if(::sem_wait(this->l) == -1,
                   "MFrontLock::MFrontLock: "
                   "semaphore can't be aquired");
#endif
  }  // end of MFrontLock::lock()

  void MFrontLock::unlock() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "MFrontLock::unlock: "
                     << "unlocking semaphore\n";
    }
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    ::ReleaseMutex(this->ghMutex);
#else
    ::sem_post(this->l);
#endif
  }  // end of MFrontLock::unlock()

  MFrontLock::~MFrontLock() {
    this->unlock();
  }  // end of MFrontLock::~MFrontLock()

  MFrontLockGuard::MFrontLockGuard() {
    MFrontLock::getMFrontLock().lock();
  }  // end of MFrontLockGuard::MFrontLockGuard

  MFrontLockGuard::~MFrontLockGuard() {
    MFrontLock::getMFrontLock().unlock();
  }  // end of MFrontLockGuard::~MFrontLockGuard

}  // end of namespace mfront
