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

#include "MFront/BehaviourBrickBase.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
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
   *   orthotropic (through `@OrthotropicBehaviour` keyword).  This
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
  struct StandardElasticityBrick : public BehaviourBrickBase {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     * \param[in] p    : parameters
     * \param[in] d    : data
     */
    StandardElasticityBrick(AbstractBehaviourDSL&,
                            BehaviourDescription&,
                            const Parameters&,
                            const DataMap&);
    //! \return the name of the brick
    virtual std::string getName() const override;
    /*!
     * \return the list of supported modelling hypotheses.
     */
    virtual std::vector<Hypothesis> getSupportedModellingHypotheses()
        const override;
    //! complete the variable description
    virtual void completeVariableDeclaration() const override;
    //! method called at the end of the input file processing
    virtual void endTreatment() const override;
    /*!
     * destructor
     */
    virtual ~StandardElasticityBrick();

   protected:
    /*!
     * \brief declared the computeStress and computeFinalStress when the
     * requiresStiffnessTensor attribute has been set.
     */
    virtual void declareComputeStressWhenStiffnessTensorIsDefined() const;
    /*!
     * treat the case of isotropic behaviours
     * \param[in] d: local data structure
     */
    virtual void treatIsotropicBehaviour(LocalDataStructure&) const;
    /*!
     * treat the case of isotropic behaviours
     */
    virtual void treatOrthotropicBehaviour() const;
    /*!
     * \brief add support for the AXISYMMETRICALGENERALISEDPLANESTRESS
     * modelling hypothesis
     */
    virtual void addAxisymmetricalGeneralisedPlaneStressSupport() const;
    /*!
     * \brief add support for the PLANESTRESS modelling hypothesis
     */
    virtual void addPlaneStressSupport() const;
    /*!
     * \brief add the generic tangent operator computation
     */
    virtual void addGenericTangentOperatorSupport() const;
    /*!
     * \brief add the generic prediction operator computation
     */
    virtual void addGenericPredictionOperatorSupport() const;
    /*!
     * \brief declare the compute elastic prediction method
     */
    virtual void declareComputeElasticPredictionMethod() const;
    //! plane stress support;
    bool pss = true;
    //! generic prediction operator support
    bool gto = true;
    //! generic tangent operator support
    bool gpo = true;
  };  // end of struct StandardElasticityBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_ELASTICITYBEHAVIOURBRICK_H */
