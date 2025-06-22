/*!
 * \file  mfront/include/MFront/RungeKuttaGenericBehaviourDSL.hxx
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

#ifndef LIB_MFRONT_RUNGEKUTTAGENERICBEHAVIOURDSL_HXX
#define LIB_MFRONT_RUNGEKUTTAGENERICBEHAVIOURDSL_HXX

#include "MFront/RungeKuttaDSLBase.hxx"

namespace mfront {

  /*!
   * \brief Implementation of a generic behaviour using explicit algorithms
   */
  struct RungeKuttaGenericBehaviourDSL : public RungeKuttaDSLBase {
    //! constructor
    RungeKuttaGenericBehaviourDSL();
    //! \return the name of the DSL
    static std::string getName();
    //! \return a description of the DSL
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! \brief destructor
    ~RungeKuttaGenericBehaviourDSL() noexcept override;
  };  // end of struct RungeKuttaGenericBehaviourDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_RUNGEKUTTAGENERICBEHAVIOURDSL_HXX */
