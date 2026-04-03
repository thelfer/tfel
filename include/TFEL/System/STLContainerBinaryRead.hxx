/*!
 * \file  include/TFEL/System/STLContainerBinaryRead.hxx
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

#ifndef LIB_TFEL_SYSTEM_STLCONTAINERBINARYREADE_HXX
#define LIB_TFEL_SYSTEM_STLCONTAINERBINARYREADE_HXX

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>

#include "TFEL/System/BinaryRead.hxx"

namespace tfel::system {

  template <typename charT, typename traits, typename Alloc>
  struct BinaryReader<std::basic_string<charT, traits, Alloc>> {
    static void exe(const int f, std::basic_string<charT, traits, Alloc>& res) {
      using size_type =
          typename std::basic_string<charT, traits, Alloc>::size_type;
      using value_type =
          typename std::basic_string<charT, traits, Alloc>::value_type;
      auto s = binary_read<size_type>(f);
      res.clear();
      res.resize(s);
      if (::read(f, &res[0], s * sizeof(value_type)) == -1) {
        systemCall::throwSystemError("BinaryReader<T>::exe", errno);
      }
    }
  };

  template <typename T1, typename T2>
  struct BinaryReader<std::pair<T1, T2>> {
    static void exe(const int f, std::pair<T1, T2>& res) {
      binary_read(f, res.first);
      binary_read(f, res.second);
    }
  };

  template <typename T, typename Allocator>
  struct BinaryReader<std::vector<T, Allocator>> {
    static void exe(const int f, std::vector<T, Allocator>& res) {
      const auto s =
          binary_read<typename std::vector<T, Allocator>::size_type>(f);
      res.clear();
      res.resize(s);
      for (auto& v : res) {
        binary_read(f, v);
      }
    }
  };

  template <typename T, typename Allocator>
  struct BinaryReader<std::list<T, Allocator>> {
    static void exe(const int f, std::list<T, Allocator>& res) {
      const auto s =
          binary_read<typename std::list<T, Allocator>::size_type>(f);
      res.clear();
      res.resize(s);
      for (auto& v : res) {
        binary_read(f, v);
      }
    }
  };

  template <typename T, typename Allocator>
  struct BinaryReader<std::set<T, Allocator>> {
    static void exe(const int f, std::set<T, Allocator>& res) {
      const auto s = binary_read<decltype(res.size())>(f);
      for (decltype(res.size()) i = 0; i != s; ++i) {
        T value;
        binary_read(f, value);
        res.insert(std::move(value));
      }
    }
  };

  template <typename Key, typename Data, typename Compare, typename Alloc>
  struct BinaryReader<std::map<Key, Data, Compare, Alloc>> {
    static void exe(const int f, std::map<Key, Data, Compare, Alloc>& res) {
      using size_type = typename std::map<Key, Data, Compare, Alloc>::size_type;
      auto s = binary_read<size_type>(f);
      res.clear();
      for (size_type i = 0; i != s; ++i) {
        auto k = binary_read<Key>(f);
        auto v = binary_read<Data>(f);
        res.insert({std::move(k), std::move(v)});
      }
    }
  };  // end of BinaryReader<std::map...>

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEM_STLCONTAINERBINARYREADE_HXX */
