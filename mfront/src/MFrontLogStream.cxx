/*!
 * \file   mfront/src/MFrontLogStream.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  10 jun 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <memory>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
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
    tfel::raise_if(this->ps == nullptr,
                   "LogStream::setLogStream: "
                   "can't allocate ofstream obect");
    tfel::raise_if(!(*(this->ps)),
                   "LogStream::setLogStream: "
                   "can't open file '" +
                       f + "'");
  }  // end of LogStream::setLogStream

  std::ostream& LogStream::getStream() {
    if (this->ps == nullptr) {
      return *s;
    }
    return *(this->ps);
  }  // end of LogStream::getStream()

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

  void setVerboseMode(const std::string& l) {
    if (l == "quiet") {
      setVerboseMode(VERBOSE_QUIET);
    } else if (l == "level0") {
      setVerboseMode(VERBOSE_LEVEL0);
    } else if (l == "level1") {
      setVerboseMode(VERBOSE_LEVEL1);
    } else if (l == "level2") {
      setVerboseMode(VERBOSE_LEVEL2);
    } else if (l == "level3") {
      setVerboseMode(VERBOSE_LEVEL3);
    } else if (l == "debug") {
      setVerboseMode(VERBOSE_DEBUG);
    } else if (l == "full") {
      setVerboseMode(VERBOSE_FULL);
    } else {
      tfel::raise("setVerboseMode: insupported verbose level '" + l + "'");
    }
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

  namespace internals {

    static bool& getUnicodeOutputOption() {
      static bool b = true;
      return b;
    }  // end of getUnicodeOutputOption()
  }    // end of namespace internals

  void setUnicodeOutputOption(const bool b) {
    mfront::internals::getUnicodeOutputOption() = b;
  }  // end of setUnicodeOutputOption

  bool getUnicodeOutputOption() {
    return mfront::internals::getUnicodeOutputOption();
  }  // end of getUnicodeOutputOption

}  // end of namespace mfront
