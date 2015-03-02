/*! 
 * \file   mfront/include/MFront/LaTeXBehaviourAnalyser.hxx
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

#ifndef LIB_MFRONT_MFRONTLATEXBEHAVIOURANALYSER_H_
#define LIB_MFRONT_MFRONTLATEXBEHAVIOURANALYSER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/BehaviourAnalyser.hxx"

namespace mfront{
  
  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct TFEL_VISIBILITY_EXPORT LaTeXBehaviourAnalyser
    : public BehaviourAnalyser
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
    endTreatement(const BehaviourDescription&,
		  const FileDescription&);
    /*!
     * \brief reset the interface
     */
    virtual void
    reset(void);
    /*!
     * destructor
     */
    virtual ~LaTeXBehaviourAnalyser();
  }; // end of struct LaTeXBehaviourAnalyser

} // end of namespace mfront  

#endif /* LIB_MFRONT_MFRONTLATEXBEHAVIOURANALYSER_H_ */

