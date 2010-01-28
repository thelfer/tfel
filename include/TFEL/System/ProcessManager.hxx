/*!
 * \file   ProcessManager.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Nov 2007
 */

#include<string>
#include<vector>
#include<map>

#include<sys/types.h>
#include<signal.h>

#include"TFEL/System/SystemError.hxx"
#include"TFEL/System/rstreamView.hxx"
#include"TFEL/System/wstreamView.hxx"

namespace tfel
{

  namespace system
  {

    struct ProcessManager
    {
      // a simple alias
      typedef pid_t ProcessId;
      // a simple alias
      typedef int   StreamId;
      // a simple alias
      typedef std::map<ProcessId,StreamId> StreamMap;
      // a simple alias
      typedef rstreamView<true> rstream;
      // a simple alias
      typedef wstreamView<true> wstream;

      enum RedirectionType{None,StdIn,StdOut,StdInAndOut};
      
      ProcessManager();

      void
      killProcess(const ProcessId);
      
      void
      sendSignal(const ProcessId,const int = SIGTERM);

      /*
       * create a new process
       * \param const std::string&, command to be executed. The first word
       * is the program name, the others options
       * \param const RedirectionType, tells if standard file descriptors must be
       * redirected.
       * \return const ProcessId, the pid of the new process
       */
      ProcessId
      createProcess(const std::string&,
		    const RedirectionType = None);
      
      /*
       * create a new process
       * \param const std::string&, command to be executed. The first word
       * is the program name, the others options
       * \param const std::string&, name of a file to which the
       * new process standard input is redirected. This option is ignored
       * if this name is void.
       * \param const std::string&, name of a file to which the
       * new process standard output is redirected. This option is ignored
       * if this name is void.
       * \return const ProcessId, the pid of the new process
       */
      ProcessId
      createProcess(const std::string&,
		    const std::string&,
		    const std::string&);

      /*
       * execute the command and wait until is end
       * \param const std::string&, command to be executed. The first word
       * is the program name, the others options
       * \param const std::string&, name of a file to which the
       * new process standard input is redirected. This option is ignored
       * if this name is void.
       * \param const std::string&, name of a file to which the
       * new process standard output is redirected. This option is ignored
       * if this name is void.
       */
      void
      execute(const std::string&,
	      const std::string& = "",
	      const std::string& = "");
      
      void
      stopOnSignals(const bool);

      wstream
      getInputStream(const ProcessId);
      
      rstream
      getOutputStream(const ProcessId);
      
      /*!
       * destructor.
       * kills all registred process and close all input/output file descriptors.
       */
      ~ProcessManager();
      
    private:
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
       * \return const ProcessId, the pid of the new process
       */
      ProcessId
      createProcess(const std::string&,
		    const StreamId *const,
		    const StreamId *const,
		    StreamMap&,StreamMap&);


      void sigChildHandler(const int);

      void terminateHandler(const int);

      void
      closeProcessFiles(const ProcessId);
      
      struct Process
      {
	ProcessId id;
	bool isRunning;
	bool exitStatus;
	int exitValue;
      }; // end of struct Process

      std::vector<Process> processes;

      std::vector<Process>::reverse_iterator
      findProcess(const ProcessId);

      std::vector<Process>::const_reverse_iterator
      findProcess(const ProcessId) const;

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

    }; // end of struct ProcessManager

  } // end of namespace system

} // end of namespace tfel
