/*!
 * \file  mfront/include/MFront/RungeKuttaModelDSL.hxx
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

#ifndef LIB_MFRONT_RUNGEKUTTAMODELDSL_HXX
#define LIB_MFRONT_RUNGEKUTTAMODELDSL_HXX

#include "MFront/RungeKuttaDSLBase.hxx"

namespace mfront {

  /*!
   * \brief Implementation of a model using explicit algorithms
   */
  struct RungeKuttaModelDSL : public RungeKuttaDSLBase {
    //! \brief constructor
    RungeKuttaModelDSL();
    //! \return the name of the DSL
    static std::string getName();
    //! \return a description of the DSL
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief destructor
    ~RungeKuttaModelDSL() noexcept override;
  };  // end of struct RungeKuttaModelDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_RUNGEKUTTAMODELDSL_HXX */
