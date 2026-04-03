/*!
 * \file   mfront/include/MFront/IsotropicMisesCreepDSL.hxx
 * \brief  This file declares the IsotropicMisesCreepDSL class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICMISESCREEPPARSER_HXX
#define LIB_MFRONTISOTROPICMISESCREEPPARSER_HXX

#include <string>
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  struct IsotropicMisesCreepDSL : public IsotropicBehaviourDSLBase {
    //! \brief return the name of the DSL
    static std::string getName();
    //! \brief return a short description of the DSL
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    IsotropicMisesCreepDSL(const DSLOptions&);
    //
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    void endsInputFileProcessing() override;
    //! \brief destructor
    ~IsotropicMisesCreepDSL() override;

   protected:
    bool handleStrainHardening() const override;
    //
    std::unique_ptr<AbstractBehaviourCodeGenerator> getCodeGenerator()
        const override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONTISOTROPICMISESCREEPPARSER_HXX */
