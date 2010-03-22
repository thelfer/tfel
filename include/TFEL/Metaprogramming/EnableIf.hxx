/*!
 * \file   EnableIf.hxx
 * \brief  This file implements the EnableIf template class.
 * 
 * \author Helfer Thomas
 * \date   19 aoû 2006
 */

#ifndef _LIB_TFEL_ENABLE_IF_HXX_
#define _LIB_TFEL_ENABLE_IF_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{
    
    /*!
     * \class EnableIf
     * \param bool, condition to be checked.
     * \param typename T, type returned if condition is true.
     * \return T, if the condition is true
     *
     * EnableIf is one of the central part of TFEL. 
     * It enables template functions or template function 
     * members to be part of the overload resolution set or not, 
     * depending if the condition holds true or not. To do this, 
     * EnableIf is only defined if the condition is true.
     * If it is false, it is only declared. So, for all types T,
     * EnableIf<true,T>::type returns T while EnableIf<false,T>::type 
     * is not defined. This enables to impose constraints on 
     * template parameters. For example, the following function will 
     * only be defined is its paramater is a real :
     * \code
     * template<typename T>
     * typename EnableIf<
     *   IsReal<T>::cond,
     *	 void
     * >::type
     * f(const T);
     * \endcode
     *
     * EnableIf heavily relies on the SFINAE 
     * (substitution-failure-is-not-an-error) principle
     * \latexonly\cite{vandevoorde02:_c_templ}\endlatexonly: 
     * if an invalid argument or return type is formed during 
     * the instantiation of a function template, the instantiation
     * is removed from the overload resolution set instead of causing
     * a compilation error. More details about SFINAE may be found on 
     * wikipedia:  http://en.wikipedia.org/wiki/Substitution_Failure_Is_Not_An_Error.
     *
     *
     *
     * EnableIf design was greatly inspired by the boost libray:
     *     http://www.boost.org/libs/utility/enable_if.html
     *
     * \latexonly
     * EnableIf was discussed in length in the 
     * literature\cite{jaakko03:_funct,jaakko03:_concep,abrahams04:_template_metaprogramming}.
     * \endlatexonly
     *
     * \author Helfer Thomas
     * \date   19 aoû 2006
     */
    template<bool c, typename T>
    struct EnableIf;

    /*!
     * Partial specialisation of cond is true.
     */    
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL EnableIf<true,T>{
      /*!
       * Returned type
       */
      typedef T type;
    };
    
  } // end of meta namespace

} // end of TFEL namespace
  

#endif /* _LIB_TFEL_ENABLE_IF_HXX */

