/*!
 * \file  include/TFEL/System/STLContainerBinaryWrite.hxx
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

#ifndef LIB_TFEL_SYSTEM_STLCONTAINERBINARYWRITE_HXX
#define LIB_TFEL_SYSTEM_STLCONTAINERBINARYWRITE_HXX

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>

#include "TFEL/System/BinaryWrite.hxx"

namespace tfel {

  namespace system {

    /*!
     * \brief partial specialisation of `BinaryWriter` for
     * the `std::pair` class.
     */
    template <typename T1, typename T2>
    struct BinaryWriter<std::pair<T1, T2>> {
      /*!
       * \brief write a `std::pair` to the stream
       * \param[in] f: stream id
       * \param[in] v: value
       */
      static void exe(const int f, const std::pair<T1, T2>& v) {
        binary_write(f, v.first);
        binary_write(f, v.second);
      }
    };  // end of BinaryWriter<std::pair..>
    /*!
     * \brief partial specialisation of `BinaryWriter` for
     * the `std::vector` class.
     */
    template <typename T, typename Allocator>
    struct BinaryWriter<std::vector<T, Allocator>> {
      /*!
       * \brief write a `std::vector` to the stream
       * \param[in] f: stream id
       * \param[in] v: value
       */
      static void exe(const int f, const std::vector<T, Allocator>& v) {
        binary_write(f, v.size());
        for (const auto& e : v) {
          binary_write(f, e);
        }
      }
    };  // end of BinaryWriter<std::vector..>
    /*!
     * \brief partial specialisation of `BinaryWriter` for
     * the `std::string` class.
     */
    template <typename charT, typename traits, typename Alloc>
    struct BinaryWriter<std::basic_string<charT, traits, Alloc>> {
      //! a simple alias
      using basic_string = std::basic_string<charT, traits, Alloc>;
      /*!
       * \brief write a basic_string to the stream
       * \param[in] f: stream id
       * \param[in] v: value
       */
      static void exe(const int f, const basic_string& v) {
        //! a simple alias
        using value_type =
            typename std::basic_string<charT, traits, Alloc>::value_type;
        binary_write(f, v.size());
        systemCall::write(f, &v[0], v.size() * sizeof(value_type));
      }
    };  // end of struct BinaryWriter<std::basic_string<charT,traits,Alloc> >
    /*!
     * \brief partial specialisation of `BinaryWriter` for
     * the `std::list` class.
     */
    template <typename T, typename Allocator>
    struct BinaryWriter<std::list<T, Allocator>> {
      /*!
       * \brief write a `std::list` to the stream
       * \param[in] f: stream id
       * \param[in] v: value
       */
      static void exe(const int f, const std::list<T, Allocator>& v) {
        binary_write(f, v.size());
        for (const auto& e : v) {
          binary_write(f, e);
        }
      }
    };  // end of BinaryWriter
    /*!
     * \brief partial specialisation of `BinaryWriter` for
     * the `std::set` class.
     */
    template <typename T, typename Allocator>
    struct BinaryWriter<std::set<T, Allocator>> {
      /*!
       * \brief write a `std::set` to the stream
       * \param[in] f: stream id
       * \param[in] v: value
       */
      static void exe(const int f, const std::set<T, Allocator>& v) {
        binary_write(f, v.size());
        for (const auto& e : v) {
          binary_write(f, e);
        }
      }
    };  // end of BinaryWriter
    /*!
     * \brief partial specialisation of `BinaryWriter` for
     * the `std::map` class.
     */
    template <typename Key, typename Data, typename Compare, typename Alloc>
    struct BinaryWriter<std::map<Key, Data, Compare, Alloc>> {
      /*!
       * \brief write a `std::map` to the stream
       * \param[in] f: stream id
       * \param[in] v: value
       */
      static void exe(const int f,
                      const std::map<Key, Data, Compare, Alloc>& v) {
        binary_write(f, v.size());
        for (const auto& e : v) {
          binary_write(f, e);
        }
      }
    };  // end of BinaryWriter<std::map...>

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_STLCONTAINERBINARYWRITE_HXX */
