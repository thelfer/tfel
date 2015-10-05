/*!
 * \file   mtest/include/MTest/Scheme.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 oct. 2015
 */

#ifndef _LIB_MTEST_SCHEME_HXX_
#define _LIB_MTEST_SCHEME_HXX_

#include<string>

#include"TFEL/Tests/Test.hxx"
#include"MTest/Config.hxx"

namespace mtest{

  /*!
   * a scheme describe a specific kind of computation.
   */
  struct MTEST_VISIBILITY_EXPORT Scheme
    :  public tfel::tests::Test
  {
    //! desctructor
    virtual ~Scheme();
  }; // end of struct Scheme
  
} // end of namespace mtest

#endif /* _LIB_MTEST_SCHEME_HXX_ */
