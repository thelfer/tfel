/*!
 * \file   mfront/include/MFront/CamClayBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   25/02/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CAMCLAYBRICKBEHAVIOURBRICK_HXX
#define LIB_MFRONT_CAMCLAYBRICKBEHAVIOURBRICK_HXX

#include <memory>
#include "MFront/BehaviourBrickBase.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront {

  namespace bbrick {
    // forward declaration
    struct StressPotential;
  }  // end of namespace bbrick

  /*!
   * This brick provides a brick based on the Cam-Clay non linear elastic
   * behaviour.
   */
  struct CamClayBrick : public BehaviourBrickBase {
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : mechanical behaviour description
     */
    CamClayBrick(AbstractBehaviourDSL&, BehaviourDescription&);
    std::string getName() const override;
    BehaviourBrickDescription getDescription() const override;
    std::vector<bbrick::OptionDescription> getOptions(
        const bool) const override;
    void initialize(const Parameters&, const DataMap&) override;
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    void completeVariableDeclaration() const override;
    void endTreatment() const override;
    //! destructor
    ~CamClayBrick() override;

   protected:
    std::shared_ptr<bbrick::StressPotential> ddif2;
  };  // end of struct CamClayBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_CAMCLAYBRICKBEHAVIOURBRICK_H */
