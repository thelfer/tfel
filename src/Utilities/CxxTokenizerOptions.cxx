/*!
 * \file   CxxTokenizerOptions.cxx
 * \brief
 * \author Thomas Helfer
 * \date   6/02/018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/CxxTokenizerOptions.hxx"

namespace tfel {

  namespace utilities {

    CxxTokenizerOptions::CxxTokenizerOptions() = default;
    CxxTokenizerOptions::CxxTokenizerOptions(const CxxTokenizerOptions&) =
        default;
    CxxTokenizerOptions::CxxTokenizerOptions(CxxTokenizerOptions&&) = default;
    CxxTokenizerOptions& CxxTokenizerOptions::operator=(
        const CxxTokenizerOptions&) = default;
    CxxTokenizerOptions& CxxTokenizerOptions::operator=(CxxTokenizerOptions&&) =
        default;
    CxxTokenizerOptions::~CxxTokenizerOptions() = default;

  }  // end of namespace utilities

}  // end of namespace tfel
