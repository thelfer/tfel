/*!
 * \file   ProcessManager.h
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2007
 */

#ifndef LIB_TFEL_PROCESSMANAGER_H_
#define LIB_TFEL_PROCESSMANAGER_H_ 

#ifdef	__cplusplus
extern "C" {
#endif /* LIB_TFEL_PROCESSMANAGER_H_ */
  
  int
  processManager_wifsignaled(int);
  
  int
  processManager_wifstopped(int);
  
  int
  processManager_wifexited(int);
  
  int
  processManager_wexitstatus(int);

#ifndef _AIX
  int
  processManager_wtermsig(int);
  
  int
  processManager_wcoredump(int);
#endif _AIX
  
#ifdef	__cplusplus
}
#endif /* LIB_TFEL_PROCESSMANAGER_H_ */

#endif /* LIB_TFEL_PROCESSMANAGER_H_ */
