/*!
 * \file  mtest/include/MTest/CastemUmatFiniteStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMUMATFINITESTRAINBEHAVIOUR_HXX
#define LIB_MTEST_CASTEMUMATFINITESTRAINBEHAVIOUR_HXX

#include <memory>
#include "TFEL/Utilities/Data.hxx"
#include "MTest/CastemUmatStandardBehaviour.hxx"
#include "MTest/CastemFiniteStrainBehaviour.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL CastemUmatFiniteStrainBehaviour
      : public CastemFiniteStrainBehaviour,
        public CastemUmatStandardBehaviour {
    //! a simple alias
    using Hypothesis = CastemFiniteStrainBehaviour::Hypothesis;
    //! a simple alias
    using Parameters = tfel::utilities::Data;
    /*!
     * \param[in] l: library
     * \param[in] f: function
     * \param[in] p: parameters
     * \param[in] h: modelling hypothesis
     */
    static std::shared_ptr<Behaviour> buildCastemUmatFiniteStrainBehaviour(
        const std::string&,
        const std::string&,
        const Parameters&,
        const Hypothesis);
    /*!
     * \param[in] bd: umat behaviour description
     * \param[in] cn: material name
     */
    CastemUmatFiniteStrainBehaviour(const StandardBehaviourDescription&,
                                    const std::string&);
    std::vector<std::string> getOptionalMaterialProperties() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    const char* getBehaviourNameForUMATFunctionCall() const override;
    //! destructor
    ~CastemUmatFiniteStrainBehaviour() override;

   protected:
    //! material name
    char mname[16u];
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMUMATFINITESTRAINBEHAVIOUR_HXX */
