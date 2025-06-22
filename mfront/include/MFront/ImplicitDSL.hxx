/*!
 * \file  mfront/include/MFront/ImplicitDSL.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTIMPLICITPARSER_H_
#define LIB_MFRONT_MFRONTIMPLICITPARSER_H_

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   *
   */
  struct ImplicitDSL : public ImplicitDSLBase {
    static std::string getName(void);
    static std::string getDescription(void);
    ImplicitDSL();
    ~ImplicitDSL();
  };  // end of struct ImplicitDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTIMPLICITPARSER_H_ */
