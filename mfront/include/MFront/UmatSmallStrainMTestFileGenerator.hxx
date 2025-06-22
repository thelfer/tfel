/*!
 * \file  mfront/include/MFront/UmatSmallStrainMTestFileGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_UMATSMALLSTRAINMTESTFILEGENERATOR_HXX
#define LIB_MFRONT_UMATSMALLSTRAINMTESTFILEGENERATOR_HXX

#include "MFront/MFrontConfig.hxx"
#include "MFront/MTestFileGeneratorBase.hxx"

namespace mfront {

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT UmatSmallStrainMTestFileGenerator
      final : public mfront::MTestFileGeneratorBase {
    /*!
     * constructor
     * \param[in] i : interface
     * \param[in] l : library
     * \param[in] b : behaviour
     */
    UmatSmallStrainMTestFileGenerator(const std::string&,
                                      const std::string&,
                                      const std::string&);
    /*!
     * set the strain at the beginning of the time step
     * \param[in] e : strain
     */
    virtual void setStrainTensor(const double* const);
    /*!
     * set the strain increment
     * \param[in] e : strain increments
     */
    virtual void setStrainTensorIncrement(const double* const);
    /*!
     * set stresses
     * \param[in] s : stresses
     */
    virtual void setStressTensor(const double* const);
    /*!
     * destructor
     */
    virtual ~UmatSmallStrainMTestFileGenerator();

   protected:
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    virtual void writeBehaviourDeclaration(std::ostream&) const override;
    /*!
     * write driving variables
     * \param[in] os : output stream
     */
    virtual void writeDrivingVariables(std::ostream&) const override;
    //! interface name
    std::string interface;
    //! library name
    std::string library;
    //! behaviour name
    std::string behaviour;
    //! strain
    double eto[6];
    //! strain increment
    double deto[6];
    //! stress increment
    double stress[6];
  };  // end of struct UmatSmallStrainMTestFileGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_UMATSMALLSTRAINMTESTFILEGENERATOR_HXX */
