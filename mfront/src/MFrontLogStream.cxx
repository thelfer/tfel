/*!
 * \file   mfront/src/MFrontLogStream.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 jun 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <memory>

#include "MFront/MFrontLogStream.hxx"

namespace mfront {

  struct LogStream {
    static LogStream& getLogStream();
    LogStream();
    void setLogStream(const std::string&);
    void setLogStream(std::ostream&);
    std::ostream& getStream();

   private:
    std::ostream* s;
    std::shared_ptr<std::ofstream> ps;
  };  // end of struct LogStream

  LogStream::LogStream() : s(&std::cout) {}  // end of LogStream::LogStream()

  void LogStream::setLogStream(std::ostream& os) {
    if (this->ps != nullptr) {
      this->ps->close();
    }
    this->ps = std::make_shared<std::ofstream>();
    this->s = &os;
  }  // end of

  void LogStream::setLogStream(const std::string& f) {
    if (this->ps != nullptr) {
      this->ps->close();
    }
    this->ps = std::make_shared<std::ofstream>(f);
    if (this->ps == nullptr) {
      throw(
          std::runtime_error("LogStream::setLogStream: "
                             "can't allocate ofstream obect"));
    }
    if (!(*(this->ps))) {
      throw(
          std::runtime_error("LogStream::setLogStream: "
                             "can't open file '" +
                             f + "'"));
    }
  }  // end of LogStream::setLogStream

  std::ostream& LogStream::getStream() {
    if (this->ps == nullptr) {
      return *s;
    }
    return *(this->ps);
  }  // end of LogStream::getStream(void)

  LogStream& LogStream::getLogStream() {
    static LogStream log;
    return log;
  }  // end of LogStream::getLogStream

  VerboseLevel& getVerboseMode() {
    static VerboseLevel verboseMode = VERBOSE_LEVEL1;
    return verboseMode;
  }  // end of getVerboseMode()

  void setVerboseMode(const VerboseLevel l) {
    getVerboseMode() = l;
  }  // end of setVerboseMode

  std::ostream& getLogStream() {
    auto& log = LogStream::getLogStream();
    return log.getStream();
  }  // end of function getLogStream

  void setLogStream(const std::string& f) {
    auto& log = LogStream::getLogStream();
    log.setLogStream(f);
  }  // end of function setLogStream

  void setLogStream(std::ostream& os) {
    auto& log = LogStream::getLogStream();
    log.setLogStream(os);
  }  // end of function setLogStream

}  // end of namespace mfront
