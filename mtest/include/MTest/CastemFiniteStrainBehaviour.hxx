/*!
 * \file   mtest/include/MTest/CastemFiniteStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR_HXX
#define LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR_HXX

#include "MTest/CastemStandardBehaviour.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL CastemFiniteStrainBehaviour
      : public CastemStandardBehaviour {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemFiniteStrainBehaviour(const Hypothesis,
                                const std::string&,
                                const std::string&);
    /*!
     * \param[in] umb: behaviour description
     */
    CastemFiniteStrainBehaviour(const StandardBehaviourDescription&);

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
    /*!
     * \return the string passed to the UMAT function through the
     * CMNAME parameter.
     */
    const char* getBehaviourNameForUMATFunctionCall() const override;
    //! destructor
    ~CastemFiniteStrainBehaviour() override;

   protected:
    /*!
     * \brief compute the elastic stiffness
     * \param[out] Kt   : tangent operator
     * \param[in]  mp   : material properties
     * \param[in]  drot : rotation matrix (Fortran convention)
     */
    virtual void computeElasticStiffness(
        tfel::math::matrix<real>&,
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR_HXX */
