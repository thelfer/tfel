/*!
 * \file   PlasticInelasticFlow.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_PLASTICINELASTICFLOW_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_PLASTICINELASTICFLOW_HXX

#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief describe an inelastic flow defined by a plastic law.
     * \note the base class must declare a state variable named `p`+id.
     */
    struct PlasticInelasticFlow : InelasticFlowBase {
      std::vector<OptionDescription> getOptions() const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      //! destructor
      ~PlasticInelasticFlow() override;

     protected:
      std::string buildFlowImplicitEquations(const BehaviourDescription&,
                                             const StressPotential&,
                                             const std::string&,
                                             const bool) const override;

      /*!
       *\brief maximum equivalent stress factor
       *
       * A factor, denoted \f$alpha\f$ in this description, which gives the
       * maximal equivalent stress allowed before damping the current Newton
       * step. This maximal equivalent stress is defined as \f$alpha * R\f$
       * where \f$R\f$ is the current elastic limit. \f$alpha\f$ must be greater
       * than one.
       *
       * If \f$alpha\f$ is lower than one, which is the default, the code
       * performing this check is not generated.
       */
      double maximum_equivalent_stress_factor = -1;
      /*!
       * \brief a factor which gives the maximum number of iterations below
       * which the check on the maximum equivalent stress is performed.
       */
      double equivalent_stress_check_maximum_iteration_factor = 0.5;

    };  // end of struct PlasticInelasticFlow

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_PLASTICINELASTICFLOW_HXX */
