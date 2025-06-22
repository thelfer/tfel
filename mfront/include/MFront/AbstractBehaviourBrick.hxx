/*!
 * \file   mfront/include/MFront/AbstractBehaviourBrick.hxx
 * \brief
 * \author Helfer Thomas
 * \date   October 20, 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURBRICK_H_
#define LIB_MFRONT_MFRONTBEHAVIOURBRICK_H_

#include <map>
#include <string>
#include <vector>
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace tfel {
  namespace utilities {
    // forward declaration
    struct Data;
  }  // namespace utilities
}  // namespace tfel

namespace mfront {

  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct BehaviourData;

  namespace bbrick {
    // forward declaration
    struct RequirementManager;
  }  // end of namespace bbrick

  /*!
   * BehaviourBrick are ready-to use block used to build a complex
   * behaviour.
   */
  struct AbstractBehaviourBrick {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    /*!
     * Object used to pass parameters to BehaviourBricks constructor
     * The key   is the parameter name.
     * The value is the parameter value.
     */
    using Parameters = std::map<std::string, std::string>;
    //! a simple alias
    using Parameter = Parameters::value_type;
    //! a simple alias
    using Data = tfel::utilities::Data;
    //! a simple alias
    using DataMap = std::map<std::string, Data>;
    /*!
     * \brief return the name of the brick
     */
    virtual std::string getName() const = 0;
    /*!
     * \brief This method returns the list of supported modelling
     * hypotheses that are supported by the brick.
     *
     * This method is called after the input file processing. Two
     * cases may arise:
     * - if the user has specified a list of supported modelling
     *   hypotheses, this list is used to check if the user's request
     *   can be fulfilled. If the brick returns an empty list, it
     *   means that the brick does not have any restriction.
     * - if the user has not specified the list of supported modelling
     *   hypotheses, this method is used to select a list of supported
     *   modelling hypotheses. If the brick returns an empty list, it
     *   means that the brick does not participate to the modelling
     *   hypotheses selection. If all the bricks returns an empty
     *   list, a set of default modelling hypotheses is selected.
     */
    virtual std::vector<Hypothesis> getSupportedModellingHypotheses(
        void) const = 0;
    /*!
     * \brief add requirements for the given modelling hypothesis
     * \param[in] r : requirement manager
     * \param[in] h : modelling hypothesis
     */
    virtual void addRequirements(bbrick::RequirementManager&,
                                 const Hypothesis) const = 0;
    //! ends the file treatment
    virtual void endTreatment() const = 0;
    //! descructor
    virtual ~AbstractBehaviourBrick();
  };  // end of struct AbstractBehaviourBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURBRICK_H */
