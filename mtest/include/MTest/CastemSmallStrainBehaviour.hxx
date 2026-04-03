/*!
 * \file  mtest/include/MTest/CastemSmallStrainBehaviour.hxx
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

#ifndef LIB_MTEST_CASTEMSMALLSTRAINUMATBEHAVIOUR_HXX
#define LIB_MTEST_CASTEMSMALLSTRAINUMATBEHAVIOUR_HXX

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/CastemStandardBehaviour.hxx"

namespace mtest {

  /*!
   * \brief A class to handle mechanical beheaviours written using the umat
   * interface of the `Cast3M` finite element solver. This class is based on
   * the legacy version of the `Cast3M` interface.
   */
  struct TFEL_VISIBILITY_LOCAL CastemSmallStrainBehaviour
      : public CastemStandardBehaviour {
    /*!
     * \brief constructor
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemSmallStrainBehaviour(const Hypothesis,
                               const std::string&,
                               const std::string&);
    /*!
     * \param[in] umb: behaviour description
     */
    CastemSmallStrainBehaviour(const StandardBehaviourDescription&);
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
    const char* getBehaviourNameForUMATFunctionCall() const override;
    //! \brief destructor
    ~CastemSmallStrainBehaviour() override;

   protected:
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt    : tangent operator
     * \param[in,out] s     : current state
     * \param[out]    wk    : workspace
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

  /*!
   * \brief a class to handle mechanical beheaviours written using the umat
   * interface of the `Cast3M` finite element solver  for `Cast3M` versions
   * greater than 2021.
   */
  struct TFEL_VISIBILITY_LOCAL Castem21SmallStrainBehaviour
      : public CastemSmallStrainBehaviour {
    // inheriting constructors
    using CastemSmallStrainBehaviour::CastemSmallStrainBehaviour;
    //! \brief destructor
    ~Castem21SmallStrainBehaviour() override;

   protected:
    //! \return the version of the `Cast3M` interface
    CastemInterfaceVersion getCastemInterfaceVersion() const override;
  };  // end of struct Castem21SmallStrainBehaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMSMALLSTRAINUMATBEHAVIOUR_HXX */
