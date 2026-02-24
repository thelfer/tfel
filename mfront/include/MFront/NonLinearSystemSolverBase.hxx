/*!
 * \file  mfront/include/MFront/NonLinearSystemSolverBase.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERBASE_HXX
#define LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERBASE_HXX

#include <iosfwd>
#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/NonLinearSystemSolver.hxx"

namespace mfront {

  // forward declaraction
  struct BehaviourDescription;

  // forward declaraction
  struct VariableDescription;

  /*!
   * base class for non linear system solver providing some helpfull
   * common methods
   */
  struct NonLinearSystemSolverBase : public NonLinearSystemSolver {
    /*!
     * \brief brief build a suitable specialisation of the class implementing
     * the algorithm.
     * \param[in] bd: behaviour description
     * \param[in] h: hypothesis
     * \param[in] n: class name
     */
    static std::string buildExternalAlgorithmClassName(
        const BehaviourDescription&, const Hypothesis, const std::string&);
    /*!
     * \return the part of the jacobian where the partial derivative
     * of the first variable with respect to the second variable is
     * stored
     * \param[in] v1  : first  variable description
     * \param[in] v2  : second variable description
     */
    static std::string getJacobianPart(const VariableDescription&,
                                       const VariableDescription&,
                                       const SupportedTypes::TypeSize&,
                                       const SupportedTypes::TypeSize&,
                                       const std::string& = "this->jacobian",
                                       const std::string& = "");
    /*!
     * \return write the code comparing the jacobian to a numerical one
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     */
    static void writeEvaluateNumericallyComputedBlocks(
        std::ostream&, const BehaviourDescription&, const Hypothesis);
    /*!
     * \return write the code comparing the jacobian to a numerical one
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     * \param[in] nj  : name of the variable holding the numerical jacobian
     */
    static void writeComparisonToNumericalJacobian(std::ostream&,
                                                   const BehaviourDescription&,
                                                   const Hypothesis,
                                                   const std::string&);
    /*!
     * \return write the code limiting the value of the increment of
     * an integration variable based on information given by the user
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     * \param[in] v   : name of the variable holding the integration variable
     * increment
     */
    static void writeLimitsOnIncrementValues(std::ostream&,
                                             const BehaviourDescription&,
                                             const Hypothesis,
                                             const std::string&);
    /*!
     * \return write the code limiting the value of the increment of
     * a state variable based on physical bounds
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     */
    static void writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(
        std::ostream&, const BehaviourDescription&, const Hypothesis);
    /*!
     * \return write the code limiting the value of the increment of
     * a state variable based on physical bounds
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     */
    static void
    writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(
        std::ostream&, const BehaviourDescription&, const Hypothesis);
    std::vector<std::string> getReservedNames() const override;
    std::vector<std::string> getMemberNames() const override;
    void initializeNumericalParameters(std::ostream&,
                                       const BehaviourDescription&,
                                       const Hypothesis) const override;

   protected:
    //! destructor
    ~NonLinearSystemSolverBase() override;
  };  // end of struct NonLinearSystemSolverBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERBASE_HXX */
