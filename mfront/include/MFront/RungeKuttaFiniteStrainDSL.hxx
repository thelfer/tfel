/*!
 * \file  mfront/include/MFront/RungeKuttaFiniteStrainDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  17/10/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_RUNGEKUTTAFINITESTRAINPARSER_HXX
#define LIB_MFRONT_RUNGEKUTTAFINITESTRAINPARSER_HXX

#include "MFront/RungeKuttaDSLBase.hxx"

namespace mfront {

  /*!
   * \brief a structure describing a dsl dedicated to the
   * implementation of finite strain behaviours using explicit
   * algorithms.
   */
  struct RungeKuttaFiniteStrainDSL : public RungeKuttaDSLBase {
    //! \return the name of the dsl
    static std::string getName();
    //! \return the description of the dsl
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! constructor
    RungeKuttaFiniteStrainDSL();
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! destructor
    virtual ~RungeKuttaFiniteStrainDSL() noexcept;
  };  // end of struct RungeKuttaFiniteStrainDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_RUNGEKUTTAFINITESTRAINPARSER_HXX */
