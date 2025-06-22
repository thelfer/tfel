/*!
 * \file   mfront/include/MFront/FortranMaterialPropertyInterface.hxx
 * \brief  This file declares the FortranMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   1 décembre 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTFORTRANLAWINTERFACE_HXX
#define LIB_MFRONTFORTRANLAWINTERFACE_HXX

#include "MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  struct FortranMaterialPropertyInterface
      : public CMaterialPropertyInterfaceBase {
    static std::string getName();

    FortranMaterialPropertyInterface();
    //
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    void writeInterfaceSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~FortranMaterialPropertyInterface() override;

   private:
    void writeArgumentsList(std::ostream&,
                            const MaterialPropertyDescription&) const override;
    void writeInterfaceSpecificVariables(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeSrcPreprocessorDirectives(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeBeginHeaderNamespace(std::ostream&) const override;
    void writeEndHeaderNamespace(std::ostream&) const override;
    void writeBeginSrcNamespace(std::ostream&) const override;
    void writeEndSrcNamespace(std::ostream&) const override;
    std::string getHeaderFileName(const std::string&,
                                  const std::string&) const override;
    std::string getSrcFileName(const std::string&,
                               const std::string&) const override;
    std::string getFunctionName(
        const MaterialPropertyDescription&) const override;
    std::string getCheckBoundsFunctionName(
        const MaterialPropertyDescription&) const override;
    bool requiresCheckBoundsFunction() const override;

  };  // end of MfrontFortranMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTFORTRANLAWINTERFACE_HXX */
