/*! 
 * \file  mtest/include/MTest/MTestParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_MTESTPARSER_H_
#define LIB_MTEST_MTESTPARSER_H_ 

#include<map>
#include<string>
#include<vector>
#include<memory>

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/SingleStructureSchemeParser.hxx"

namespace mtest
{

  // forward declaration
  struct MTest;
  
  /*!
   * MTestParser is used to parse mtest file.
   */
  struct MTEST_VISIBILITY_EXPORT MTestParser
    : public SingleStructureSchemeParser
  {
    /*!
     * default constructor
     */
    MTestParser();
    /*!
     * execute mtest parser on a file
     * \param[out] t : structure to be filled
     * \param[in] f : file name
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    void execute(MTest&,const std::string&,
		 const std::map<std::string,std::string>&);
    /*!
     * execute mtest parser on a string
     * \param[in] s : string
     */
    void parseString(MTest&,
		     const std::string&);
    //! \return the list of keywords
    virtual std::vector<std::string>
    getKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsHelp() const;
    /*!
     * display the description of a keyword
     */
    virtual void
    displayKeyWordDescription(const std::string&) const;
    /*!
     * destructor
     */
    virtual ~MTestParser();
  protected:
    //! a simple alias
    typedef void (MTestParser::* CallBack)(MTest&,
					   TokensContainer::const_iterator&);
    /*!
     * execute mtest parser after reading a file or parsing a string
     * \param[out] t : structure to be filled
     */
    void execute(MTest& t);
    /*!
     * register the call backs associated with each command
     */
    virtual void
    registerCallBacks(void) override;
    /*!
     * \brief handle the @CompareToNumericalTangentOperator keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleCompareToNumericalTangentOperator(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @TangentOperatorComparisonCriterium keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleTangentOperatorComparisonCriterium(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @NumericalTangentOperatorPerturbationValue keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleNumericalTangentOperatorPerturbationValue(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @Test keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleTest(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @RotationMatrix keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleRotationMatrix(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @StrainEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleStrainEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @DeformationGradientEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleDeformationGradientEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @OpeningDisplacementEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleOpeningDisplacementEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @DrivingVariableEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleDrivingVariableEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @StressEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleStressEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @CohesiveForceEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleCohesiveForceEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ThermodynamicForceEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleThermodynamicForceEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ModellingHypothesis keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleModellingHypothesis(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @Strain keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleStrain(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @DeformationGradient keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleDeformationGradient(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @OpeningDisplacement keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleOpeningDisplacement(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @DrivingVariable keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleDrivingVariable(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @Stress keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleStress(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @CohesiveForce keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleCohesiveForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ThermodynamicForce keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleThermodynamicForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedStress keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedStress(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedCohesiveForce keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedCohesiveForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedThermodynamicForce keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedThermodynamicForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedStrain keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedStrain(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedDeformationGradient keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedDeformationGradient(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedOpeningDisplacement keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedOpeningDisplacement(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ImposedDrivingVariable keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleImposedDrivingVariable(MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief treat the current keyword
     * \param[in,out] p : position in the input file
     */
    bool treatKeyword(MTest&,TokensContainer::const_iterator&);
  private:
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void registerCallBack(const std::string&,const CallBack&);
  }; // end of struct MTestParser

} // end of namespace mtest

#endif /* LIB_MTEST_MTESTPARSER_H_ */

