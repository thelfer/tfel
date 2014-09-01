/*! 
 * \file   mfront/include/MFront/MFrontBehaviourParserCommon.ixx
 * \brief
 * \author Helfer Thomas
 * \brief  14 févr. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX_
#define _LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX_ 

namespace mfront
{

  template<typename T,
	   typename T2>
  MFrontBehaviourParserCommon::StandardVariableModifier<T,T2>::StandardVariableModifier(T& i,
											const Hypothesis h,
											const MPtr p)
    : instance(i),
      hypothesis(h),
      mptr(p)
  {} // end of StandardVariableModifier<T,T2>::StandardVariableModifier

  template<typename T,
	   typename T2>
  std::string
  MFrontBehaviourParserCommon::StandardVariableModifier<T,T2>::exe(const std::string& v,
								   const bool b)
  {
    return (this->instance.*mptr)(this->hypothesis,v,b);
  } // end of StandardVariableModifier<T,T2>::exe

  template<typename T,
	   typename T2>
  MFrontBehaviourParserCommon::StandardVariableModifier<T,T2>::~StandardVariableModifier()
  {} // end of StandardVariableModifier<T,T2>::~StandardVariableModifier

  template<typename T,
	   typename T2>
  tfel::utilities::shared_ptr<MFrontBehaviourParserCommon::VariableModifier>
  MFrontBehaviourParserCommon::makeVariableModifier(T& i,
						    const Hypothesis h,
						    std::string (T2::* p)(const Hypothesis,
									  const std::string&,
									  const bool))
  {
    using namespace tfel::utilities;
    return shared_ptr<VariableModifier>(new StandardVariableModifier<T,T2>(i,h,p));
  } // end of makeVariableModifier

  template<typename T,
	   typename T2>
  MFrontBehaviourParserCommon::StandardWordAnalyser<T,T2>::StandardWordAnalyser(T& i,
										const Hypothesis h,
										const MPtr p)
    : instance(i),
      mptr(p),
      hypothesis(h)
  {} // end of StandardWordAnalyser<T,T2>::StandardWordAnalyser

  template<typename T,
	   typename T2>
  void
  MFrontBehaviourParserCommon::StandardWordAnalyser<T,T2>::exe(const std::string& v)
  {
    (this->instance.*mptr)(this->hypothesis,v);
  } // end of StandardWordAnalyser<T,T2>::exe

  template<typename T,
	   typename T2>
  MFrontBehaviourParserCommon::StandardWordAnalyser<T,T2>::~StandardWordAnalyser()
  {} // end of StandardWordAnalyser<T,T2>::~StandardWordAnalyser

  template<typename T,
	   typename T2>
  tfel::utilities::shared_ptr<MFrontBehaviourParserCommon::WordAnalyser>
  MFrontBehaviourParserCommon::makeWordAnalyser(T& i,
						const Hypothesis h,
						void (T2::* p)(const Hypothesis,
								    const std::string&))
  {
    using namespace tfel::utilities;
    return shared_ptr<WordAnalyser>(new StandardWordAnalyser<T,T2>(i,h,p));
  } // end of makeWordAnalyser

  template<typename T,
	   typename T2>
  MFrontBehaviourParserCommon::CodeBlockOptions
  MFrontBehaviourParserCommon::readCodeBlock(T& child,
					     const std::string& n,
					     std::string (T2::* m)(const Hypothesis,
								   const std::string&,
								   const bool),
					     const bool b,
					     const bool s)

  {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o,s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->readCodeBlock(child,o,n,m,b);
    return o;
  }

  template<typename T,typename T2,typename T3>
  MFrontBehaviourParserCommon::CodeBlockOptions
  MFrontBehaviourParserCommon::readCodeBlock(T& child,
					     const std::string& n,
					     std::string (T2::* m)(const Hypothesis,
								   const std::string&,
								   const bool),
					     void (T3::* a)(const Hypothesis,
							    const std::string&),
					     const bool b,
					     const bool s)

  {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o,s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->readCodeBlock(child,o,n,m,a,b);
    return o;
  } // end of MFrontBehaviourParserCommon::readCodeBlock

  template<typename T,typename T2,typename T3>
  void
  MFrontBehaviourParserCommon::readCodeBlock(T& child,
					     const MFrontBehaviourParserCommon::CodeBlockOptions& o,
					     const std::string& n,
					     std::string (T2::* m)(const Hypothesis,
								   const std::string&,
								   const bool),
					     void (T3::* a)(const Hypothesis,
							    const std::string&),
					     const bool b)

  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    const set<Hypothesis>& h = o.hypotheses;
    set<Hypothesis>::const_iterator ph;
    const TokensContainer::const_iterator beg = this->current;
    for(ph=h.begin();ph!=h.end();++ph){
      this->current = beg;
      shared_ptr<VariableModifier> vm(makeVariableModifier(child,*ph,m));
      shared_ptr<WordAnalyser>     wa(makeWordAnalyser(child,*ph,a));
      const CodeBlock& c = this->readNextBlock(b,"{","}",true,true,vm,wa);
      this->disableVariableDeclaration(*ph);
      this->mb.setCode(*ph,n,c,o.m,o.p);
    }
  } // end of MFrontBehaviourParserCommon::readCodeBlock

  template<typename T,typename T2>
  void
  MFrontBehaviourParserCommon::readCodeBlock(T& child,
					     const MFrontBehaviourParserCommon::CodeBlockOptions& o,
					     const std::string& n,
					     std::string (T2::* m)(const Hypothesis,
								   const std::string&,
								   const bool),
					     const bool b)
  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    const set<Hypothesis>& h = o.hypotheses;
    set<Hypothesis>::const_iterator ph;
    const TokensContainer::const_iterator beg = this->current;
    for(ph=h.begin();ph!=h.end();++ph){
      this->current = beg;
      shared_ptr<VariableModifier> vm(makeVariableModifier(child,*ph,m));
      const CodeBlock& c = this->readNextBlock(vm,b);
      this->disableVariableDeclaration(*ph);
      this->mb.setCode(*ph,n,c,o.m,o.p);
    }
  } // end of MFrontBehaviourParserCommon::readCodeBlock

  template<typename T,
	   typename T2>
  MFrontBehaviourParserCommon::CodeBlockOptions
  MFrontBehaviourParserCommon::readCodeBlock(T& child,
					     const std::string& n1,
					     const std::string& n2,
					     std::string (T2::* m1)(const Hypothesis,
								    const std::string&,
								    const bool),
					     std::string (T2::* m2)(const Hypothesis,
								    const std::string&,
								    const bool),
					     const bool b,
					     const bool s)

  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    CodeBlockOptions o;
    this->readCodeBlockOptions(o,s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->readCodeBlock(child,o,n1,n2,m1,m2,b);
    return o;
  } // end of MFrontBehaviourParserCommon::readCodeBlock

  template<typename T,
	   typename T2>
  void
  MFrontBehaviourParserCommon::readCodeBlock(T& child,
					     const MFrontBehaviourParserCommon::CodeBlockOptions& o,
					     const std::string& n1,
					     const std::string& n2,
					     std::string (T2::* m1)(const Hypothesis,
								    const std::string&,
								    const bool),
					     std::string (T2::* m2)(const Hypothesis,
								    const std::string&,
								    const bool),
					     const bool b)

  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    const set<Hypothesis>& h = o.hypotheses;
    set<Hypothesis>::const_iterator ph;
    const TokensContainer::const_iterator beg = this->current;
    for(ph=h.begin();ph!=h.end();++ph){
      this->current = beg;
      CodeBlock c1;
      CodeBlock c2;
      this->readNextBlock(c1,c2,makeVariableModifier(child,*ph,m1),
			  makeVariableModifier(child,*ph,m2),b);
      this->disableVariableDeclaration(*ph);
      this->mb.setCode(*ph,n1,c1,o.m,o.p);
      this->mb.setCode(*ph,n2,c2,o.m,o.p);
    }
  } // end of MFrontBehaviourParserCommon::readCodeBlock

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX */

