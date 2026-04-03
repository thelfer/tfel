/*!
 * \file  mtest/include/MTest/GenericBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_GENERICBEHAVIOUR_HXX
#define LIB_MTEST_GENERICBEHAVIOUR_HXX

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MFront/GenericBehaviour/BehaviourData.hxx"
#include "MTest/StandardBehaviourBase.hxx"

namespace mtest {

  /*!
   * \brief A class to handle mechanical beheaviours written using the
   * generic behaviour interface.
   */
  struct TFEL_VISIBILITY_LOCAL GenericBehaviour : public StandardBehaviourBase {
    /*!
     * \param[in] h: modelling hypothesis
     * \param[in] l: library name
     * \param[in] b: behaviour name
     */
    GenericBehaviour(const Hypothesis, const std::string&, const std::string&);
    /*!
     * \param[in] h: modelling hypothesis
     * \param[in] l: library name
     * \param[in] b: behaviour name
     * \param[in] params: parameters
     */
    GenericBehaviour(const Hypothesis,
                     const std::string&,
                     const std::string&,
                     const ParametersMap&);

    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;

    std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;

    std::pair<bool, real> integrate(CurrentState&,
                                    BehaviourWorkSpace&,
                                    const real,
                                    const StiffnessMatrixType) const override;

    std::vector<std::string> getOptionalMaterialProperties() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;

    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;

    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;

    void allocateWorkSpace(BehaviourWorkSpace&) const override;
    //! destructor
    ~GenericBehaviour() override;

   protected:
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt:    tangent operator
     * \param[in,out] s :    current state
     * \param[out]    wk:    workspace
     * \param[in]     dt:    time increment
     * \param[in]     ktype: type of the stiffness matrix
     * \param[in]     b:     if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual std::pair<bool, real> call_behaviour(tfel::math::matrix<real>&,
                                                 CurrentState&,
                                                 BehaviourWorkSpace&,
                                                 const real,
                                                 const StiffnessMatrixType,
                                                 const bool) const;

    virtual void executeFiniteStrainBehaviourStressPreProcessing(
        BehaviourWorkSpace&, mfront::gb::BehaviourData&) const;

    virtual void executeFiniteStrainBehaviourStressPostProcessing(
        BehaviourWorkSpace&, mfront::gb::BehaviourData&) const;

    virtual void executeFiniteStrainBehaviourTangentOperatorPreProcessing(
        mfront::gb::BehaviourData&, const StiffnessMatrixType) const;

    virtual void executeFiniteStrainBehaviourTangentOperatorPostProcessing(
        mfront::gb::BehaviourData&) const;

    //! \brief pointer to the function
    tfel::system::GenericBehaviourFctPtr fct;
    /*!
     * \brief pointer to the function in charge of rotating the gradients from
     * the global frame to the material frame
     */
    tfel::system::GenericBehaviourRotateGradientsFctPtr rg_fct = nullptr;
    /*!
     * \brief pointer to the function in charge of rotating the thermodynamic
     * forces from the material frame to the global frame
     */
    tfel::system::GenericBehaviourRotateThermodynamicForcesFctPtr rtf_fct =
        nullptr;
    /*!
     * \brief pointer to the function in charge of rotating the tangent
     * operator blocks from the material frame to the global frame
     */
    tfel::system::GenericBehaviourRotateThermodynamicForcesFctPtr rto_fct =
        nullptr;
    //! \brief stress measure requested for finite strain behaviours
    enum {
      CAUCHY,  //!< Cauchy stress
      PK2,     //!< Second Piola-Kirchoff stress
      PK1      //!< First Piola-Kirchoff stress
    } stress_measure = CAUCHY;
    /*!
     * \brief type of finite strain tangent operator requested for finite
     * strain behaviours
     */
    enum {
      DSIG_DF, /*!< derivative of the Cauchy stress with respect to the
                    deformation gradient */
      DS_DEGL, /*!< derivative of the second Piola-Kirchoff stress with
                    respect to the Green-Lagrange strain */
      DPK1_DF, /*!< derivative of the first Piola-Kirchoff stress with
                   respect to the deformation gradient  */
      DTAU_DDF /*!< derivative of the Kirchoff stress with
                   respect to the spatial increment of the deformation gradient,
                  i.e. F1 * F0 ^ -1  */
    } fsto = DSIG_DF;
  };  // end of struct GenericBehaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_GENERICBEHAVIOUR_HXX */
