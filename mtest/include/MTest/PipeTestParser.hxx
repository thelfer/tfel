/*!
 * \file  mtest/include/MTest/PipeTestParser.hxx
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

#ifndef LIB_MTEST_PIPETESTPARSER_H
#define LIB_MTEST_PIPETESTPARSER_H

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/SingleStructureSchemeParser.hxx"

namespace mtest {

  // forward declaration
  struct PipeTest;

  /*!
   * PipeTestParser is used to parse mtest file.
   */
  struct MTEST_VISIBILITY_EXPORT PipeTestParser
      : public SingleStructureSchemeParser {
    /*!
     * default constructor
     */
    PipeTestParser();
    /*!
     * execute mtest parser on a file
     * \param[out] t:    structure to be filled
     * \param[in] f:     file name
     * \param[in] ecmds: external commands
     * \param[in] s:     substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    void execute(PipeTest&,
                 const std::string&,
                 const std::vector<std::string>&,
                 const std::map<std::string, std::string>&);
    /*!
     * execute mtest parser on a string
     * \param[in] s : string
     */
    void parseString(PipeTest&, const std::string&);
    //! \return the list of keywords
    virtual std::vector<std::string> getKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void displayKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void displayKeyWordsHelp() const;
    /*!
     * display the description of a keyword
     */
    virtual void displayKeyWordDescription(const std::string&) const;
    /*!
     * destructor
     */
    ~PipeTestParser() override;

   protected:
    /*!
     * \brief handle the `@RadialLoading` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleRadialLoading(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@AxialLoading` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleAxialLoading(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@InnerRadius` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleInnerRadius(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@OuterRadius` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleOuterRadius(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@NumberOfElements` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleNumberOfElements(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ElementType` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleElementType(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@PerformSmallStrainAnalysis` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handlePerformSmallStrainAnalysis(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@InnerPressureEvolution` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleInnerPressureEvolution(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@OuterPressureEvolution` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleOuterPressureEvolution(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@InnerRadiusEvolution` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleInnerRadiusEvolution(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@OuterRadiusEvolution` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleOuterRadiusEvolution(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@AxialForceEvolution` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleAxialForceEvolution(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@AxialGrowthEvolution` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleAxialGrowthEvolution(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@GasEquationOfState` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleGasEquationOfState(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@FillingPressure` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleFillingPressure(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@FillingTemperature` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleFillingTemperature(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@DisplacementEpsilon` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleDisplacementEpsilon(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@ResidualEpsilon` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleResidualEpsilon(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@Profile` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleProfile(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@Test` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleTest(PipeTest&, tokens_iterator&);
    /*!
     * \brief handle the `@AdditionalOutputs` keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void handleAdditionalOutputs(PipeTest&, tokens_iterator&);
    //! a simple alias
    typedef void (PipeTestParser::*CallBack)(PipeTest&, tokens_iterator&);
    /*!
     * execute mtest parser after reading a file or parsing a string
     * \param[out] t: structure to be filled
     */
    void execute(PipeTest& t);
    /*!
     * register the call backs associated with each command
     */
    void registerCallBacks() override;
    /*!
     * \brief treat the current keyword
     * \param[in,out] p: position in the input file
     */
    bool treatKeyword(PipeTest&, tokens_iterator&);

   private:
    //! callbacks
    std::map<std::string, CallBack> callbacks;
    /*!
     * register a call back
     * \param[in] k: key word
     * \param[in] p: pointer to a member function
     */
    void registerCallBack(const std::string&, const CallBack&);
  };  // end of struct PipeTestParser

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPETESTPARSER_H */
