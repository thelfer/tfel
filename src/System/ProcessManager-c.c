/*!
 * \file   ProcessManager.c
 * \brief
 *
 * \author Helfer Thomas
 * \date 09/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
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
