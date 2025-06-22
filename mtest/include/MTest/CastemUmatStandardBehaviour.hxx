/*!
 * \file   CastemUmatStandardBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 sept. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMUMATSTANDARDBEHAVIOUR_HXX
#define LIB_MTEST_CASTEMUMATSTANDARDBEHAVIOUR_HXX

#include <string>
#include <memory>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/Behaviour.hxx"

namespace mtest {

  /*!
   * an helper class used by to gather common code between the
   * `CastemUmatSmallStrainBehaviour` and
   * `CastemUmatFiniteStrainBehaviour` classes.
   */
  struct CastemUmatStandardBehaviour {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;

   protected:
    /*!
     * \param[in] l: library
     * \param[in] f: function
     * \param[in] p: parameters
     * \param[in] t: type of behaviour
     * \param[in] h: modelling hypothesis
     */
    static std::shared_ptr<Behaviour> buildCastemUmatStandardBehaviour(
        const std::string&,
        const std::string&,
        const tfel::utilities::Data&,
        const int,
        const Hypothesis);
    /*!
     * \param[in] mp: material properties
     * \param[in] evm: currently defined evolution
     * \param[in] stype: symmetry of behaviour
     * \param[in] h: modelling hypothesis
     */
    static void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&,
        const EvolutionManager&,
        const int,
        const Hypothesis);

  };  // end of struct CastemUmatStandardBehaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMUMATSTANDARDBEHAVIOUR_HXX */
