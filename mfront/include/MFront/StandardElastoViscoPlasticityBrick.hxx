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
  } // end of namespace bbrick

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
    std::shared_ptr<bbrick::StressPotential> stress_potential;
  }; // end of StandardElastoViscoPlasticityBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX */
