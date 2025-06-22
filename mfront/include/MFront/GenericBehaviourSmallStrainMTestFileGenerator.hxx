/*!
 * \file mfront/include/MFront/GenericBehaviourSmallStrainMTestFileGenerator.hxx
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

#ifndef LIB_MFRONT_GENERICBEHAVIOURSMALLSTRAINMTESTFILEGENERATOR_HXX
#define LIB_MFRONT_GENERICBEHAVIOURSMALLSTRAINMTESTFILEGENERATOR_HXX

#include "MFront/MFrontConfig.hxx"
#include "MFront/MTestFileGeneratorBase.hxx"

namespace mfront {

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT
      GenericBehaviourSmallStrainMTestFileGenerator final
      : public mfront::MTestFileGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] l : library
     * \param[in] b : behaviour
     */
    GenericBehaviourSmallStrainMTestFileGenerator(const std::string&,
                                                  const std::string&);
    /*!
     * set the strain at the beginning of the time step
     * \param[in] e : strain
     */
    virtual void setStrainTensorAtTheBeginningOfTheTimeStep(
        const double* const);
    /*!
     * set the strain at the end of the time step
     * \param[in] e : strain
     */
    virtual void setStrainTensorAtTheEndOfTheTimeStep(const double* const);
    /*!
     * set stresses
     * \param[in] s : stresses
     */
    virtual void setStressTensor(const double* const);
    /*!
     * destructor
     */
    ~GenericBehaviourSmallStrainMTestFileGenerator() override;

   protected:
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    void writeBehaviourDeclaration(std::ostream&) const override;
    /*!
     * \brief write driving variables
     * \param[in] os : output stream
     */
    void writeGradients(std::ostream&) const override;
    //! library name
    std::string library;
    //! behaviour name
    std::string behaviour;
    //! strain
    double eto0[6];
    //! strain increment
    double eto1[6];
    //! stress increment
    double stress[6];
  };  // end of struct GenericBehaviourSmallStrainMTestFileGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOURSMALLSTRAINMTESTFILEGENERATOR_HXX */
