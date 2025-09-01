/*!
 * \file   src/System/ProcessManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03 nov 2007
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
#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cassert>

#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/SignalManager.hxx"
#include "TFEL/System/ProcessManager.h"
#include "TFEL/System/ProcessManager.hxx"

namespace tfel::system {

  ProcessManager::Command::~Command() = default;

  ProcessManager::ProcessManager() : shallStopOnSignals(false) {
    auto& signalManager = SignalManager::getSignalManager();
    struct sigaction action;
    auto f = &ProcessManager::sigChildHandler;
    // blocking all signals during treatment of sigChildHandler
    sigfillset(&(action.sa_mask));
    action.sa_flags = 0;
    this->sHandler =
        signalManager.registerHandler(SIGCHLD, sigMemFun(*this, f), action);
    this->stopOnSignals(true);
  }  // end of ProcessManager::ProcessManager

  void ProcessManager::stopOnSignals(const bool b) {
    using namespace std;
    auto& signalManager = SignalManager::getSignalManager();
    if (b == this->shallStopOnSignals) {
      return;
    }
    if (!this->shallStopOnSignals) {
      struct sigaction action;
      this->shallStopOnSignals = true;
      // blocking all signals during treatment of terminateHandler
      sigfillset(&(action.sa_mask));
      action.sa_flags = SA_RESETHAND;
      auto f = &ProcessManager::terminateHandler;
      this->sHandlerSIGBUS =
          signalManager.registerHandler(SIGBUS, sigMemFun(*this, f), action);
      this->sHandlerSIGSEGV =
          signalManager.registerHandler(SIGSEGV, sigMemFun(*this, f), action);
      this->sHandlerSIGFPE =
          signalManager.registerHandler(SIGFPE, sigMemFun(*this, f), action);
      this->sHandlerSIGABRT =
          signalManager.registerHandler(SIGABRT, sigMemFun(*this, f), action);
      this->sHandlerSIGHUP =
          signalManager.registerHandler(SIGHUP, sigMemFun(*this, f), action);
      this->sHandlerSIGILL =
          signalManager.registerHandler(SIGILL, sigMemFun(*this, f), action);
      this->sHandlerSIGTERM =
          signalManager.registerHandler(SIGTERM, sigMemFun(*this, f), action);
      this->sHandlerSIGINT =
          signalManager.registerHandler(SIGINT, sigMemFun(*this, f), action);
      this->sHandlerSIGQUIT =
          signalManager.registerHandler(SIGQUIT, sigMemFun(*this, f), action);
    } else {
      this->shallStopOnSignals = false;
      signalManager.removeHandler(this->sHandlerSIGBUS);
      signalManager.removeHandler(this->sHandlerSIGSEGV);
      signalManager.removeHandler(this->sHandlerSIGFPE);
      signalManager.removeHandler(this->sHandlerSIGABRT);
      signalManager.removeHandler(this->sHandlerSIGHUP);
      signalManager.removeHandler(this->sHandlerSIGILL);
      signalManager.removeHandler(this->sHandlerSIGTERM);
      signalManager.removeHandler(this->sHandlerSIGINT);
      signalManager.removeHandler(this->sHandlerSIGQUIT);
    }
  }  // end of ProcessManager::stopOnSignals

  void ProcessManager::closeProcessFiles(const ProcessManager::ProcessId id) {
    using namespace std;
    map<ProcessId, StreamId>::iterator p;
    if ((p = this->inputs.find(id)) != this->inputs.end()) {
      if (::close(p->second) == -1) {
        ostringstream msg;
        msg << "ProcessManager::closeProcessFiles : "
            << "can't close file descriptor " << p->second
            << " associated with process " << id;
        systemCall::throwSystemError(msg.str(), errno);
      }
      this->inputs.erase(p);
    }
    if ((p = this->outputs.find(id)) != this->outputs.end()) {
      if (::close(p->second) == -1) {
        ostringstream msg;
        msg << "ProcessManager::closeProcessFiles : "
            << "can't close file descriptor " << p->second
            << " associated with process " << id;
        systemCall::throwSystemError(msg.str(), errno);
      }
      this->outputs.erase(p);
    }
    if ((p = this->inputFiles.find(id)) != this->inputFiles.end()) {
      if (::close(p->second) == -1) {
        ostringstream msg;
        msg << "ProcessManager::closeProcessFiles : "
            << "can't close file descriptor " << p->second
            << " associated with process " << id;
        systemCall::throwSystemError(msg.str(), errno);
      }
      this->inputFiles.erase(p);
    }
    if ((p = this->outputFiles.find(id)) != this->outputFiles.end()) {
      if (::close(p->second) == -1) {
        ostringstream msg;
        msg << "ProcessManager::closeProcessFiles : "
            << "can't close file descriptor " << p->second
            << " associated with process " << id;
        systemCall::throwSystemError(msg.str(), errno);
      }
      this->outputFiles.erase(p);
    }
  }  // end of ProcessManager::closeProcessFiles

  void ProcessManager::terminateHandler(const int) {
    // treating handled processes
    auto& signalManager = SignalManager::getSignalManager();
    signalManager.removeHandler(this->sHandler);
    for (const auto& p : this->processes) {
      if (p.isRunning) {
        this->sendSignal(p.id);
        ::waitpid(p.id, nullptr, 0);
        this->closeProcessFiles(p.id);
      }
    }
    exit(-1);
  }  // end of ProcessManager::terminateHandler

  void ProcessManager::setProcessExitStatus(ProcessManager::Process& p,
                                            const int status) {
    if ((::processManager_wifexited(status)) ||
        (::processManager_wifsignaled(status))) {
      if ((::processManager_wifexited(status))) {
        // process died normally
        p.exitStatus = true;
        p.exitValue = ::processManager_wexitstatus(status);
      } else {
        // process died du to to a signal
        p.exitStatus = false;
        p.exitValue = -1;
      }
      this->closeProcessFiles(p.id);
      p.isRunning = false;
    } else if (!::processManager_wifstopped(status)) {
      std::ostringstream msg;
      msg << "ProcessManager::sigChildHandler : "
          << "unknown status for child " << p.id;
      systemCall::throwSystemError(msg.str(), errno);
    }
  }  // end of ProcessManager::setProcessExitStatus

  void ProcessManager::sigChildHandler(const int) {
    // treating handled processes
    for (auto& t : this->processes) {
      if (t.isRunning) {
        int status;
        int ret = waitpid(t.id, &status, WNOHANG);
        if (ret == t.id) {
          this->setProcessExitStatus(t, status);
        }
      }
    }
  }  // end of ProcessManager::sigChildHandler

  ProcessManager::ProcessId ProcessManager::createProcess(
      ProcessManager::Command& cmd) {
    using namespace std;
    int cfd[2];  //< pipe to the child
    int ffd[2];  //< pipe to the father
    pid_t pid;
    sigset_t nSigSet;
    sigset_t oSigSet;
    // creating pipes
    if (pipe(ffd) == -1) {
      string msg("ProcessManager::createProcess : pipe creation failed");
      systemCall::throwSystemError(msg, errno);
    }
    if (pipe(cfd) == -1) {
      string msg("ProcessManager::createProcess : pipe creation failed");
      systemCall::throwSystemError(msg, errno);
    }
    // entering a critical section, dont want to be disturbed by signals
    sigfillset(&nSigSet);
    sigprocmask(SIG_BLOCK, &nSigSet, &oSigSet);
    // forking
    pid = fork();
    if (pid == -1) {
      // fork failed
      // closing the pipes
      close(cfd[0]);
      close(cfd[1]);
      close(ffd[0]);
      close(ffd[1]);
      string msg("ProcessManager::createProcess : fork failed");
      systemCall::throwSystemError(msg, errno);
    }
    if (pid == 0) {
      char buf[3];
      ssize_t readChar;
      // the child
      close(ffd[0]);
      close(cfd[1]);
      // we are in the child
      this->cleanUp();
      // restoring the previous signal mask
      sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
      // wait that the father has made its administrative job
      while ((readChar = read(cfd[0], buf, 2u)) == -1) {
        if (errno != EINTR) {
          break;
        }
      }
      assert(readChar == 2);
      if (!cmd.execute(cfd[0], ffd[1])) {
        exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
    }
    // here we are in the father
    close(ffd[1]);
    close(cfd[0]);
    // registering
    this->inputs.insert({pid, cfd[1]});
    this->outputs.insert({pid, ffd[0]});
    Process proc;
    proc.id = pid;
    proc.isRunning = true;
    this->processes.push_back(proc);
    // registering is made, tell the child
    write(cfd[1], "OK", 2u);
    // the pipe was broken, which means that exec succeed
    sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
    return pid;
  }  // end of ProcessManager::createProcess

  ProcessManager::ProcessId ProcessManager::createProcess(
      const std::string& cmd,
      const ProcessManager::StreamId* const in,
      const ProcessManager::StreamId* const out,
      ProcessManager::StreamMap& ins,
      ProcessManager::StreamMap& outs,
      const std::map<std::string, std::string>& e) {
    using namespace std;
    vector<string> tmp;
    char buf[3];
    int cfd[2];  //< pipe to the child
    int ffd[2];  //< pipe to the father
    ssize_t readChar;
    pid_t pid;
    int status;
    vector<string>::const_iterator p;
    char** argv;
    char** p2;
    buf[2] = '\0';
    sigset_t nSigSet;
    sigset_t oSigSet;
    // splitting the argument
    if (cmd.empty()) {
      throw(SystemError("ProcessManager::createProcess : empty command"));
    }
    istringstream args(cmd);
    copy(istream_iterator<string>(args), istream_iterator<string>(),
         back_inserter(tmp));
    // creating pipes
    if (pipe(ffd) == -1) {
      string msg("ProcessManager::createProcess : pipe creation failed");
      systemCall::throwSystemError(msg, errno);
    }
    if (pipe(cfd) == -1) {
      string msg("ProcessManager::createProcess : pipe creation failed");
      systemCall::throwSystemError(msg, errno);
    }
    // entering a critical section, dont want to be disturbed by signals
    sigfillset(&nSigSet);
    sigprocmask(SIG_BLOCK, &nSigSet, &oSigSet);
    // forking
    pid = fork();
    if (pid == -1) {
      // fork failed
      // closing the pipes
      close(cfd[0]);
      close(cfd[1]);
      close(ffd[0]);
      close(ffd[1]);
      string msg("ProcessManager::createProcess : fork failed");
      systemCall::throwSystemError(msg, errno);
    }
    if (pid == 0) {
      // the child
      // we are in the child
      close(ffd[0]);
      close(cfd[1]);
      // makes the pipe to be close on exec
      status = fcntl(ffd[1], F_GETFD);
      status |= FD_CLOEXEC;
      fcntl(ffd[1], F_SETFD, status);
      if (in[0] != -1) {
        dup2(in[0], STDIN_FILENO);
      }
      if (out[0] != -1) {
        dup2(out[1], STDOUT_FILENO);
        dup2(out[1], STDERR_FILENO);
      }
      argv = static_cast<char**>(malloc((tmp.size() + 2) * sizeof(char*)));
      p = tmp.begin();
      p2 = argv;
      while (p != tmp.end()) {
        *p2 = static_cast<char*>(malloc((p->size() + 1) * sizeof(char)));
        strcpy(*p2, p->c_str());
        ++p;
        ++p2;
      }
      *p2 = nullptr;
      // wait that the father has made its administrative job
      while ((readChar = read(cfd[0], buf, 2u)) == -1) {
        if (errno != EINTR) {
          break;
        }
      }
      assert(readChar == 2);
      close(cfd[0]);
      // restoring the previous signal mask
      sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
      // calling the external process
      for (const auto& ev : e) {
        ::setenv(ev.first.c_str(), ev.second.c_str(), 1);
      }
      // calling the command
      execvp(argv[0], argv);
      // called failed, tells the father, free memory and quit
      write(ffd[1], "NO", 2u);
      close(ffd[1]);
      p2 = argv;
      while (p2 != argv + tmp.size() + 1) {
        free(*p2);
        ++p2;
      }
      free(argv);
      exit(EXIT_FAILURE);
    }
    // here we are in the father
    close(ffd[1]);
    close(cfd[0]);
    // registering
    if (in[0] != -1) {
      ins.insert({pid, in[1]});
    }
    if (out[0] != -1) {
      outs.insert({pid, out[0]});
    }
    Process proc;
    proc.id = pid;
    proc.isRunning = true;
    this->processes.push_back(proc);
    // registering is made, tell the child
    write(cfd[1], "OK", 2u);
    close(cfd[1]);
    // waiting for the child to do its job
    while ((readChar = read(ffd[0], buf, 2u)) == -1) {
      if (errno != EINTR) {
        break;
      }
    }
    close(ffd[0]);
    if (readChar > 0) {
      // something was in the pipe, which means that exec failed
      if (in[0] != -1) {
        ins.erase(ins.find(pid));
      }
      if (out[0] != -1) {
        outs.erase(outs.find(pid));
      }
      this->processes.pop_back();
      waitpid(pid, &status, 0);
      sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
      string msg("ProcessManager::createProcess : call to execvp failed ");
      msg += "(can't execute command '" + cmd + "')";
      throw(SystemError(msg));
    }
    // the pipe was broken, which means that exec succeed
    sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
    return pid;
  }  // end of ProcessManager::createProces

  ProcessManager::ProcessId ProcessManager::createProcess(
      const std::string& cmd,
      const std::string& inputFile,
      const std::string& outputFile,
      const std::map<std::string, std::string>& e) {
    using namespace std;
    const bool in = !inputFile.empty();
    const bool out = !outputFile.empty();
    int fdIn[2];
    int fdOut[2];
    fdIn[0] = -1;
    fdIn[1] = -1;
    fdOut[0] = -1;
    fdOut[1] = -1;
    pid_t pid;
    if (in) {
      fdIn[0] = open(inputFile.c_str(), O_RDONLY);
      if (fdIn[0] == -1) {
        string msg("ProcessManager::createProcess : can't open file ");
        msg += inputFile;
        systemCall::throwSystemError(msg, errno);
      }
      fdIn[1] = fdIn[0];
    }
    if (out) {
      fdOut[0] = open(outputFile.c_str(),
                      O_WRONLY | O_CREAT | O_TRUNC,  //< file open in write mode
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);  //< 0644
      if (fdOut[0] == -1) {
        if (in) {
          close(fdIn[0]);
        }
        string msg("ProcessManager::createProcess : can't open file ");
        msg += outputFile;
        systemCall::throwSystemError(msg, errno);
      }
      fdOut[1] = fdOut[0];
    }
    try {
      pid = this->createProcess(cmd, fdIn, fdOut, this->inputFiles,
                                this->outputFiles, e);
    } catch (...) {
      // something went wrong in createProcess,
      // cleaning up and rethrow
      if (in) {
        close(fdIn[0]);
      }
      if (out) {
        close(fdOut[0]);
      }
      throw;
    }
    return pid;
  }  // end of ProcessManager::createProcess

  ProcessManager::ProcessId ProcessManager::createProcess(
      const std::string& cmd, const ProcessManager::RedirectionType flag) {
    using namespace std;
    int fdIn[2];   //< pipe file descriptor;
    int fdOut[2];  //< pipe file descriptor;
    fdIn[0] = -1;
    fdIn[1] = -1;
    fdOut[0] = -1;
    fdOut[1] = -1;
    pid_t pid = -1;
    if ((flag == StdIn) || (flag == StdInAndOut)) {
      // creating the pipe
      if (pipe(fdIn) == -1) {
        string msg("ProcessManager::createProcess : pipe failed");
        systemCall::throwSystemError(msg, errno);
      }
    }
    if ((flag == StdOut) || (flag == StdInAndOut)) {
      // creating the pipe
      if (pipe(fdOut) == -1) {
        // cleaning up a bit
        if ((flag == StdIn) || (flag == StdInAndOut)) {
          close(fdIn[0]);
          close(fdIn[1]);
        }
        string msg("ProcessManager::createProcess : pipe failed");
        systemCall::throwSystemError(msg, errno);
      }
    }
    try {
      pid = this->createProcess(cmd, fdIn, fdOut, this->inputs, this->outputs);
    } catch (...) {
      // something went wrong in createProcess, cleaning up
      if ((flag == StdIn) || (flag == StdInAndOut)) {
        close(fdIn[0]);
        close(fdIn[1]);
      }
      if ((flag == StdOut) || (flag == StdInAndOut)) {
        close(fdOut[0]);
        close(fdOut[1]);
      }
      throw;
    }
    return pid;
  }  // end of ProcessManager::createProcess

  void ProcessManager::killProcess(const ProcessId pid) {
    using namespace std;
    auto p = this->findProcess(pid);
    auto pe = this->processes.rend();
    if (p == pe) {
      ostringstream msg;
      msg << "ProcessManager::killProcess : ";
      msg << "no process associated with pid " << pid;
      throw(SystemError(msg.str()));
    }
    if (p->isRunning) {
      this->sendSignal(pid, SIGKILL);
    }
  }  // end of ProcessManager::killProcess

  void ProcessManager::wait(const ProcessId pid) {
    using namespace std;
    int status;
    auto p = this->findProcess(pid);
    if (p == this->processes.rend()) {
      ostringstream msg;
      msg << "ProcessManager::sendSignal : process " << pid
          << " is not registred";
      throw(SystemError(msg.str()));
    }
    if (!p->isRunning) {
      return;
    }
    ::waitpid(pid, &status, 0);
    this->setProcessExitStatus(*p, status);
  }  // end of ProcessManager::wait

  void ProcessManager::sendSignal(const ProcessId pid, const int signal) {
    using namespace std;
    auto p = this->findProcess(pid);
    if (p == this->processes.rend()) {
      ostringstream msg;
      msg << "ProcessManager::sendSignal : process " << pid
          << " is not registred";
      throw(SystemError(msg.str()));
    }
    if (!p->isRunning) {
      ostringstream msg;
      msg << "ProcessManager::sendSignal : process " << pid
          << " is not running";
      throw(SystemError(msg.str()));
    }
    if (::kill(pid, signal) == -1) {
      ostringstream msg;
      msg << "ProcessManager::sendSignal : sending signal "
          << "to process " << pid << " failed";
      systemCall::throwSystemError(msg.str(), errno);
    }
  }  // end of ProcessManager::sendSignal

  ProcessManager::~ProcessManager() {
    using namespace std;
    auto& signalManager = SignalManager::getSignalManager();
    vector<Process>::iterator p;
    map<ProcessId, StreamId>::iterator p2;
    int status;
    sigset_t nSigSet;
    sigset_t oSigSet;
    // blocking all signals
    sigfillset(&nSigSet);
    sigprocmask(SIG_BLOCK, &nSigSet, &oSigSet);
    // killing all remaining processes
    for (p = this->processes.begin(); p != this->processes.end(); ++p) {
      if (p->isRunning) {
        try {
          this->sendSignal(p->id, SIGKILL);
        } catch (SystemError& e) {
          cerr << "ProcessManager::~ProcessManager : FIXME "
               << "sending signal to process " << p->id << " failed ("
               << e.what() << ").\n";
        }
        if (::waitpid(p->id, &status, 0) == -1) {
          cerr << "ProcessManager::~ProcessManager : FIXME "
               << "waipid for process " << p->id << " failed ("
               << strerror(errno) << ").\n";
        }
        p2 = this->inputs.find(p->id);
        if (p2 != this->inputs.end()) {
          if (::close(p2->second) == -1) {
            cerr << "ProcessManager::~ProcessManager : FIXME "
                 << "can't close file descriptor " << p2->second
                 << " associated with process " << p2->first << " ("
                 << strerror(errno) << ").\n";
          }
        }
        p2 = this->outputs.find(p->id);
        if (p2 != this->outputs.end()) {
          if (::close(p2->second) == -1) {
            cerr << "ProcessManager::~ProcessManager : FIXME "
                 << "can't close file descriptor " << p2->second
                 << " associated with process " << p2->first << " ("
                 << strerror(errno) << ").\n";
          }
        }
        p2 = this->inputFiles.find(p->id);
        if (p2 != this->inputFiles.end()) {
          if (::close(p2->second) == -1) {
            cerr << "ProcessManager::~ProcessManager : FIXME "
                 << "can't close file descriptor " << p2->second
                 << " associated with process " << p2->first << " ("
                 << strerror(errno) << ").\n";
          }
        }
        p2 = this->outputFiles.find(p->id);
        if (p2 != this->outputFiles.end()) {
          if (::close(p2->second) == -1) {
            cerr << "ProcessManager::~ProcessManager : FIXME "
                 << "can't close file descriptor " << p2->second
                 << " associated with process " << p2->first << " ("
                 << strerror(errno) << ").\n";
          }
        }
      }
    }
    // removing the handle
    signalManager.removeHandler(this->sHandler);
    if (this->shallStopOnSignals) {
      signalManager.removeHandler(this->sHandlerSIGSEGV);
      signalManager.removeHandler(this->sHandlerSIGFPE);
      signalManager.removeHandler(this->sHandlerSIGABRT);
      signalManager.removeHandler(this->sHandlerSIGHUP);
      signalManager.removeHandler(this->sHandlerSIGILL);
      signalManager.removeHandler(this->sHandlerSIGINT);
      signalManager.removeHandler(this->sHandlerSIGQUIT);
    }
    // restoring the previous signal mask
    sigprocmask(SIG_SETMASK, &oSigSet, nullptr);
  }  // end of ProcessManager::~ProcessManager

  ProcessManager::wstream ProcessManager::getInputStream(
      const ProcessManager::ProcessId id) const {
    auto throw_if = [](const bool c, const std::string& msg) {
      raise_if(c, "ProcessManager::getInputStream: " + msg);
    };
    const auto p = this->findProcess(id);
    const auto pe = this->processes.rend();
    throw_if(p == pe, "no process associated with pid " + std::to_string(id));
    throw_if(!p->isRunning, "process associated with pid " +
                                std::to_string(id) + " is not running");
    const auto p2 = this->inputs.find(id);
    throw_if(p2 == this->inputs.end(),
             "no stream associated with pid " + std::to_string(id));
    return {p2->second};
  }  // end of ProcessManager::getInputStream

  ProcessManager::rstream ProcessManager::getOutputStream(
      const ProcessManager::ProcessId id) const {
    auto throw_if = [](const bool c, const std::string& msg) {
      raise_if(c, "ProcessManager::getOutputStream: " + msg);
    };
    const auto p = this->findProcess(id);
    const auto pe = this->processes.rend();
    throw_if(p == pe, "no process associated with pid " + std::to_string(id));
    throw_if(!p->isRunning,
             "process associated with "
             "pid " +
                 std::to_string(id) + " is not running");
    const auto p2 = this->outputs.find(id);
    throw_if(p2 == this->outputs.end(),
             "no stream associated with pid " + std::to_string(id));
    return {p2->second};
  }  // end of ProcessManager::getOutputStream

  void ProcessManager::execute(const std::string& cmd,
                               const std::string& in,
                               const std::string& out,
                               const std::map<std::string, std::string>& e) {
    auto pid = this->createProcess(cmd, in, out, e);
    const auto p = this->findProcess(pid);
    assert(p != this->processes.rend());
    auto cond = p->isRunning;
    while (cond) {
      cond = p->isRunning;
      if (cond) {
        pause();
      }
    }
    if (!p->exitStatus) {
      throw(
          SystemError("ProcessManager::execute : '"
                      "' exited du to a signal"));
    }
    if (p->exitValue != EXIT_SUCCESS) {
      std::ostringstream msg;
      msg << "ProcessManager::execute : '" << cmd
          << "' exited abnormally with value " << p->exitValue;
      raise<SystemError>(msg.str());
    }
  }  // end of ProcessManager::execute

  std::vector<ProcessManager::Process>::reverse_iterator
  ProcessManager::findProcess(const ProcessId pid) {
    auto p = this->processes.rbegin();
    auto pe = this->processes.rend();
    while (p != pe) {
      if (p->id == pid) {
        break;
      }
      ++p;
    }
    return p;
  }  // end of ProcessManager::findProcess

  std::vector<ProcessManager::Process>::const_reverse_iterator
  ProcessManager::findProcess(const ProcessId pid) const {
    auto p = this->processes.rbegin();
    auto pe = this->processes.rend();
    while (p != pe) {
      if (p->id == pid) {
        break;
      }
      ++p;
    }
    return p;
  }  // end of ProcessManager::findProcess

  void ProcessManager::cleanUp() {
    // clean up
    this->processes.clear();
    this->inputs.clear();
    this->outputs.clear();
    this->inputFiles.clear();
    this->outputFiles.clear();
  }  // end of ProcessManager::cleanUp

}  // end of namespace tfel::system
