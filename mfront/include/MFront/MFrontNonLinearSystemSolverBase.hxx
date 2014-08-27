/*! 
 * \file  MFrontNonLinearSystemSolverBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 */

#ifndef _LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERBASE_H_
#define _LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERBASE_H_ 

#include<iosfwd>
#include<string>

#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/SupportedTypes.hxx"
#include"MFront/MFrontNonLinearSystemSolver.hxx"

namespace mfront{

  // forward declaraction
  struct MechanicalBehaviourDescription;

  // forward declaraction
  struct VariableDescription;

  /*!
   * base class for non linear system solver providing some helpfull
   * common methods
   */
  struct MFrontNonLinearSystemSolverBase
    : public MFrontNonLinearSystemSolver
  {
    /*!
     * \return the part of the jacobian where the partial derivative
     * of the first variable with respect to the second variable is
     * stored
     * \param[in] mb  : mechanical behaviour description
     * \param[in] v1  : first  variable description
     * \param[in] v2  : second variable description
     
     */
    static std::string
    getJacobianPart(const MechanicalBehaviourDescription&,
		    const VariableDescription&,
		    const VariableDescription&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const std::string& = "this->jacobian",
		    const std::string& = "");
    /*!
     * \return write the code comparing the jacobian to a numerical one
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     * \param[in] nj  : name of the variable holding the numerical jacobian
     */
    static void
    writeComparisonToNumericalJacobian(std::ostream&,
				       const MechanicalBehaviourDescription&,
				       const tfel::material::ModellingHypothesis::Hypothesis,
				       const std::string&);
    /*!
     * \return write the code limiting the value of the increment of
     * an integration variable based on information given by the user
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     * \param[in] v   : name of the variable holding the integration variable increment
     */
    static void
    writeLimitsOnIncrementValues(std::ostream&,
				 const MechanicalBehaviourDescription&,
				 const tfel::material::ModellingHypothesis::Hypothesis,
				 const std::string&);
    /*!
     * \return write the code limiting the value of the increment of
     * a state variable based on physical bounds
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     */
    static void
    writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(std::ostream&,
								    const MechanicalBehaviourDescription&,
								    const tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * \return write the code limiting the value of the increment of
     * a state variable based on physical bounds
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     */
    static void
    writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(std::ostream&,
										    const MechanicalBehaviourDescription&,
										    const tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * \return reserved names that ought to be used by on the non
     * linear system solvers:
     * - error     : current residual. The exact meaning depends on the
                     underlying solver
     * - epsilon   : stopping criterium value
     * - iter      : current iteration, automatically declared as a member
     * - iterMax   : maximum number of iteration
     * - converged : boolean stating that convergence is reached
     */
    virtual std::vector<std::string>
    getReservedNames(void) const;
  protected:
    /*!
     * destructor
     */
    ~MFrontNonLinearSystemSolverBase();  
    
  }; // end of struct MFrontNonLinearSystemSolverBase

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERBASE_H */

