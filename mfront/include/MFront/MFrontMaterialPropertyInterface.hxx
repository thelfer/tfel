/*!
 * \file   mfront/include/MFront/MFrontMaterialPropertyInterface.hxx
 * \brief  This file declares the MFrontMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTLAWINTERFACE_HXX
#define LIB_MFRONT_MFRONTLAWINTERFACE_HXX

#include "MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  struct MFrontMaterialPropertyInterface
      : public CMaterialPropertyInterfaceBase {
    //
    static std::string getName();
    // \brief constructor
    MFrontMaterialPropertyInterface();
    //
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    std::string getFunctionName(
        const MaterialPropertyDescription&) const override;
    std::string getHeaderFileName(const std::string&,
                                  const std::string&) const override;
    void writeLawSymbol(std::ostream&,
                        const MaterialPropertyDescription&) const override;
    void writeMaterialSymbol(std::ostream&,
                             const MaterialPropertyDescription&) const override;
    void writeInterfaceSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~MFrontMaterialPropertyInterface() override;

   private:
    //
    std::vector<std::string> getSupportedFloatingPointTypes() const override;
    bool shallGenerateOverloadedFunctionForQuantities() const override;
    void writeBeginHeaderNamespace(std::ostream&) const override;
    void writeEndHeaderNamespace(std::ostream&) const override;
    void writeBeginSrcNamespace(std::ostream&) const override;
    void writeEndSrcNamespace(std::ostream&) const override;
    std::string getSrcFileName(const std::string&,
                               const std::string&) const override;
    std::string getCheckBoundsFunctionName(
        const MaterialPropertyDescription&) const override;
    void writeFileDescriptionSymbols(std::ostream&,
                                     const MaterialPropertyDescription&,
                                     const FileDescription&) const override;
    void writeBuildIdentifierSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeEntryPointSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeTFELVersionSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeUnitSystemSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeMaterialKnowledgeTypeSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeVariablesNamesSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    void writeVariablesBoundsSymbols(
        std::ostream&, const MaterialPropertyDescription&) const override;
    bool requiresCheckBoundsFunction() const override;

  };  // end of MFrontMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTLAWINTERFACE_HXX */
