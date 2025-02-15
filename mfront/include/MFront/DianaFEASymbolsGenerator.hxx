/*!
 * \file   DianaFEASymbolsGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08/01/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DIANAFEASYMBOLSGENERATOR_HXX
#define LIB_MFRONT_DIANAFEASYMBOLSGENERATOR_HXX

#include "MFront/SymbolsGenerator.hxx"

namespace mfront {

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct MFRONT_VISIBILITY_EXPORT DianaFEASymbolsGenerator
      : public SymbolsGenerator {
    //! constructor
    DianaFEASymbolsGenerator();
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
    bool handleStrainMeasure() const override;
    //! destructor
    ~DianaFEASymbolsGenerator() override;
  };  // end of struct DianaFEASymbolsGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_DIANAFEASYMBOLSGENERATOR_HXX */
