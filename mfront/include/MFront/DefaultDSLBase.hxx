/*!
 * \file   mfront/include/MFront/DefaultDSLBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTPARSERBASE_HXX
#define LIB_MFRONT_MFRONTPARSERBASE_HXX

#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct DefaultDSLBase : public BehaviourDSLBase<DefaultDSLBase> {
    //! \brief default constructor
    DefaultDSLBase();
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! \brief default destructor
    ~DefaultDSLBase() override;

   protected:
    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;
    void completeVariableDeclaration() override;
    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;
    void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const override;
    virtual void treatProvidesTangentOperator();
    virtual void treatProvidesSymmetricTangentOperator();
  };  // end of struct DefaultDSLBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTPARSERBASE_HXX */
