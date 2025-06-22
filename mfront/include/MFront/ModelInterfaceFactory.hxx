/*!
 * \file   mfront/include/MFront/ModelInterfaceFactory.hxx
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

#ifndef LIB_MFRONMODELTINTERFACEFACTORY_HXX_
#define LIB_MFRONMODELTINTERFACEFACTORY_HXX_

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT ModelInterfaceFactory {
    typedef std::shared_ptr<AbstractModelInterface> (*InterfaceCreator)(void);

    static ModelInterfaceFactory& getModelInterfaceFactory();

    std::vector<std::string> getRegistredInterfaces(void) const;

    void registerInterfaceCreator(const std::string&, InterfaceCreator);
    /*!
     * \return true if the given interface exists
     * \param[in] n : interface name
     */
    bool exists(const std::string&) const;
    /*!
     * \return a newly created interface
     * \param[in] n : interface name
     */
    std::shared_ptr<AbstractModelInterface> getInterface(
        const std::string&) const;

    ~ModelInterfaceFactory();

   private:
    typedef std::map<std::string, InterfaceCreator> InterfaceCreatorsContainer;

    TFEL_VISIBILITY_LOCAL
    ModelInterfaceFactory();

    TFEL_VISIBILITY_LOCAL
    InterfaceCreatorsContainer& getInterfaceCreatorsMap(void) const;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONMODELTINTERFACEFACTORY_HXX_ */
