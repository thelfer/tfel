/*!
 * \file   AsterSymbolsGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTERSYMBOLSGENERATOR_HXX
#define LIB_MFRONT_ASTERSYMBOLSGENERATOR_HXX

#include "MFront/SymbolsGenerator.hxx"

namespace mfront {

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct MFRONT_VISIBILITY_EXPORT AsterSymbolsGenerator
      : public SymbolsGenerator {
    //! constructor
    AsterSymbolsGenerator();
    void writeSpecificSymbols(std::ostream&,
                              const BehaviourInterfaceBase&,
                              const BehaviourDescription&,
                              const FileDescription&,
                              const std::string&) const override;
    void writeBehaviourTypeSymbols(std::ostream&,
                                   const BehaviourInterfaceBase&,
                                   const BehaviourDescription&,
                                   const std::string&) const override;
    void writeBehaviourKinematicSymbols(std::ostream&,
                                        const BehaviourInterfaceBase&,
                                        const BehaviourDescription&,
                                        const std::string&) const override;
    void writeAdditionalSymbols(std::ostream&,
                                const BehaviourInterfaceBase&,
                                const BehaviourDescription&,
                                const FileDescription&,
                                const std::string&,
                                const Hypothesis) const override;
    bool handleStrainMeasure() const override;
    //! destructor
    ~AsterSymbolsGenerator() override;
  };  // end of struct AsterSymbolsGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_ASTERSYMBOLSGENERATOR_HXX */
