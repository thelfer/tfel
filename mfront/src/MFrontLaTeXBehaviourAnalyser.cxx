/*! 
 * \file   mfront/src/MFrontLaTeXBehaviourAnalyser.cxx
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
