/*!
 * \file  mfront/include/MFront/MFrontLock.hxx
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

#ifndef LIB_MFRONT_MFRONTLOCK_HXX
#define LIB_MFRONT_MFRONTLOCK_HXX

#include <atomic>

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief The Great Mfront Lock
   *
   * This lock is shared by all the mfront processes of a given user. It
   * is used to serialize the accesses to the files which may be written
   * concurrently by several mfront processes running in the same
   * directory (in particular the files generated for external mfront
   * files which can be used by several behaviours or material
   * properties).
   *
   * The lock is reentrant for a given process. It is designed to
   * serialize accesses between processes and not between threads.
   */
  struct MFRONT_VISIBILITY_EXPORT MFrontLock {
    static MFrontLock& getMFrontLock();
    //
    MFrontLock(const MFrontLock&) = delete;
    MFrontLock(MFrontLock&&) = delete;
    MFrontLock& operator=(const MFrontLock&) = delete;
    MFrontLock& operator=(MFrontLock&&) = delete;
    //! \brief acquire the lock (recursive: only the outermost call
    ///  acquires the underlying lock)
    void lock();
    //! \brief release the lock (only the outermost call releases the
    ///  underlying lock)
    void unlock();

   private:
    MFrontLock();
    ~MFrontLock();
    //! \brief number of acquisitions of the lock by this process
    std::atomic<int> count;
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    HANDLE ghMutex;
#else
    //! \brief file descriptor of the lock file
    int fd;
#endif
  };  // end of struct MFrontLock

  /*!
   * structure performing RAII on MFrontLock objects.
   */
  struct MFRONT_VISIBILITY_EXPORT MFrontLockGuard {
    //! \brief constructor
    MFrontLockGuard();
    //
    MFrontLockGuard(MFrontLockGuard&&) = delete;
    MFrontLockGuard(const MFrontLockGuard&) = delete;
    MFrontLockGuard& operator=(MFrontLockGuard&&) = delete;
    MFrontLockGuard& operator=(const MFrontLockGuard&) = delete;
    //! \brief destructor
    ~MFrontLockGuard();
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTLOCK_H */
