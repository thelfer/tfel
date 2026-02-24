/*!
 * \file   include/TFEL/System/basic_wstream.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_BASIC_WSTREAM_HXX
#define LIB_TFEL_SYSTEM_BASIC_WSTREAM_HXX

#include <string>
#include <cctype>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::system {

  template <typename Child, typename Traits>
  struct basic_wstream {
    operator int();

    operator int() const;

    basic_wstream& operator<<(const char&);

    basic_wstream& operator<<(const bool&);

    basic_wstream& operator<<(const short&);

    basic_wstream& operator<<(const unsigned short&);

    basic_wstream& operator<<(const int&);

    basic_wstream& operator<<(const unsigned int&);

    basic_wstream& operator<<(const long&);

    basic_wstream& operator<<(const unsigned long&);

    basic_wstream& operator<<(const float&);

    basic_wstream& operator<<(const double&);

    basic_wstream& operator<<(const long double&);

    basic_wstream& operator<<(const void*&);

    basic_wstream& operator<<(const char*);

    basic_wstream& operator<<(const std::string&);

    void write(const char* const, const size_t);

    void write(const bool* const, const size_t);

    void write(const short* const, const size_t);

    void write(const unsigned short* const, const size_t);

    void write(const int* const, const size_t);

    void write(const unsigned int* const, const size_t);

    void write(const long* const, const size_t);

    void write(const unsigned long* const, const size_t);

    void write(const float* const, const size_t);

    void write(const double* const, const size_t);

    void write(const long double* const, const size_t);

   protected:
    basic_wstream();

    basic_wstream(const basic_wstream&);

    basic_wstream& operator=(const basic_wstream&);

    static void write(int, const void* const, const size_t);

  };  // end of struct basic_rstream

  struct TFELSYSTEM_VISIBILITY_EXPORT BlockingStreamWriter {
    static void write(int, const void* const, const size_t);
  };  // end of BlockingStreamWriter

  struct TFELSYSTEM_VISIBILITY_EXPORT NonBlockingStreamWriter {
    static void write(int, const void* const, const size_t);
  };  // end of NonBlockingStreamWriter

}  // end of namespace tfel::system

#include "TFEL/System/basic_wstream.ixx"

#endif /* LIB_TFEL_SYSTEM_BASIC_WSTREAM_HXX */
