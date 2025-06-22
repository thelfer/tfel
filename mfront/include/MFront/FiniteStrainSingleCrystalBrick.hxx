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
     * \param[in] p    : parameters
     * \param[in] p    : data
     */
    FiniteStrainSingleCrystalBrick(AbstractBehaviourDSL&,
                                   BehaviourDescription&,
                                   const Parameters&,
                                   const DataMap&);
    //! \return the name of the brick
    virtual std::string getName() const override;
    //! \return the list of supported modelling hypotheses.
    virtual std::vector<Hypothesis> getSupportedModellingHypotheses()
        const override;
    //! complete the variable description
    virtual void completeVariableDeclaration() const override;
    //! method called at the end of the input file processing
    virtual void endTreatment() const override;
    //! destructor
    virtual ~FiniteStrainSingleCrystalBrick();
  };  // end of struct FiniteStrainSingleCrystalBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_FINITESTRAINSINGLECRYSTALBRICK_H */
