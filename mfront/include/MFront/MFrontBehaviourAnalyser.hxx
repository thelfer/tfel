/*! 
 * \file   mfront/include/MFront/MFrontBehaviourAnalyser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  19 mai 2014
 */

#ifndef _LIB_MFRONT_MFRONTBEHAVIOURANALYSER_H_
#define _LIB_MFRONT_MFRONTBEHAVIOURANALYSER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

namespace mfront{
  
  // forward declaration
  struct MechanicalBehaviourDescription;

  // forward declaration
  struct MFrontFileDescription;

  /*!
   * This is the abstract base class of all behaviour analysers
   */
  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourAnalyser
  {
    /*!
     * \return a pair containing a boolean which is true if the
     * keyword was handled, and an iterator past the last token
     * treated.
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) = 0;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatement(const MechanicalBehaviourDescription&,
		  const MFrontFileDescription&) = 0;
    /*!
     * \brief reset the interface
     */
    virtual void
    reset(void) = 0;
    /*!
     * destructor
     */
    virtual ~MFrontBehaviourAnalyser();
  }; // end of struct MFrontBehaviourAnalyser

} // end of namespace mfront  

#endif /* _LIB_MFRONT_MFRONTBEHAVIOURANALYSER_H */

