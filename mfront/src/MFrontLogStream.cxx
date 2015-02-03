/*! 
 * \file   mfront/src/MFrontLogStream.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 jun 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include<iostream>
#include<stdexcept>

#include<memory>

#include"MFront/MFrontLogStream.hxx"

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
    std::shared_ptr<std::ofstream> ps;
  }; // end of struct LogStream

  LogStream::LogStream()
    : s(&std::cout)
  {} // end of LogStream::LogStream()

  void
  LogStream::setLogStream(std::ostream& os)
  {
    using namespace std;
    if(this->ps.get()!=nullptr){
      this->ps->close();
    }
    this->ps = shared_ptr<ofstream>();
    this->s = &os;
  } // end of 

  void
  LogStream::setLogStream(const std::string& f)
  {
    using namespace std;
    if(this->ps.get()!=nullptr){
      this->ps->close();
    }
    this->ps = shared_ptr<ofstream>(new ofstream(f.c_str()));
    if(this->ps.get()==nullptr){
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
    if(this->ps.get()==nullptr){
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
    static VerboseLevel verboseMode = VERBOSE_LEVEL1;
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
    auto& log = LogStream::getLogStream();
    return log.getStream();
  } // end of function getLogStream

  void
  setLogStream(const std::string& f)
  {
    auto& log = LogStream::getLogStream();
    log.setLogStream(f);
  } // end of function setLogStream

  void
  setLogStream(std::ostream& os){
    auto& log = LogStream::getLogStream();
    log.setLogStream(os);
  } // end of function setLogStream

} // end of namespace mfront

