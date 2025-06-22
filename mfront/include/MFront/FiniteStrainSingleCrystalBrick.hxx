/*!
 * \file   FiniteStrainSingleCrystalBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   October,20 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_FINITESTRAINSINGLECRYSTALBRICK_HXX
#define LIB_MFRONT_FINITESTRAINSINGLECRYSTALBRICK_HXX

#include "MFront/BehaviourBrickBase.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct LocalDataStructure;

  /*!
   */
  struct FiniteStrainSingleCrystalBrick : public BehaviourBrickBase {
    static const char* const shiftedDeformationGradientOption;
    static const char* const shiftedDeformationGradientAttribute;
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     */
    FiniteStrainSingleCrystalBrick(AbstractBehaviourDSL&,
                                   BehaviourDescription&);
    BehaviourBrickDescription getDescription() const override;
    std::vector<bbrick::OptionDescription> getOptions(
        const bool) const override;
    std::string getName() const override;
    void initialize(const Parameters&, const DataMap&) override;
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    void completeVariableDeclaration() const override;
    void endTreatment() const override;
    //! destructor
    ~FiniteStrainSingleCrystalBrick() override;
  };  // end of struct FiniteStrainSingleCrystalBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_FINITESTRAINSINGLECRYSTALBRICK_H */
