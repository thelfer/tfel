/*!
 * \file   mtest/include/MTest/OxidationStatusEvolution.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/10/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_OXIDATIONSTATUSEVOLUTION_HXX
#define LIB_MTEST_OXIDATIONSTATUSEVOLUTION_HXX

#include <map>
#include "MTest/Config.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  // forward declaration
  struct PipeMesh;

  /*!
   * \brief structure in charge of representing the evolution of the oxidation
   * status.
   */
  struct OxidationStatusEvolution : Evolution {
    /*!
     * \brief constructor
     * \param[in] mesh: mesh description
     * \param[in] m: evolution manager
     */
    OxidationStatusEvolution(const PipeMesh&, const EvolutionManager&);
    //
    real operator()(const real) const override;
    bool isConstant() const override;
    void setValue(const real) override;
    void setValue(const real, const real) override;
    /*!
     * \brief set the oxidation length at the inner boundary at the given time
     * \param[in] t: time
     * \param[in] l: oxidation length
     */
    virtual void setInnerBoundaryOxidationLengthEvolution(const real,
                                                          const real);
    /*!
     * \brief set the oxidation length at the outer boundary at the given time
     * \param[in] t: time
     * \param[in] l: oxidation length
     */
    virtual void setOuterBoundaryOxidationLengthEvolution(const real,
                                                          const real);
    //! \brief destructor
    ~OxidationStatusEvolution() override;

   private:
    /*!
     * \return the oxidation length at the inner boundary at the given time
     * \param[in] t: time
     */
    virtual real getInnerBoundaryOxidationLength(const real) const;
    /*!
     * \return the oxidation length at the outer boundary at the given time
     * \param[in] t: time
     */
    virtual real getOuterBoundaryOxidationLength(const real) const;
    //! \brief inner radius
    const real inner_radius;
    //! \brief outer radius
    const real outer_radius;
    //! \brief externally defined evolutions
    const EvolutionManager& evm;
    //! \brief oxidation length at the inner boundary as a function of time
    std::map<real, real> inner_boundary_oxidation_length_evolution;
    //! \brief oxidation length at the outer boundary as a function of time
    std::map<real, real> outer_boundary_oxidation_length_evolution;
  };  // end of struct OxidationStatusEvolution

}  // end of namespace mtest

#endif /* LIB_MTEST_OXIDATIONSTATUSEVOLUTION_HXX */
