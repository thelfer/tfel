/*!
 * \file   mfront/include/MFront/CMaterialPropertyInterfaceBase.hxx
 * \brief  This file declares the CMaterialPropertyInterfaceBase class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTCLAWINTERFACEBASE_HXX
#define LIB_MFRONTCLAWINTERFACEBASE_HXX

#include <string>
#include <fstream>
#include <string_view>

#include "MFront/LawFunction.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  /*!
   * \brief Base class for a set of material properties interface.
   */
  struct CMaterialPropertyInterfaceBase
      : public AbstractMaterialPropertyInterface {
    /*!
     * \return the position of the variable in the inputs plus one.
     * \param[in] mpd: material property description
     * \param[in] n:   variable name
     */
    static VariableDescriptionContainer::size_type getVariableNumber(
        const MaterialPropertyDescription&, const std::string&);
    //! constructor
    CMaterialPropertyInterfaceBase();
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    //! destructor
    ~CMaterialPropertyInterfaceBase() override;

   protected:
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeBeginHeaderNamespace(std::ostream&) const = 0;
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeEndHeaderNamespace(std::ostream&) const = 0;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeInterfaceSymbol(
        std::ostream&, const MaterialPropertyDescription&) const = 0;
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeBeginSrcNamespace(std::ostream&) const = 0;
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeEndSrcNamespace(std::ostream&) const = 0;
    //
    virtual bool requiresCheckBoundsFunction() const = 0;
    /*!
     * \return the name of the generated function.
     * \param[in] mpd: material property description
     */
    virtual std::string getFunctionName(
        const MaterialPropertyDescription&) const = 0;
    /*!
     * \param[in] mpd: material property description
     */
    virtual std::string getCheckBoundsFunctionName(
        const MaterialPropertyDescription&) const = 0;
    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string getHeaderFileName(const std::string&,
                                          const std::string&) const = 0;
    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string getSrcFileName(const std::string&,
                                       const std::string&) const = 0;
    /*!
     * \return the list of supported floating-point types. Supported
     * floating-point types are `float`, `double` and `long double`. For each
     * type one overload (or two if quantities are supported) will be generated.
     *
     * The default implementation only returns `double`.
     */
    virtual std::vector<std::string> getSupportedFloatingPointTypes() const;
    /*!
     * \brief if the material property supports quantities, this methods
     * returns if an overloaded function supporting quantities shall be
     * generated.
     *
     * The default implementation of this method returns `false`.
     */
    virtual bool shallGenerateOverloadedFunctionForQuantities() const;
    /*!
     * \brief write the list of arguments
     * \param[out] os: output stream
     * \param[in] mpd: material property description
     * \param[in] t: floating-point type
     * \param[in] b: boolean stating if quantities must be used
     */
    virtual void writeArgumentsList(std::ostream&,
                                    const MaterialPropertyDescription&,
                                    const std::string_view,
                                    const bool) const;
    /*!
     * \brief write the body of the material property function
     * \param[out] os: output stream
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] floating_point_type: floating-point type
     * \param[in] use_qt: use_quantities on input/outputs
     */
    virtual void writeMaterialPropertyBody(std::ostream&,
                                           const MaterialPropertyDescription&,
                                           const FileDescription&,
                                           const std::string_view,
                                           const bool) const;
    /*!
     * \brief write the body of the check bounds function
     * \param[out] os: output stream
     * \param[in] mpd: material property description
     * \param[in] floating_point_type: floating-point type
     */
    virtual void writeMaterialPropertyCheckBoundsBody(
        std::ostream&,
        const MaterialPropertyDescription&,
        const std::string_view) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in] mpd : material property description
     */
    virtual void writeInterfaceSpecificVariables(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[out] mpd: material property description
     * \param[out] mpd: file description
     */
    virtual void writeFileDescriptionSymbols(std::ostream&,
                                             const MaterialPropertyDescription&,
                                             const FileDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[out] mpd: material property description
     */
    virtual void writeBuildIdentifierSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeEntryPointSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeTFELVersionSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeUnitSystemSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeLawSymbol(std::ostream&,
                                const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeMaterialSymbol(std::ostream&,
                                     const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeMaterialKnowledgeTypeSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  name: name of the generated material property
     * \param[in]  mpd: material property description
     */
    virtual void writeVariablesNamesSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  name: name of the generated material property
     * \param[in]  mpd: material property description
     */
    virtual void writeVariablesBoundsSymbols(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeHeaderPreprocessorDirectives(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeSrcPreprocessorDirectives(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     * \param[in] floating_point_type: floating-point type
     * \param[in] use_qt: use quantities
     */
    virtual void writeCErrorTreatment(std::ostream&,
                                      const MaterialPropertyDescription&,
                                      const std::string_view,
                                      const bool) const;

    /*!
     * \brief write the `try` statement before the material property body
     * \param[out] os:  output file stream
     */
    virtual void writeCxxTryBlock(std::ostream&) const;
    /*!
     * \brief write catch blocks before the material property body
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     * \param[in] floating_point_type: floating-point type
     * \param[in] use_qt: use quantities
     */
    virtual void writeCxxCatchBlock(std::ostream&,
                                    const MaterialPropertyDescription&,
                                    const std::string_view,
                                    const bool) const;
    /*!
     * \brief write catch blocks before the material property body
     * \param[out] os:  output file stream
     * \param[in] msh: code returning the error message (a string must be
     * escaped) \param[in] mpd: material property description \param[in]
     * floating_point_type: floating-point type \param[in] use_qt: use
     * quantities
     */
    virtual void returnInvalidValue(std::ostream&,
                                    std::string_view,
                                    const MaterialPropertyDescription&,
                                    const std::string_view,
                                    const bool) const;
    //
    virtual std::string getCallingConvention() const;
    /*!
     * \return the name used to generate the symbols associated with the
     * material property.
     * \param[in] mpd: material property description
     */
    virtual std::string getSymbolName(const MaterialPropertyDescription&) const;
    /*!
     * \brief : write the source file. The headerFile member has been
     * opened appropriately and will be closed after the call.
     * \param[in] mpd : material property description
     * \param[in] fd  : file description
     */
    virtual void writeHeaderFile(const MaterialPropertyDescription&,
                                 const FileDescription&) const;
    /*!
     * \brief : write the source file. The srcFile member has been
     * opened appropriately and will be closed after the call.
     * \param[in] mpd : material property description
     * \param[in] fd  : file description
     */
    virtual void writeSrcFile(const MaterialPropertyDescription&,
                              const FileDescription&) const;

  };  // end of MfrontCMaterialPropertyInterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONTCLAWINTERFACEBASE_HXX */
