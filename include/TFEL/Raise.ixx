/*!
 * \file   Raise.ixx
 * \brief
 * \author Thomas Helfer
 * \date   20/09/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_RAISE_IXX
#define LIB_TFEL_RAISE_IXX

namespace tfel {

  template <typename Exception>
  [[noreturn]] void raise() {
    Exception e;
    throw(std::move(e));
  }  // end of raise

  template <typename Exception, typename... Args>
  [[noreturn]] void raise(Args&&... a) {
    Exception e(std::forward<Args>(a)...);
    throw(std::move(e));
  }  // end of raise

  template <typename Exception>
  void raise_if(const bool c) {
    if (c) {
      raise<Exception>();
    }
  }  // end of raise

  template <typename Exception, typename... Args>
  void raise_if(const bool c, Args&&... a) {
    if (c) {
      raise<Exception>(std::forward<Args>(a)...);
    }
  }  // end of raise

}  // end of namespace tfel

#endif /* LIB_TFEL_RAISE_IXX */
