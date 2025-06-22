/*!
 * \file   mfront/include/MFront/LawFunction.hxx
 * \brief  This file declares the LawFunction class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_LAWFUNCTION_HXX
#define LIB_LAWFUNCTION_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT LawFunction {
    LawFunction();
    LawFunction(LawFunction&&);
    LawFunction(const LawFunction&);
    LawFunction& operator=(LawFunction&&);
    LawFunction& operator=(const LawFunction&);
    ~LawFunction();
    std::string body;
    unsigned int line = 0u;
    bool modified = false;
  };  // end of struct LawFunction

}  // end of namespace mfront

#endif /* LIB_LAWFUNCTION_HXX */
