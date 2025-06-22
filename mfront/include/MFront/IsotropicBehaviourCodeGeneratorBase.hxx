
/*!
 * \file   mfront/include/MFront/IsotropicBehaviourCodeGeneratorBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ISOTROPICBEHAVIOURCODEGENERATORBASE_HXX
#define LIB_MFRONT_ISOTROPICBEHAVIOURCODEGENERATORBASE_HXX

#include "MFront/BehaviourCodeGeneratorBase.hxx"

namespace mfront {

  struct IsotropicBehaviourCodeGeneratorBase : BehaviourCodeGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     */
    IsotropicBehaviourCodeGeneratorBase(const FileDescription&,
                                        const BehaviourDescription&,
                                        const BehaviourInterfaceMap&);
    //! \brief destructor
    ~IsotropicBehaviourCodeGeneratorBase() override;

   protected:
    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;
    void writeBehaviourParserSpecificTypedefs(std::ostream&) const override;
    void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const override;
    void writeBehaviourComputePredictionOperator(
        std::ostream&, const Hypothesis) const override;
    void writeBehaviourComputeTangentOperator(std::ostream&,
                                              const Hypothesis) const override;
    /*!
     * \brief this method returns if the elastic strain at the beginning of the
     * time step shall be computed from the stress at the beginning of the time
     * step and if the elastic strain at the end of the time step is required.
     *
     * This method returns true if the stress update algorithm is used and the
     * elastic material properties are defined and non constant.
     */
    virtual bool shallComputeTheElasticStrain() const;
    /*!
     * \brief this method writes the computation of deviatoric part of the trial
     * stress, the normal. If required, it also write the computation of the
     * elastic strain at the beginning of the time step.
     *
     * \param[in] os: output stream
     */
    virtual void writeBehaviourIntegratorPreprocessingStep(std::ostream&) const;
    /*!
     * \brief this method writes the computation of final stress.  If required,
     * it also write the computation of the elastic strain at the end of the
     * time step.
     *
     * \param[in] os: output stream
     */
    virtual void writeBehaviourIntegratorPostprocessingStep(
        std::ostream&) const;
  };  // end of IsotropicBehaviourCodeGeneratorBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_ISOTROPICBEHAVIOURCODEGENERATORBASE_HXX */
