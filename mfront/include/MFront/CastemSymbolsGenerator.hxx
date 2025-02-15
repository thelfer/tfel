/*!
 * \file   CastemSymbolsGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMSYMBOLSGENERATOR_HXX
#define LIB_MFRONT_CASTEMSYMBOLSGENERATOR_HXX

#include "MFront/SymbolsGenerator.hxx"

namespace mfront {

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct MFRONT_VISIBILITY_EXPORT CastemSymbolsGenerator
      : public SymbolsGenerator {
    //! constructor
    CastemSymbolsGenerator();
    void writeMainVariablesSymbols(std::ostream &,
                                   const BehaviourInterfaceBase &,
                                   const BehaviourDescription &,
                                   const std::string &) const override;
    void writeBehaviourTypeSymbols(std::ostream &,
                                   const BehaviourInterfaceBase &,
                                   const BehaviourDescription &,
                                   const std::string &) const override;
    void writeBehaviourKinematicSymbols(std::ostream &,
                                        const BehaviourInterfaceBase &,
                                        const BehaviourDescription &,
                                        const std::string &) const override;
    void writeAdditionalSymbols(std::ostream &,
                                const BehaviourInterfaceBase &,
                                const BehaviourDescription &,
                                const FileDescription &,
                                const std::string &,
                                const Hypothesis) const override;
    void writeSpecificSymbols(std::ostream &,
                              const BehaviourInterfaceBase &,
                              const BehaviourDescription &,
                              const FileDescription &,
                              const std::string &) const override;
    bool handleStrainMeasure() const override;
    //! destructor
    ~CastemSymbolsGenerator() override;
  };  // end of struct CastemSymbolsGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_CASTEMSYMBOLSGENERATOR_HXX */
