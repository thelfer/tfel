/*!
 * \file   mfront/include/MFront/ImplicitCodeGeneratorBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_IMPLICITCODEGENERATORBASE_HXX
#define LIB_MFRONT_IMPLICITCODEGENERATORBASE_HXX

#include "MFront/BehaviourCodeGeneratorBase.hxx"

namespace mfront {

  // forward declaration
  struct NonLinearSystemSolver;

  struct ImplicitCodeGeneratorBase : BehaviourCodeGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     * \param[in] s: non linear system
     */
    ImplicitCodeGeneratorBase(const FileDescription&,
                              const BehaviourDescription&,
                              const BehaviourInterfaceMap&,
                              const NonLinearSystemSolver&,
                              const std::set<std::string>&,
                              const std::set<std::string>&);
    //! \brief destructor
    ~ImplicitCodeGeneratorBase() override;

   protected:
    /*!
     * \brief a structure describing a view of a derivative in a tiny matrix.
     */
    struct DerivativeViewDescription {
      //! \brief name of the view to be created
      std::string derivative_name;
      //! \brief name of the matrix frow which the view is created
      std::string matrix_name;
      //! \brief variable which is derivatived
      VariableDescription first_variable;
      //! \brief variable with respect to which the derivation is made
      VariableDescription second_variable;
      //! \brief number of rows of the matrix
      SupportedTypes::TypeSize matrix_number_of_rows;
      //! \brief number of columns of the matrix
      SupportedTypes::TypeSize matrix_number_of_columns;
      //! \brief row at which the view starts
      SupportedTypes::TypeSize derivative_row_position;
      //! \brief column at which the view starts
      SupportedTypes::TypeSize derivative_column_position;
    };  // end of struct DerivativeViewDescription
    /*!
     * \brief write the declaration and initialisation of the view of a
     * derivative in a matrix
     * \param[in,out] os: output stream
     * \param[in] d: description of the derivative' view
     */
    virtual void writeDerivativeView(std::ostream&,
                                     const DerivativeViewDescription&) const;

    void writeBehaviourFriends(std::ostream&, const Hypothesis) const override;

    void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourIntegrator(std::ostream&,
                                  const Hypothesis) const override;

    virtual void writeComputeFdF(std::ostream&, const Hypothesis) const;

    void writeBehaviourParserSpecificInheritanceRelationship(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;

    void writeBehaviourParserSpecificTypedefs(std::ostream&) const override;

    void writeBehaviourParserSpecificMembers(std::ostream&,
                                             const Hypothesis) const override;

    void writeBehaviourIntegrationVariablesIncrements(
        std::ostream&, const Hypothesis) const override;

    std::string getLocalVariablesInitializers(const Hypothesis) const override;

    std::string getIntegrationVariablesIncrementsInitializers(
        const Hypothesis) const override;

    void writeBehaviourParserSpecificInitializeMethodPart(
        std::ostream&, const Hypothesis) const override;

    virtual void writeComputeNumericalJacobian(std::ostream&,
                                               const Hypothesis) const;

    void writeBehaviourComputeTangentOperatorBody(
        std::ostream&, const Hypothesis, const std::string&) const override;
    /*!
     * \brief write the `computePartialJacobianInvert` methods. Those methods
     * may be
     * used to compute the consistent tangent operator.
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeComputePartialJacobianInvert(std::ostream&,
                                                   const Hypothesis) const;
    //! \brief non linear solver
    const NonLinearSystemSolver& solver;

#pragma message("shall be removed, this is ugly !")
    const std::set<std::string>& jacobianPartsUsedInIntegrator;
#pragma message("shall be removed, this is ugly !")
    const std::set<std::string>& integrationVariablesIncrementsUsedInPredictor;

  };  // end of ImplicitCodeGeneratorBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_IMPLICITCODEGENERATORBASE_HXX */
