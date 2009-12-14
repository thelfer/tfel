/*!
 * \file   wstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 */

#include<iostream>
#include<cstdlib>
#include"System/wfstream.hxx"
#include"System/rfstream.hxx"

int
main(void)
{
  using namespace std;

  tfel::system::wfstream f("test.bin");
  f << "toto" << 12.;
  f << "tutu";
  f.close();

  return EXIT_SUCCESS;
}
