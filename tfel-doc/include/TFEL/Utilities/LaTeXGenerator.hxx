/*! 
 * \file  tfel-doc/include/TFEL/Utilities/LaTeXConvertion.hxx
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

#ifndef _LIB_TFEL_UTILITIES_LATEXCONVERTION_H_
#define _LIB_TFEL_UTILITIES_LATEXCONVERTION_H_ 

#include<map>
#include<vector>
#include<string>
#include<iosfwd>

namespace tfel
{

  namespace utilities
  {

    // forward declaration
    struct TestDocumentation;

    /*!
     * generate ofa LaTeX documentation
     */
    void
    printLaTeXFile(std::ostream&,
		   const std::map<std::string,std::vector<TestDocumentation>>&,
		   const std::string&,
		   const std::string&,
		   const std::string&,
		   const bool,
		   const bool);

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_LATEXCONVERTION_H */

