/*!
 * \file   tests/System/wstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<cstdlib>
#include"TFEL/System/wfstream.hxx"
#include"TFEL/System/rfstream.hxx"

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
