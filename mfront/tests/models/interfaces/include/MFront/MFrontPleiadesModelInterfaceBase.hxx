/*!
 * \file
 * mfront/tests/models/interfaces/include/MFront/MFrontPleiadesModelInterfaceBase.hxx
 * \brief  This file declares the MFrontPleiadesModelinterface class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPLEIADESMODELINTERFACE_HXX
#define LIB_MFRONTPLEIADESMODELINTERFACE_HXX

#include <fstream>
#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  struct MFrontPleiadesModelInterfaceBase : public AbstractModelInterface {
    MFrontPleiadesModelInterfaceBase();

    virtual void declareReservedNames(std::set<std::string>&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d  : target description
     * \param[in]  md : model description
     */
    virtual void getTargetsDescription(TargetsDescription&,
                                       const ModelDescription&) override;
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual void writeOutputFiles(const FileDescription&,
                                  const ModelDescription&) override;

   protected:
    virtual void openOutputFiles();

    virtual void closeOutputFiles();

    virtual void generateOutputFiles(const FileDescription&,
                                     const ModelDescription&);

    virtual void writeHeaderFile(const FileDescription&,
                                 const ModelDescription&);

    virtual void writeSpecificPrivateMethodDeclaration(const FileDescription&,
                                                       const ModelDescription&);

    virtual void writeSrcFile(const FileDescription&, const ModelDescription&);

    virtual void writeAssignDefaultValue(const VariableDescription&);

    virtual std::string getGenTypeMethod(const std::string& type) const;

    virtual void setOutputFileNames(const ModelDescription&) = 0;

    virtual void writeGetGlobalParameter(const VariableDescription&,
                                         const ModelDescription&);

    virtual void writeGetConstantMaterialProperty(const VariableDescription&,
                                                  const ModelDescription&);

    virtual void buildDomainName() = 0;

    virtual std::string getApplicationName() const = 0;

    virtual void generateDomainsList(const ModelDescription&);

    virtual void writeFunctionCall(
        const ModelDescription&,
        const std::vector<ModelDescription::Function>&);

    virtual void writeInitializeMethod(const ModelDescription&);

    virtual void writeInitializeOutputMethod(const ModelDescription&);

    virtual void writeInitializeInputMethod(const ModelDescription&);

    virtual void writeInitializeParametersMethod(const ModelDescription&);

    virtual bool initializeDefaultDomainListInConstrutor(
        const ModelDescription&) const;

    std::set<std::string> domains;

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
    bool hasSpecializedDomain;
  };  // end of class MFrontPleiadesModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTPLEIADESMODELINTERFACE_H */
