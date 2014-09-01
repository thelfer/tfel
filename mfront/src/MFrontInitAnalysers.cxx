/*!
 * \file   mfront/src/MFrontInitAnalysers.cxx
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

#include"MFront/MFrontBehaviourAnalyserProxy.hxx"
#include"MFront/MFrontLaTeXBehaviourAnalyser.hxx"
#include"MFront/MFrontMarkdownBehaviourAnalyser.hxx"

namespace mfront
{

  void
  initAnalysers(void)
  {
    static const char * const latexProxyNames[2] = {"latex","Latex"};
    MFrontBehaviourAnalyserProxy<MFrontLaTeXBehaviourAnalyser> latexProxy(latexProxyNames,latexProxyNames+2u);
    static const char * const markdownProxyNames[1u] = {"markdown"};
    MFrontBehaviourAnalyserProxy<MFrontMarkdownBehaviourAnalyser> markdownProxy(markdownProxyNames,markdownProxyNames+1u);
  } // end of initAnalysers(void)

} // end of namespace mfront
