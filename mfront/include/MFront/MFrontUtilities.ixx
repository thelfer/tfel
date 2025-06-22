/*!
 * \file   MFrontUtilities.ixx
 * \brief
 * \author Thomas Helfer
 * \date   15 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTUTILITIES_IXX
#define LIB_MFRONT_MFRONTUTILITIES_IXX

namespace mfront {

  template <typename Container>
  void insert_if(std::vector<std::string>& d, const Container& s) {
    for (const auto& v : s) {
      insert_if(d, v);
    }
  }

  template <typename Value>
  void insert_if(std::vector<std::string>& d,
                 const std::initializer_list<Value>& s) {
    for (const auto& v : s) {
      insert_if(d, v);
    }
  }

  template <typename T>
  void read(T& r,
            tfel::utilities::CxxTokenizer::const_iterator& p,
            const tfel::utilities::CxxTokenizer::const_iterator pe) {
    r = read<T>(p, pe);
  }  // end of read

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTUTILITIES_IXX */
