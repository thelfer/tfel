/*! 
 * \file  mfront/include/MFront/MFrontLock.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 nov 2009
 */

#ifndef   _LIB_MFRONT_MFRONTLOCK_H_
#define   _LIB_MFRONT_MFRONTLOCK_H_ 

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#include <windows.h>
#else
#include<semaphore.h>
#endif

namespace mfront{

  //! The Great Mfront Lock
  struct MFrontLock
  {
    static MFrontLock&
    getMFrontLock();
    void lock();
    void unlock();
  private:
    MFrontLock();
    MFrontLock(const MFrontLock&);
    MFrontLock&
    operator = (const MFrontLock&);
    ~MFrontLock();
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    HANDLE ghMutex;
#else
    sem_t * l;
#endif
  }; // end of struct MFrontLock

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTLOCK_H */

