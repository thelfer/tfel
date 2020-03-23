/*!
 * \file  mtest/include/MTest/AbaqusExplicitBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
    static std::string getBehaviourName(const std::string&, const Hypothesis);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AbaqusExplicitBehaviour(const Hypothesis,
                            const std::string&,
                            const std::string&);

    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;

    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;

    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;

    bool doPackagingStep(BehaviourWorkSpace&,
                         const CurrentStateView&) const override;

    std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentStateView&,
        const StiffnessMatrixType) const override;

    std::pair<bool, real> integrate(CurrentStateView&,
                                    BehaviourWorkSpace&,
                                    const real,
                                    const StiffnessMatrixType) const override;

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
