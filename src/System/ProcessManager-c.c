/*!
 * \file   ProcessManager.c
 * \brief
 *
 * \author Helfer Thomas
 * \date   09 nov 2007
 */

#include <sys/wait.h>
#include "TFEL/System/ProcessManager.h"

int processManager_wifexited(int status) {
  return WIFEXITED(status);
} /* end of processManager_wifexited */

int processManager_wifsignaled(int status) {
  return WIFSIGNALED(status);
} /* end of processManager_wifsignaled */

int processManager_wifstopped(int status) {
  return WIFSTOPPED(status);
} /* end of processManager_wifstopped */

int processManager_wexitstatus(int status) {
  return WEXITSTATUS(status);
} /* end of processManager_wexitstatus */

#ifndef _AIX
int processManager_wtermsig(int status) {
  return WTERMSIG(status);
} /* end of processManager_wtermsig */

int processManager_wcoredump(int status) {
#ifdef WCOREDUMP
  return WCOREDUMP(status);
#else
  return 0;
#endif
} /* end of processManager_wcoredump */

#endif
