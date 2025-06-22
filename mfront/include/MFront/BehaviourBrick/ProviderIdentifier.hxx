/*!
 * \file   ProviderIdentifier.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_PROVIDERIDENTIFIER_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_PROVIDERIDENTIFIER_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief available requirement provider
     */
    enum class ProviderIdentifier {
      MATERIALPROPERTY,
      //      MATERIALLAW,
      INTEGRATIONVARIABLE,
      AUXILIARYSTATEVARIABLE,
      EXTERNALSTATEVARIABLE,
      LOCALVARIABLE,
      STATICVARIABLE,
      PARAMETER
    };  // end of struct variable source

    /*!
     * \return a string associated to a provider identifier
     * \param[in] id : identifier
     */
    MFRONT_VISIBILITY_EXPORT std::string convertProviderIdentifierToString(
        const ProviderIdentifier&);

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_PROVIDERIDENTIFIER_HXX */
