/*!
 * \file   mfront/include/MFront/OpenRadiossInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   16 mars 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_OPENRADIOSSEXPLICITINTERFACE_HXX
#define LIB_MFRONT_OPENRADIOSSEXPLICITINTERFACE_HXX

#include "MFront/GenericBehaviourInterface.hxx"

namespace mfront {

  /*!
   * Interface dedicated to the OpenRadioss finite element solver
   */
  struct OpenRadiossInterface : public GenericBehaviourInterface {
    //! \return the name of the interface
    static std::string getName();
    std::string getInterfaceName() const override;
    std::string getLibraryName(const BehaviourDescription&) const override;
    //
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    //! destructor
    ~OpenRadiossInterface() override;
  };
  
}  // end of namespace mfront

#endif /* LIB_MFRONT_OPENRADIOSSEXPLICITINTERFACE_HXX */
