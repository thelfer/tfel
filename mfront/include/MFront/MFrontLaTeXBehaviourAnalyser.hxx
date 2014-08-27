/*! 
 * \file   MFrontLaTeXBehaviourAnalyser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  19 mai 2014
 */

#ifndef _LIB_MFRONT_MFRONTLATEXBEHAVIOURANALYSER_H_
#define _LIB_MFRONT_MFRONTLATEXBEHAVIOURANALYSER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/MFrontBehaviourAnalyser.hxx"

namespace mfront{
  
  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct TFEL_VISIBILITY_EXPORT MFrontLaTeXBehaviourAnalyser
    : public MFrontBehaviourAnalyser
  {
    /*!
     * \return the name of the analyser
     */
    static std::string
    getName(void);
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
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatement(const MechanicalBehaviourDescription&,
		  const MFrontFileDescription&);
    /*!
     * \brief reset the interface
     */
    virtual void
    reset(void);
    /*!
     * destructor
     */
    virtual ~MFrontLaTeXBehaviourAnalyser();
  }; // end of struct MFrontLaTeXBehaviourAnalyser

} // end of namespace mfront  

#endif /* _LIB_MFRONT_MFRONTLATEXBEHAVIOURANALYSER_H */

