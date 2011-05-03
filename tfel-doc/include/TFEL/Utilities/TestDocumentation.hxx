/*! 
 * \file  TestDocumentation.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
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
      void writeLaTexDescription(std::ostream& log,
				 const std::string& l) const;

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

