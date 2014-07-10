/*! 
 * \file  Global.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#ifndef _LIB_TFEL_UTILITIES_GLOBAL_H_
#define _LIB_TFEL_UTILITIES_GLOBAL_H_ 

#include<map>
#include<string>

namespace tfel{

  namespace utilities{
    
    std::map<std::string,std::map<std::string,std::string> >&
    getTypeMap(void);
    
    std::map<std::string,std::string>&
    getTypeMap(const std::string& l);
    
    std::map<std::string,std::map<std::string,std::string> >&
    getCategoriesMap(void);
    
    std::map<std::string,std::string>&
    getCategoriesMap(const std::string& l);
    
    std::map<std::string,std::map<std::string,std::string> >&
    getKeysMap(void);
    
    std::map<std::string,std::string>&
    getKeysMap(const std::string& l);
    
    std::map<std::string,std::map<std::string,std::string> >&
    getTranslationsMap(void);
    
    std::map<std::string,std::string>&
    getTranslationsMap(const std::string& l);
    
    std::string
    getTranslation(const std::string& s,
		   const std::string& l);
    
    std::string
    getKeyValue(const std::string&,
		const std::string&);

    std::string
    getCategory(const std::string&,
		const std::string&);

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_GLOBAL_H */

