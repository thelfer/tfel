/*!
 * \file   include/TFEL/Math/General/RunTimeCheck.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_RUNTIMECHECK_H_
#define _LIB_TFEL_RUNTIMECHECK_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/MathException.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel{

  namespace math{

    /*
     * \class RunTimeCheck
     * \brief Type used to compare two runtime properties.
     * \param RunTimeProperties, type of twe runtime properties used.
     */    
    template<typename RunTimeProperties>
    struct RunTimeCheck
    {
      
      /*
       * \brief Check if two runtime properties matches.
       * This is the default implementation which takes two arguments,
       * and checks if they are equals. If they are, return a copy of
       * the first one. If not, throws an exception.
       * \param const RunTimeProperties, first property.
       * \param const RunTimeProperties, second property.
       * \return const RunTimeProperties.
       * \require fdq
       * \pre RunTimeProperties must be comparable.
       * \pre RunTimeProperties must be copy-able.
       * \throw MathRunTimeExpection, if test fails.
       */
      TFEL_MATH_INLINE
      static RunTimeProperties 
      exe(const RunTimeProperties& a,
	  const RunTimeProperties& b)
      {
	if(a!=b){
	  throw(MathRunTimeException("RunTimeProperties exception : RunTimeProperties does not match"));
	}
	return a;
      } // end of exe.
      
    }; // end of RunTimeCheck.

    /*
     * Partial Specialisation for EmptyRunTimeProperties.
     * \see RunTimeCheck.
     * \see EmptyRunTimeProperties.
     */
    template<>
    struct RunTimeCheck<EmptyRunTimeProperties>
    {

      /*
       * A do nothing function.
       * \param const EmptyRunTimeProperties, first property  (not used).
       * \param const EmptyRunTimeProperties, second property (not used).
       * \return const EmptyRunTimeProperties.
       */
      TFEL_MATH_INLINE
      static const EmptyRunTimeProperties 
      exe(const EmptyRunTimeProperties,const EmptyRunTimeProperties)
      {
	return EmptyRunTimeProperties();
      } // end of exe
      
    }; // end of RunTimeCheck<EmptyRunTimeProperties>

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_RUNTIMECHECK_H */

