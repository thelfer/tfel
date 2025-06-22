/*!
 * \file  mfront/include/MFront/ImplicitFiniteStrainDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_IMPLICITFINITESTRAINPARSER_HXX
#define LIB_MFRONT_IMPLICITFINITESTRAINPARSER_HXX

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   *
   */
  struct ImplicitFiniteStrainDSL : public ImplicitDSLBase {
    ImplicitFiniteStrainDSL();

    static std::string getName();

    static std::string getDescription();

    virtual ~ImplicitFiniteStrainDSL() noexcept;

  };  // end of struct ImplicitFiniteStrainDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_IMPLICITFINITESTRAINPARSER_HXX */
