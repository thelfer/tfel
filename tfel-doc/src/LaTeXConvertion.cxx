/*! 
 * \file  tfel-doc/src/LaTeXConvertion.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Utilities/LaTeXConvertion.hxx"

namespace tfel
{

  namespace utilities
  {

    std::string
    LaTeXConvertion::capitalize(const std::string& s)
    {
      using namespace std;
      string r(s);
      if(r[0]=='é'){
	r[0]='É';
      } else if(r[0]=='è'){
	r[0]='È';
      } else if(r[0]=='ê'){
	r[0]='Ê';
      } else if(r[0]=='à'){
	r[0]='À';
      } else if(r[0]=='ç'){
	r[0]='Ç';
      } else {
	r[0] = static_cast<char>(toupper(r[0]));
      }
      return r;
    } // end of capitalize

  } // end of namespace utilities

} // end of namespace tfel

