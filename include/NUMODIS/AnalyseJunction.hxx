/*!
 * \file   include/NUMODIS/AnalyseJunction.hxx
 * \brief  analyse the strength of a junction within the line tension model.
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_NUMODIS_DUPUY2017_ANALYSEJUNCTION_HXX
#define LIB_NUMODIS_DUPUY2017_ANALYSEJUNCTION_HXX

#include <vector>
#include "NUMODIS/Config.hxx"

namespace numodis {

  struct Crystallo;
  struct GSystem;
  struct IPlane;

  namespace Dupuy2017 {
    TFELNUMODIS_VISIBILITY_EXPORT void precomputeJunctionParameters(
        const Crystallo* pcrystallo,
        const GSystem& gsystem1,
        const GSystem& gsystem2,
        std::vector<double>& burgers,
        std::vector<double>& phi);

    TFELNUMODIS_VISIBILITY_EXPORT
    double computeNormalizedStress(double nu,
                                   const std::vector<double>& burgers,
                                   const std::vector<double>& phi);

    TFELNUMODIS_VISIBILITY_EXPORT double computeHeuristicHardeningCoefficient(
        const Crystallo* pcrystallo,
        double nu,
        const IPlane& iplane1,
        const IPlane& iplane2,
        double bj,
        double sij);

    TFELNUMODIS_VISIBILITY_EXPORT double
    computeHeuristicNonColinearHardeningCoefficient(double nu, double sij);

    TFELNUMODIS_VISIBILITY_EXPORT double
    computeHeuristicColinearHardeningCoefficient(const Crystallo* pcrystallo,
                                                 double nu,
                                                 const IPlane& iplane1,
                                                 const IPlane& iplane2);

  }  // namespace Dupuy2017

}  // end of namespace numodis

#endif /* LIB_NUMODIS_DUPUY2017_ANALYSEJUNCTION_HXX */
