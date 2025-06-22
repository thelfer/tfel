/*!
 * \file   mfront/include/MFront/DefaultDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPARSER_HXX
#define LIB_MFRONTPARSER_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT DefaultDSL : public DefaultDSLBase {
    static std::string getName();

    static std::string getDescription();

    DefaultDSL();

    ~DefaultDSL();

  };  // end of struct DefaultDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTPARSER_HXX */
