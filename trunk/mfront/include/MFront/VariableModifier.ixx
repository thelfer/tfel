/*! 
 * \file  VariableModifier.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 14 févr. 2013
 */

#ifndef _LIB_MFRONT_VARIABLEMODIFIER_IXX_
#define _LIB_MFRONT_VARIABLEMODIFIER_IXX_ 

namespace mfront
{

  template<typename T,
	   typename T2>
  StandardVariableModifier<T,T2>::StandardVariableModifier(T& i,
							   const StandardVariableModifier<T,T2>::MPtr p)
    : instance(i),
      mptr(p)
  {} // end of StandardVariableModifier<T,T2>::StandardVariableModifier

  template<typename T,
	   typename T2>
  std::string
  StandardVariableModifier<T,T2>::exe(const std::string& v,
				      const bool b)
  {
    return (this->instance.*mptr)(v,b);
  } // end of StandardVariableModifier<T,T2>::exe

  template<typename T,
	   typename T2>
  StandardVariableModifier<T,T2>::~StandardVariableModifier()
  {} // end of StandardVariableModifier<T,T2>::~StandardVariableModifier

  template<typename T,
	   typename T2>
  tfel::utilities::shared_ptr<VariableModifier>
  makeVariableModifier(T& i,std::string (T2::* p)(const std::string&,const bool))
  {
    using namespace tfel::utilities;
    return shared_ptr<VariableModifier>(new StandardVariableModifier<T,T2>(i,p));
  } // end of makeVariableModifier

} // end of namespace mfront

#include"MFront/VariableModifier.ixx"

#endif /* _LIB_MFRONT_VARIABLEMODIFIER_IXX */

