/*!
 * \file   mfront/includes/MFront/StandardElastoViscoPlasticityBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX
#define LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX

#include <memory>
#include "MFront/BehaviourBrickBase.hxx"

namespace mfront {

  namespace bbrick {
    // forward declaration
    struct StressPotential;
    // forward declaration
    struct InelasticFlow;
  }  // end of namespace bbrick

  /*!
   * \brief The `StandardElastoViscoPlasticityBrick` is meant to describe a
   * behaviour based on:
   * - a stress potential describing how the stress is computed. The most
   *   used one will probably be the `Hooke` stress potential.
   * - a list of inelastic flows which describe how inelastic strains evolve.
   */
  struct StandardElastoViscoPlasticityBrick : public BehaviourBrickBase {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     * \param[in] p    : parameters
     * \param[in] d    : data
     */
    StandardElastoViscoPlasticityBrick(AbstractBehaviourDSL&,
                                       BehaviourDescription&,
                                       const Parameters&,
                                       const DataMap&);
    //! \return the name of the brick
    std::string getName() const override;
    //! \return the list of supported modelling hypotheses.
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    //! complete the variable description
    void completeVariableDeclaration() const override;
    //! method called at the end of the input file processing
    void endTreatment() const override;
    //! destructor
    ~StandardElastoViscoPlasticityBrick() override;

   private:
    //! stress potential
    std::shared_ptr<bbrick::StressPotential> stress_potential;
    //! inelastic flows
    std::vector<std::shared_ptr<bbrick::InelasticFlow>> flows;

  };  // end of StandardElastoViscoPlasticityBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX */
