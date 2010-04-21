/*!
 * \file   ThemeManager.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 fév 2008
 */

#ifndef _LIB_TFEL_THEMEMANAGER_HXX_
#define _LIB_TFEL_THEMEMANAGER_HXX_ 

#include<string>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"TFEL/Graphics/GraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {

    struct TFEL_VISIBILITY_EXPORT ThemeProxy
    {
      typedef tfel::utilities::shared_ptr<GraphTheme> ThemePtr;
      virtual const std::string
      getName(void) const = 0;
      virtual const ThemePtr
      getTheme(void) const = 0;
      virtual ~ThemeProxy();
    }; // end of struct ThemeProxy

    template<typename T>
    struct StandardThemeProxy
      : public ThemeProxy
    {
      StandardThemeProxy(const std::string&,
			 const bool = true);
      const std::string
      getName(void) const;
      const ThemeProxy::ThemePtr
      getTheme(void) const;
      ~StandardThemeProxy();
    private:
      const std::string name;
    }; // end of struct StandardThemeProxy

    struct TFEL_VISIBILITY_EXPORT ThemeManager
    {
      typedef tfel::utilities::shared_ptr<GraphTheme> ThemePtr;

      static ThemeManager&
      getThemeManager();

      void
      loadLibrary(const std::string&);
      
      const ThemePtr
      getTheme(const std::string&);

      void
      addTheme(const ThemeProxy * const);

      const std::vector<std::string>
      getAvailableThemesNames(void) const;

    private:

      TFEL_VISIBILITY_LOCAL 
      ThemeManager();

      TFEL_VISIBILITY_LOCAL 
      ThemeManager(const ThemeManager&);

      TFEL_VISIBILITY_LOCAL 
      ThemeManager&
      operator=(const ThemeManager&);

      TFEL_VISIBILITY_LOCAL 
      ~ThemeManager();

      std::map<std::string,tfel::utilities::shared_ptr<const ThemeProxy> > proxies;

    }; // end of struct ThemeManager

  } // end of namespace graphics

} // end of namespace tfel

#include"TFEL/Graphics/ThemeManager.ixx"

#endif /* _LIB_TFEL_THEMEMANAGER_HXX */

