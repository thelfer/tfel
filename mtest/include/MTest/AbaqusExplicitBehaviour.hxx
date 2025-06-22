/*!
 * \file  mtest/include/MTest/AbaqusExplicitBehaviour.hxx
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

#ifndef LIB_MTEST_MTESTABAQUSEXPLICITBEHAVIOUR_HXX
#define LIB_MTEST_MTESTABAQUSEXPLICITBEHAVIOUR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/StandardBehaviourBase.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the abaqus
   * interface
   */
  struct MTEST_VISIBILITY_EXPORT AbaqusExplicitBehaviour
      : public StandardBehaviourBase {
    /*!
     * \return the suffix associated with a modelling hypothesis
     * \param[in] h: hypothesis
     */
    static std::string getHypothesisSuffix(const Hypothesis);
    /*!
     * \return the name of the behaviour by stripping the function
     * name from the suffix describing the modelling hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] f : function implementation the behaviour
     */
    static std::string extractBehaviourName(const std::string&,
                                            const Hypothesis);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AbaqusExplicitBehaviour(const Hypothesis,
                            const std::string&,
                            const std::string&);
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    /*!
     * \param[out] v : initial values of the driving variables
     */
    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    /*!
     * \brief execute the packaging step. This victious step is done
     * at the beginning of the computation.
     * \return a boolean
     * \param[out] wk : behaviour workspace
     * \param[in] s   : current state
     */
    bool doPackagingStep(CurrentState&, BehaviourWorkSpace&) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out] wk    : behaviour workspace
     * \param[in]  s     : current state
     * \param[in]  ktype : type of the stiffness matrix
     */
    std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[in,out] s     : current state
     * \param[out]    wk    : behaviour workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     */
    std::pair<bool, real> integrate(CurrentState&,
                                    BehaviourWorkSpace&,
                                    const real,
                                    const StiffnessMatrixType) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : behaviour workspace
     */
    void allocate(BehaviourWorkSpace&) const override;
    //! destructor
    ~AbaqusExplicitBehaviour() override;

   protected:
    //! the abaqus fonction
    tfel::system::AbaqusExplicitFctPtr fct;
    //! orthotropy management policy
    unsigned short omp = 0u;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTABAQUSEXPLICITBEHAVIOUR_HXX */
