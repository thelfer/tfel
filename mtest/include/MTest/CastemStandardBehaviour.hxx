/*!
 * \file   mtest/include/MTest/CastemStandardBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMSTANDARDUMATBEHAVIOUR_H_
#define LIB_MTEST_CASTEMSTANDARDUMATBEHAVIOUR_H_

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/UmatBehaviourBase.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL CastemStandardBehaviour
      : public UmatBehaviourBase {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemStandardBehaviour(const Hypothesis,
                            const std::string&,
                            const std::string&);
    /*!
     * \param[in] umb: behaviour description
     * \param[in] h:   modelling hypothesis
     */
    CastemStandardBehaviour(const UmatBehaviourDescription&, const Hypothesis);
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType getDefaultStiffnessMatrixType(
        void) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : workspace
     */
    virtual void allocate(BehaviourWorkSpace&) const override;
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
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
    /*!
     * \return the string passed to the UMAT function through the
     * CMNAME parameter.
     */
    virtual const char* getBehaviourNameForUMATFunctionCall(void) const = 0;
    /*!
     * destructor
     */
    ~CastemStandardBehaviour();

   protected:
    /*!
     * \brief build the appropriate material properties for the
     * modelling hypothesis from the ones declared in the current
     * state and store them in `wk.mps`. This method has been
     * introduced to properly handle the plane stress hypothesis if
     * this hypothesis is handled by the `Cast3M` interface and not
     * the the behaviour directly.
     *
     * \param[out] wk: workspace
     * \param[in] s: current state
     */
    virtual void buildMaterialProperties(BehaviourWorkSpace&,
                                         const CurrentState&) const;
    //! the umat fonction
    tfel::system::CastemFctPtr fct;
    /*!
     * The umat interface can handle plane stress by calling the
     * generalised plane strain version of the behaviour.  In this
     * case, the hypothesis used by the behaviour is different than
     * the hypothesis used to perform the computation. This flag
     * distinguishes this case.
     */
    bool usesGenericPlaneStressAlgorithm = false;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMSTANDARDUMATBEHAVIOUR_H_ */
