/*!
 * \file   mfront/include/MFront/BehaviourBrick/DDIF2StressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX

#include <array>
#include <memory>
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;
  // forward declaration
  struct LocalDataStructure;

}  // end of namespace mfront

namespace mfront::bbrick {

  /*!
   * \brief class describing the computation of the stress through the
   * standard DDIF2 law.
   */
  struct DDIF2StressPotential : HookeStressPotential {
    //! \brief constructor
    DDIF2StressPotential();
    std::string getName() const override;
    std::vector<OptionDescription> getOptions(const BehaviourDescription&,
                                              const bool) const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const DataMap&) override;
    void completeVariableDeclaration(
        BehaviourDescription&, const AbstractBehaviourDSL&) const override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&) const override;
    //! destructor
    ~DDIF2StressPotential() override;

   protected:
    //! \brief supported algorithms
    enum {
      STATUS, /*!< An algorithm based on statuses. The statuses allows
               * solving the implicit equations based on an assumptions on the
               * damage state (see `tfel::material::DDIF2State`). Those
               * assumptions are check after convergence. If not satisfied,
               * the assumtions are modified and the implicit resolution is
               * restarted. */
      UPDATE_IMPLICIT_EQUATIONS_DURING_ITERATIONS /*!< algorithm used in
                                                   * version prior to 3.3.
                                                   * A priori less robust
                                                   * than the algorithm
                                                   * based on status
                                                   */
    } algorithm = STATUS;
    //! fracture stress
    std::array<MaterialProperty, 3u> sr;
    //! softening slopes
    std::array<MaterialProperty, 3u> rp;
    //! fracture energy
    std::array<MaterialProperty, 3u> gc;
    /*!
     * \brief if true, the implicit resolution is first performed with a null
     * time step.
     */
    bool firstConvergeOnDamage = true;
    //! handle pressure on crack surface
    bool pr = false;
  };  // end of struct DDIF2StressPotential

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX */
