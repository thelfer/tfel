/*! 
 * \file  LaTeXConvertion.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
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

