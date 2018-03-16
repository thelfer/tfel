/*!
 * \file   include/MFront/BehaviourBrick/MisesStressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing an inelastic potential.
     */
    struct MisesStressCriterion {
      //! destructor
      virtual ~MisesStressCriterion();
    };  // end of struct MisesStressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX */
