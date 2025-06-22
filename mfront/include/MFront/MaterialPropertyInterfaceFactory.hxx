/*!
 * \file   mfront/include/MFront/MaterialPropertyInterfaceFactory.hxx
 * \brief
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTLAWINTERFACEFACTORY_HXX_
#define LIB_MFRONTLAWINTERFACEFACTORY_HXX_

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT MaterialPropertyInterfaceFactory {
    typedef std::shared_ptr<AbstractMaterialPropertyInterface> (
        *InterfaceCreator)(void);

    static MaterialPropertyInterfaceFactory&
    getMaterialPropertyInterfaceFactory();

    std::vector<std::string> getRegistredInterfaces(void) const;

    void registerInterfaceCreator(const std::string&, InterfaceCreator);

    void registerInterfaceAlias(const std::string&, const std::string&);

    void registerInterfaceDependency(const std::string&, const std::string&);

    std::vector<std::string> getInterfaceDependencies(const std::string&) const;
    /*!
     * \return true if the given interface exists
     * \param[in] n : interface name
     */
    bool exists(const std::string&) const;
    /*!
     * \return a newly created interface
     * \param[in] n : interface name
     */
    std::shared_ptr<AbstractMaterialPropertyInterface> getInterface(
        const std::string&) const;

    ~MaterialPropertyInterfaceFactory();

   private:
    typedef std::map<std::string, std::string> AliasContainer;
    typedef std::map<std::string, InterfaceCreator> InterfaceCreatorsContainer;
    typedef std::map<std::string, std::vector<std::string>>
        InterfaceDependencyContainer;

    TFEL_VISIBILITY_LOCAL
    MaterialPropertyInterfaceFactory();

    MaterialPropertyInterfaceFactory(const MaterialPropertyInterfaceFactory&) =
        delete;
    MaterialPropertyInterfaceFactory(MaterialPropertyInterfaceFactory&&) =
        delete;
    MaterialPropertyInterfaceFactory& operator=(
        const MaterialPropertyInterfaceFactory&) = delete;
    MaterialPropertyInterfaceFactory& operator=(
        MaterialPropertyInterfaceFactory&&) = delete;

    TFEL_VISIBILITY_LOCAL
    InterfaceDependencyContainer& getDependenciesMap(void) const;

    TFEL_VISIBILITY_LOCAL
    InterfaceCreatorsContainer& getInterfaceCreatorsMap(void) const;

    TFEL_VISIBILITY_LOCAL
    AliasContainer& getAliasesMap(void) const;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONTLAWINTERFACEFACTORY_HXX_ */
