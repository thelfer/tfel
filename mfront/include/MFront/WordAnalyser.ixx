/*! 
 * \file  WordAnalyser.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 14 févr. 2013
 */

#ifndef _LIB_MFRONT_WORDANALYSER_IXX_
#define _LIB_MFRONT_WORDANALYSER_IXX_ 

namespace mfront
{

  template<typename T,
	   typename T2>
  StandardWordAnalyser<T,T2>::StandardWordAnalyser(T& i,
							   const StandardWordAnalyser<T,T2>::MPtr p)
    : instance(i),
      mptr(p)
  {} // end of StandardWordAnalyser<T,T2>::StandardWordAnalyser

  template<typename T,
	   typename T2>
  void
  StandardWordAnalyser<T,T2>::exe(const std::string& v)
  {
    (this->instance.*mptr)(v);
  } // end of StandardWordAnalyser<T,T2>::exe

  template<typename T,
	   typename T2>
  StandardWordAnalyser<T,T2>::~StandardWordAnalyser()
  {} // end of StandardWordAnalyser<T,T2>::~StandardWordAnalyser

  template<typename T,
	   typename T2>
  tfel::utilities::shared_ptr<WordAnalyser>
  makeWordAnalyser(T& i,void (T2::* p)(const std::string&))
  {
    using namespace tfel::utilities;
    return shared_ptr<WordAnalyser>(new StandardWordAnalyser<T,T2>(i,p));
  } // end of makeWordAnalyser

} // end of namespace mfront

#include"MFront/WordAnalyser.ixx"

#endif /* _LIB_MFRONT_WORDANALYSER_IXX */

