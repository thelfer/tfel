/*!
 * \file   SchemeParserBase.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_SCHEMEPARSERBASE_HXX_
#define LIB_MTEST_SCHEMEPARSERBASE_HXX_

#include <memory>
#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  // forward declaration
  struct SchemeBase;
  // forward declaration
  struct Evolution;

  struct MTEST_VISIBILITY_EXPORT SchemeParserBase
      : public tfel::utilities::CxxTokenizer {
    //! a simple alias
    using tokens_iterator = TokensContainer::const_iterator;
    /*!
     * \brief allow an lonely ';'
     * \param[in,out] p : position in the input file
     */
    void handleLonelySeparator(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @Author keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleAuthor(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @Date keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleDate(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @OutputFrequency keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleOutputFrequency(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @Description keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleDescription(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @PredictionPolicy keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handlePredictionPolicy(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @Times keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleTimes(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @Real keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleReal(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @StiffnessMatrixType keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleStiffnessMatrixType(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @StiffnessUpdatePolicy keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleStiffnessUpdatePolicy(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @UseCastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleUseCastemAccelerationAlgorithm(SchemeBase&,
                                                      tokens_iterator&);
    /*!
     * handle the @CastemAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleCastemAccelerationTrigger(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @CastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleCastemAccelerationPeriod(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @AccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleAccelerationAlgorithm(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @AccelerationAlgorithmParameter keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleAccelerationAlgorithmParameter(SchemeBase&,
                                                      tokens_iterator&);
    /*!
     * handle the @DynamicTimeStepScaling keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleDynamicTimeStepScaling(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @MaximalTimeStep keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaximalTimeStep(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @MinimalTimeStep keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMinimalTimeStep(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @MaximalTimeStepScalingFactor keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaximalTimeStepScalingFactor(SchemeBase&,
                                                    tokens_iterator&);
    /*!
     * handle the @MinimalTimeStepScalingFactor keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMinimalTimeStepScalingFactor(SchemeBase&,
                                                    tokens_iterator&);
    /*!
     * handle the @MaximumNumberOfIterations keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaximumNumberOfIterations(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @MaximumNumberOfSubSteps keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaximumNumberOfSubSteps(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @Evolution keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleEvolution(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @OutputFile keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleOutputFile(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @XMLOutputFile keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleXMLOutputFile(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @OutputFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleOutputFilePrecision(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @ResidualFile keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleResidualFile(SchemeBase&, tokens_iterator&);
    /*!
     * handle the @ResidualFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleResidualFilePrecision(SchemeBase&, tokens_iterator&);
    //! destructor
    ~SchemeParserBase();

   protected:
    /*!
     * \brief register call backs
     */
    virtual void registerCallBacks();
    /*!
     * \return the file path associated with the keyword documentation
     * \param[in] s: scheme
     * \param[in] k: keyword
     */
    virtual std::string getDocumentationFilePath(const std::string&,
                                                 const std::string&) const;
    /*!
     * \return everything from the given starting point up to the next
     * semi-colon.
     * \param[in,out] p : position in the input file
     */
    virtual std::string readUntilEndOfInstruction(tokens_iterator&);
    /*!
     * \return a real value
     * \param[in] t : structure to be filled
     * \param[in,out] p : position in the input file
     */
    virtual real readDouble(SchemeBase&, tokens_iterator&);
    /*!
     * read a time
     * \param[in,out] t : structure to be filled
     * \param[in,out] p : position in the input file
     */
    virtual real readTime(SchemeBase&, tokens_iterator&);
    /*!
     * \brief parse an evolution
     * \param[in,out] t    : structure to be filled
     * \param[in]     type : evolution type
     * \param[in,out] p    : position in the input file
     */
    std::shared_ptr<Evolution> parseEvolution(SchemeBase&,
                                              const std::string&,
                                              tokens_iterator&);
    /*!
     * \brief try to read the type of an evolution
     * \param[in,out] p : position in the input file
     */
    std::string readEvolutionType(tokens_iterator&);
    /*!
     * read an array
     * The expected size of the array is given by the output vector
     * \param[out]    v : array to be read
     * \param[in,out] t : structure to be filled
     * \param[in,out] p : position in the input file
     */
    void readArrayOfSpecifiedSize(std::vector<real>&,
                                  SchemeBase&,
                                  tokens_iterator&);
    /*!
     * \brief treat the current keyword
     * \param[in,out] p : position in the input file
     */
    bool treatKeyword(SchemeBase&, tokens_iterator&);
    //! \return the list of keywords
    std::vector<std::string> getKeyWordsList(void) const;
    //! a simple alias
    using CallBack = void (SchemeParserBase::*)(SchemeBase&, tokens_iterator&);
    //! callbacks
    std::map<std::string, CallBack> callbacks;
    //! input file
    std::string file;

   private:
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void registerCallBack(const std::string&, const CallBack&);
  };  // end of struct SchemeParserBase

}  // end of namespace mtest

#endif /* LIB_MTEST_SCHEMEPARSERBASE_HXX_ */
