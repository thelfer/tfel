/*! 
 * \file   mfront/include/MFront/MarkdownBehaviourAnalyser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  19 mai 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTMARKDOWNBEHAVIOURANALYSER_H_
#define LIB_MFRONT_MFRONTMARKDOWNBEHAVIOURANALYSER_H_ 

#include<ostream>

#include"MFront/MFrontConfig.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/BehaviourAnalyser.hxx"

namespace mfront{

  //! forward declaration
  struct VariableDescriptionContainer;

  //! forward declaration
  struct BehaviourData;
  
  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct MFRONT_VISIBILITY_EXPORT MarkdownBehaviourAnalyser
    : public BehaviourAnalyser
  {
    /*!
     * \return the name of the analyser
     */
    static std::string
    getName(void);
    /*!
     * constructor
     */
    MarkdownBehaviourAnalyser();
    /*!
     * \return a pair containing a boolean which is true if the
     * keyword was handled, and an iterator past the last token
     * treated.
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    /*!
     * \brief write output files
     * \param[in] mb : mechanical behaviour description
     * \param[in] fd : mfront file description
     */
    virtual void
    endTreatement(const BehaviourDescription&,
		  const FileDescription&);
    //! destructor
    virtual ~MarkdownBehaviourAnalyser();
  protected:
    /*!
     * \brief treat the english output case
     * \param[out] out : output file
     * \param[in] mb   : mechanical behaviour description
     * \param[in] fd   : mfront file description
     */
    virtual void
    treatEnglishOutput(std::ostream&,
		       const BehaviourDescription&,
		       const FileDescription&) const;
    /*!
     * \brief treat the french output case
     * \param[out] out : output file
     * \param[in]  mb  : mechanical behaviour description
     * \param[in]  fd  : mfront file description
     */
    virtual void
    treatFrenchOutput(std::ostream&,
		      const BehaviourDescription&,
		      const FileDescription&) const;
    /*!
     * internal structure gathering data from mechanical behaviour
     * description
     */
    struct Data
    {
      Data() = default;
      Data(Data&&) = default;
      Data(const Data&) = default;
      Data& operator=(Data&&) = default;
      Data& operator=(const Data&) = default;
      ~Data() noexcept;
      std::string name;
      std::string type;
      std::string description;
      std::string externalName;
      std::vector<tfel::material::ModellingHypothesis::Hypothesis> hypotheses;
      unsigned short arraySize;
    };
    static std::vector<Data>
    getData(const BehaviourDescription&,
	    const VariableDescriptionContainer& (BehaviourData::*)(void) const);
    void
    printData(std::ostream&,
	      const BehaviourDescription&,
	      const std::vector<Data>&) const;
    /*!
     * output file language
     */
    std::string language;
  }; // end of struct MarkdownBehaviourAnalyser

} // end of namespace mfront  

#endif /* LIB_MFRONT_MFRONTMARKDOWNBEHAVIOURANALYSER_H_ */

