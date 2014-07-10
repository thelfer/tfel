/*! 
 * \file  tfel-doc.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#include<cstdlib>

#include"TFEL/Utilities/TestDocMain.hxx"

int
main(const int argc,
     const char* const * const argv)
{
  using namespace tfel::utilities;
  TestDocMain m(argc,argv);
  return m.execute();
} // end of main

