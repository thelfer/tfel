/*!
 * \file   StaticAssert.hxx
 * \brief  This file defines the TFEL_STATIC_ASSERT macro.
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_STATICASSERT_H_
#define _LIB_TFEL_STATICASSERT_H_ 

#include"TFEL/Macros.hxx"
#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    namespace internals{

      /*!
       * An helper class. Only its specialisation 
       * for true is defined.
       * \author Helfer Thomas
       * \date   28 Aug 2006
       */
      template<bool>
      struct StaticTest;
      
      /*!
       * Partial specialisation of StaticTest
       * \author Helfer Thomas
       * \date   28 Aug 2006
       */
      template<>
      struct TFEL_VISIBILITY_LOCAL StaticTest<true>{};
      
      /*!
       * A do nothing class
       * \author Helfer Thomas
       * \date   28 Aug 2006
       */
      template<unsigned int N>
      struct TFEL_VISIBILITY_LOCAL Dummy{};

    }// end of namespace internals    

  } // end of namespace meta

} // end of namespace tfel

#ifndef TFEL_NOSTATICDEBUG 
/*!
 * \def TFEL_STATIC_ASSERT
 * \brief Causes an compilation error if its arguments is not true.
 *
 * This macro was taken from the boost library (http://boost.org/).
 * See http://www.boost.org/doc/html/boost_staticassert.html for
 *  more details about it. The following was extracted from this refercene:
 * \latexonly\begin{quote}\endlatexonly 
 * TFEL_STATIC_ASSERT(x) generates a compile time error message if the 
 * integral-constant-expression x is not true. In other words it
 * is the compile time equivalent of the assert macro; this is
 * sometimes known as a "compile-time-assertion", but will be
 * called a "static assertion" throughout these docs. Note that
 * if the condition is true, then the macro will generate neither
 * code nor data - and the macro can also be used at either namespace,
 * class or function scope. When used in a template, the static
 * assertion will be evaluated at the time the template is
 * instantiated; 
 * this is particularly useful for validating template parameters.
 *
 * You can use TFEL_STATIC_ASSERT at any place where you can place a
 * declaration,that is at class, function or namespace scope.
 * \latexonly\end{quote}\endlatexonly
 *
 * This macro is disabled if the flag NOSTATICDEBUG defined.
 * \see NOSTATICDEBUG.
 *
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */
#define TFEL_STATIC_ASSERT(x) typedef tfel::meta::internals::Dummy<sizeof(tfel::meta::internals::StaticTest<static_cast<bool>(x)>)> TFEL_PP_JOIN(static_assert_typedef_,__LINE__)
#else
#define TFEL_STATIC_ASSERT(x)
#endif

#endif /* _LIB_TFEL_STATICASSERT_H */

