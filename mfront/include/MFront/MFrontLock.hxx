/*! 
 * \file  MFrontLock.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 nov 2009
 */

#ifndef   _LIB_MFRONT_MFRONTLOCK_H_
#define   _LIB_MFRONT_MFRONTLOCK_H_ 

#include<semaphore.h>

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
    sem_t * l;
  }; // end of struct MFrontLock

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTLOCK_H */

