/*!
 * \file   IsConvertibleTo.hxx
 * \brief  This file implements the IsConvertibleTo metafunction.  
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_ISCONVERTIBLETO_H_
#define _LIB_TFEL_ISCONVERTIBLETO_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \class IsConvertibleTo
     * \brief  The class IsConvertibleTo test if a given 
     * class T is convertible to a type U.
     * This idea of this class was found in 
     * Andrei Alexandrescu's book
     * \latexonly\cite{alexandrescu01:_c_desig}\endlatexonly
     * \htmlonly, modern C++ Design : generic programming 
     * and design patterns applied\endhtmlonly.
     *
     * \param T, type to be tested.
     * \param U, we want to know if T is convertible to U.
     * \return bool, true if T is convertible to U, false otherwise.
     *
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T, typename U>
    class TFEL_VISIBILITY_LOCAL IsConvertibleTo{
      /*!
       * \brief A first type.
       */
      typedef char Small;
      /*!
       * \brief A Second type which size is higher than Small
       */
      class TFEL_VISIBILITY_LOCAL Big{Small dummy[2];};
      /*!
       * \brief Declaration of function Test for every class 
       * convertible to U. Returns a Small.
       */
      static Small Test(const U&);
      /*!
       * \brief Declaration of function Test for every class that 
       * is not convertible to U. Returns a Big. It uses ellipsis, 
       * which makes this function the last choice for the the compiler. 
       */
      static Big  Test(...);
      /*!
       * \brief A function returning a T.
       * A small trick for classes that are not 
       * default constructible.
       */
      static T MakeT(void);
    public:
      /*!
       * \brief the result of the metafunction. 
       * True if T is convertible to U, false otherwise.
       */
      static const bool cond = (sizeof(Test(MakeT()))==sizeof(Small));
      /*!
       * \brief An helper function to get the result. 
       *  This was mostly - in fact only - introduced to get rid 
       * of a g++ warning saying that all members functions of
       * IsConvertibleTo are private...
       * \param void, no input.
       * \return cond, the result of the metafunction.
       */
      static 
      bool 
      getCond()
      {
	return cond;
      }
    };

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_ISCONVERTIBLETO_H */

