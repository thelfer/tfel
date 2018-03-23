/*!
 * \file   StressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIAL_HXX

#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress.
     */
    struct StressPotential {
      //! a simple alias
      using ModellingHypothesis = tfel::material::ModellingHypothesis;
      //! a simple alias
      using Hypothesis = ModellingHypothesis::Hypothesis;
      //! \return the name of the stress potential
      virtual std::string getName() const = 0;
      /*!
       * \brief This method returns the list of supported modelling
       * hypotheses that are supported by the stress potential.
       *
       * This method is called after the input file processing. Two
       * cases may arise:
       * - if the user has specified a list of supported modelling
       *   hypotheses, this list is used to check if the user's request
       *   can be fulfilled. If the stress potential returns an empty list, it
       *   means that the stress potential does not have any restriction.
       * - if the user has not specified the list of supported modelling
       *   hypotheses, this method is used to select a list of supported
       *   modelling hypotheses. If the stress potential returns an empty list,
       *   it means that the stress potential does not participate to the
       *   modelling hypotheses selection. If all the stress potentials returns
       *   an empty list, a set of default modelling hypotheses is selected.
       */
      virtual std::vector<Hypothesis> getSupportedModellingHypotheses()
          const = 0;
      //! method called at the end of the input file processing
      virtual void endTreatment() const = 0;
      //! complete the variable description
      virtual void completeVariableDeclaration() const = 0;
      //! destructor
      virtual ~StressPotential();
    };  // end of struct StressPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIAL_HXX */
