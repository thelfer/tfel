/*!
 * \file   src/System/rfstream.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   14 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <unistd.h>
#include "TFEL/System/System.hxx"
#include "TFEL/System/rfstream.hxx"

namespace tfel::system {

  rfstream::rfstream()
      : std::shared_ptr<int>(new int(-1)) {}  // end of rfstream::rfstream

  rfstream::rfstream(const rfstream&) = default;

  rfstream& rfstream::operator=(const rfstream& src) {
    if (this == &src) {
      return *this;
    }
    this->close();
    basic_rstream<rfstream, stream_traits<rfstream>>::operator=(src);
    std::shared_ptr<int>::operator=(src);
    return *this;
  }  // end of rfstream::operator

  rfstream::rfstream(const std::string& name, const int flags)
      : std::shared_ptr<int>(new int(-1)) {
    this->open(name, flags);
  }  // end of rfstream::rfstream

  void rfstream::open(const std::string& name, const int flags) {
    int fd;
    if (*(this->get()) != -1) {
      // closing the previous file
      this->close();
    }
    fd = ::open(name.c_str(), flags);
    if (fd == -1) {
      systemCall::throwSystemError(
          "rfstream::open: failed "
          "to open file '" +
              name + "'.",
          errno);
    }
    shared_ptr<int>::operator=(std::make_shared<int>(fd));
  }  // end of rfstream::open

  void rfstream::close() {
    if (*(this->get()) == -1) {
      return;
    }
    if (this->use_count() == 1) {
      if (::close(*(this->get())) == -1) {
        systemCall::throwSystemError(
            "rfstream::close: failed to "
            "close file.",
            errno);
      }
    }
    shared_ptr<int>::operator=(std::make_shared<int>(-1));
  }  // end of rfstream::close

  int rfstream::getFileDescriptor() const {
    return *(this->get());
  }  // end of rfstream::getFileDescriptor

  rfstream::~rfstream() {
    try {
      this->close();
    } catch (...) {
    }
  }  // end of rfstream::~rfstream

}  // end of namespace tfel::system
