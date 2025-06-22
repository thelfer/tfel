/*!
 * \file   mfront/include/MFront/RungeKuttaDSL.hxx
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

#ifndef LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX
#define LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX

#include "MFront/RungeKuttaDSLBase.hxx"

namespace mfront {

  /*!
   * \brief DSL for integrating a mechanical behaviour using
   * Runge-Kutta algorithms.
   */
  struct RungeKuttaDSL : public RungeKuttaDSLBase {
    //! \return the name of the DSL
    static std::string getName();
    //! \return the short description of the DSL
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! constructor
    RungeKuttaDSL();
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! destructor
    ~RungeKuttaDSL() override;
  };  // end of struct RungeKuttaDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX */
