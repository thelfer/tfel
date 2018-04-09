/*!
 * \file   include/MFront/BehaviourBrick/StressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX

#include <string>
#include <memory>
#include <vector>
#include "MFront/BehaviourDescription.hxx"

namespace tfel {
  namespace utilities {
    // forward declaration
    struct Data;
  }  // end of namespace utilities
}  // end of namespace tfel

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

  namespace bbrick {

    // forward declaration
    struct OptionDescription;

    /*!
     * \brief class describing a stress criterion
     */
    struct StressCriterion {
      //! a simple alias
      using Data = tfel::utilities::Data;
      //! a simple alias
      using DataMap = std::map<std::string, Data>;
      /*!
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] id: flow id
       * \param[in] d: options
       */
      virtual void initialize(BehaviourDescription&,
                              AbstractBehaviourDSL&,
                              const std::string&,
                              const DataMap&) = 0;
      //! \return the flow options
      virtual std::vector<OptionDescription> getOptions() const = 0;
      /*!
       * \return the code that defines `seqel`+id, the elastic prediction of
       * the criterion.
       * \param[in] id: flow id
       */
      virtual std::string computeElasticPrediction(
          const std::string&) const = 0;
      /*!
       * \return the code computing the criterion.
       * The code defines a variable named "seq"+id
       * \param[in] id: flow id
       */
      virtual std::string computeCriterion(const std::string&) const = 0;
      /*!
       * \return the code computing the criterion and its normal.
       * The code defines the variables named "seq"+id and "n"+id
       * \param[in] id: flow id
       */
      virtual std::string computeNormal(const std::string&) const = 0;
      /*!
       * \brief return the code computing the criterion and its normal.
       * The code defines the variables named "seq"+id, "n"+id and
       * "dn"+id+"_ds".
       * \param[in] id: flow id
       */
      virtual std::string computeNormalDerivative(const std::string&) const = 0;
      //! destructor
      virtual ~StressCriterion();
    };  // end of struct StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX */
