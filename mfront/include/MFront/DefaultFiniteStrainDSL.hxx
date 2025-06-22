/*!
 * \file   mfront/include/MFront/DefaultFiniteStrainDSL.hxx
 * \brief
 * \author Helfer Thomas
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTDEFAULTFINITESTRAINPARSER_HXX_
#define LIB_MFRONT_MFRONTDEFAULTFINITESTRAINPARSER_HXX_

#include <string>
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  struct DefaultFiniteStrainDSL : public DefaultDSLBase {
    static std::string getName(void);

    static std::string getDescription(void);

    DefaultFiniteStrainDSL();

    ~DefaultFiniteStrainDSL();

   protected:
    virtual void writeBehaviourParserSpecificIncludes(void) override;

  };  // end of struct DefaultFiniteStrainDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTDEFAULTFINITESTRAINPARSER_HXX_ */
