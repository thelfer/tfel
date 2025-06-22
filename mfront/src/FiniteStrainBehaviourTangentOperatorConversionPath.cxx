/*!
 * \file  mfront/src/FiniteStrainBehaviourTangentOperatorConversionPath.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <algorithm>
#include "MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx"

namespace mfront {

  FiniteStrainBehaviourTangentOperatorConversionPath::const_iterator
  FiniteStrainBehaviourTangentOperatorConversionPath::find(
      const FiniteStrainBehaviourTangentOperatorConversionPath::
          TangentOperatorFlag t) const {
    return std::find_if(
        this->begin(), this->end(),
        [&t](const FiniteStrainBehaviourTangentOperatorConversion& v) {
          return v.to() == t;
        });
  }  // end of FiniteStrainBehaviourTangentOperatorConversionPath::find

  void FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(
      std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>& r,
      const TangentOperatorFlag& b,
      const std::vector<TangentOperatorFlag>& cp,  //<! current path
      const std::vector<TangentOperatorFlag>& k,
      const std::vector<FiniteStrainBehaviourTangentOperatorConversion>&
          converters) {
    using std::find;
    using std::vector;
    vector<TangentOperatorFlag> current_path(cp);
    current_path.push_back(b);
    for (const auto& c : converters) {
      if (c.from() == b) {
        if (!((find(k.begin(), k.end(), c.to()) != k.end()) ||
              (find(cp.begin(), cp.end(), c.to()) != cp.end()))) {
          // not an known operator
          vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
          FiniteStrainBehaviourTangentOperatorConversionPath::
              getConversionsPath(paths, c.to(), current_path, k, converters);
          if (!paths.empty()) {
            vector<FiniteStrainBehaviourTangentOperatorConversionPath>::
                const_iterator pp;
            for (pp = paths.begin(); pp != paths.end(); ++pp) {
              r.push_back(*pp);
              r.back().insert(r.back().begin(), c);
            }
          } else {
            FiniteStrainBehaviourTangentOperatorConversionPath path;
            path.push_back(c);
            r.push_back(path);
          }
        }
      }
    }
  }  // end of
     // FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath

  std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>
  FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(
      const TangentOperatorFlag& b,
      const std::vector<TangentOperatorFlag>& k,
      const std::vector<FiniteStrainBehaviourTangentOperatorConversion>&
          converters) {
    std::vector<FiniteStrainBehaviourTangentOperatorConversionPath> r;
    FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(
        r, b, k, std::vector<TangentOperatorFlag>(), converters);
    return r;
  }  // end of
     // FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath

  FiniteStrainBehaviourTangentOperatorConversionPath
  FiniteStrainBehaviourTangentOperatorConversionPath::getShortestPath(
      const std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>&
          paths,
      const FiniteStrainBehaviourTangentOperatorConversionPath::
          TangentOperatorFlag& t) {
    // shortest path
    FiniteStrainBehaviourTangentOperatorConversionPath path;
    for (const auto& p : paths) {
      auto pc = p.find(t);
      if (pc != p.end()) {
        ++pc;
        // we found a conversion path candidate
        if (path.empty()) {
          // no previous path
          path.insert(path.begin(), p.begin(), pc);
        } else {
          // a previous path exists, select the shortest
          const auto l = pc - p.begin();
          if (l < path.size()) {
            path.clear();
            path.insert(path.begin(), p.begin(), pc);
          }
        }
      }
    }
    return path;
  }  // end of
     // FiniteStrainBehaviourTangentOperatorConversionPath::getShortestPath

}  // end of namespace mfront
