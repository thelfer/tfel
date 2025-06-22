/*!
 * \file  mtest/include/MTest/MistralBehaviour.hxx
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

#ifndef LIB_MTEST_MISTRALBEHAVIOUR_HXX
#define LIB_MTEST_MISTRALBEHAVIOUR_HXX

#include <memory>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/CastemSmallStrainBehaviour.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL MistralBehaviour
      : public CastemSmallStrainBehaviour {
    //! a simple alias
    using Parameters = tfel::utilities::Data;
    /*!
     * \param[in] l: library
     * \param[in] f: function
     * \param[in] parameters: parameters file
     * \param[in] h: modelling hypothesis
     */
    static std::shared_ptr<Behaviour> buildMistralBehaviour(const std::string&,
                                                            const std::string&,
                                                            const Parameters&,
                                                            const Hypothesis);
    std::vector<std::string> getOptionalMaterialProperties() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    /*!
     * \return the string passed to the UMAT function through the
     * CMNAME parameter.
     */
    const char* getBehaviourNameForUMATFunctionCall() const override;
    //! destructor
    ~MistralBehaviour() override;

   protected:
    /*!
     * \param[in] bd: umat behaviour description
     * \param[in] cn: material name
     * \param[in] mp: behaviour parameters (defined through material
     *                properties)
     * \param[in] yg: Young modulus
     * \param[in] nu: Poisson ratio
     * \param[in] SENSIP1:
     * \param[in] SENSIP2:
     * \param[in] ICBASE:
     */
    MistralBehaviour(const StandardBehaviourDescription&,
                     const std::string&,
                     const std::vector<double>&,
                     const std::pair<double, double>&,
                     const std::pair<double, double>&,
                     const int,
                     const int,
                     const int);
    /*!
     * \brief values of the parameters (additional material properties
     * in Mistral behaviours
     */
    const std::vector<double> parameters;
    //! Young modulus
    const std::pair<double, double> young_modulus;
    //! Poisson ratio
    const std::pair<double, double> poisson_ratio;
    char mname[16];
    const int SENSIP1;
    const int SENSIP2;
    const int ICBASE;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MISTRALBEHAVIOUR_HXX */
