/*!
 * \file   LocalDataStructure.hxx
 * \brief
 * \author Thomas Helfer
 * \date   02 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_LOCALDATASTRUCTURE_HXX
#define LIB_LOCALDATASTRUCTURE_HXX

#include <map>
#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * structure used to create local data structure
   */
  struct MFRONT_VISIBILITY_EXPORT LocalDataStructure {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! description of a variable
    struct Variable {
      //! type of the variable
      std::string type;
      //! name of the variable
      std::string name;
    };
    //! name of the variable using the local data structure
    std::string name;
    /*!
     * add a new variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: variables
     */
    LocalDataStructure& addVariable(const Hypothesis, const Variable&);
    //! \brief return the list of specialised hypotheses
    std::vector<Hypothesis> getSpecialisedHypotheses() const;
    /*!
     * \brief return the list of variables of variables of the given
     * hypothesis
     * \param[in] h: modelling hypothesis
     */
    const std::vector<Variable>& get(const Hypothesis) const;
    /*!
     * \brief return true if a variable has been defined
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     */
    bool contains(const Hypothesis, const std::string&) const;

   private:
    //! variable for the undefined hypothesis
    std::vector<Variable> uv;
    //! variables for specialised hypothesis
    std::map<Hypothesis, std::vector<Variable>> sv;
  };  // end of LocalDataStructure

}  // end of namespace mfront

#endif /* LIB_LOCALDATASTRUCTURE_HXX */
