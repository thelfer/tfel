/*!
 * \file   mfront/include/MFront/DefaultGenericBehaviourDSL.hxx
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

#ifndef LIB_MFRONT_MFRONTDEFAULTGENERICBEHAVIOURDSL_HXX
#define LIB_MFRONT_MFRONTDEFAULTGENERICBEHAVIOURDSL_HXX

#include <string>
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  /*!
   * \brief Implementation of a generic behaviour
   */
  struct DefaultGenericBehaviourDSL : public DefaultDSLBase {
    //! \return the name of the DSL
    static std::string getName();
    //! \return a description of the DSL
    static std::string getDescription();
    //! \brief default constructor
    DefaultGenericBehaviourDSL();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief destructor
    ~DefaultGenericBehaviourDSL() override;
  };  // end of struct DefaultGenericBehaviourDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTDEFAULTGENERICBEHAVIOURDSL_HXX */
