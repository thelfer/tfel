/*!
 * \file   mfront/src/InitAnalysers.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mai 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/BehaviourAnalyserProxy.hxx"
#include"MFront/LaTeXBehaviourAnalyser.hxx"
#include"MFront/MarkdownBehaviourAnalyser.hxx"

namespace mfront
{

  void
  initAnalysers(void)
  {
    static constexpr const char * latexProxyNames[2] = {"latex","Latex"};
    BehaviourAnalyserProxy<LaTeXBehaviourAnalyser> latexProxy(latexProxyNames,latexProxyNames+2u);
    static constexpr const char * markdownProxyNames[1u] = {"markdown"};
    BehaviourAnalyserProxy<MarkdownBehaviourAnalyser> markdownProxy(markdownProxyNames,markdownProxyNames+1u);
  } // end of initAnalysers(void)

} // end of namespace mfront
