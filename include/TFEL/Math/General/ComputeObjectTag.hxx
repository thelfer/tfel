/*!
 * \file   ComputeObjectTag.hxx
 * \brief  This file declares the ComputeObjectTag class
 * \author Helfer Thomas
 * \date   16 Oct 2006
 */

#ifndef _LIB_TFEL_COMPUTEOBJECTTAG_H_
#define _LIB_TFEL_COMPUTEOBJECTTAG_H_ 

namespace tfel{

  namespace math{

    /*!
     * \class ScalarTag
     * \brief Helper class to characterise scalars.
     */ 
    struct ScalarTag;

    /*!
     * \class UnaryOperatorTag
     * \brief Helper class to characterise operators.
     */ 
    struct UnaryOperatorTag;

    /*!
     * \class ComputeObjectTag
     * \brief a metafunction which compute the tag.
     * associated with a given type.
     * \param typename T, type of which we want to know the associated.
     * \return type, the associated tag.
     * \note   returns InvalidType if not tag matches.
     */
    template<typename T>
    struct ComputeObjectTag;

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/General/ComputeObjectTag.ixx"

#endif /* _LIB_TFEL_COMPUTEOBJECTTAG_H */

