/*!
 * \file   ThemeManager.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 fév 2008
 */

#include<stdexcept>
#include<utility>
#include"TFEL/Graphics/ThemeManager.hxx"
#include"TFEL/Graphics/StandardGraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {

    ThemeProxy::~ThemeProxy()
    {} // end of ThemeProxy::~ThemeProxy()
      
    void
    ThemeManager::loadLibrary(const std::string& lib)
    {
      using namespace tfel::system;
      ExternalLibraryManager& lm = ExternalLibraryManager::getExternalLibraryManager();
      lm.loadLibrary(lib);
    }
      
    const ThemeManager::ThemePtr
    ThemeManager::getTheme(const std::string& name)
    {
      using namespace std;
      using namespace tfel::utilities;
      map<string,shared_ptr<const ThemeProxy> >::const_iterator p;
      p = this->proxies.find(name);
      if(p==this->proxies.end()){
	string msg("ThemeManager::getTheme : ");
	msg += "no theme named "+name+" registred.";
	throw(runtime_error(msg));
      }
      return p->second->getTheme();
    } // end of ThemeManager::getTheme

    void
    ThemeManager::addTheme(const ThemeProxy* const proxy)
    {
      using namespace std;
      using namespace tfel::utilities;
      const string& name = proxy->getName();
      map<string,shared_ptr<const ThemeProxy> >::const_iterator p;
      p = this->proxies.find(name);
      if(p!=this->proxies.end()){
	string msg("ThemeManager::getTheme : ");
	msg += "a theme named "+name+" has already been registred.";
	throw(runtime_error(msg));
      }
      this->proxies.insert(make_pair(name,shared_ptr<const ThemeProxy>(proxy)));
    } // end of ThemeManager::getTheme

    const std::vector<std::string>
    ThemeManager::getAvailableThemesNames(void) const
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<string> res;
      res.reserve(this->proxies.size());
      map<string,shared_ptr<const ThemeProxy> >::const_iterator p;
      for(p=this->proxies.begin();p!=this->proxies.end();++p){
	res.push_back(p->first);
      }
      return res;
    } // end of ThemeManager::getAvailableThemesNames(void) const

    ThemeManager&
    ThemeManager::getThemeManager()
    {
      static ThemeManager themeManager;
      return themeManager;
    } // end of ThemeManager::~ThemeManager()

    ThemeManager::ThemeManager()
    {
      this->addTheme(new StandardThemeProxy<StandardGraphTheme>("StandardGraphTheme",false));
    } // end of ThemeManager::ThemeManager()

    ThemeManager::~ThemeManager()
    {} // end of ThemeManager::~ThemeManager

  } // end of namespace graphics

} // end of namespace tfel

