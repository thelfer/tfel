/*!
 * \file  include/TFEL/System/BinaryRead.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_BINARYREAD_H_
#define LIB_TFEL_SYSTEM_BINARYREAD_H_

#include <unistd.h>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/System.hxx"

#define TFEL_SYSTEM_BINARY_READER_DECLARATION(X)        \
  /*!                                                   \
   * read a value in a stream                           \
   * \param [in]  f : input stream                      \
   * \param [out] v : value read                        \
   */                                                   \
  template <>                                           \
  struct TFELSYSTEM_VISIBILITY_EXPORT BinaryReader<X> { \
    /*!                                                 \
     * write a value in a stream                        \
     * \param [in] f : input stream                     \
     * \param [in] v : value to be read                 \
     */                                                 \
    static void exe(const int, X&);                     \
  };

namespace tfel {

  namespace system {

    // forward declaration
    template <typename>
    struct BinaryReader;

    TFEL_SYSTEM_BINARY_READER_DECLARATION(bool)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(unsigned char)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(unsigned short)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(unsigned int)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(long unsigned int)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(char)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(short)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(int)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(long int)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(float)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(double)
    TFEL_SYSTEM_BINARY_READER_DECLARATION(long double)

    template <>
    struct TFELSYSTEM_VISIBILITY_EXPORT BinaryReader<void*> {
      /*!
       * write a value in a stream
       * \param [in] f : input stream
       * \param [in] v : value to be read
       */
      static void exe(const int, void*&);
    };

    /*!
     * an helper class to read value in a stream
     * \param T : type to be read
     */
    template <typename T>
    struct EnumBinaryReader {
      //! a simple alias
      using integer = typename std::underlying_type<T>::type;
      static void exe(const int s, T& v) {
        union {
          T v1;
          integer v2;
        } value;
        BinaryReader<integer>::exe(s, value.v2);
        v = value.v1;
      }
    };
    /*!
     * an helper class to write value in a stream
     * \param T : type to be read
     */
    template <typename T>
    struct PointerBinaryReader {
      static void exe(const int s, T& v) {
        union {
          void* v1;
          T v2;
        } value;
        BinaryReader<void*>::exe(s, value.v1);
        v = value.v2;
      }
    };
    /*!
     * an helper class to write value in a stream
     * \param T : type to be read
     */
    template <typename T>
    struct EmptyBinaryReader {
      static void exe(const int, T&) {}
    };
    /*!
     * an helper class to read value in a stream
     * \param T : type to be read
     */
    template <typename T>
    struct StandardBinaryReader {
      static void exe(const int s, T& v) {
        if (::read(s, static_cast<T*>(&v), sizeof(T)) == -1) {
          systemCall::throwSystemError("BinaryReader<T>::exe", errno);
        }
      }
    };
    /*!
     * an helper class to read value in a stream
     * \param T : type to be read
     */
    template <typename T>
    struct BinaryReader
        : std::conditional<
              std::is_enum<T>::value,
              EnumBinaryReader<T>,
              typename std::conditional<
                  std::is_pointer<T>::value,
                  PointerBinaryReader<T>,
                  typename std::conditional<std::is_empty<T>::value,
                                            EmptyBinaryReader<T>,
                                            StandardBinaryReader<T>>::type>::
                  type>::type {};  // end of struct BinaryReader
    /*!
     * read a value in a stream
     * \param [in]  f : input stream
     * \param [out] v : value read
     */
    template <typename T>
    void binary_read(const int f, T& v) {
      BinaryReader<T>::exe(f, v);
    }  // end of binary_read
    /*!
     * read a value in a stream
     * \param [in]  f : input stream
     * \return value read
     */
    template <typename T>
    T binary_read(const int f) {
      auto res = T{};
      binary_read(f, res);
      return res;
    }  // end of binary_read

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_BINARYREAD_H_ */
