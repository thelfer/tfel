/*!
 * \file  mtest/include/MTest/MTestParser.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 09 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTPARSER_HXX
#define LIB_MTEST_MTESTPARSER_HXX

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/SingleStructureSchemeParser.hxx"

namespace mtest {

  // forward declaration
  struct MTest;

  /*!
   * \brief class used to parse `MTest` file.
   */
  struct MTEST_VISIBILITY_EXPORT MTestParser
      : public SingleStructureSchemeParser {
    /*!
     * default constructor
     */
    MTestParser();
    /*!
     * execute mtest parser on a file
     * \param[out] t:    structure to be filled
     * \param[in] f:     file name
     * \param[in] ecmds: external commands
     * \param[in] s:     substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    void execute(MTest&,
                 const std::string&,
                 const std::vector<std::string>&,
                 const std::map<std::string, std::string>&);
    /*!
     * execute mtest parser on a string
     * \param[in] s : string
     */
    void parseString(MTest&, const std::string&);
    //! \return the list of keywords
    virtual std::vector<std::string> getKeyWordsList() const;
    //! \brief display the list of keywords
    virtual void displayKeyWordsList() const;
    //! \brief display the list of keywords
    virtual void displayKeyWordsHelp() const;
    //! \brief display the description of a keyword
    virtual void displayKeyWordDescription(const std::string&) const;
    //! \brief destructor
    ~MTestParser() override;

   protected:
    //! a simple alias
    using CallBack = void (MTestParser::*)(MTest&, tokens_iterator&);
    /*!
     * execute mtest parser after reading a file or parsing a string
     * \param[out] t : structure to be filled
     */
    void execute(MTest& t);
    /*!
     * register the call backs associated with each command
     */
    void registerCallBacks() override;
    /*!
     * \brief handle the `@Event` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleEvent(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@CompareToNumericalTangentOperator` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleCompareToNumericalTangentOperator(MTest&,
                                                         tokens_iterator&);
    /*!
     * \brief handle the `@TangentOperatorComparisonCriterium` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleTangentOperatorComparisonCriterion(MTest&,
                                                          tokens_iterator&);
    /*!
     * \brief handle the `@NumericalTangentOperatorPerturbationValue`
     * keyword.
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleNumericalTangentOperatorPerturbationValue(
        MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@Test` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleTest(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@RotationMatrix` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleRotationMatrix(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@StrainEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleStrainEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@DeformationGradientEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleDeformationGradientEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@OpeningDisplacementEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleOpeningDisplacementEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@GradientEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleGradientEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@StressEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleStressEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@CohesiveForceEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleCohesiveForceEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ThermodynamicForceEpsilon` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleThermodynamicForceEpsilon(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ModellingHypothesis` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleModellingHypothesis(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@Strain` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleStrain(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@DeformationGradient` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleDeformationGradient(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@OpeningDisplacement` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleOpeningDisplacement(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@Gradient` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleGradient(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@Stress` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleStress(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@CohesiveForce` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleCohesiveForce(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ThermodynamicForce` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleThermodynamicForce(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedStress` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedStress(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedCohesiveForce` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedCohesiveForce(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedThermodynamicForce` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedThermodynamicForce(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@NonLinearConstraint`` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleNonLinearConstraint(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedStrain` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedStrain(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedDeformationGradient` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedDeformationGradient(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedOpeningDisplacement` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedOpeningDisplacement(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ImposedGradient` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleImposedGradient(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@UserDefinedPostProcessing` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handleUserDefinedPostProcessing(MTest&, tokens_iterator&);
    /*!
     * \brief handle the `@PrintLagrangeMultipliers` keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    virtual void handlePrintLagrangeMultipliers(MTest&, tokens_iterator&);
    /*!
     * \brief read the options associated with a constraint
     * \param[in] m: calling method
     * \param[in,out] p: position in the input file
     */
    ConstraintOptions readConstraintOptions(const std::string&,
                                            tokens_iterator&);
    /*!
     * \brief treat the current keyword
     * \param[in,out] t: `MTest` object to be configured
     * \param[in,out] p: position in the input file
     */
    bool treatKeyword(MTest&, tokens_iterator&);

   private:
    //! callbacks
    std::map<std::string, CallBack> callbacks;
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void registerCallBack(const std::string&, const CallBack&);
  };  // end of struct MTestParser

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTPARSER_HXX */
