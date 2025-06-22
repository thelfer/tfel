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
    };  // end of struct PlasticInelasticFlow

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_PLASTICINELASTICFLOW_HXX */
