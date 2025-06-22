/*!
 * \file  mfront/include/MFront/ImplicitDSL.hxx
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

#ifndef LIB_MFRONT_MFRONTIMPLICITPARSER_HXX
#define LIB_MFRONT_MFRONTIMPLICITPARSER_HXX

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   *
   */
  struct ImplicitDSL : public ImplicitDSLBase {
    static std::string getName();
    static std::string getDescription();
    ImplicitDSL();
    ~ImplicitDSL();
  };  // end of struct ImplicitDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTIMPLICITPARSER_HXX */
