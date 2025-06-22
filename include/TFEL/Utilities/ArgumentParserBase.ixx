/*!
 * \file   include/TFEL/Utilities/ArgumentParserBase.ixx
 * \brief  This file implements methods of class ArgumentParserBase.
 * \author Thomas Helfer
 * \date   17 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_ARGUMENTPARSERBASE_IXX
#define LIB_TFEL_UTILITIES_ARGUMENTPARSERBASE_IXX

namespace tfel::utilities {

  template <typename Child>
  ArgumentParserBase<Child>::ArgumentParserBase() = default;

  template <typename Child>
  ArgumentParserBase<Child>::ArgumentParserBase(const int argc,
                                                const char* const* const argv)
      : ArgumentParser(argc, argv) {
  }  // end of ArgumentParserBase<Child>::ArgumentParserBase

  template <typename Child>
  void ArgumentParserBase<Child>::registerNewCallBack(const std::string& k,
                                                      const MemberFuncPtr& f,
                                                      const std::string& d,
                                                      const bool b) {
    const CallBack c{d, std::bind(std::mem_fn(f), static_cast<Child*>(this)),
                     b};
    this->registerCallBack(k, c);
  }

  template <typename Child>
  void ArgumentParserBase<Child>::registerNewCallBack(const std::string& k,
                                                      const std::string& a,
                                                      const MemberFuncPtr& f,
                                                      const std::string& d,
                                                      const bool b) {
    const CallBack c{d, std::bind(std::mem_fn(f), static_cast<Child*>(this)),
                     b};
    this->registerCallBack(k, a, c);
  }

  template <typename Child>
  ArgumentParserBase<Child>::~ArgumentParserBase() = default;

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_ARGUMENTPARSERBASE_IXX */
