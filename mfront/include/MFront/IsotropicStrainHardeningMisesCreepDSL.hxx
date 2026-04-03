/*!
 * \file   mfront/include/MFront/IsotropicStrainHardeningMisesCreepDSL.hxx
 * \brief  This file declares the IsotropicStrainHardeningMisesCreepDSL class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_HXX
#define LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_HXX

#include <string>
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  struct IsotropicStrainHardeningMisesCreepDSL
      : public IsotropicBehaviourDSLBase {
    //! \return the name of the dsl
    static std::string getName();
    //! \brief default constructor
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    IsotropicStrainHardeningMisesCreepDSL(const DSLOptions&);
    //
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    void endsInputFileProcessing() override;
    //! \brief destructor
    ~IsotropicStrainHardeningMisesCreepDSL() override;

   protected:
    std::unique_ptr<AbstractBehaviourCodeGenerator> getCodeGenerator()
        const override;
  };  // end of struct IsotropicStrainHardeningMisesCreepDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_HXX */
