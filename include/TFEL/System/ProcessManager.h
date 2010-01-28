/*!
 * \file   ProcessManager.h
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2007
 */

#ifndef _LIB_TFEL_PROCESSMANAGER_H_
#define _LIB_TFEL_PROCESSMANAGER_H_ 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
  
  int
  processManager_wifsignaled(int);
  
  int
  processManager_wifstopped(int);
  
  int
  processManager_wifexited(int);
  
  int
  processManager_wexitstatus(int);
  
  int
  processManager_wtermsig(int);
  
  int
  processManager_wcoredump(int);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif  /* !PROCESSMANAGER_H_ */
