/*!
 * \file  src/System/ChildProcess.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sys/types.h>
#include <csignal>

#include "TFEL/System/ChildProcess.hxx"

namespace tfel {

  namespace system {

    ChildProcess::ChildProcess(const int i, const int o)
        : pid(-1),
          inputStream(i),
          outputStream(o) {}  // end of ChildProcess::ChildProcess

    ChildProcess::~ChildProcess() {
      if (this->pid != -1) {
        ::kill(this->pid, SIGKILL);
      }
      ::close(this->inputStream);
      ::close(this->outputStream);
    }  // end of ChildProcess::~ChildProcess

    pid_t ChildProcess::getPid() const {
      return this->pid;
    }  // end of ChildProcess::getPid

    void ChildProcess::setPid(const pid_t p) {
      this->pid = p;
    }  // end of ChildProcess::setPid

    int ChildProcess::getInputStream() const {
      return this->inputStream;
    }  // end of ChildProcess::getOuputStream

    int ChildProcess::getOutputStream() const {
      return this->outputStream;
    }  // end of ChildProcess::getOuputStream

    ChildProcessCreationFailedException::ChildProcessCreationFailedException()
        : SystemError(strerror(errno)) {
    }  // end of
       // ChildProcessCreationFailedException::ChildProcessCreationFailedException

    ChildProcessCreationFailedException::
        ~ChildProcessCreationFailedException() noexcept = default;

  }  // end of namespace system

}  // end of namespace tfel
