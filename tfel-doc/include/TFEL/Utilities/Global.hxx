/*!
 * \file  tfel-doc/include/TFEL/Utilities/Global.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_GLOBAL_HXX
#define LIB_TFEL_UTILITIES_GLOBAL_HXX

#include <map>
#include <string>

namespace tfel::utilities {

  std::map<std::string, std::map<std::string, std::string>>& getTypeMap();

  std::map<std::string, std::string>& getTypeMap(const std::string& l);

  std::map<std::string, std::map<std::string, std::string>>& getCategoriesMap();

  std::map<std::string, std::string>& getCategoriesMap(const std::string& l);

  std::map<std::string, std::map<std::string, std::string>>& getKeysMap();

  std::map<std::string, std::string>& getKeysMap(const std::string& l);

  std::map<std::string, std::map<std::string, std::string>>&
  getTranslationsMap();

  std::map<std::string, std::string>& getTranslationsMap(const std::string& l);

  std::string getTranslation(const std::string& s, const std::string& l);

  std::string getKeyValue(const std::string&, const std::string&);

  std::string getCategory(const std::string&, const std::string&);

  std::string capitalize(const std::string&);

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_GLOBAL_H */
