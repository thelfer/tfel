/*!
 * \file   mfront/include/MFront/DSLProxy.hxx
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

#ifndef LIB_MFRONTPARSERPROXY_HXX_
#define LIB_MFRONTPARSERPROXY_HXX_

#include <string>
#include <vector>
#include <memory>

#include "MFront/DSLFactory.hxx"
#include "MFront/AbstractDSL.hxx"

namespace mfront {

  template <typename DSLType>
  struct DSLProxy {
    DSLProxy();
    /*!
     * \param[in] n: alternative name for the DSL
     */
    DSLProxy(const std::string&);
    /*!
     * \param[in] names: alternative names for the DSL
     */
    DSLProxy(const std::vector<std::string>&);

    static std::shared_ptr<AbstractDSL> createParser();
  };

}  // end of namespace mfront

#include "MFront/DSLProxy.ixx"

#endif /* LIB_MFRONTPARSERPROXY_HXX_ */
