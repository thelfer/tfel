/*!
 * \file   StressPotentialBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALBASE_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct BehaviourDescription;

  namespace bbrick {

    /*!
     * \brief an helper class aimed at simplifying the implementation of stress
     * potentials.
     */
    struct StressPotentialBase : StressPotential {
      //! a simple alias
      using DataMap = std::map<std::string, tfel::utilities::Data>;
      /*!
       * \param[in] dsl_: abstract behaviour dsl
       * \param[in] bd_: behaviour description
       */
      StressPotentialBase(AbstractBehaviourDSL&,
                          BehaviourDescription&);
      //! destructor
      ~StressPotentialBase() override;

     protected:
      // reference to the DSL
      AbstractBehaviourDSL& dsl;
      // reference to the behaviour description
      BehaviourDescription& bd;

    };  // end of struct StressPotentialBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALBASE_HXX */
