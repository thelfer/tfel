/*!
 * \file   mtest/include/MTest/CastemStandardBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  18/11/2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMSTANDARDUMATBEHAVIOUR_HXX
#define LIB_MTEST_CASTEMSTANDARDUMATBEHAVIOUR_HXX

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/StandardBehaviourBase.hxx"
#include "MTest/CastemInterfaceVersion.hxx"

namespace mtest {

  /*!
   * \brief a class to handle mechanical beheaviours written using the umat
   * interface of the `Cast3M` finite element solver.
   */
  struct TFEL_VISIBILITY_LOCAL CastemStandardBehaviour
      : public StandardBehaviourBase {
    /*!
     * \brief constructor
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemStandardBehaviour(const Hypothesis,
                            const std::string&,
                            const std::string&);
    /*!
     * \brief constructor
     * \param[in] umb: behaviour description
     */
    CastemStandardBehaviour(const StandardBehaviourDescription&);
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    void allocateWorkSpace(BehaviourWorkSpace&) const override;
    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    std::vector<std::string> getOptionalMaterialProperties() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    /*!
     * \return the string passed to the UMAT function through the
     * CMNAME parameter.
     */
    virtual const char* getBehaviourNameForUMATFunctionCall() const = 0;
    //! destructor
    ~CastemStandardBehaviour() override;

   protected:
    //! \return the version of the `Cast3M` interface
    virtual CastemInterfaceVersion getCastemInterfaceVersion() const;
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
    /*!
     * The umat interface can handle plane stress by calling the
     * generalised plane strain version of the behaviour.  In this
     * case, the hypothesis used by the behaviour is different than
     * the hypothesis used to perform the computation. This flag
     * distinguishes this case.
     *
     * This flag must be set but the derived classes.
     */
    bool usesGenericPlaneStressAlgorithm = false;
    //! the umat fonction
    tfel::system::CastemFctPtr fct;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMSTANDARDUMATBEHAVIOUR_HXX */
