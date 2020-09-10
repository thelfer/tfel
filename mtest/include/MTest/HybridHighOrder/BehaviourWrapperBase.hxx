/*!
 * \file   mtest/include/MTest/HybridHighOrder/BehaviourWrapperBase.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   19/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_HYBRIDHIGHORDER_BEHAVIOURWRAPPERBASE_HXX
#define LIB_MTEST_HYBRIDHIGHORDER_BEHAVIOURWRAPPERBASE_HXX

#include "HybridHighOrder/Element/Behaviour.hxx"
#include "MTest/BehaviourWorkSpace.hxx"

namespace mtest{

  // forward declaration
  struct Behaviour;

  namespace hho {

    /*!
     * \brief a base for behaviour wrapper classes
     */
    struct BehaviourWrapperBase : ::hho::element::Behaviour {
      /*!
       * \brief constructor
       * \param[in] b: mtest behaviour
       */
      BehaviourWrapperBase(const mtest::Behaviour&);

      ::hho::Hypothesis get_hypothesis() const override;

      ::hho::element::Behaviour::Symmetry get_symmetry() const override;

      std::vector<::hho::element::Variable> get_gradients() const override;

      ::hho::size_type get_gradients_size() const override;

      std::vector<::hho::element::Variable> get_thermodynamic_forces()
          const override;

      ::hho::size_type get_thermodynamic_forces_size() const override;

      std::vector<::hho::element::Variable> get_material_properties()
          const override;

      ::hho::size_type get_material_properties_size() const override;

      std::vector<::hho::element::Variable> get_internal_state_variables()
          const override;

      ::hho::size_type get_internal_state_variables_size() const override;

      std::vector<::hho::element::Variable> get_external_state_variables()
          const override;

      ::hho::size_type get_external_state_variables_size() const override;

      std::vector<std::pair<::hho::element::Variable, ::hho::element::Variable>>
      get_tangent_operators_blocks() const override;

      ::hho::size_type get_tangent_operator_blocks_size() const override;

      void integrate(::hho::element::IntegrationPointState &,
                     ::hho::span<::hho::real>,
                     ::hho::span<const ::hho::real>,
                     ::hho::span<const ::hho::real>,
                     const ::hho::element::IntegrationPointState &,
                     const ::hho::element::IntegrationType,
                     const ::hho::real) const override;

      //! \brief destructor
      ~BehaviourWrapperBase() noexcept;

     protected:
      //!\brief underlying MTest' behaviour
      const mtest::Behaviour &behaviour;
      //! \brief workspace
      mutable BehaviourWorkSpace wk;
    };  // end of struct BehaviourWrapperBase

  }  // end of namespace hho

} // end of namespace mtest

#endif /* LIB_MTEST_HYBRIDHIGHORDER_BEHAVIOURWRAPPERBASE_HXX */
