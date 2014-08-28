/*! 
 * \file   mfront/src/MFrontLaTeXBehaviourAnalyser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  19 mai 2014
 */

#include"MFront/MFrontLaTeXBehaviourAnalyser.hxx"

namespace mfront
{

  std::string
  MFrontLaTeXBehaviourAnalyser::getName(void)
  {
    return "LaTeX";
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontLaTeXBehaviourAnalyser::treatKeyword(const std::string&,
					   tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
					   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    return std::make_pair(false,p);
  }

  void
  MFrontLaTeXBehaviourAnalyser::endTreatement(const MechanicalBehaviourDescription&,
					      const MFrontFileDescription&)
  {} // end of MFrontLaTeXBehaviourAnalyser::endTreatement

  void
  MFrontLaTeXBehaviourAnalyser::reset(void)
  {} // end of MFrontLaTeXBehaviourAnalyser::reset
  
  MFrontLaTeXBehaviourAnalyser::~MFrontLaTeXBehaviourAnalyser()
  {} // end of MFrontBehaviourLaTeXAnalyser::~MFrontBehaviourLaTeXAnalyser

} // end of namespace mfront  
