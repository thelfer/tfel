/*!
 * \file   DDIF2Brick.hxx
 * \brief
 * \author Helfer Thomas
 * \date   October,20 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DDIF2BRICKBEHAVIOURBRICK_H_
#define LIB_MFRONT_DDIF2BRICKBEHAVIOURBRICK_H_

#include "MFront/StandardElasticityBrick.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct MaterialPropertyDescription;
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct LocalDataStructure;

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
   *   orthotropic (through @OrthotropicBehaviour keyword).  This
   *   parameter can not be used if the `@RequiresStiffnessTensor` has
   *   been used.
   * - `NoPlaneStressSupport`. If this parameter is given, plane
   *   stress support is desactivated.
   * - `NoGenericTangentOperator`. If this parameter is given, the
   *   generic computation of the tangent operator will not be
   *   provided.
   * - `NoGenericPredictionOperator`. If this parameter is given, the
   *   generic computation of the prediction operator will not be
   *   provided.
   */
  struct DDIF2Brick : public StandardElasticityBrick {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     * \param[in] p    : parameters
     * \param[in] d    : data
     */
    DDIF2Brick(AbstractBehaviourDSL&,
               BehaviourDescription&,
               const Parameters&,
               const DataMap&);
    //! \return the name of the brick
    virtual std::string getName() const override;
    /*!
     * \return the list of supported modelling hypotheses.
     */
    virtual std::vector<Hypothesis> getSupportedModellingHypotheses(
        void) const override;
    //! ends the file treatment
    virtual void endTreatment(void) const override;
    //! destructor
    virtual ~DDIF2Brick();

   protected:
    //! fracture stress
    std::shared_ptr<MaterialPropertyDescription> sr;
    //! softening slope
    std::shared_ptr<MaterialPropertyDescription> rp;
  };  // end of struct DDIF2Brick

}  // end of namespace mfront

#endif /* LIB_MFRONT_DDIF2BRICKBEHAVIOURBRICK_H */
