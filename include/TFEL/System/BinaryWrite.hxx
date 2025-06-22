/*!
 * \file  include/TFEL/System/BinaryWrite.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 27 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_BINARYWRITE_HXX
#define LIB_TFEL_SYSTEM_BINARYWRITE_HXX

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/System.hxx"

#define TFEL_SYSTEM_BINARY_WRITER_DECLARATION(X)        \
  template <>                                           \
  struct TFELSYSTEM_VISIBILITY_EXPORT BinaryWriter<X> { \
    /*!                                                 \
     * write a value in a stream                        \
     * \param [in] f : input stream                     \
     * \param [in] v : value to be written              \
     */                                                 \
    static void exe(const int, const X&);               \
  };  // end of struct BinaryWriter< X >

namespace tfel {

  namespace system {

    // forward declaration
    template <typename>
    struct BinaryWriter;

    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(bool)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(unsigned char)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(unsigned short)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(unsigned int)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(long unsigned int)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(char)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(short)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(int)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(long int)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(float)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(double)
    TFEL_SYSTEM_BINARY_WRITER_DECLARATION(long double)

    template <>
    struct TFELSYSTEM_VISIBILITY_EXPORT BinaryWriter<const void*> {
      /*!
       * write a value in a stream
       * \param [in] f : input stream
       * \param [in] v : value to be written
       */
      static void exe(const int, const void* const);
    };  // end of struct BinaryWriter< X >

    /*!
     * an helper class to write value in a stream
     * \param T : type to be written
     */
    template <typename T>
    struct EnumBinaryWriter {
      //! a simple alias
      using integer = typename std::underlying_type<T>::type;
      static void exe(const int s, const T& v) {
        BinaryWriter<integer>::exe(s, static_cast<const integer&>(v));
      }
    };
    /*!
     * an helper class to write value in a stream
     * \param T : type to be written
     */
    template <typename T>
    struct PointerBinaryWriter {
      static void exe(const int s, const T v) {
        BinaryWriter<const void*>::exe(s, static_cast<const void*>(v));
      }
    };
    /*!
     * an helper class to write value in a stream
     * \param T : type to be written
     */
    template <typename T>
    struct EmptyBinaryWriter {
      static void exe(const int, const T) {}
    };
    /*!
     * an helper class to write value in a stream
     * \param T : type to be written
     */
    template <typename T>
    struct StandardBinaryWriter {
      static void exe(const int s, const T& v) {
        systemCall::write(s, static_cast<const void*>(&v), sizeof(T));
      }
    };
    /*!
     * an helper class to write value in a stream
     * \param T : type to be written
     */
    template <typename T>
    struct BinaryWriter
        : std::conditional<
              std::is_enum<T>::value,
              EnumBinaryWriter<T>,
              typename std::conditional<
                  std::is_pointer<T>::value,
                  PointerBinaryWriter<T>,
                  typename std::conditional<std::is_empty<T>::value,
                                            EmptyBinaryWriter<T>,
                                            StandardBinaryWriter<T>>::type>::
                  type>::type {};

    /*!
     * partial specialisation for c-style string
     * \param N : number of characters
     */
    template <unsigned short N>
    struct BinaryWriter<char[N]> {
      static void exe(const int f, const char v[N]) {
        BinaryWriter<unsigned short>::exe(f, N - 1);
        systemCall::write(f, &v[0], (N - 1) * sizeof(char));
      }
    };  // end of BinaryWriter

    /*!
     * write a value in a stream
     * \param [in] f : input stream
     * \param [in] v : value to be written
     */
    template <typename T>
    TFEL_INLINE void binary_write(const int f, const T& v) {
      BinaryWriter<T>::exe(f, v);
    }  // end of binary_write

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_BINARYWRITE_HXX */
