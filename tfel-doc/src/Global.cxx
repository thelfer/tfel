/*! 
 * \file  tfel-doc/src/Global.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#include<iostream>

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"

namespace tfel{

  namespace utilities{
    
    std::map<std::string,std::map<std::string,std::string> >&
    getTypeMap(void)
    {
      using namespace std;
      static map<string,map<string,string> > t;
      return t;
    }

    std::map<std::string,std::string>&
    getTypeMap(const std::string& l)
    {
      using namespace std;
      static map<string,map<string,string> > t;
      return t[l];
    }

    std::map<std::string,std::map<std::string,std::string> >&
    getCategoriesMap(void)
    {
      using namespace std;
      static map<string,map<string,string> > c;
      return c;
    }

    std::map<std::string,std::string>&
    getCategoriesMap(const std::string& l)
    {
      using namespace std;
      return getCategoriesMap()[l];
    }

    std::map<std::string,std::map<std::string,std::string> >&
    getKeysMap(void)
    {
      using namespace std;
      static map<string,map<string,string> > k;
      return k;
    }

    std::map<std::string,std::string>&
    getKeysMap(const std::string& l)
    {
      using namespace std;
      return getKeysMap()[l];
    }

    std::map<std::string,std::map<std::string,std::string> >&
    getTranslationsMap(void)
    {
      using namespace std;
      static map<string,map<string,string> > k;
      return k;
    }

    std::map<std::string,std::string>&
    getTranslationsMap(const std::string& l)
    {
      using namespace std;
      return getTranslationsMap()[l];
    }

    std::string
    getTranslation(const std::string& s,
		   const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      const map<string,string>& translations = getTranslationsMap(l);
      map<string,string>::const_iterator pk;
      if(l=="english"){
	return s;
      }
      pk = translations.find(s);
      if(pk!=translations.end()){
	return pk->second;
      }
      cerr << TerminalColors::Red;
      cerr << "getTranslation : no translation found for '" << s
	   << "' in language '"+l+"', using default" << endl;
      cerr << TerminalColors::Reset;
      return s;
    }

    std::string
    getCategory(const std::string& s,
		const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      const map<string,string>& categories = getCategoriesMap(l);
      map<string,string>::const_iterator pk;
      pk = categories.find(s);
      if(pk!=categories.end()){
	return pk->second;
      }
      cerr << TerminalColors::Red;
      cerr << "getCategory : no category found for '" << s
	   << "' in language '"+l+"', using default" << endl;
      cerr << TerminalColors::Reset;
      return s;
    }

    std::string
    getKeyValue(const std::string& s,
		const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      const map<string,string>& keys = getKeysMap(l);
      map<string,string>::const_iterator pk;
      if(s.empty()){
	return s;
      }
      if(s[0]=='!'){
	return s.substr(1);
      }
      pk = keys.find(s);
      if(pk!=keys.end()){
	return pk->second;
      }
      cerr << TerminalColors::Red;
      cerr << "getKeyValue : no translation found for key '" << s
	   << "' in language '"+l+"', using default" << endl;
      cerr << TerminalColors::Reset;
      return s;
    }

  } // end of namespace utilities

} // end of namespace tfel
