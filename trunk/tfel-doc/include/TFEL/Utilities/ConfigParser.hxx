/*! 
 * \file  ConfigParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#ifndef _LIB_TFEL_UTILITIES_CONFIGPARSER_H_
#define _LIB_TFEL_UTILITIES_CONFIGPARSER_H_ 

#include<map>

#include"TFEL/Utilities/ParserBase.hxx"

namespace tfel{

  namespace utilities {

    struct ConfigParser
      : public ParserBase
    {
      /*!
       * \param m : configuration to be read
       * \param f : file name
       */
      void execute(std::map<std::string,std::map<std::string,std::string> >&,
		   const std::string&);
      
    }; // end of struct ConfigParser

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_CONFIGPARSER_H */

