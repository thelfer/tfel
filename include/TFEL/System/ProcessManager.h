/*!
 * \file   include/TFEL/System/ProcessManager.h
 * \brief
 *§
 * \author Thomas Helfer
 * \date 09/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_PROCESSMANAGER_H
#define LIB_TFEL_SYSTEM_PROCESSMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int processManager_wifsignaled(int);

int processManager_wifstopped(int);

int processManager_wifexited(int);

int processManager_wexitstatus(int);

#ifndef _AIX
int processManager_wtermsig(int);

int processManager_wcoredump(int);
#endif /* _AIX */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_TFEL_SYSTEM_PROCESSMANAGER_H */
