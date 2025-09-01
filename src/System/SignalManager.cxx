/*!
 * \file   src/System/SignalManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef __CYGWIN__
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif /* __CYGWIN__ */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cassert>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "TFEL/System/SystemError.hxx"
#include "TFEL/System/SignalManager.hxx"

namespace tfel::system {

  SignalManager& SignalManager::getSignalManager() {
    static SignalManager signalManager;
    return signalManager;
  }

  void SignalManager::callGdb(const int fd, const char* const id) {
    // we cannot use an instance of ProcessManager
    // ProcessManager uses code which is not async-safe
    char buf[3];
    int ffd[2];  //< pipe to the father
    int in[2];   //< pipe to gdb
    ssize_t readChar;
    pid_t pid;
    int status;
    buf[2] = '\0';
    // creating pipe
    if (::pipe(in) == -1) {
      std::cerr << "callGdb : pipe creation failed (" << strerror(errno)
                << ")\n";
      return;
    }
    // creating pipe
    if (pipe(ffd) == -1) {
      std::cerr << "callGdb : pipe creation failed (" << strerror(errno)
                << ")\n";
      return;
    }
    // forking
    pid = ::fork();
    if (pid == -1) {
      // fork failed
      // closing the pipes
      ::close(ffd[0]);
      ::close(ffd[1]);
      std::cerr << "callGdb : fork failed (" << strerror(errno) << ")\n";
      return;
    }
    if (pid == 0) {
      // the child
      // we are in the child
      ::close(ffd[0]);
      // makes the pipe to be close on exec
      status = ::fcntl(ffd[1], F_GETFD);
      status |= FD_CLOEXEC;
      ::fcntl(ffd[1], F_SETFD, status);
      if (in[0] != -1) {
        ::dup2(in[0], STDIN_FILENO);
      }
      // calling the external process
      ::execlp("gdb", "gdb", "-q", nullptr);
      // called failed, tells the father, free memory and quit
      ::write(ffd[1], "NO", 2u);
      ::close(ffd[1]);
      return;
    }
    // here we are in the father
    ::close(ffd[1]);
    // waiting for the child to do its job
    while ((readChar = ::read(ffd[0], buf, 2u)) == -1) {
      if (errno != EINTR) {
        break;
      }
    }
    ::close(ffd[0]);
    if (readChar > 0) {
      // something was in the pipe, which means that exec failed
      waitpid(pid, &status, 0);
      std::cerr << "callGdb : call to execvp failed\n";
      // tells the father to die
      ::write(fd, "NO", 2u);
      return;
    }
    ::write(in[1], "attach ", strlen("attach "));
    ::write(in[1], id, strlen(id));
    ::write(in[1], "\n", sizeof(char));
    ::write(in[1], "backtrace\n", strlen("backtrace\n"));
    ::write(in[1], "quit\n", strlen("quit\n"));
    ::write(in[1], "y\n", strlen("y\n"));
    ::waitpid(pid, &status, 0);
    // tells the father to die
    ::write(fd, "OK", 2u);
  }  // end of SignalManager::callGdb

  void SignalManager::printBackTrace(const int) {
    std::ostringstream ospid;
    std::string spid;
    char id[16];
    char buf[3];
    int res[2];
    int status;
    pid_t pid;
    ssize_t readChar;
    buf[2] = '\0';
    ospid << getpid();
    spid = ospid.str();
    if (spid.size() >= 15) {
      std::cerr << "print_trace : pid too high\n";
      return;
    }
    std::copy(spid.begin(), spid.end(), id);
    id[spid.size()] = '\0';
    // creating a first pipe
    if (pipe(res) == -1) {
      std::cerr << "print_trace : pipe creation failed (" << strerror(errno)
                << ")\n";
      return;
    }
    // forking
    pid = fork();
    if (pid == -1) {
      // fork failed
      // closing the pipes
      ::close(res[0]);
      ::close(res[1]);
      ::fprintf(stderr, "print_trace : fork creation failed (%s)\n",
                strerror(errno));
      return;
    }
    if (pid == 0) {
      ::close(res[0]);
      SignalManager::callGdb(res[1], id);
    }
    ::close(res[1]);
    while ((readChar = read(res[0], buf, 2u)) == -1) {
      if (errno != EINTR) {
        break;
      }
    }
    ::close(res[0]);
    assert(readChar > 0);
    static_cast<void>(
        readChar);  // disable a warning in icpc when NDEBUG is defined
    ::waitpid(pid, &status, 0);
  }  // end of SignalManager::printBackTrace

  SignalManager::SignalManager() : handlerNbr(0u) {}

  void SignalManager::eraseHandlers() {
    for (auto& c : SignalManager::callBacks) {
      for (auto& ptr : c.second) {
        delete ptr.second;
      }
    }
    SignalManager::callBacks.clear();
  }  // end of SignalManager::eraseHandlers

  SignalManager::~SignalManager() {
    SignalManager::eraseHandlers();
  }  // end of SignalManager()::~SignalManager

  unsigned short SignalManager::registerHandler(const int sig,
                                                SignalHandler* const f,
                                                struct sigaction& action) {
    unsigned short handler;
    action.sa_handler = &SignalManager::treatAction;
    handler = SignalManager::handlerNbr;
    SignalManager::callBacks[sig].insert({handler, f});
    ++SignalManager::handlerNbr;
    sigaction(sig, &action, nullptr);
    return handler;
  }  // end of SignalManager::registerHandler

  unsigned short SignalManager::registerHandler(const int sig,
                                                SignalHandler* const f) {
    struct sigaction action;
    sigemptyset(&(action.sa_mask));
    if ((sig == SIGSEGV) || (sig == SIGFPE)) {
      action.sa_flags = SA_RESETHAND;
    } else {
      action.sa_flags = 0;
    }
    return SignalManager::registerHandler(sig, f, action);
  }  // end of SignalManager::registerHandler

  void SignalManager::removeHandler(const unsigned short id) {
    sigset_t nSigSet;
    sigset_t oSigSet;
    // blocking all signals
#ifdef sigfillset
    sigfillset(&nSigSet);
#else  /* sigfillset */
    ::sigfillset(&nSigSet);
#endif /* sigfillset */
#ifdef sigprocmask
    sigprocmask(SIG_BLOCK, &nSigSet, &oSigSet);
#else  /* sigprocmask */
    ::sigprocmask(SIG_BLOCK, &nSigSet, &oSigSet);
#endif /* sigprocmask */
    bool found = false;
    // removing the handle
    for (auto p = SignalManager::callBacks.begin();
         (p != SignalManager::callBacks.end()) && (!found); ++p) {
      auto p2 = p->second.begin();
      while ((p2 != p->second.end()) && (!found)) {
        if (p2->first == id) {
          delete p2->second;
          auto p3 = p2;
          ++p2;
          p->second.erase(p3);
          found = true;
        } else {
          ++p2;
        }
      }
    }
    // restoring the previous signal mask
#ifdef sigprocmask
    sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
#else  /* sigprocmask */
    ::sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
#endif /* sigprocmask */
    if (!found) {
      std::cerr << "SignalManager::removeHandler : "
                << "unknown handler " << id << '\n';
    }
  }  // end of SignalManager::removeHandler

  void SignalManager::treatAction(int sig) {
    auto& sm = SignalManager::getSignalManager();
    const auto p = sm.callBacks.find(sig);
    if (p != sm.callBacks.end()) {
      for (const auto& c : p->second) {
        c.second->execute(sig);
      }
    }
  }  // end of SignalManager::treatAction

}  // end of namespace tfel::system
