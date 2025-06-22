/*!
 * \file   include/TFEL/System/basic_wstream.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   14 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_BASIC_WSTREAM_IXX_
#define LIB_TFEL_BASIC_WSTREAM_IXX_

#include <cstring>

#include "TFEL/Config/TFELConfig.hxx"
#include <type_traits>

namespace tfel {

  namespace system {

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>::operator int(void) {
      return static_cast<Child* const>(this)->getFileDescriptor();
    }  // end of basic_wstream<Child,Traits>::operator int

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>::operator int(void) const {
      return static_cast<const Child*>(this)->getFileDescriptor();
    }  // end of basic_wstream<Child,Traits>::operator const int

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(int fd,
                                                          const void* const buf,
                                                          const size_t count) {
      typedef
          typename std::conditional<Traits::isBlocking, BlockingStreamWriter,
                                    NonBlockingStreamWriter>::type Writer;
      Writer::write(fd, buf, count);
    }

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const char& c) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &c, sizeof(char));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const bool& b) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &b, sizeof(bool));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const short& s) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &s, sizeof(short));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const unsigned short& u) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &u, sizeof(unsigned short));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const int& i) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &i, sizeof(int));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const unsigned int& u) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &u, sizeof(unsigned int));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const long& l) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &l, sizeof(long));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const unsigned long& u) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &u, sizeof(unsigned long));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const float& f) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &f, sizeof(float));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const double& d) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &d, sizeof(double));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_wstream<Child, Traits>&
    basic_wstream<Child, Traits>::operator<<(const long double& l) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &l, sizeof(long double));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    basic_wstream<Child, Traits>& basic_wstream<Child, Traits>::operator<<(
        const void*& p) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &p, sizeof(p));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    basic_wstream<Child, Traits>& basic_wstream<Child, Traits>::operator<<(
        const std::string& s) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, &s[0], s.size());
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    basic_wstream<Child, Traits>& basic_wstream<Child, Traits>::operator<<(
        const char* s) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, s, strlen(s));
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator<<

    template <typename Child, typename Traits>
    basic_wstream<Child, Traits>::basic_wstream() {
    }  // end of basic_wstream<Child,Traits>::basic_wstream

    template <typename Child, typename Traits>
    basic_wstream<Child, Traits>::basic_wstream(const basic_wstream&) {
    }  // end of basic_wstream<Child,Traits>::basic_wstream

    template <typename Child, typename Traits>
    basic_wstream<Child, Traits>& basic_wstream<Child, Traits>::operator=(
        const basic_wstream<Child, Traits>&) {
      return *this;
    }  // end of basic_wstream<Child,Traits>::operator=

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const char* const c,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, c, size * sizeof(char));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const bool* const b,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, b, size * sizeof(bool));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const short* const s,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, s, size * sizeof(short));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(
        const unsigned short* const u, const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, u, size * sizeof(unsigned short));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const int* const i,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, i, size * sizeof(int));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(
        const unsigned int* const u, const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, u, size * sizeof(unsigned int));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const long* const l,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, l, size * sizeof(long));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(
        const unsigned long* const u, const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, u, size * sizeof(unsigned long));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const float* const f,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, f, size * sizeof(float));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(const double* const d,
                                                          const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, d, size * sizeof(double));
    }  // end of basic_wstream<Child,Traits>::write

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_wstream<Child, Traits>::write(
        const long double* const l, const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_wstream<Child, Traits>::write(fd, l, size * sizeof(long double));
    }  // end of basic_wstream<Child,Traits>::write

  }  // namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_BASIC_WSTREAM_IXX_ */
