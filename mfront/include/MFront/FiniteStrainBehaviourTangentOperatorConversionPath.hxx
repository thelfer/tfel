/*!
 * \file
 * mfront/include/MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERSIONPATH_HXX
#define LIB_MFRONT_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERSIONPATH_HXX

#include <vector>
#include "MFront/FiniteStrainBehaviourTangentOperatorConversion.hxx"

namespace mfront {

  /*!
   * a structure describing a conversion path
   */
  struct FiniteStrainBehaviourTangentOperatorConversionPath
      : protected std::vector<FiniteStrainBehaviourTangentOperatorConversion> {
    //! a simple alias
    typedef tfel::material::FiniteStrainBehaviourTangentOperatorBase::Flag
        TangentOperatorFlag;
    /* make public some methods from the underlying vector class */
    using std::vector<
        FiniteStrainBehaviourTangentOperatorConversion>::const_iterator;
    using std::vector<
        FiniteStrainBehaviourTangentOperatorConversion>::size_type;
    using std::vector<FiniteStrainBehaviourTangentOperatorConversion>::insert;
    using std::vector<
        FiniteStrainBehaviourTangentOperatorConversion>::push_back;
    using std::vector<FiniteStrainBehaviourTangentOperatorConversion>::begin;
    using std::vector<FiniteStrainBehaviourTangentOperatorConversion>::end;
    using std::vector<FiniteStrainBehaviourTangentOperatorConversion>::empty;
    using std::vector<FiniteStrainBehaviourTangentOperatorConversion>::size;
    using std::vector<FiniteStrainBehaviourTangentOperatorConversion>::clear;
    /*!
     * \return an iterator to the conversion that leads to the specified
     * tangent operator or end() if none was found.
     */
    const_iterator find(const TangentOperatorFlag) const;
    /*!
     * get all path starting from a point
     * \param[in] b : starting point
     * \param[in] k : known tangent operator (defined by the user)
     * \param[in] converters : known conversions
     */
    static std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>
    getConversionsPath(
        const TangentOperatorFlag&,
        const std::vector<TangentOperatorFlag>&,
        const std::vector<FiniteStrainBehaviourTangentOperatorConversion>&);
    /*!
     * get the shortest path leading to the given tangent operator (if any)
     * \param[in] paths : all the possible paths starting from
     * user-defined tangent operators.
     * \param[in] t     : search tangent operator
     */
    static FiniteStrainBehaviourTangentOperatorConversionPath getShortestPath(
        const std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>&,
        const TangentOperatorFlag&);

   private:
    /*!
     * get all path starting from a point
     * \param[in] b : starting point
     * \param[in] c : path that led to b
     * \param[in] k : known tangent operator (defined by the user)
     * \param[in] converters : known conversions
     */
    static void getConversionsPath(
        std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>&,
        const TangentOperatorFlag&,
        const std::vector<TangentOperatorFlag>&,
        const std::vector<TangentOperatorFlag>&,
        const std::vector<FiniteStrainBehaviourTangentOperatorConversion>&);
  };  // endof struct FiniteStrainBehaviourTangentOperatorConversionPath

}  // end of namespace mfront

#endif /* LIB_MFRONT_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERSIONPATH_HXX */
