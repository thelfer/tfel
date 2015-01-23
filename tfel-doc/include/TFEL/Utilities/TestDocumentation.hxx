/*! 
 * \file  tfel-doc/include/TFEL/Utilities/TestDocumentation.hxx
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

#ifndef _LIB_TFEL_UTILITIES_TESTDOCUMENTATION_H_
#define _LIB_TFEL_UTILITIES_TESTDOCUMENTATION_H_ 

#include<map>
#include<vector>
#include<string>
#include<ostream>

namespace tfel{

  namespace utilities {

    struct TestDocumentation
    {
      TestDocumentation() = default;
      TestDocumentation(const TestDocumentation&) = default;
      TestDocumentation(TestDocumentation&&) = default;
      TestDocumentation&
      operator=(const TestDocumentation& ) = default;
      TestDocumentation&
      operator=(TestDocumentation&&) = default;

      void writeLaTexDescription(std::ostream&,
				 const std::string&, 
				 const std::string&) const;
      //! destructor
      ~TestDocumentation();
      
      std::string name;
      std::string date;
      std::string author;
      std::string src;
      std::string install;
      std::map<std::string,
	       std::vector<std::string> > models;
      std::map<std::string,std::string> descriptions;
      std::map<std::string,
	       std::vector<std::string> > keys;
    };

  } // end of namespace utilities
  
} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TESTDOCUMENTATION_H */

