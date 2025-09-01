/*!
 * \file   mfront/include/AbstractBehaviourCodeGenerator.hxx
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

#ifndef LIB_MFRONT_ABSTRACTBEHAVIOURCODEGENERATOR_HXX
#define LIB_MFRONT_ABSTRACTBEHAVIOURCODEGENERATOR_HXX

#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  /*!
   * \brief a deleguate class aimed a generating the code associated with a
   * behaviour.
   */
  struct AbstractBehaviourCodeGenerator {
    //! \brief a simple alias
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    //! \brief a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! \brief a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    /*!
     * \brief generate the output files
     * \param[in] mhs: modelling hypotheses to be treated
     */
    virtual void generateOutputFiles(const std::set<Hypothesis>&) = 0;
    /*!
     * \brief write the call to a material property
     * \param[out] out: output stream
     * \param[in]  m:   material property description
     * \param[in]  f:   function converting input variable name.
     * The function f can be used to specify how evaluate a variable value.
     * For example, if we want to evaluate the variable name 'V' at
     * the end of the time step, we could make f return V+dV
     */
    virtual void writeMaterialPropertyEvaluation(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        std::function<std::string(const MaterialPropertyInput&)>&) const = 0;
    /*!
     * \return if the generation of the source file is required
     *
     * The source file contains implementations of functions common to all
     * interfaces.
     */
    virtual bool isSrcFileRequired() const = 0;
    //! \brief destructor
    virtual ~AbstractBehaviourCodeGenerator();
  };  // end of AbstractBehaviourCodeGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTBEHAVIOURCODEGENERATOR_HXX */
