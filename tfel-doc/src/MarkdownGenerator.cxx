/*! 
 * \file  tfel-doc/src/MarkdownConvertion.cxx
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

#include<fstream>
#include<iostream>

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"
#include"TFEL/Utilities/MarkdownGenerator.hxx"

namespace tfel
{

  namespace utilities
  {
    void
    printMarkdownFile(std::ostream& log,
		      const std::map<std::string,std::vector<TestDocumentation> >& tests,
		      const std::string& d,
		      const std::string& prefix,
		      const std::string& l,
		      const bool fragment,
		      const bool split)
    {
    }
    
  } // end of namespace utilities

} // end of namespace tfel

