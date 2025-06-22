/*!
 * \file   mfront/include/MFront/BehaviourInterfaceFactory.hxx
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

#ifndef LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_
#define LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT BehaviourInterfaceFactory {
    typedef std::shared_ptr<AbstractBehaviourInterface> (*InterfaceCreator)(
        void);

    static BehaviourInterfaceFactory& getBehaviourInterfaceFactory();

    std::vector<std::string> getRegistredInterfaces(void) const;
    /*!
     * \return true if the given interface exists
     * \param[in] n : interface name
     */
    bool exists(const std::string&) const;
    /*!
     * \return a newly created interface
     * \param[in] n : interface name
     */
    std::shared_ptr<AbstractBehaviourInterface> getInterface(
        const std::string&);

    void registerInterfaceCreator(const std::string&, InterfaceCreator);

    void registerInterfaceAlias(const std::string&, const std::string&);

    ~BehaviourInterfaceFactory();

   private:
    typedef std::map<std::string, std::string> AliasContainer;
    typedef std::map<std::string, InterfaceCreator> InterfaceCreatorsContainer;

    TFEL_VISIBILITY_LOCAL BehaviourInterfaceFactory() = default;
    BehaviourInterfaceFactory(const BehaviourInterfaceFactory&) = delete;
    BehaviourInterfaceFactory(BehaviourInterfaceFactory&&) = delete;
    BehaviourInterfaceFactory& operator=(const BehaviourInterfaceFactory&) =
        delete;
    BehaviourInterfaceFactory& operator=(BehaviourInterfaceFactory&&) = delete;

    TFEL_VISIBILITY_LOCAL
    InterfaceCreatorsContainer& getInterfaceCreatorsMap(void) const;

    TFEL_VISIBILITY_LOCAL
    AliasContainer& getAliasesMap(void) const;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_ */
