/*!
 * \file   mfront/tests/models/interfaces/include/MFront/LicosModelInterface.hxx
 * \brief  This file declares the MFrontModelInterface class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMODELINTERFACE_HXX
#define LIB_MFRONTMODELINTERFACE_HXX

#include <fstream>
#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  struct MFrontModelInterface final : public AbstractModelInterface {
    static std::string getName();

    MFrontModelInterface();

    ~MFrontModelInterface() override;

    /*!
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;

    void declareReservedNames(std::set<std::string>&) override;

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void writeOutputFiles(const FileDescription&,
                          const ModelDescription&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d  : target description
     * \param[in]  md : model description
     */
    void getTargetsDescription(TargetsDescription&,
                               const ModelDescription&) override;

   private:
    virtual void openOutputFiles();

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual void generateOutputFiles(const FileDescription&,
                                     const ModelDescription&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual void writeHeaderFile(const FileDescription&,
                                 const ModelDescription&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual void writeSrcFile(const FileDescription&, const ModelDescription&);

    void writeInitializeIntputsVariablesInitialValues(const ModelDescription&);

    void writeInitializeConstantMaterialProperties(const ModelDescription&);

    void writeInitializeInputsVariablesDepths(const ModelDescription&);

    void writeInitializeOutputsVariablesInitialValues(const ModelDescription&);

    void writeInitializeOutputsVariablesDepths(const ModelDescription&);

    void writeStaticVariableInitialization(const FileDescription&,
                                           const ModelDescription&,
                                           const StaticVariableDescription&);

    void writeAssignDefaultValue(const VariableDescription&);

    void closeOutputFiles();

    static VariableDescriptionContainer::const_iterator findVariableDescription(
        const VarContainer&, const std::string&);

    std::string getGenTypeGetMethod(const std::string&) const;

    std::string getGenTypeIsMethod(const std::string&) const;

   private:
    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
  };  // end of class MFrontModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTMODELINTERFACE_H */
