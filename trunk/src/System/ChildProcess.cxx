/*! 
 * \file  ChildProcess.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 */

#include<sys/types.h>
#include<signal.h>

#include"TFEL/System/ChildProcess.hxx"

namespace tfel
{

  namespace system
  {

    ChildProcess::ChildProcess(const int i,
			       const int o)
      : pid(-1),
	inputStream(i),
	outputStream(o)	
    {} // end of ChildProcess::ChildProcess

    ChildProcess::~ChildProcess()
    {
      if(this->pid!=-1){
	::kill(this->pid,SIGKILL);
      }
      ::close(this->inputStream);
      ::close(this->outputStream);
    } // end of ChildProcess::~ChildProcess

    pid_t
    ChildProcess::getPid(void) const
    {
      return this->pid;
    } // end of ChildProcess::getPid

    void
    ChildProcess::setPid(const pid_t p)
    {
      this->pid = p;
    } // end of ChildProcess::setPid

    int
    ChildProcess::getInputStream(void) const
    {
      return this->inputStream;
    } // end of ChildProcess::getOuputStream

    int
    ChildProcess::getOutputStream(void) const
    {
      return this->outputStream;
    } // end of ChildProcess::getOuputStream

    ChildProcessCreationFailedException::ChildProcessCreationFailedException()
      : SystemError(strerror(errno))
    {} // end of ChildProcessCreationFailedException::ChildProcessCreationFailedException()

  } // end of namespace system

} // end of namespace tfel
