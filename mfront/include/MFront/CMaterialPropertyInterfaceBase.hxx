/*!
 * \file   mfront/include/MFront/CMaterialPropertyInterfaceBase.hxx
 * \brief  This file declares the CMaterialPropertyInterfaceBase class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTCLAWINTERFACEBASE_H_
#define LIB_MFRONTCLAWINTERFACEBASE_H_

#include <string>
#include <fstream>

#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct CMaterialPropertyInterfaceBase
      : public AbstractMaterialPropertyInterface {
    CMaterialPropertyInterfaceBase();
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
                                  const FileDescription&) override;
    //! destructor
    virtual ~CMaterialPropertyInterfaceBase();

   protected:
    virtual void writeParameterList(std::ostream&,
                                    const VariableDescriptionContainer&);

    virtual void writeInterfaceSpecificVariables(
        const VariableDescriptionContainer&);

    virtual void writeBeginHeaderNamespace(void) = 0;

    virtual void writeEndHeaderNamespace(void) = 0;

    virtual void writeBeginSrcNamespace(void) = 0;

    virtual void writeEndSrcNamespace(void) = 0;

    virtual void writeHeaderPreprocessorDirectives(
        const MaterialPropertyDescription&);

    virtual void writeSrcPreprocessorDirectives(
        const MaterialPropertyDescription&);

    virtual void writeCErrorTreatment(std::ostream&,
                                      const MaterialPropertyDescription&) const;

    virtual std::string getCallingConvention(void) const;

    virtual bool requiresCheckBoundsFunction(void) const = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string getFunctionName(const std::string&,
                                        const std::string&) const = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string getCheckBoundsFunctionName(
        const std::string&, const std::string&) const = 0;

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
                                 const FileDescription&);
    /*!
     * \brief : write the source file. The srcFile member has been
     * opened appropriately and will be closed after the call.
     * \param[in] mpd : material property description
     * \param[in] fd  : file description
     */
    void writeSrcFile(const MaterialPropertyDescription&,
                      const FileDescription&);

    std::ofstream headerFile;

    std::ofstream srcFile;

    std::string headerFileName;

    std::string srcFileName;

  };  // end of MfrontCMaterialPropertyInterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONTCLAWINTERFACEBASE_H_ */
