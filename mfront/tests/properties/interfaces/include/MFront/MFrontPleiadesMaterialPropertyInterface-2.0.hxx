/*!
 * \file
 * mfront/tests/properties/interfaces/include/MFront/MFrontPleiadesMaterialPropertyInterface-2.0.hxx
 * \brief  This file declares the MFrontPleiadesMaterialPropertyInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPLEIADESLAWINTERFACE_H_
#define LIB_MFRONTPLEIADESLAWINTERFACE_H_

#include <string>
#include <fstream>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct MFrontPleiadesMaterialPropertyInterface final
      : public AbstractMaterialPropertyInterface {
    MFrontPleiadesMaterialPropertyInterface();

    static std::string getName(void);
    /*!
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<
        bool,
        tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
        const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
        override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) override;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) override;

    virtual ~MFrontPleiadesMaterialPropertyInterface();

   private:
    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const std::string&, includes
     * \param const VarContainer&, inputs
     */
    void writeHeaderFile(const std::string&,
                         const std::string&,
                         const std::string&,
                         const std::string&,
                         const std::string&,
                         const std::string&,
                         const VarContainer&);

    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, output name
     * \param const VarContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const std::map<std::string,std::string>&, glossary names,
     * \param const std::map<std::string,std::string>&, entry names,
     * \param const StaticVarContainer&, static variables
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of
     * the law
     */
    void writeSrcFile(const std::string&,
                      const std::string&,
                      const std::string&,
                      const std::string&,
                      const std::string&,
                      const VarContainer&,
                      const std::vector<std::string>&,
                      const std::map<std::string, std::string>&,
                      const std::map<std::string, std::string>&,
                      const StaticVarContainer& staticVars,
                      const std::vector<std::string>&,
                      const std::map<std::string, double>&,
                      const LawFunction&,
                      const std::vector<VariableBoundsDescription>&,
                      const std::vector<VariableBoundsDescription>&);

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;
  };  // end of MfrontPleiadesMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTPLEIADESLAWINTERFACE_H */
