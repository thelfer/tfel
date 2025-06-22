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

  /*!
   * \brief a DSL handling strain based behaviours integrated using an user
   * defined scheme.
   */
  struct DefaultDSL : public DefaultDSLBase {
    //! \brief return the name of the DSL
    static std::string getName();
    //! \brief return a short description of the DSL
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief  constructor
    DefaultDSL();
    //! \brief  destructor
    ~DefaultDSL() override;
  };  // end of struct DefaultDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTPARSER_HXX */
