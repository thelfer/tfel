/*!
 * \file   DDIF2Brick.hxx
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

#ifndef LIB_MFRONT_DDIF2BRICKBEHAVIOURBRICK_HXX
#define LIB_MFRONT_DDIF2BRICKBEHAVIOURBRICK_HXX

#include <memory>
#include "MFront/BehaviourBrickBase.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct StressPotential;
  }  // namespace bbrick

  /*!
   * This brick provides the DDIF2 damage behaviour as a basis to build complex
   * behaviours.
   */
  struct DDIF2Brick : public BehaviourBrickBase {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     * \param[in] p    : parameters
     * \param[in] d    : data
     */
    DDIF2Brick(AbstractBehaviourDSL&,
               BehaviourDescription&,
               const Parameters&,
               const DataMap&);
    //! \return the name of the brick
    std::string getName() const override;
    /*!
     * \return the list of supported modelling hypotheses.
     */
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    //! complete the variable description
    void completeVariableDeclaration() const override;
    //! method called at the end of the input file processing
    void endTreatment() const override;
    //! destructor
    ~DDIF2Brick() override;

   protected:
    std::shared_ptr<bbrick::StressPotential> ddif2;
  };  // end of struct DDIF2Brick

}  // end of namespace mfront

#endif /* LIB_MFRONT_DDIF2BRICKBEHAVIOURBRICK_H */
