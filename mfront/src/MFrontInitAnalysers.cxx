/*!
 * \file   mfront/src/MFrontInitAnalysers.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mai 2014
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
