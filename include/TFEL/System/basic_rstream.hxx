/*!
 * \file   include/TFEL/System/basic_rstream.hxx
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

#ifndef LIB_TFEL_BASIC_RSTREAM_HXX
#define LIB_TFEL_BASIC_RSTREAM_HXX

#include <cctype>
#include <string>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace system {

    template <typename Child, typename Traits>
    struct basic_rstream {
      operator int();

      operator int() const;

      basic_rstream& operator>>(char&);

      basic_rstream& operator>>(bool&);

      basic_rstream& operator>>(short&);

      basic_rstream& operator>>(unsigned short&);

      basic_rstream& operator>>(int&);

      basic_rstream& operator>>(unsigned int&);

      basic_rstream& operator>>(long&);

      basic_rstream& operator>>(unsigned long&);

      basic_rstream& operator>>(float&);

      basic_rstream& operator>>(double&);

      basic_rstream& operator>>(long double&);

      basic_rstream& operator>>(void*&);

      void read(char* const, const size_t);

      void read(bool* const, const size_t);

      void read(short* const, const size_t);

      void read(unsigned short* const, const size_t);

      void read(int* const, const size_t);

      void read(unsigned int* const, const size_t);

      void read(long* const, const size_t);

      void read(unsigned long* const, const size_t);

      void read(float* const, const size_t);

      void read(double* const, const size_t);

      void read(long double* const, const size_t);

      const std::string getLine();

     protected:
      basic_rstream();

      basic_rstream(const basic_rstream&);

      basic_rstream& operator=(const basic_rstream&);

      static void read(int, void* const, const size_t);

    };  // end of struct basic_rstream

    struct TFELSYSTEM_VISIBILITY_EXPORT BlockingStreamReader {
      static void read(int, void* const, const size_t);
    };  // end of BlockingStreamReader

    struct TFELSYSTEM_VISIBILITY_EXPORT NonBlockingStreamReader {
      static void read(int, void* const, const size_t);
    };  // end of NonBlockingStreamReader

  }  // namespace system

}  // end of namespace tfel

#include "TFEL/System/basic_rstream.ixx"

#endif /* LIB_TFEL_BASIC_RSTREAM_HXX */
