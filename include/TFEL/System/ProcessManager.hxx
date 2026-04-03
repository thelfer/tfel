/*!
 * \file   include/TFEL/System/ProcessManager.hxx
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

#ifndef LIB_TFEL_SYSTEM_PROCESSMANAGER_HXX
#define LIB_TFEL_SYSTEM_PROCESSMANAGER_HXX

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <csignal>
#include <sys/types.h>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/SystemError.hxx"
#include "TFEL/System/rstreamView.hxx"
#include "TFEL/System/wstreamView.hxx"

namespace tfel::system {

  /*!
   * \brief a class used to handle external processes
   */
  struct TFELSYSTEM_VISIBILITY_EXPORT ProcessManager {
    //! \brief a simple alias
    using ProcessId = pid_t;
    //! \brief a simple alias
    using StreamId = int;
    //! \brief a simple alias
    using rstream = rstreamView<true>;
    //! \brief a simple alias
    using wstream = wstreamView<true>;
    /*!
     * \brief base class to pass commands to sub processes
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT Command {
      virtual bool execute(const StreamId, const StreamId) = 0;
      //! destructor
      virtual ~Command();
    };  // end of struct Command

    enum RedirectionType { None, StdIn, StdOut, StdInAndOut };

    ProcessManager();

    virtual void killProcess(const ProcessId);

    virtual void sendSignal(const ProcessId, const int = SIGTERM);

    /*!
     * create a new process
     * \param const std::string&, command to be executed. The first word
     * is the program name, the others options
     * \param const RedirectionType, tells if standard file descriptors must be
     * redirected.
     * \return const ProcessId, the pid of the new process
     */
    virtual ProcessId createProcess(const std::string&,
                                    const RedirectionType = None);
    /*!
     * create a new process
     * \param[in] command: command to be executed. The first word
     * is the program name, the others options
     * \param[in] const std::string&, name of a file to which the
     * new process standard input is redirected. This option is ignored
     * if this name is void.
     * \param const std::string&, name of a file to which the
     * new process standard output is redirected. This option is ignored
     * if this name is void.
     * \param[in] e : additionnal environment variables for the child process
     * \return the pid of the new process
     */
    virtual ProcessId createProcess(
        const std::string&,
        const std::string&,
        const std::string&,
        const std::string&,
        const std::map<std::string, std::string>& = {});
    /*!
     * create a new process
     * \param[in] command: command to be executed. The first word
     * is the program name, the others options
     * \param[in] const std::string&, name of a file to which the
     * new process standard input is redirected. This option is ignored
     * if this name is void.
     * \param const std::string&, name of a file to which the
     * new process standard output is redirected. This option is ignored
     * if this name is void.
     * \param[in] e : additionnal environment variables for the child process
     * \return the pid of the new process
     */
    virtual ProcessId createProcess(
        const std::string&,
        const std::string&,
        const std::string&,
        const std::map<std::string, std::string>& = {});
    /*!
     * execute the command and wait until its end
     * \param const std::string&, command to be executed. The first word
     * is the program name, the others options
     * \param const std::string&, name of a file to which the
     * new process standard input is redirected. This option is ignored
     * if this name is void.
     * \param const std::string&, name of a file to which the
     * new process standard output is redirected. This option is ignored
     * if this name is void.
     * \param[in] e : additionnal environment variables for the child process
     */
    virtual void execute(const std::string&,
                         const std::string&,
                         const std::string& = "",
                         const std::string& = "",
                         const std::map<std::string, std::string>& = {});
    /*!
     * execute the command and wait until its end
     * \param const std::string&, command to be executed. The first word
     * is the program name, the others options
     * \param const std::string&, name of a file to which the
     * new process standard input is redirected. This option is ignored
     * if this name is void.
     * \param const std::string&, name of a file to which the
     * new process standard output is redirected. This option is ignored
     * if this name is void.
     * \param[in] e : additionnal environment variables for the child process
     */
    virtual void execute(const std::string&,
                         const std::string& = "",
                         const std::string& = "",
                         const std::map<std::string, std::string>& = {});

    virtual void stopOnSignals(const bool);

    virtual wstream getInputStream(const ProcessId) const;

    virtual rstream getOutputStream(const ProcessId) const;

    virtual ProcessId createProcess(ProcessManager::Command&);

    virtual void wait(const ProcessId);

    /*!
     * destructor.
     * kills all registred process and close all input/output file descriptors.
     */
    virtual ~ProcessManager();

   protected:
    virtual void cleanUp();

   private:
    struct TFEL_VISIBILITY_LOCAL Process {
      ProcessId id;
      bool isRunning;
      bool exitStatus;
      int exitValue;
    };  // end of struct Process

    //! \brief a simple alias
    using StreamMap = std::map<ProcessId, StreamId>;

    /*
     * create a new process
     * \param const std::string&, command to be executed. The first word
     * is the program name, the others options
     * \param file descriptor to which the new process standard input is
     * redirected (ignored if -1)
     * \param file descriptor to which the new process standard output is
     * redirected (ignored if -1)
     * \param StreamMap&, map to which the input file descriptor is
     * registred (if any)
     * \param StreamMap&, map to which the output file descriptor is
     * registred (if any)
     * \param[in] e : additionnal environment variables for the child process
     * \return the pid of the new process
     */
    TFEL_VISIBILITY_LOCAL ProcessId
    createProcess(const std::string&,
                  const std::string&,
                  const StreamId* const,
                  const StreamId* const,
                  StreamMap&,
                  StreamMap&,
                  const std::map<std::string, std::string>& = {});

    TFEL_VISIBILITY_LOCAL
    void sigChildHandler(const int);

    [[noreturn]] TFEL_VISIBILITY_LOCAL void terminateHandler(const int);

    TFEL_VISIBILITY_LOCAL void setProcessExitStatus(Process&, const int);

    TFEL_VISIBILITY_LOCAL void closeProcessFiles(const ProcessId);

    TFEL_VISIBILITY_LOCAL
    std::shared_ptr<Process> findProcess(const ProcessId);

    TFEL_VISIBILITY_LOCAL
    std::shared_ptr<const Process> findProcess(const ProcessId) const;

    std::vector<std::shared_ptr<Process>> processes;

    StreamMap inputs;
    StreamMap outputs;
    StreamMap inputFiles;
    StreamMap outputFiles;
    unsigned short sHandler;
    unsigned short sHandlerSIGBUS;
    unsigned short sHandlerSIGSEGV;
    unsigned short sHandlerSIGFPE;
    unsigned short sHandlerSIGABRT;
    unsigned short sHandlerSIGHUP;
    unsigned short sHandlerSIGILL;
    unsigned short sHandlerSIGINT;
    unsigned short sHandlerSIGTERM;
    unsigned short sHandlerSIGQUIT;
    bool shallStopOnSignals;

  };  // end of struct ProcessManager

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEM_PROCESSMANAGER_HXX */
