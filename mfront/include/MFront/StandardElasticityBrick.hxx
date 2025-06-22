/*!
 * \file   StandardElasticityBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   October,20 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ELASTICITYBEHAVIOURBRICK_HXX
#define LIB_MFRONT_ELASTICITYBEHAVIOURBRICK_HXX

#include <memory>
#include "MFront/BehaviourBrickBase.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct LocalDataStructure;

  namespace bbrick {

    struct StressPotential;
  }

  /*!
   * BehaviourBrick describing standard elasticity for small strain behaviours
   * and handles various enhancements to behaviours based on standard
   * elasticity:
   * - Generic support to plane stress.
   * - Generic way to compute the tangent operator (implicit parser
   *   only)
   * This extra features can be disabled using dedicated parameters.
   *
   * This BehaviourBricks allows the followig parameters:
   * - `Isotropic`. This parameter can be used to declare that the
   *   elastic stiffness is isotropic even though the behaviour is
   *   orthotropic. This parameter can not be used if the
   *   `@RequiresStiffnessTensor` has been used.
   * - `Orthotropic`. This requires the behaviour to de declared
   *   orthotropic (through `@OrthotropicBehaviour` keyword).  This
   *   parameter can not be used if the `@RequiresStiffnessTensor` has
   *   been used.
   *
   * This behaviour brick relies on the Hooke stress potential. The options
   * passed to this brick are forwarded to the Hooke stress potential.
   */
  struct StandardElasticityBrick : public BehaviourBrickBase {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     * \param[in] p    : parameters
     * \param[in] d    : options
     */
    StandardElasticityBrick(AbstractBehaviourDSL&,
                            BehaviourDescription&,
                            const Parameters&,
                            const DataMap&);
    //! \return the name of the brick
    std::string getName() const override;
    /*!
     * \return the list of supported modelling hypotheses.
     */
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    //! complete the variable description
    void completeVariableDeclaration() const override;
    //! method called at the end of the input file processing
    void endTreatment() const override;
    //! \brief destructor
    ~StandardElasticityBrick() override;

   protected:
    std::shared_ptr<bbrick::StressPotential> hooke;
  };  // end of struct StandardElasticityBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_ELASTICITYBEHAVIOURBRICK_H */
