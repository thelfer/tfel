/*!
 * \file   include/MFront/BehaviourBrick/StressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing an inelastic potential.
     */
    struct StressCriterion {
      //! destructor
      virtual ~StressCriterion();
    };  // end of struct StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX */
