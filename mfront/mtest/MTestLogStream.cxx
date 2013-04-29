/*! 
 * \file  LogStream.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 jun 2011
 */

#include<fstream>
#include<iostream>
#include<stdexcept>

#include"TFEL/Utilities/SmartPtr.hxx"

#include"MFront/MTestLogStream.hxx"

namespace mfront
{

  struct LogStream
  {
    static LogStream&
    getLogStream();
    LogStream();
    void
    setLogStream(const std::string&);
    void
    setLogStream(std::ostream&);
    std::ostream&
    getStream(void);
  private:
    std::ostream* s;
    tfel::utilities::shared_ptr<std::ofstream> ps;
  }; // end of struct LogStream

  LogStream::LogStream()
    : s(&std::cout)
  {} // end of LogStream::LogStream()

  void
  LogStream::setLogStream(std::ostream& os)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(this->ps.get()!=0){
      this->ps->close();
    }
    this->ps = shared_ptr<ofstream>();
    this->s = &os;
  } // end of 

  void
  LogStream::setLogStream(const std::string& f)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(this->ps.get()!=0){
      this->ps->close();
    }
    this->ps = shared_ptr<ofstream>(new ofstream(f.c_str()));
    if(this->ps.get()==0){
      string msg("LogStream::setLogStream : ");
      msg += "can't allocate ofstream obect";
      throw(runtime_error(msg));
    }
    if(!(*(this->ps.get()))){
      string msg("LogStream::setLogStream : ");
      msg += "can't open file '"+f+"'";
      throw(runtime_error(msg));
    }
  } // end of LogStream::setLogStream

  std::ostream&
  LogStream::getStream(void)
  {
    if(this->ps.get()==0){
      return *s;
    }
    return *(this->ps);
  } // end of LogStream::getStream(void)

  LogStream&
  LogStream::getLogStream(void)
  {
    static LogStream log;
    return log;
  } // end of LogStream::getLogStream

  VerboseLevel&
  getVerboseMode()
  {
    static VerboseLevel verboseMode = VERBOSE_LEVEL2;
    return verboseMode;
  } // end of getVerboseMode()

  void
  setVerboseMode(const VerboseLevel l)
  {
    getVerboseMode()=l;
  } // end of setVerboseMode
  
  std::ostream&
  getLogStream()
  {
    LogStream& log = LogStream::getLogStream();
    return log.getStream();
  } // end of function getLogStream

  void
  setLogStream(const std::string& f)
  {
    LogStream& log = LogStream::getLogStream();
    log.setLogStream(f);
  } // end of function setLogStream

  void
  setLogStream(std::ostream& os){
    LogStream& log = LogStream::getLogStream();
    log.setLogStream(os);
  } // end of function setLogStream

} // end of namespace mfront

