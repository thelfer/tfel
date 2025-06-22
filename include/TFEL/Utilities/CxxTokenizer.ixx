/*!
 * \file   include/TFEL/Utilities/CxxTokenizer.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   12 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CXXTOKENIZER_IXX_
#define LIB_TFEL_CXXTOKENIZER_IXX_

namespace tfel {

  namespace utilities {

    template <typename InputIterator>
    void CxxTokenizer::import(const InputIterator p, const InputIterator pe) {
      this->clear();
      this->tokens.insert(this->tokens.begin(), p, pe);
    }  // end of CxxTokenizer::import

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_CXXTOKENIZER_IXX_ */
