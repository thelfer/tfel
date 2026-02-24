/*!
 * \file   mfront/include/MFront/Fortran03MaterialPropertyInterface.hxx
 * \brief  This file declares the Fortran03MaterialPropertyInterface class.
 * \author Thomas Helfer
 * \date   1 décembre 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTFORTRAN03LAWINTERFACE_HXX
#define LIB_MFRONTFORTRAN03LAWINTERFACE_HXX

#include "MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  struct Fortran03MaterialPropertyInterface
      : public CMaterialPropertyInterfaceBase {
    static std::string getName();

    Fortran03MaterialPropertyInterface();
    //
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    void writeInterfaceSymbol(
        std::ostream&, const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~Fortran03MaterialPropertyInterface() override;

   private:
    void writeArgumentsList(std::ostream&,
                            const MaterialPropertyDescription&,
                            const std::string_view,
                            const bool) const override;
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

   private:
    std::string module;

  };  // end of Fortran03MaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTFORTRAN03LAWINTERFACE_HXX */
