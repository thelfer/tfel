/*!
 * \file  mtest/include/MTest/AsterCohesiveZoneModel.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_H_
#define LIB_MTEST_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_H_

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/UmatBehaviourBase.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterCohesiveZoneModel
      : public UmatBehaviourBase {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterCohesiveZoneModel(const Hypothesis,
                           const std::string&,
                           const std::string&);
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the aster (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    /*!
     * \param[out] v : initial values of the driving variables
     */
    virtual void getDrivingVariablesDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : workspace
     */
    virtual void allocate(BehaviourWorkSpace&) const override;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType getDefaultStiffnessMatrixType(
        void) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out] wk    : workspace
     * \param[in]  s     : current state
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out/in] s     : current state
     * \param[out]    wk    : workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     */
    virtual std::pair<bool, real> integrate(
        CurrentState&,
        BehaviourWorkSpace&,
        const real,
        const StiffnessMatrixType) const override;
    /*!
     * \brief some interfaces requires dummy material properties to be
     * declared. For example, the Cast3M finite element solver
     * requires the mass density and some extra material properties
     * describing orthotropic axes to be declared.  This method is
     * meant to automatically declare those if they are not defined by
     * the user.
     * \param[out] mp  : evolution manager where
     * \param[in]  evm : evolution manager
     */
    virtual void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    //! destructor
    virtual ~AsterCohesiveZoneModel();

   protected:
    /*!
     * \brief call the mechanical behaviour
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt    : tangent operator
     * \param[in,out] s     : current state
     * \param[out]    wk    : work space
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     * \param[in]     b     : if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual std::pair<bool, real> call_behaviour(tfel::math::matrix<real>&,
                                                 CurrentState&,
                                                 BehaviourWorkSpace&,
                                                 const real,
                                                 const StiffnessMatrixType,
                                                 const bool) const;
    //! the aster fonction
    tfel::system::AsterFctPtr fct;
  };  // end of struct AsterCohesiveZoneModel

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_H_ */
