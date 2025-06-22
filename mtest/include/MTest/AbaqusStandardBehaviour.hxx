/*!
 * \file  mtest/include/MTest/AbaqusStandardBehaviour.hxx
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

#ifndef LIB_MTEST_MTESTABAQUSSTANDARDBEHAVIOUR_H_
#define LIB_MTEST_MTESTABAQUSSTANDARDBEHAVIOUR_H_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/UmatBehaviourBase.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the abaqus
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AbaqusStandardBehaviour
      : public UmatBehaviourBase {
    /*!
     * \return the name of the behaviour by stripping the function
     * name from the suffix describing the modelling hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] f : function implementation the behaviour
     */
    static std::string getBehaviourName(const std::string&, const Hypothesis);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AbaqusStandardBehaviour(const Hypothesis,
                            const std::string&,
                            const std::string&);
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
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType getDefaultStiffnessMatrixType(
        void) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out] wk    : behaviour workspace
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
     * \param[in,out] s     : current state
     * \param[out]    wk    : behaviour workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     */
    virtual std::pair<bool, real> integrate(
        CurrentState&,
        BehaviourWorkSpace&,
        const real,
        const StiffnessMatrixType) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : behaviour workspace
     */
    virtual void allocate(BehaviourWorkSpace&) const override;
    //! destructor
    virtual ~AbaqusStandardBehaviour();

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
    //! the abaqus fonction
    tfel::system::AbaqusFctPtr fct;
    //! orthotropy management policy
    unsigned short omp = 0u;
    //! save tangent operator
    bool savesTangentOperator = false;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTABAQUSSTANDARDBEHAVIOUR_H_ */
