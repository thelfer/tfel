/*!
 * \file  mfront/include/MFront/MFrontLock.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 nov 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTLOCK_H_
#define LIB_MFRONT_MFRONTLOCK_H_

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <semaphore.h>
#endif

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  //! The Great Mfront Lock
  struct MFRONT_VISIBILITY_EXPORT MFrontLock {
    static MFrontLock& getMFrontLock();
    void lock();
    void unlock();

   private:
    MFrontLock();
    MFrontLock(const MFrontLock&) = delete;
    MFrontLock(MFrontLock&&) = delete;
    MFrontLock& operator=(const MFrontLock&) = delete;
    MFrontLock& operator=(MFrontLock&&) = delete;
    ~MFrontLock();
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    HANDLE ghMutex;
#else
    sem_t* l;
#endif
  };  // end of struct MFrontLock

  /*!
   * structure performing RAII on MFrontLock objects.
   */
  struct MFRONT_VISIBILITY_EXPORT MFrontLockGuard {
    //! constructor
    MFrontLockGuard();
    //! destructor
    ~MFrontLockGuard();

   private:
    MFrontLockGuard(MFrontLockGuard&&) = delete;
    MFrontLockGuard(const MFrontLockGuard&) = delete;
    MFrontLockGuard& operator=(MFrontLockGuard&&) = delete;
    MFrontLockGuard& operator=(const MFrontLockGuard&) = delete;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTLOCK_H */
