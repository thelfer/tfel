/*!
 * \file   mfront/includes/MFront/StandardElastoViscoPlasticityBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX
#define LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX

namespace mfront {

  namespace sevpb {

    /*!
     * \brief
     */
    struct IsotropicHardeningRule {};  // end of struct IsotropicHardeningRule

    /*!
     * \brief
     */
    struct KinematicHardeningRule {};  // end of struct KinematicHardeningRule

    /*!
     * \brief
     */
    struct InelasticPotential {};  // end of struct InelasticPotential

    /*!
     * \brief description of a standard elasto-viscoplastic behaviour.
     *
     * A standard elasto-viscoplastic behaviour is defined by:
     *
     * - the split of the total strain in an elastic strain and one or several
     *   (visco)-plastic strain.
     * - a stress potential which enables to relate the stress to the elastic
     *   strain. This stress potential may define damage variables.
     * - several inelastic flows defined by:
     *   - a plastic potential. A plastic potential is defined based on a
     *     certain scalar potential, the choice of a stress criterion an
     *     isotropic hardening rule and one or several kinematic hardening
     *     rules.
     *   - a plastic flow potential (by default, equal to the plastic potential)
     */
    struct SEVPBehaviourDescription {
      //! stress relationship
      std::shared_ptr<StressPotential> stress_potential;
      //! list of inelastic flows
      std::vector<std::shared_ptr<InelasticFlow>> inelastic_flows;
    };

  }  // end of namespace sevpb

  struct StandardElastoViscoPlasticityBrick {};

}  // end of namespace mfront

#endif /* LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX */
