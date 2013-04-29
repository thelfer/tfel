/*! 
 * \file  MTestDocParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 avril 2013
 */

#include"TFEL/Utilities/MTestDocParser.hxx"

namespace tfel{

  namespace utilities {
  
    MTestDocParser::MTestDocParser(const std::string& f)
      : MTest(f)
    {} // end of MTestDocParser::MTestDocParser
    
    void
    MTestDocParser::execute(std::map<std::string,std::vector<TestDocumentation> >&)
    {} // end of MTestDocParser::execute

  } // end of namespace utilities

} // end of namespace tfel
