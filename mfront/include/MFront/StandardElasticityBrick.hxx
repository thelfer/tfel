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
  }  // end of namespace bbrick

  /*!
   * a behaviour brick describing standard elasticity for small strain
   * behaviours and handles various enhancements to behaviours based
   * on standard elasticity:
   * - generic support to plane stress.
   * - generic way to compute the tangent operator (implicit parser
   *   only)
   * This extra features can be disabled using dedicated parameters.
   *
   * This brick relies on the Hooke stress potential. The options
   * passed to this brick are forwarded to the Hooke stress potential.
   */
  struct StandardElasticityBrick : public BehaviourBrickBase {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     */
    StandardElasticityBrick(AbstractBehaviourDSL&, BehaviourDescription&);
    std::string getName() const override;
    BehaviourBrickDescription getDescription() const override;
    std::vector<bbrick::OptionDescription> getOptions(
        const bool) const override;
    void initialize(const Parameters&, const DataMap&) override;
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    void completeVariableDeclaration() const override;
    void endTreatment() const override;
    //! \brief destructor
    ~StandardElasticityBrick() override;

   protected:
    std::shared_ptr<bbrick::StressPotential> hooke;
  };  // end of struct StandardElasticityBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_ELASTICITYBEHAVIOURBRICK_H */
