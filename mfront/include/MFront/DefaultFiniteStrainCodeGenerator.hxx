/*!
 * \file   mfront/include/MFront/DefaultFiniteStrainCodeGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DEFAULTFINITESTRAINCODEGENERATOR_HXX
#define LIB_MFRONT_DEFAULTFINITESTRAINCODEGENERATOR_HXX

#include "MFront/DefaultCodeGeneratorBase.hxx"

namespace mfront {

  struct DefaultFiniteStrainCodeGenerator : DefaultCodeGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     */
    DefaultFiniteStrainCodeGenerator(const FileDescription&,
                                     const BehaviourDescription&,
                                     const BehaviourInterfaceMap&);
    //! \brief destructor
    ~DefaultFiniteStrainCodeGenerator() override;

   protected:
    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;
  };  // end of DefaultFiniteStrainCodeGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_DEFAULTFINITESTRAINCODEGENERATOR_HXX */
