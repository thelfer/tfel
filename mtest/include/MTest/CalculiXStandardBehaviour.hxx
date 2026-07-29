/*!
 * \file  mtest/include/MTest/CalculiXStandardBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  03/08/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTCALCULIXSTANDARDBEHAVIOUR_HXX
#define LIB_MTEST_MTESTCALCULIXSTANDARDBEHAVIOUR_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/StandardBehaviourBase.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the CalculiX
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL CalculiXStandardBehaviour
      : public StandardBehaviourBase {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CalculiXStandardBehaviour(const Hypothesis,
                              const std::string&,
                              const std::string&);
    //
    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;
    std::pair<bool, real> integrate(CurrentState&,
                                    BehaviourWorkSpace&,
                                    const real,
                                    const StiffnessMatrixType) const override;
    void allocateWorkSpace(BehaviourWorkSpace&) const override;
    //! \brief destructor
    ~CalculiXStandardBehaviour() override;

   protected:
    /*!
     * \brief call the mechanical behaviour
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt    : tangent operator
     * \param[out/in] s     : current state
     * \param[out]    wk    : behaviour workspace
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
                                                 const bool) const = 0;
    /*!
     * \brief build the tangent operator from the value returned by
     * the behaviour
     * \return the tangent operator as an st2tost2.
     * \param[in] D: values of the tangent operator
     */
    virtual tfel::math::st2tost2<3u, real> convertTangentOperator(
        const real* const) const;
    //! the CalculiX fonction
    tfel::system::CalculiXFctPtr fct;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTCALCULIXSTANDARDBEHAVIOUR_HXX */
