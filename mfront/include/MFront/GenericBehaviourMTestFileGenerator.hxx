/*!
 * \file
 * mfront/include/MFront/GenericBehaviourMTestFileGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19/03/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOURMTESTFILEGENERATOR_HXX
#define LIB_MFRONT_GENERICBEHAVIOURMTESTFILEGENERATOR_HXX

#include "MFront/MFrontConfig.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/MTestFileGeneratorBase.hxx"

namespace mfront {

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT GenericBehaviourMTestFileGenerator
      final : public mfront::MTestFileGeneratorBase {
    /*!
     * \brief a small helper structure describing the gradients and
     * thermodynamic forces of the behaviour
     */
    struct BehaviourVariablesDescription {
      struct VariableDescription {
        const char* const name;
        SupportedTypes::TypeFlag type;
      };
      const unsigned short ngradients;
      const VariableDescription* const gradients;
      const VariableDescription* const thermodynamic_forces;
    };
    /*!
     * \brief constructor
     * \param[in] bvd: description of the gradients and thermodynamic forces of
     *                 the behaviour
     * \param[in] l: library
     * \param[in] b: behaviour
     */
    GenericBehaviourMTestFileGenerator(const BehaviourVariablesDescription&,
                                       const char* const,
                                       const char* const);
    /*!
     * \brief set the gradients at the beginning of the time step
     * \param[in] g : gradient
     */
    virtual void setGradientsAtTheBeginningOfTheTimeStep(const double* const);
    /*!
     * \brief set the gradients at the end of the time step
     * \param[in] g : gradient
     */
    virtual void setGradientsAtTheEndOfTheTimeStep(const double* const);
    /*!
     * \brief set thermodynamic forces
     * \param[in] thfs: thermodynamic forces
     */
    virtual void setThermodynamicForcesAtTheBeginningOfTheTimeStep(
        const double* const);
    //! \brief destructor
    ~GenericBehaviourMTestFileGenerator() override;

   protected:
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    void writeBehaviourDeclaration(std::ostream&) const override;
    /*!
     * write driving variables
     * \param[in] os : output stream
     */
    void writeGradients(std::ostream&) const override;
    /*!
     * \brief description of the gradients and thermodynamic forces of the
     * behaviour
     */
    const BehaviourVariablesDescription& d;
    //! \brief library name
    const char* const library;
    //! \brief behaviour name
    const char* const behaviour;
    //! \brief gradients at the beginning of the time step
    const double* gradients_bts;
    //! \brief gradients at the end of the time step
    const double* gradients_ets;
    //! \brief thermodynamic forces at the beginning of the time step
    const double* th_forces_bts;
  };  // end of struct GenericBehaviourMTestFileGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOURMTESTFILEGENERATOR_HXX */
