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
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
                                  const FileDescription&) const override;
    //! destructor
    virtual ~CMaterialPropertyInterfaceBase();

   protected:
    virtual void writeParameterList(std::ostream&,
                                    const VariableDescriptionContainer&) const;
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeInterfaceSpecificVariables(
        std::ostream&, const VariableDescriptionContainer&) const;
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
    virtual void writeMaterialSymbol(std::ostream&,
                                     const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeInterfaceSymbol(
        std::ostream&, const MaterialPropertyDescription&) const = 0;
    /*!
     * \param[out] os:  output file stream
     * \param[in]  mpd: material property description
     */
    virtual void writeMaterialKnowledgeTypeSymbol(
        std::ostream&, const MaterialPropertyDescription&) const;
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeBeginSrcNamespace(std::ostream&) const = 0;
    /*!
     * \param[out] os:  output file stream
     */
    virtual void writeEndSrcNamespace(std::ostream&) const = 0;
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
     */
    virtual void writeCErrorTreatment(std::ostream&,
                                      const MaterialPropertyDescription&) const;

    virtual std::string getCallingConvention() const;

    virtual bool requiresCheckBoundsFunction() const = 0;
    /*!
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
