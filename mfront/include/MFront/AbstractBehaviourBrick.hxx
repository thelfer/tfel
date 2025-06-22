/*!
 * \file   mfront/include/MFront/AbstractBehaviourBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   October 20, 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURBRICK_HXX
#define LIB_MFRONT_MFRONTBEHAVIOURBRICK_HXX

#include <map>
#include <string>
#include <vector>
#include <utility>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/BehaviourBrickDescription.hxx"

namespace tfel::utilities {
  // forward declaration
  struct Data;
}  // end of namespace tfel::utilities

namespace mfront {

  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct BehaviourData;

  namespace bbrick {
    // forward declaration
    struct OptionDescription;
  }  // end of namespace bbrick

  /*!
   * \brief an a behaviour' brick is a way to alleviate to build a complex
   * behaviour.
   */
  struct AbstractBehaviourBrick {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! a simple alias
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    //! a simple alias
    using tokens_iterator = CxxTokenizer::const_iterator;
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
    //! \brief return the name of the brick
    virtual std::string getName() const = 0;
    //! \return a description of the brick
    virtual BehaviourBrickDescription getDescription() const = 0;
    /*!
     * \param[in] b: if true, all options are returned. Otherwise, only the most
     * adapted options for the behaviour are returned.
     * \return the description of the brick options
     */
    virtual std::vector<bbrick::OptionDescription> getOptions(
        const bool) const = 0;
    /*!
     * \brief intialize the brick
     * \param[in] p: parameters
     * \param[in] d: data
     */
    virtual void initialize(const Parameters&, const DataMap&) = 0;
    /*!
     * \brief treat a keyword
     * \param[in] key: keyword to be treated
     * \param[in,out] p:   current position in the file
     * \param[in] pe:  iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&, tokens_iterator&, const tokens_iterator) = 0;
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
    virtual std::vector<Hypothesis> getSupportedModellingHypotheses() const = 0;
    //! method called at the end of the input file processing
    virtual void endTreatment() const = 0;
    //! complete the variable description
    virtual void completeVariableDeclaration() const = 0;
    //! descructor
    virtual ~AbstractBehaviourBrick();
  };  // end of struct AbstractBehaviourBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURBRICK_H */
