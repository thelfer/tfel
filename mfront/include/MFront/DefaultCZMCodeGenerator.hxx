/*!
 * \file   mfront/include/MFront/DefaultCZMCodeGenerator.hxx
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

#ifndef LIB_MFRONT_DEFAULTCZMCODEGENERATOR_HXX
#define LIB_MFRONT_DEFAULTCZMCODEGENERATOR_HXX

#include "MFront/DefaultCodeGeneratorBase.hxx"

namespace mfront {

  struct DefaultCZMCodeGenerator : DefaultCodeGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     */
    DefaultCZMCodeGenerator(const FileDescription&,
                            const BehaviourDescription&,
                            const BehaviourInterfaceMap&);
    //! \brief destructor
    ~DefaultCZMCodeGenerator() override;

   protected:
    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;
    std::string getLocalVariablesInitializers(const Hypothesis) const override;

  };  // end of DefaultCZMCodeGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_DEFAULTCZMCODEGENERATOR_HXX */
