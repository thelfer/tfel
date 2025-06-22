/*!
 * \file   include/TFEL/System/basic_rstream.ixx
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

#ifndef LIB_TFEL_BASIC_RSTREAMIXX
#define LIB_TFEL_BASIC_RSTREAMIXX

#include "TFEL/Config/TFELConfig.hxx"
#include <type_traits>

namespace tfel {

  namespace system {

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>::operator int() {
      return static_cast<Child* const>(this)->getFileDescriptor();
    }  // end of basic_rstream<Child,Traits>::operator int

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>::operator int() const {
      return static_cast<const Child*>(this)->getFileDescriptor();
    }  // end of basic_rstream<Child,Traits>::operator const int

    template <typename Child, typename Traits>
    void basic_rstream<Child, Traits>::read(int fd,
                                            void* const buf,
                                            const size_t count) {
      typedef
          typename std::conditional<Traits::isBlocking, BlockingStreamReader,
                                    NonBlockingStreamReader>::type Reader;
      Reader::read(fd, buf, count);
    }

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(char& c) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &c, sizeof(char));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(bool& b) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &b, sizeof(bool));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(short& s) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &s, sizeof(short));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(unsigned short& u) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &u, sizeof(unsigned short));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(int& i) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &i, sizeof(int));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(unsigned int& u) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &u, sizeof(unsigned int));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(long& l) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &l, sizeof(long));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(unsigned long& u) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &u, sizeof(unsigned long));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(float& f) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &f, sizeof(float));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(double& d) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &d, sizeof(double));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    TFEL_INLINE2 basic_rstream<Child, Traits>&
    basic_rstream<Child, Traits>::operator>>(long double& l) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &l, sizeof(long double));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    basic_rstream<Child, Traits>& basic_rstream<Child, Traits>::operator>>(
        void*& p) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, &p, sizeof(p));
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator>>

    template <typename Child, typename Traits>
    basic_rstream<Child, Traits>::basic_rstream() {
    }  // end of basic_rstream<Child,Traits>::basic_rstream

    template <typename Child, typename Traits>
    basic_rstream<Child, Traits>::basic_rstream(const basic_rstream&) {
    }  // end of basic_rstream<Child,Traits>::basic_rstream

    template <typename Child, typename Traits>
    basic_rstream<Child, Traits>& basic_rstream<Child, Traits>::operator=(
        const basic_rstream<Child, Traits>&) {
      return *this;
    }  // end of basic_rstream<Child,Traits>::operator=

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(char* const c,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, c, size * sizeof(char));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(bool* const b,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, b, size * sizeof(bool));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(short* const s,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, s, size * sizeof(short));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(
        unsigned short* const u, const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, u, size * sizeof(unsigned short));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(int* const i,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, i, size * sizeof(int));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(unsigned int* const u,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, u, size * sizeof(unsigned int));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(long* const l,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, l, size * sizeof(long));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(unsigned long* const u,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, u, size * sizeof(unsigned long));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(float* const f,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, f, size * sizeof(float));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(double* const d,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, d, size * sizeof(double));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    TFEL_INLINE2 void basic_rstream<Child, Traits>::read(long double* const l,
                                                         const size_t size) {
      int fd = static_cast<const Child*>(this)->getFileDescriptor();
      basic_rstream<Child, Traits>::read(fd, l, size * sizeof(long double));
    }  // end of basic_rstream<Child,Traits>::read

    template <typename Child, typename Traits>
    const std::string basic_rstream<Child, Traits>::getLine() {
      using namespace std;
      string res;
      char c;
      this->operator>>(c);
      while (c != '\n') {
        res += c;
      }
      return res;
    }  // end of basic_rstream<Child,Traits>::getLine

  }  // namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_BASIC_RSTREAMIXX */
